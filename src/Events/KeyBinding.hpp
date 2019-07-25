#pragma once

#include <string>
#include <general_config.hpp>

#ifdef RPG_BUILD_GUI
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#endif

namespace events {

class KeyBinding
{
public:
    enum LayerKey {
        NONE = 0,
        CTRL = 1 >> 0,
        ALT  = 1 >> 1,
        SHIFT= 1 >> 2
    };
    enum Key {
        A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
        F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,
        Tab,Space,Enter, Home, End, Comma, SemiColon, Escape,
        Left, Right, Up, Down,
        NOT_BINDED
    };

    KeyBinding() = default;
    KeyBinding(Key key, LayerKey layer = NONE);

    bool isKey(Key key, LayerKey layer) const { return key == m_key && layer == m_layer; }
    std::string toString() const;
#ifdef RPG_BUILD_GUI
    bool isKey(sf::Event::KeyEvent keyboard) const;
    static Key keyFromSFML(sf::Keyboard::Key key);
#endif

private:
    Key m_key = NOT_BINDED;
    LayerKey m_layer = NONE;
};

} // namespace events
