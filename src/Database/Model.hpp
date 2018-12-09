#pragma once

namespace database {

namespace Model {

    namespace Character {
    constexpr char TABLE[] = "Character";
    constexpr char NAME[] = "name";


    } // namespace Character

    namespace Position {
    constexpr char TABLE[] = "Position";
    constexpr char FK_CHARACTER[] = "fkCharacterName";
    constexpr char X[] = "x";
    constexpr char Y[] = "y";
    constexpr char Z[] = "z";
    constexpr char FK_MAP[] = "fkMapName";
    } // namespace Position

} // namespace Model

} // namespace database
