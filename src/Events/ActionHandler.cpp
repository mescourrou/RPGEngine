#include "ActionHandler.hpp"
#include <InstrumentationTimer.hpp>

namespace events
{
ActionHandler ActionHandler::instance;

/**
 * @brief Get the keybinding of the named action
 * @param name Name of the action
 * @return KeyBinding
 */
KeyBinding ActionHandler::getKeyBinding(const std::string& name)
{
    PROFILE_FUNCTION();
    auto it = std::find_if(instance.m_actions.begin(), instance.m_actions.end(),
                           [&name](const Action & a)
    {
        return a.name == name;
    });
    if (it != instance.m_actions.end())
        return it->keyBinding;
    return {};
}

/**
 * @brief Set the KeyBinding of the given action
 * @param actionName Name of the action
 * @param key KeyBinding to apply
 */
void ActionHandler::setKeyBinding(const std::string& actionName,
                                  const KeyBinding& key)
{
    PROFILE_FUNCTION();
    auto it = std::find_if(instance.m_actions.begin(), instance.m_actions.end(),
                           [&actionName](const Action & a)
    {
        return a.name == actionName;
    });
    if (it != instance.m_actions.end())
        it->keyBinding = key;
}

/**
 * @brief Execute the given action
 * @param actionName Name of the action to execute
 */
void ActionHandler::execute(const std::string& actionName)
{
    PROFILE_FUNCTION();
    const auto& it = std::find_if(instance.m_actions.begin(),
                                  instance.m_actions.end(),
                                  [&actionName](const Action & a)
    {
        return a.name == actionName;
    });
    if (it != instance.m_actions.end())
    {
        execute(*it);
    }
    else
    {
        throw ActionHandlerException("Action " + actionName + " unknown",
                                     ActionHandlerException::UNKNOWN_ACTION);
    }
}

/**
 * @brief Get the list of actions
 */
std::list<std::string> ActionHandler::actionList()
{
    PROFILE_FUNCTION();
    std::list<std::string> ret;
    for (const auto& a : instance.m_actions)
    {
        ret.push_back(a.name);
    }

    return ret;
}

#ifdef RPG_BUILD_GUI
/**
 * @brief Add an action
 * @param name Name of the action
 * @param func Function to execute when the action is triggered
 * @param keyBinding KeyBinding of the action
 */
void ActionHandler::addAction(const std::string& name,
                              const std::function<void ()>& func,
                              const KeyBinding& keyBinding)
{
    PROFILE_FUNCTION();
    auto it = std::find_if(instance.m_actions.begin(), instance.m_actions.end(),
                           [&name](const Action & a)
    {
        return a.name == name;
    });
    if (it == instance.m_actions.end())
        instance.m_actions.push_back(Action{name, keyBinding, std::vector<std::function<void(void)>>{func}});
    else
    {
        if (!keyBinding.isKey(KeyBinding::NOT_BINDED, KeyBinding::NONE))
            it->keyBinding = keyBinding;
        it->functionList.push_back(func);
    }
}

/**
 * @brief Trigger the actions corresponding to the SFML event given
 * @param event Event to process
 */
void ActionHandler::processSFMLEvent(const sf::Event::KeyEvent& event)
{
    PROFILE_FUNCTION();
    auto it = std::find_if(instance.m_actions.begin(),
                           instance.m_actions.end(), [&event](const Action & a)
    {
        return a.keyBinding.isKey(event);
    });
    if (it != instance.m_actions.end())
        execute(*it);
}
#endif

/**
 * @brief Execute the action
 * @param action Action to execute
 */
void ActionHandler::execute(const ActionHandler::Action& action)
{
    PROFILE_FUNCTION();
    for (const auto& a : action.functionList)
    {
        WorkerThread::newWork(a);
    }
}

} // namespace events