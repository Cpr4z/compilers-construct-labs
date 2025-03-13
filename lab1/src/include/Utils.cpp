#include "Utils.h"

#include <format>
#include <map>
#include <set>
#include <boost/bimap.hpp>
#include <boost/signals2.hpp>
#include "../MyStack.hpp"

namespace Utils
{
namespace tokenConstants
{
    static std::map<Token, OperatorPriority> OPERATIONS;
    static std::set<Token> ALPHABET;
    static std::set<Token> TOKENS;

    auto getOtherTokens()
    {
        if (TOKENS.empty())
        {
            TOKENS.emplace('(');
            TOKENS.emplace(')');
            TOKENS.emplace('+');
        }
        return TOKENS;
    }

    auto getAlphabet()
    {
        if (ALPHABET.empty()) {
            for (char token = 'a'; token <= 'z'; ++token)
            {
                ALPHABET.emplace(token);
            }
        }
        return ALPHABET;
    }

    auto getOperations()
    {
        if (OPERATIONS.empty())
        {
            OPERATIONS.emplace('|', 1);
            OPERATIONS.emplace('.', 2);
            OPERATIONS.emplace('*', 3);
        }
        return OPERATIONS;
    }

    auto mergeAllTokens()
    {
        std::set<Token> allTokens;

        for (const auto& [key, _]: getOperations())
        {
            allTokens.insert(key);
        }

        auto alpa = getAlphabet();
        allTokens.insert(alpa.begin(), alpa.end());
        auto other = getOtherTokens();
        allTokens.insert(other.begin(), other.end());

        return allTokens;
    }
}

namespace preprocessing
{
bool isValidRegex(std::string& regex)
{
    if (regex.empty())
    {
        return false;
    }

    std::erase_if(regex, [](Token tkn){ return isspace(tkn); });

    return std::ranges::all_of(regex, [allTokens = tokenConstants::mergeAllTokens()](Token tkn){
        return allTokens.contains(tkn);
    });
}

void preprocessRegex(std::string& regex)
{
    //replace + to .*
    // a+ -> a.a*
    size_t pos = 0;
    while ((pos = regex.find('+', pos)) != std::string::npos)
    {
        regex.replace(pos, 1, std::format(".{}*", regex[pos-1]));
        pos += 2;
    }

    // replace ab to a.b
    // a(a|b) to a.(a|b)
    // (a|b)a to (a|b).a
    size_t prev = 0;
    size_t current = 1;
    while (prev != regex.length())
    {
        if ((std::isalpha(regex[prev]) && std::isalpha(regex[current])) ||
            (std::isalpha(regex[prev]) && (regex[current] == '(')) ||
            ((isOperator(regex[prev]) && regex[prev] != '.') && regex[current] == '(') ||
            ((regex[prev] == ')') && std::isalpha(regex[current])))
        {
            regex.replace(prev, 2, std::format("{}.{}", regex[prev], regex[current]));
            prev += 2;
            current += 2;
        }
            // a*b
            // a*.b
        else if ((regex[prev] == '*') && (std::isalpha(regex[current])))
        {
            regex.replace(prev, 2, std::format("{}.{}", regex[prev], regex[current]));
            prev += 2;
            current += 2;
        }
        else if ((std::isalpha(regex[prev - 1]) && (regex[prev] == '*') && (std::isalpha(regex[current]))))
        {
            regex.replace(prev, 2, std::format("{}.{}", regex[prev], regex[current]));
            prev += 2;
            current += 2;
        }
        else
        {
            ++prev;
            ++current;
        }
    }
}

bool isOperator(Token tkn)
{
    return tokenConstants::getOperations().contains(tkn);
}

[[nodiscard]] std::vector<std::string> toPostfix(const std::string& regex)
{
    std::stack<Token> stack;
    std::vector<std::string> result;

    for (const Token tkn : regex)
    {
        if (std::isalnum(tkn))
        {
            result.emplace_back(1, tkn);
        }
        else if (tkn == '(')
        {
            stack.push(tkn);
        }
        else if (tkn == ')')
        {
            while (!stack.empty() && stack.top() != '(')
            {
                result.emplace_back(1, stack.top());
                stack.pop();
            }
            stack.pop();
        }
        else if (isOperator(tkn))
        {
            while (!stack.empty() &&
                   tokenConstants::getOperations()[stack.top()] >= tokenConstants::getOperations()[tkn])
            {
                result.emplace_back(1, stack.top());
                stack.pop();
            }
            stack.push(tkn);
        }
    }

    while (!stack.empty())
    {
        result.emplace_back(1, stack.top());
        stack.pop();
    }
    return result;
}

[[nodiscard]] TokensSequence validateRegex(std::string&& regex)
{
    if (isValidRegex(regex))
    {
        preprocessRegex(regex);
        TokensSequence reversedPolskaNotation = toPostfix(regex);
        return reversedPolskaNotation;
    }
    return {};
}
}

namespace AutomationType
{
static boost::bimap<e, std::string_view> AUTO_DETERMINANT;

std::string_view getFileNameByType(e type)
{
    if (AUTO_DETERMINANT.empty())
    {
        AUTO_DETERMINANT.insert({e::NFA, File::nfa_file});
        AUTO_DETERMINANT.insert({e::DFA, File::dfa_file});
    }

    auto it = AUTO_DETERMINANT.left.find(type);
    if (it != AUTO_DETERMINANT.left.end())
    {
        return it->get_right();
    }
    return {};
}
}

namespace VizuType
{
static boost::bimap<FileType::e, std::pair<std::string_view, std::string_view>> DIR_DETERMINANT;

std::string_view getInfo(FileType::e type, InfoType::e infoType)
{
    if (DIR_DETERMINANT.empty())
    {
        DIR_DETERMINANT.insert({FileType::e::DOT, {Constants::Dir::dot, Constants::Ext::dot_ext}});
        DIR_DETERMINANT.insert({FileType::e::PNG, {Constants::Dir::png, Constants::Ext::png_ext}});
    }

    auto it = DIR_DETERMINANT.left.find(type);
    if (it != DIR_DETERMINANT.left.end())
    {
        return infoType == InfoType::DIR ? it->get_right().first : it->get_right().second;
    }
    return {};
}
}

namespace Transformation
{
NFAStateSet GetEpsilonClosure(NFAStateSet& states)
{
    NFAStateSet closure = states;
    MyStack<NFAStatePtr> stack;

    for (const auto& state: states)
    {
        stack.push(state);
    }

    while (!stack.empty())
    {
        NFAStatePtr state;
        if (stack.pop(state))
        {
            auto it = state->m_transitions.find(tokenConstants::EPSILON);
            if (it != state->m_transitions.end())
            {
                for (const auto& nextState : it->second)
                {
                    if (closure.find(nextState) == closure.end())
                    {
                        closure.insert(nextState);
                        stack.push(nextState);
                    }
                }
            }
        }
    }
    return closure;
}
}

}