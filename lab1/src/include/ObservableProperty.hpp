#pragma once
#include <boost/signals2.hpp>

template<typename T>
class ObservableProperty
{
public:
    using SignalType = boost::signals2::signal<void()>;

    template<typename U>
    requires std::convertible_to<U, T>
    constexpr explicit(!std::is_convertible_v<U, T>)
    ObservableProperty(U&& value)
        noexcept(std::is_nothrow_constructible_v<T, U>)
        requires std::is_trivially_constructible_v<T, U>
        : m_value(std::forward<U>(value)) {}

    template<typename Fn>
    requires std::is_invocable_v<Fn>
    void Connect(Fn&& fn)
    {
        m_connections.push_back(m_signal.connect(std::forward<Fn>(fn)));
    }

    template<typename U>
    requires std::convertible_to<U, T>
    void SetValue(U&& value) noexcept(std::is_nothrow_assignable_v<T&, U>)
    {
        if (m_value != value)
        {
            T oldValue = m_value;
            m_value = std::forward<U>(value);
            m_signal();
        }
    }

    template<typename U>
    requires std::convertible_to<U, T>
    ObservableProperty& operator=(U&& value) noexcept(std::is_nothrow_assignable_v<T&, U>)
    {
        SetValue(std::forward<U>(value));
        return *this;
    }

    constexpr const T& GetValue() const noexcept { return m_value; };

private:
    T m_value;
    SignalType m_signal;
    std::vector<boost::signals2::connection> m_connections;
};
