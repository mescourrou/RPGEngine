#pragma once

#include <initializer_list>
#include <functional>
#include <map>

template <typename T>
class StateMachine
{
public:
    StateMachine(std::initializer_list<T> list);

    void addEntryStateAction(const T& state, const std::function<void(void)>& func);
    void addExitStateAction(const T& state, const std::function<void(void)>& func);

    void changeState(const T& newState);
    const T& state() const { return m_currentState; }

private:
    T m_currentState;
    using actionPair = std::pair<std::vector<std::function<void(void)>>, std::vector<std::function<void(void)>>>;
    std::map<T, actionPair> m_states;
};

template<typename T>
StateMachine<T>::StateMachine(std::initializer_list<T> list)
{
    bool firstStateInitialized = false;
    for (auto& state : list)
    {
        if (!firstStateInitialized)
        {
            m_currentState = state;
            firstStateInitialized = true;
        }
        m_states[state] = actionPair();
    }
}

template<typename T>
void StateMachine<T>::addEntryStateAction(const T &state, const std::function<void ()> &func)
{
    if (m_states.find(state) != m_states.end())
    {
        m_states.at(state).first.push_back(func);
    }
}

template<typename T>
void StateMachine<T>::addExitStateAction(const T &state, const std::function<void ()> &func)
{
    if (m_states.find(state) != m_states.end())
    {
        m_states.at(state).second.push_back(func);
    }
}

template<typename T>
void StateMachine<T>::changeState(const T &newState)
{
    if (newState != m_currentState && m_states.find(newState) != m_states.end())
    {
        for (auto& action : m_states.at(m_currentState).second)
        {
            action();
        }
        m_currentState = newState;
        for (auto& action : m_states.at(m_currentState).first)
        {
            action();
        }
    }

}
