#pragma once
#include <iostream>
#include <functional>

namespace Utils
{

template<typename Fn, typename ...Args>
bool invokeNoexcept(Fn&& func, Args&&... args) noexcept
{
    bool result = true;
    try
    {
        std::invoke(std::forward<Fn>(func), std::forward<Args>(args)...);
    }
    catch (...)
    {
        result = false;
    }
    return result;
}

template<typename Fn, typename ...Args>
void invokeWithFrozenConsoleStream(Fn&& func, Args&&... args)
{
    std::cout.setstate(std::ios_base::badbit);
    std::invoke(std::forward<Fn>(func), std::forward<Args>(args)...);
    std::cout.clear();

}

} // namespace utils
