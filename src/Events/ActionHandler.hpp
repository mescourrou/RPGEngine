#pragma once

#include <general_config.hpp>
#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include <BaseException.hpp>

#include <KeyBinding.hpp>
#include <WorkerThread.hpp>

#ifdef RPG_BUILD_GUI
#include <SFML/Window/Event.hpp>
#endif

namespace events
{

CREATE_EXCEPTION_CLASS(ActionHandler,
                       ADD_EXCEPTION_CODE(UNKNOWN_ACTION))

class ActionHandler
{
    struct Action
    {
        std::string name;
        KeyBinding keyBinding;
        std::vector<std::function<void(void)>> functionList;
    };

  public:
    static void addAction(std::string name, std::function<void(void)> func,
                          const KeyBinding& keyBinding = {});

    static KeyBinding getKeyBinding(const std::string& name);
    static void setKeyBinding(const std::string& actionName, const KeyBinding& key);

    static void execute(const std::string& actionName);
    static std::list<std::string> actionList();

#ifdef RPG_BUILD_GUI
    static void processSFMLEvent(const sf::Event::KeyEvent& event);
#endif

  protected:
    static ActionHandler instance;
    ActionHandler() = default;

    static void execute(const Action& action);

    std::list<Action> m_actions;
};

} // namespace events
