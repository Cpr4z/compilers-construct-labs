#include "parser.hpp"

Node Parser::parse()
{
    return parseProgram();
}

Node Parser::parseProgram()
{
    Node node;
    node.m_data = Grammar::Token{Grammar::TokenType::Operator, "<программа>"};
    node.m_children.push_back(parseBlock());
    return node;
}

Node Parser::parseBlock()
{
    Node node;
    node.m_data = Grammar::Token{Grammar::TokenType::Operator, "<блок>"};

    expect(Grammar::TokenType::OpenBrace);
    Node body = parseOperatorList();
    expect(Grammar::TokenType::CloseBrace);
    node.m_children.push_back(body);
    return node;
}

Node Parser::parseOperatorList()
{
    Node node;
    node.m_data = Grammar::Token{Grammar::TokenType::Operator, "<список операторов>"};
    Node op = parseOperator();
    node.m_children.push_back(op);
    Node tail = parseTail();
    if (!tail.m_children.empty())
        node.m_children.push_back(tail);
    return node;

// for defending
//    Node node;
//    node.m_data = Grammar::Token{Grammar::TokenType::Operator, "<список операторов>"};
//    while (!match(Grammar::TokenType::CloseBrace))
//    {
//        Node op = parseOperator();
//        expect(Grammar::TokenType::Semicolon);
//        node.m_children.push_back(op);
//    }
//    return node;
}

Node Parser::parseOperator()
{
    Node node;
    node.m_data = Grammar::Token{Grammar::TokenType::Operator, "<оператор>"};
    size_t checkpoint = position();
    if (match(Grammar::TokenType::Identifier))
    {
        Grammar::Token id = advance();
        if (match(Grammar::TokenType::Assign))
        {
            advance();
            Node expr = parseExpression();
            node.m_children.emplace_back(id);
            node.m_children.emplace_back(Grammar::Token{Grammar::TokenType::Assign, "="});
            node.m_children.push_back(expr);
            return node;
        }
        else
        {
            rewind(checkpoint);
        }
    }
    else if (match(Grammar::TokenType::OpenBrace))
    {
        node.m_children.push_back(parseBlock());
    }
    else
    {
        throw std::runtime_error("Expected operator (assignment or block)");
    }
    return node;
}

Node Parser::parseTail()
{
    Node node;
    node.m_data = Grammar::Token{Grammar::TokenType::Operator, "<хвост>"};

    if (match(Grammar::TokenType::Semicolon))
    {
        advance();
        if (match(Grammar::TokenType::CloseBrace))
        {
            return node;
        }

        Node nextOp = parseOperator();
        Node nextTail = parseTail();

        node.m_children.push_back(nextOp);
        if (!nextTail.m_children.empty())
            node.m_children.push_back(nextTail);
    }
    return node;
}

Node Parser::parseExpression()
{
    if (match(Grammar::TokenType::Identifier))
    {
        size_t innerCheckpoint = position();
        Grammar::Token id = advance();
        if (match(Grammar::TokenType::Assign))
        {
            advance();
            Node rhs = parseExpression();

            Node assignNode;
            assignNode.m_data = Grammar::Token{Grammar::TokenType::Operator, "="};
            assignNode.m_children.emplace_back(id);
            assignNode.m_children.push_back(rhs);
            return assignNode;
        }
        rewind(innerCheckpoint);
    }

    Node node;
    node.m_data = Grammar::Token{Grammar::TokenType::Operator, "<выражение>"};
    Node left = parseSimpleExpression();
    if (match(Grammar::TokenType::RelOp1) || match(Grammar::TokenType::RelOp2))
    {
        Grammar::Token relOp = advance();
        Node right = parseSimpleExpression();

        node.m_children.push_back(left);
        node.m_children.emplace_back(relOp);
        node.m_children.push_back(right);
        return node;
    }
    return left;
}

Node Parser::parseSimpleExpression()
{
    Node node;
    node.m_data = Grammar::Token{Grammar::TokenType::Operator, "<простое выражение>"};

    bool hasUnary = false;
    Grammar::Token unarySign;

    if (match(Grammar::TokenType::PlusMinus)) {
        unarySign = advance();
        hasUnary = true;
    }

    Node term = parseTerm();

    if (hasUnary)
    {
        Node unaryNode;
        unaryNode.m_data = Grammar::Token{Grammar::TokenType::Operator, "<унарное выражение>"};
        unaryNode.m_children.emplace_back(unarySign);
        unaryNode.m_children.push_back(term);
        node.m_children.push_back(unaryNode);
    }
    else
    {
        node.m_children.push_back(term);
    }

    while (match(Grammar::TokenType::PlusMinus) || match(Grammar::TokenType::KeywordOr))
    {
        Grammar::Token op = advance();
        Node right = parseTerm();

        Node exprNode;
        exprNode.m_data = Grammar::Token{Grammar::TokenType::Operator, "<простое выражение>"};
        exprNode.m_children.push_back(node);
        exprNode.m_children.emplace_back(op);
        exprNode.m_children.push_back(right);
        node = exprNode;
    }
    return node;
}

Node Parser::parseTerm()
{
    Node node;
    node.m_data = Grammar::Token{Grammar::TokenType::Operator, "<терм>"};

    Node left = parseFactor();
    while (
            match(Grammar::TokenType::MulDiv) ||
            match(Grammar::TokenType::KeywordDiv) ||
            match(Grammar::TokenType::KeywordMod) ||
            match(Grammar::TokenType::KeywordAnd)
            )
    {
        Grammar::Token op = advance();
        Node right = parseFactor();

        Node termNode;
        termNode.m_data = Grammar::Token{Grammar::TokenType::Operator, "<терм>"};
        termNode.m_children.push_back(left);
        termNode.m_children.emplace_back(op);
        termNode.m_children.push_back(right);
        left = termNode;
    }
    return left;
}

Node Parser::parseFactor()
{
    Node node;
    node.m_data = Grammar::Token{Grammar::TokenType::Operator, "<фактор>"};
    if (match(Grammar::TokenType::KeywordNot)) {
        Grammar::Token notToken = advance();
        Node child = parseFactor();

        node.m_children.emplace_back(notToken);
        node.m_children.push_back(child);
        return node;
    }

    if (match(Grammar::TokenType::PlusMinus))
    {
        Grammar::Token unaryToken = advance();
        Node child = parseFactor();

        node.m_children.emplace_back(unaryToken);
        node.m_children.push_back(child);
        return node;
    }

    if (match(Grammar::TokenType::OpenParen))
    {
        advance();
        Node expr = parseSimpleExpression();
        expect(Grammar::TokenType::CloseParen);

        return expr;
    }

    if (match(Grammar::TokenType::Identifier) || match(Grammar::TokenType::KeywordConst))
    {
        node.m_data = advance();
        return node;
    }
    throw std::runtime_error("Expected factor but got: " + current().m_value);
}

const Grammar::Token& Parser::current() const
{
    return m_tokens.at(m_pos);
}

const Grammar::Token& Parser::advance()
{
    return m_tokens.at(m_pos++);
}

bool Parser::match(Grammar::TokenType type)
{
    return m_tokens.size() > m_pos && m_tokens[m_pos].m_tokenType == type;
}

void Parser::expect(Grammar::TokenType type)
{
    if (!match(type))
    {
        throw std::runtime_error("Expected token of type ...");
    }
    advance();
}