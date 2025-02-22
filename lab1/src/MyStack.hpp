#pragma once

#include <stack>

template<typename T>
class MyStack: public std::stack<T>
{
public:
    bool pop(T& elem)
    {
        if (std::stack<T>::empty())
        {
            return false;
        }
        elem = std::stack<T>::top();
        std::stack<T>::pop();
        return true;
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        this->c.emplace_back(std::forward<Args>(args)...);
    }
};