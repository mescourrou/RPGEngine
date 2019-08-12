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

    namespace Game {
    constexpr char TABLE[] = "Game";
    constexpr char NAME[] = "name";
    constexpr char VERSION[] = "version";
    constexpr char ENGINE_VERSION[] = "engineVersion";
    constexpr char FK_USER_CHARACTER[] = "fkUserCharacter";
    constexpr char FIRST_MAP_NAME[] = "firstMapName";

    } // namespace Game

    namespace Character {
    constexpr char TABLE[] = "Character";
    constexpr char NAME[] = "name";

    } // namespace Character

    namespace NPC {
    constexpr char TABLE[] = "NPC";
    constexpr char NAME[] = "name";
    constexpr char TYPE[] = "type";
    enum Type {
        NPC,
        VENDOR
    };

    } // namespace NPC

    namespace NPCPath {
    constexpr char TABLE[] = "NPCPath";
    constexpr char FK_NPC_NAME[] = "fkNPCName";
    constexpr char X[] = "x";
    constexpr char Y[] = "y";
    constexpr char Z[] = "z";
    } // namespace NPCPath

    namespace Position {
    constexpr char TABLE[] = "Position";
    constexpr char FK_CHARACTER[] = "fkCharacterName";
    constexpr char X[] = "x";
    constexpr char Y[] = "y";
    constexpr char Z[] = "z";
    constexpr char FK_MAP[] = "fkMapName";
    } // namespace Position

    namespace Inventory {
    constexpr char TABLE[] = "Inventory";
    constexpr char FK_CHARACTER[] = "fkCharacterName";
    constexpr char MONEY[] = "money";

        namespace InventoryObjects {
        constexpr char TABLE[] = "InventoryObjects";
        constexpr char FK_CHARACTER[] = "fkCharacterName";
        constexpr char QUANTITY[] = "quantity";
        constexpr char FK_OBJECT[] = "fkObjectName";
        } // namespace InventoryObjects

    } // namespace Inventory

    namespace Object {
    constexpr char TABLE[] = "Object";
    constexpr char NAME[] = "name";
    constexpr char TYPE[] = "type";
    namespace ObjectType { // Allowed values in TYPE
    constexpr char OBJECT[] = "Object";
    constexpr char WEAPON[] = "Weapon";
    } // namespace ObjectType
    constexpr char VALUE[] = "value";

    } // namespace Object

    namespace Money {
    constexpr char TABLE[] = "Money";
    constexpr char NAME[] = "name";
    constexpr char VALUE[] = "value";
    } // namespace Money

} // namespace Model

} // namespace database
