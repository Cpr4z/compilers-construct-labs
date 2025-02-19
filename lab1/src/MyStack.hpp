#pragma once

#include <stack>

template<typename T>
class MyStack: public std::stack<T>
{
public:
    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        this->c.emplace_back(std::forward<Args>(args)...);
    }
};