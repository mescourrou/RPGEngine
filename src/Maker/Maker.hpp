#pragma once

// Stl
#include <memory>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Event.hpp>
#include <BaseException.hpp>
#include <Config.hpp>
#include <Position.hpp>

#ifdef RPG_BUILD_GUI
namespace maker::GUI {
class MakerGUI;
}
#endif

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace database {
class Database;
}

namespace config {
class Context;
class Config;
}

namespace maker
{
/**
 * @brief Manage the exceptions of Game
 */
CREATE_EXCEPTION_CLASS(Maker,
                       ADD_EXCEPTION_CODE(VERSION))

#ifdef RPG_BUILD_TEST
class MakerTest;
#endif

/**
 * @brief Class to manage the game maker
 */
class Maker : public BaseObject
{
    DECLARE_BASEOBJECT(Maker)
#ifdef RPG_BUILD_TEST
	friend class maker::MakerTest;
#endif
public:
    /**
     * @brief Database status
     */
    enum DatabaseStatus {
        NOT_LOADED, ///< Database not loaded
        EMPTY, ///< Database empty
        NOT_VALID, ///< Database not valid
        VALID ///< Valid database model
    };
    Maker(int argc, char **argv);
    /// @brief Destructor
    ~Maker() override = default;

    bool initialize();
    bool run();

    bool doNewGame(const std::string& gameName, const std::string& directory);
    std::vector<std::string> gameList() const;
    bool doOpenGame(const std::string& gameName);
    void loadDatabase(const std::string& filename);

    bool createDatabaseModel();

    struct States {
        bool unsaved = false;
        enum {
            NONE,
            DIRECTORY_SELECTED,
            CONFIG_LOADED,
            READY
        } progression = NONE;

    };
    const States& getStates() const { return m_states; }

    struct CharacterInformations {
        std::string name;
        enum Type : int {
            NPC,
            VENDOR,
            NPC_END
        } type;

        map::Position position;

        bool isNPC() const {
            return type >= NPC && type < NPC_END;
        }

    };
    bool saveCharacter(const CharacterInformations& infos);
    bool saveCharacter(const CharacterInformations& current, const CharacterInformations& previous);
    std::vector<std::string> characterList() const;
    bool getCharacterInformations(const std::string& name, CharacterInformations &out);
    bool deleteCharacter(const std::string& name);
    events::Event<std::vector<std::string>> signalCharacterListUpdated;

    struct MoneyInformations {
        std::vector<std::string> moneyList;
        std::vector<int> values;
        int baseMoney = 0;
    };
    bool saveMoney(const MoneyInformations& infos);
    bool getMoneyInformations(MoneyInformations& out);

    std::set<std::string> getMapList();
    void setCurrentMap(const std::string& mapName);
    events::Event<std::weak_ptr<map::Map>> signalMapUdated;

private:
    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);
    void updateCharacterList();
    bool populateDirectory();

    std::shared_ptr<database::Database> m_db; ///< Database to use
    std::shared_ptr<config::Context> m_context;
    config::Config m_generalConfig;

    States m_states;

    std::string m_name;
    std::string m_dbFile;
    bool m_running = true;

    std::vector<std::string> m_characterList;

    std::shared_ptr<map::Map> m_currentMap;

#ifdef RPG_BUILD_GUI
    std::shared_ptr<GUI::MakerGUI> m_gui;
#endif
};

} // namespace maker

