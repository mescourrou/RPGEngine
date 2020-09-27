#pragma once

// Stl
#include <memory>
#include <set>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Event.hpp>
#include <BaseException.hpp>
#include <Config.hpp>
#include <Position.hpp>
#include <StateMachine.hpp>


#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

#ifdef RPG_BUILD_GUI
namespace maker::gui
{
class MakerGUI;
}
#endif


namespace databaseTools
{
class Database;
}

namespace config
{
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
    enum States
    {
        PROJECT_LOADING,
        WORKBENCH
    };
    Maker(int argc, char** argv);
    /// @brief Destructor
    ~Maker() override = default;

    bool initialize();
    bool run();

    bool doNewGame(const std::string& gameName, const std::string& directory);
    std::vector<std::string> gameList() const;
    bool doOpenGame(const std::string& gameName);
    void loadDatabase(const std::string& filename);

    bool createDatabaseModel();

    struct CharacterInformations
    {
        std::string name;
        enum Type : int
        {
            NPC,
            VENDOR,
            NPC_END
        };
        Type type;

        map::Position position;

        bool isNPC() const
        {
            return type >= NPC && type < NPC_END;
        }

    };
    bool saveCharacter(const CharacterInformations& infos);
    bool saveCharacter(const CharacterInformations& current,
                       const CharacterInformations& previous);
    std::vector<std::string> characterList() const;
    bool getCharacterInformations(const std::string& name,
                                  CharacterInformations& out);
    bool deleteCharacter(const std::string& name);


    struct MoneyInformations
    {
        std::vector<std::string> moneyList;
        std::vector<int> values;
        int baseMoney = 0;
    };
    bool saveMoney(const MoneyInformations& infos);
    bool getMoneyInformations(MoneyInformations& out);

    struct MapInformations
    {
        std::string name;
    };

    MapInformations getMapInformations(const std::string& name);
    std::set<std::string> getMapList();
    void setCurrentMap(const std::string& mapName);
    void saveMap(const MapInformations& current);
    void saveMap(const MapInformations& current, const MapInformations& previous);


    StateMachine<States> stateMachine{PROJECT_LOADING, WORKBENCH};


    ADD_EVENT(SignalCharacterListUpdated, std::vector<std::string>);
    ADD_EVENT(SignalMapUpdated, std::weak_ptr<map::Map>);

  private:
    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    void updateCharacterList();
    bool populateDirectory();

    std::shared_ptr<databaseTools::Database> m_db; ///< Database to use
    std::shared_ptr<config::Context> m_context;
    config::Config m_generalConfig;

    std::string m_name;
    std::string m_dbFile;
    bool m_running = true;

    std::vector<std::string> m_characterList;

    std::shared_ptr<map::Map> m_currentMap;

#ifdef RPG_BUILD_GUI
    std::shared_ptr<gui::MakerGUI> m_gui;
#endif
};

} // namespace maker

