#include "ActionHandler.hpp"

namespace events {
ActionHandler ActionHandler::instance;

KeyBinding ActionHandler::getKeyBinding(const std::string &name)
{
    auto it = std::find_if(instance.m_actions.begin(), instance.m_actions.end(),
                           [&name](const Action& a){return a.name == name;});
    if (it != instance.m_actions.end())
        return it->keyBinding;
    return {};
}

void ActionHandler::execute(const std::string &actionName)
{
    const auto& it = std::find_if(instance.m_actions.begin(), instance.m_actions.end(),
                                  [&actionName](const Action& a){return a.name == actionName;});
    if (it != instance.m_actions.end())
    {
        execute(*it);
    }
    else {
        throw ActionHandlerException("Action "+actionName+" unknown", ActionHandlerException::UNKNOWN_ACTION);
    }
}

#ifdef RPG_BUILD_GUI
void ActionHandler::addAction(const std::string &name, std::function<void ()> func, const KeyBinding &keyBinding)
{
    auto it = std::find_if(instance.m_actions.begin(), instance.m_actions.end(),
                           [&name](const Action& a){return a.name == name;});
    if (it == instance.m_actions.end())
        instance.m_actions.push_back(Action{name, keyBinding, std::vector<std::function<void(void)>>{func}});
    else
    {
        if (!keyBinding.isKey(KeyBinding::NOT_BINDED, KeyBinding::NONE))
            it->keyBinding = keyBinding;
        it->functionList.push_back(func);
    }
}

void ActionHandler::processSFMLEvent(const sf::Event &event)
{
    auto it = std::find_if(instance.m_actions.begin(), instance.m_actions.end(), [&event](const Action& a){
        return a.keyBinding.isKey(event.key);
    });
    if (it != instance.m_actions.end())
        execute(*it);
}
#endif

void ActionHandler::execute(const ActionHandler::Action &action)
{
    for (const auto& a : action.functionList)
    {
        WorkerThread::newWork(a);
    }
}

} // namespace events
