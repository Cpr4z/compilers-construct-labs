#include "NFA.hpp"

#include <map>
#include <vector>
#include <queue>
#include <ranges>

struct State
{
    explicit State(StateId id, bool isFinal) : m_id(id), m_isFinal(isFinal) {}

    bool operator<(const State& other) const
    {
        return m_id < other.m_id;
    }

    StateId m_id = 0;
    bool m_isFinal = false;
    std::map<Token, std::vector<StatePtr>> m_transitions;
};

struct StateComparator
{
    bool operator()(const StatePtr& lhs, const StatePtr& rhs) const
    {
        return lhs->m_id < rhs->m_id;
    }
};

class NFAImpl
{
public:
    NFAImpl() = default;
    ~NFAImpl() = default;

    void init(const StatePtr& start, const StatePtr& accept);
    bool imitate(const std::string& regex);
    void addTransition(StatePtr& first, const StatePtr& second, Token tkn);
    StatePtr createState(StateId id, bool isFinal);
    std::set<State> getStates() const;

    StatePtr getStart() const { return m_start; }
    StatePtr getAccept() const { return m_accept; }

public:

//    struct State
//    {
//        friend class NFAImpl;
//        StateId m_id = 0;
//        bool m_isFinal = false;
//        std::map<StateId, std::map<Token, std::vector<State>>> m_transitions;
//    };

    StatePtr m_start;
    StatePtr m_accept;
};

void NFAImpl::init(const StatePtr& start, const StatePtr& accept)
{
    m_start = start;
    m_accept = accept;
}

bool NFAImpl::imitate(const std::string& regex)
{
    // to do
}

void NFAImpl::addTransition(StatePtr& first, const StatePtr& second, Token tkn)
{
    first->m_transitions[tkn].push_back(second);
}

StatePtr NFAImpl::createState(StateId id, bool isFinal)
{
    return std::make_shared<State>(id, isFinal);
}

std::set<State> NFAImpl::getStates() const
{
    std::queue<StatePtr> queue;
    std::map<StateId, StatePtr> visited;

    queue.push(getStart());
    visited.emplace(getStart()->m_id, getStart());

    while (!queue.empty())
    {
        StatePtr current = queue.front();
        queue.pop();
        for (const auto& [symbol, nextStates] : current->m_transitions)
        {
            for (const auto& nextState: nextStates)
            {
                if (visited.find(nextState->m_id) != visited.end())
                {
                    visited.emplace(nextState->m_id, nextState);
                    queue.push(nextState);
                }
            }
        }
    }

    // to do replace to c++20 ranges
    //std::set<State> states(visited | std::views::values);
    std::set<State> states;
    for (const auto& [key, value]: visited)
    {
        states.insert(*value);
    }

    return states;
}
//////////////////////////////////////////////////////////////////////////////

NFA::NFA(): m_impl(std::make_unique<NFAImpl>())
{
}

NFA::~NFA()
{
}

NFAPtr NFA::Instance()
{
    return std::make_shared<NFA>();
}

bool NFA::Imitate(const std::string& regex)
{
    return m_impl->imitate(regex);
}

void NFA::Init(const StatePtr& start, const StatePtr& accept)
{
    m_impl->init(start, accept);
}

void NFA::AddTransition(StatePtr& first, const StatePtr& second, Token tkn)
{
    m_impl->addTransition(first, second, tkn);
}

StatePtr NFA::CreateState(StateId id, bool isFinal)
{
    return m_impl->createState(id, isFinal);
}

StatePtr NFA::GetStart() const
{
    return m_impl->getStart();
}

StatePtr NFA::GetAccept() const
{
    return m_impl->getAccept();
}
