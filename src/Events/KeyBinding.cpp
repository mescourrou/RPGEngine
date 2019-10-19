#include "KeyBinding.hpp"
#include <map>

namespace events
{

#ifdef RPG_BUILD_GUI
/**
 * @brief Create a keybinding
 * @param key Keyboard key
 * @param layer Layer key (ctrl, shift, alt)
 */
KeyBinding::KeyBinding(KeyBinding::Key key, KeyBinding::LayerKey layer):
    m_key(key), m_layer(layer)
{

}

/**
 * @brief Get the string description of the keybinding
 */
std::string KeyBinding::toString() const
{
    static std::map<Key, std::string> map;
    if (map.empty())
    {
        map[A] = "A";
        map[B] = "B";
        map[C] = "C";
        map[D] = "D";
        map[E] = "E";
        map[F] = "F";
        map[G] = "G";
        map[H] = "H";
        map[I] = "I";
        map[J] = "J";
        map[K] = "K";
        map[L] = "L";
        map[M] = "M";
        map[N] = "N";
        map[O] = "O";
        map[P] = "P";
        map[Q] = "Q";
        map[R] = "R";
        map[S] = "S";
        map[T] = "T";
        map[U] = "U";
        map[V] = "V";
        map[W] = "W";
        map[X] = "X";
        map[Y] = "Y";
        map[Z] = "Z";
        map[Num0] = "0";
        map[Num1] = "1";
        map[Num2] = "2";
        map[Num3] = "3";
        map[Num4] = "4";
        map[Num5] = "5";
        map[Num6] = "6";
        map[Num7] = "7";
        map[Num8] = "8";
        map[Num9] = "9";
        map[F1] = "F1";
        map[F2] = "F2";
        map[F3] = "F3";
        map[F4] = "F4";
        map[F5] = "F5";
        map[F6] = "F6";
        map[F7] = "F7";
        map[F8] = "F8";
        map[F9] = "F9";
        map[F10] = "F10";
        map[F11] = "F11";
        map[F12] = "F12";
        map[Tab] = "Tab";
        map[Space] = "Space";
        map[Enter] = "Enter";
        map[Home] = "Home";
        map[End] = "End";
        map[Comma] = "Comma";
        map[SemiColon] = "Semi-Colon";
        map[Escape] = "Escape";
        map[Left] = "Left";
        map[Right] = "Right";
        map[Up] = "Up";
        map[Down] = "Down";
        map[NOT_BINDED] = "None";
    }

    std::string ret;
    if (m_layer & CTRL)
        ret += "Ctrl+";
    if (m_layer & ALT)
        ret += "Alt+";
    if (m_layer & SHIFT)
        ret += "Shift+";
    ret += map[m_key];
    return ret;
}

/**
 * @brief Tells if the SFML KeyEvent match the keybinding
 * @param keyboard SFML KeyEvent
 */
bool KeyBinding::isKey(sf::Event::KeyEvent keyboard) const
{
    if (m_key == NOT_BINDED)
        return false;
    KeyBinding::LayerKey layer = 0;
    if (keyboard.alt)
        layer |= KeyBinding::ALT;
    if (keyboard.shift)
        layer |= KeyBinding::SHIFT;
    if (keyboard.control)
        layer |= KeyBinding::CTRL;
    if (m_layer == layer)
    {
        if (m_key == keyFromSFML(keyboard.code))
            return true;
    }
    return false;
}

/**
 * @brief Get the keybinding from the SFML KeyEvent
 * @param event SFML KeyEvent to load from
 */
KeyBinding KeyBinding::fromSFML(const sf::Event::KeyEvent& event)
{
    short layer = 0;
    if (event.alt)
        layer |= ALT;
    if (event.shift)
        layer |= SHIFT;
    if (event.control)
        layer |= CTRL;
    Key key = keyFromSFML(event.code);
    if (key == NOT_BINDED && layer != 0)
        return {};
    return KeyBinding(key, layer);

}

/**
 * @brief Map of the match between SFML and KeyBinding::Key
 */
KeyBinding::Key KeyBinding::keyFromSFML(sf::Keyboard::Key key)
{
    static std::map<sf::Keyboard::Key, KeyBinding::Key> map;
    if (map.empty())
    {
        map[sf::Keyboard::A] = A;
        map[sf::Keyboard::B] = B;
        map[sf::Keyboard::C] = C;
        map[sf::Keyboard::D] = D;
        map[sf::Keyboard::E] = E;
        map[sf::Keyboard::F] = F;
        map[sf::Keyboard::G] = G;
        map[sf::Keyboard::H] = H;
        map[sf::Keyboard::I] = I;
        map[sf::Keyboard::J] = J;
        map[sf::Keyboard::K] = K;
        map[sf::Keyboard::L] = L;
        map[sf::Keyboard::M] = M;
        map[sf::Keyboard::N] = N;
        map[sf::Keyboard::O] = O;
        map[sf::Keyboard::P] = P;
        map[sf::Keyboard::Q] = Q;
        map[sf::Keyboard::R] = R;
        map[sf::Keyboard::S] = S;
        map[sf::Keyboard::T] = T;
        map[sf::Keyboard::U] = U;
        map[sf::Keyboard::V] = V;
        map[sf::Keyboard::W] = W;
        map[sf::Keyboard::X] = X;
        map[sf::Keyboard::Y] = Y;
        map[sf::Keyboard::Z] = Z;
        map[sf::Keyboard::Num0] = Num0;
        map[sf::Keyboard::Num1] = Num1;
        map[sf::Keyboard::Num2] = Num2;
        map[sf::Keyboard::Num3] = Num3;
        map[sf::Keyboard::Num4] = Num4;
        map[sf::Keyboard::Num5] = Num5;
        map[sf::Keyboard::Num6] = Num6;
        map[sf::Keyboard::Num7] = Num7;
        map[sf::Keyboard::Num8] = Num8;
        map[sf::Keyboard::Num9] = Num9;
        map[sf::Keyboard::F1] = F1;
        map[sf::Keyboard::F2] = F2;
        map[sf::Keyboard::F3] = F3;
        map[sf::Keyboard::F4] = F4;
        map[sf::Keyboard::F5] = F5;
        map[sf::Keyboard::F6] = F6;
        map[sf::Keyboard::F7] = F7;
        map[sf::Keyboard::F8] = F8;
        map[sf::Keyboard::F9] = F9;
        map[sf::Keyboard::F10] = F10;
        map[sf::Keyboard::F11] = F11;
        map[sf::Keyboard::F12] = F12;
        map[sf::Keyboard::Tab] = Tab;
        map[sf::Keyboard::Space] = Space;
        map[sf::Keyboard::Enter] = Enter;
        map[sf::Keyboard::Home] = Home;
        map[sf::Keyboard::End] = End;
        map[sf::Keyboard::Comma] = Comma;
        map[sf::Keyboard::SemiColon] = SemiColon;
        map[sf::Keyboard::Escape] = Escape;
        map[sf::Keyboard::Left] = Left;
        map[sf::Keyboard::Right] = Right;
        map[sf::Keyboard::Up] = Up;
        map[sf::Keyboard::Down] = Down;
    }
    if (map.find(key) == map.end())
        return Key::NOT_BINDED;
    return map[key];
}
#endif

} // namespace events
