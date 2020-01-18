#pragma once

#include <initializer_list>
#include <functional>
#include <map>

/**
 * @brief List of states with in and out actions
 */
template <typename T>
class StateMachine
{
  public:
    StateMachine(std::initializer_list<T> list);

    void addEntryStateAction(const T& state, const std::function<void(void)>& func);
    void addExitStateAction(const T& state, const std::function<void(void)>& func);

    void changeState(const T& newState);
    /**
     * @brief Get the current state
     */
    const T& state() const
    {
        return m_currentState;
    }

  private:
    T m_currentState;                       ///< Current state
    using actionPair =
        std::pair<std::vector<std::function<void(void)>>, std::vector<std::function<void(void)>>>;
    std::map<T, actionPair> m_states;       ///< States
};

/**
 * @brief Initialize the states with a state list
 * @param list List of states
 */
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

/**
 * @brief Add an in action for the state
 * @param state State where to add an entry action
 * @param func Action
 */
template<typename T>
void StateMachine<T>::addEntryStateAction(const T& state,
        const std::function<void ()>& func)
{
    if (m_states.find(state) != m_states.end())
    {
        m_states.at(state).first.push_back(func);
    }
}

/**
 * @brief Add an out action for the state
 * @param state State where to add an exit action
 * @param func Action
 */
template<typename T>
void StateMachine<T>::addExitStateAction(const T& state,
        const std::function<void ()>& func)
{
    if (m_states.find(state) != m_states.end())
    {
        m_states.at(state).second.push_back(func);
    }
}

/**
 * @brief Change the state and execute the needed actions
 * @param newState State to set
 */
template<typename T>
void StateMachine<T>::changeState(const T& newState)
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
