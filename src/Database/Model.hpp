#pragma once

namespace database {

/**
 * @namespace Model
 * @brief Database model
 *
 * Contains at least the table name for each class which need a database entry.
 *
 * Then contains the name of the columns.
 *
 * The convention is to capitalize the table name and not the fields name.<br>
 * And put 'fk' in front of foreign keys
 */
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
