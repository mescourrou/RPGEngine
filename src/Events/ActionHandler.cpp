#include "ActionHandler.hpp"

namespace events
{
ActionHandler ActionHandler::instance;

KeyBinding ActionHandler::getKeyBinding(const std::string& name)
{
    auto it = std::find_if(instance.m_actions.begin(), instance.m_actions.end(),
                           [&name](const Action & a)
    {
        return a.name == name;
    });
    if (it != instance.m_actions.end())
        return it->keyBinding;
    return {};
}

void ActionHandler::setKeyBinding(const std::string& actionName,
                                  const KeyBinding& key)
{
    auto it = std::find_if(instance.m_actions.begin(), instance.m_actions.end(),
                           [&actionName](const Action & a)
    {
        return a.name == actionName;
    });
    if (it != instance.m_actions.end())
        it->keyBinding = key;
}

void ActionHandler::execute(const std::string& actionName)
{
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

std::list<std::string> ActionHandler::actionList()
{
    std::list<std::string> ret;
    for (const auto& a : instance.m_actions)
    {
        ret.push_back(a.name);
    }

    return ret;
}

#ifdef RPG_BUILD_GUI
void ActionHandler::addAction(std::string name, std::function<void ()> func,
                              const KeyBinding& keyBinding)
{
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

void ActionHandler::processSFMLEvent(const sf::Event::KeyEvent& event)
{
    auto it = std::find_if(instance.m_actions.begin(),
                           instance.m_actions.end(), [&event](const Action & a)
    {
        return a.keyBinding.isKey(event);
    });
    if (it != instance.m_actions.end())
        execute(*it);
}
#endif

void ActionHandler::execute(const ActionHandler::Action& action)
{
    for (const auto& a : action.functionList)
    {
        WorkerThread::newWork(a);
    }
}

} // namespace events
