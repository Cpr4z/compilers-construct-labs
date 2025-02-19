#pragma once

#include <string>
#include <map>
#include <set>
#include <ranges>
#include <algorithm>
#include <vector>
#include <stack>
#include <iostream>
#include <format>

using OperatorPriority = size_t;
//using Token = unsigned char;
using Token = char;
using TokensSequence = std::vector<std::string>;

namespace utils
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
        if (ALPHABET.empty())
        {
            ALPHABET.emplace('a');
            ALPHABET.emplace('b');
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
    // a| b valid delete spaces
    // a    + valid delete spaces
    // a+++ not valid regex
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

    // a(a|b)
    // a.(a|b)

    //a+
    //a.*
    //a*.

    //(a|b)a
    //(a|b).a
    void preprocessRegex(std::string& regex)
    {
        //replace + to .*
        size_t pos = 0;
        while ((pos = regex.find('+', pos)) != std::string::npos)
        {
            regex.replace(pos, 1, ".*");
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
       //std::cout << regex <<std::endl;
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
                while (!stack.empty() /*&& isOperator(stack.top())*/ &&
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

    [[nodiscard]] TokensSequence validateRegex(std::string& regex)
    {
        if (isValidRegex(regex))
        {
            preprocessRegex(regex);
            TokensSequence reversedPolskaNotation = toPostfix(regex);

            for (const auto& tkn : reversedPolskaNotation)
            {
                std::cout << tkn;
            }

            return reversedPolskaNotation;
        }
        return {};
    }
}
}