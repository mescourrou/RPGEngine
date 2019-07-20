#include "Maker.hpp"

#include <regex>
#include <fstream>

// General
#include <general_config.hpp>
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <ConfigFiles.hpp>
#include <Logger.hpp>

// Character
#include <Character.hpp>
#include <NPC.hpp>

// Object
#include <Object.hpp>
#include <Inventory.hpp>
#include <Money.hpp>
#include <Context.hpp>
#include <Game.hpp>

#ifdef RPG_BUILD_GUI
#include <MakerGUI.hpp>
#endif

#include <glog/logging.h>
#include <gflags/gflags.h>


DEFINE_int32(verbose, VERBOSE, "Verbosity level");

namespace maker {

/**
 * @brief Constructor
 */
Maker::Maker(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);
    google::AddLogSink(&Logger::getInstance());
    gflags::SetVersionString(std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "."
                             + std::to_string(VERSION_BUILD));
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    if (FLAGS_verbose > 0)
        LOG(INFO) << "Flag verbose : " << FLAGS_verbose;

    google::SetVLOGLevel("*", FLAGS_verbose);
    google::LogToStderr();
    LOG(INFO) << "Starting RPGMaker v" << static_cast<short>(VERSION_MAJOR) << "." <<
                 static_cast<short>(VERSION_MINOR) << "." << static_cast<short>(VERSION_BUILD);

    m_context = std::make_shared<config::Context>(argc, argv);
    std::string configPath = m_context->kConfigPath() + "/" + m_context->kGlobalConfigFilename();
    if (!m_generalConfig.loadFile(configPath))
    {
        if (!std::filesystem::exists(m_context->kConfigPath()))
            std::filesystem::create_directories(m_context->kConfigPath());
        if (!std::filesystem::exists(configPath))
        {
            LOG(WARNING) << "Create a new global config file here : " << configPath;
            std::ofstream configFile(configPath);
            configFile.close();
        }
        m_generalConfig.loadFile(configPath);
    }


}

bool Maker::initialize()
{
#ifdef RPG_BUILD_GUI
    m_gui = std::make_shared<GUI::MakerGUI>(m_context, this);
    if (!m_gui->initialize())
    {
        m_running = false;
        LOG(ERROR) << "Failed to initialize Maker GUI";
        return false;
    }
#endif
    return true;
}

bool Maker::run()
{
    using namespace std::chrono_literals;

    // Framerate control
    auto clock = std::chrono::high_resolution_clock::now();
    auto period = std::chrono::duration(40ms);

    while(m_running)
    {
#ifdef RPG_BUILD_GUI
        m_gui->eventManager();
#endif

#ifdef RPG_BUILD_GUI
        m_gui->draw();
#endif
        std::this_thread::sleep_until(clock+period);
        clock = std::chrono::high_resolution_clock::now();
    }
    return false;
}

bool Maker::doNewGame(const std::string &gameName, const std::string &directory)
{
    m_states.progression = States::DIRECTORY_SELECTED;
    m_states.unsaved = false;
    m_context->gameLocation() = directory;
    m_name = gameName;

    if (!populateDirectory())
        return false;

    namespace gameListFile = config::structure::gameListFile;
    m_generalConfig.setValue(m_name, gameListFile::DIRECTORY_KEY, directory);
    m_generalConfig.saveToFile();

    namespace gameGlobalFile = config::structure::globalFile;
    std::ofstream file(directory + "/" + gameGlobalFile::FILE_NAME);
    file.close();

    if (!m_context->config()->loadFile(directory + "/" + gameGlobalFile::FILE_NAME))
    {
        LOG(ERROR) << "Impossible to create the main game file";
        return false;
    }
    m_context->config()->setValue(gameGlobalFile::ressources::SECTION, gameGlobalFile::ressources::DATABASE, m_dbFile);
    m_context->config()->saveToFile();
    m_states.progression = States::CONFIG_LOADED;


    updateCharacterList();


    LOG(INFO) << "Game created";
    m_states.progression = States::READY;


    return true;
}

std::vector<std::string> Maker::gameList() const
{
    return m_generalConfig.getAllSections();
}

bool Maker::doOpenGame(const std::string &gameName)
{
    m_states.unsaved = false;
    LOG(INFO) << "Open " << gameName;
    m_name = gameName;
    m_context->gameLocation() = m_generalConfig.getValue(gameName, config::structure::gameListFile::DIRECTORY_KEY);

    m_states.progression = States::DIRECTORY_SELECTED;
    if (!m_context->config()->loadFile(m_context->gameLocation() + "/" + config::structure::globalFile::FILE_NAME))
    {
        LOG(ERROR) << "Impossible to load the game config file : " << m_context->gameLocation() + "/" + config::structure::globalFile::FILE_NAME;
        return false;
    }
    m_states.progression = States::CONFIG_LOADED;

    namespace globalFile = config::structure::globalFile;
    m_dbFile = m_context->config()->getValue(globalFile::ressources::SECTION, globalFile::ressources::DATABASE);
    if (m_dbFile.empty())
    {
        LOG(ERROR) << "No database file specified in the game config file";
        return false;
    }

    m_db = std::make_shared<database::Database>(m_context->gameLocation() + "/" + m_dbFile);
    if (!m_db)
    {
        LOG(ERROR) << "Impossible to load the database";
        return false;
    }

    updateCharacterList();
    m_states.progression = States::READY;


    return true;
}

/**
 * @brief Load the Maker from the database
 * @param filename Name of the database
 */
void Maker::loadDatabase(const std::string &filename)
{
    m_db = std::make_shared<database::Database>(filename);
}

/**
 * @brief Verify the model of the database
 * @param db Database to verify
 * @return Return true if the database is correct
 */
bool Maker::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    return
            character::Character::verifyDatabaseModel(db) &&
            character::NPC::verifyDatabaseModel(db) &&
            object::Object::verifyDatabaseModel(db) &&
            object::Money::verifyDatabaseModel(db) &&
            object::Inventory::verifyDatabaseModel(db) &&
            map::Position::verifyDatabaseModel(db) &&
            game::Game::verifyDatabaseModel(db);
}

void Maker::updateCharacterList()
{
    m_characterList.clear();
    using namespace database;
    auto result = m_db->query(Query::createQuery<Query::SELECT>(Model::Character::TABLE, m_db)
                              .column(Model::Character::NAME)
                              .sort(Model::Character::NAME));
    if (!Database::isQuerySuccessfull(result) || result.size() <= 1)
        return;

    for (unsigned int i = 1; i < result.size(); i++)
    {
        m_characterList.push_back(result.at(i).at(Model::Character::NAME));
    }
    signalCharacterListUpdated.trigger(m_characterList);
}

bool Maker::populateDirectory()
{
    if (m_states.progression < States::DIRECTORY_SELECTED)
        return false;

    if (!std::filesystem::exists(m_context->kCharacterPath()))
        std::filesystem::create_directories(m_context->kCharacterPath());
    if (!std::filesystem::exists(m_context->kMapPath()))
        std::filesystem::create_directories(m_context->kMapPath());
    if (!std::filesystem::exists(m_context->kConfigPath()))
        std::filesystem::create_directories(m_context->kConfigPath());

    m_dbFile = m_name + ".db";
    if (std::filesystem::exists(m_context->gameLocation() + "/" + m_dbFile))
        std::filesystem::remove(m_context->gameLocation() + "/" + m_dbFile);
    loadDatabase(m_context->gameLocation() + "/" + m_dbFile);
    if (!createDatabaseModel())
    {
        LOG(ERROR) << "Impossible to create the database model";
        return false;
    }

    using namespace database;
    m_db->query(Query::createQuery<Query::INSERT>(Model::Game::TABLE, m_db)
                .value(Model::Game::NAME, m_name)
                .value(Model::Game::VERSION, "1")
                .value(Model::Game::ENGINE_VERSION, std::to_string(VERSION)));

    return true;
}

/**
 * @brief Create the tables needed in the database loaded
 * @return Return true if all went well
 */
bool Maker::createDatabaseModel()
{
    if (!m_db)
        throw MakerException("No database loaded.", database::DatabaseException::MISSING_DATABASE);
    return
            character::Character::createDatabaseModel(m_db) &&
            character::NPC::createDatabaseModel(m_db) &&
            map::Position::createDatabaseModel(m_db) &&
            object::Object::createDatabaseModel(m_db) &&
            object::Money::createDatabaseModel(m_db) &&
            object::Inventory::createDatabaseModel(m_db) &&
            game::Game::createDatabaseModel(m_db);
}

bool Maker::saveCharacter(const Maker::CharacterInformations &infos)
{
    using namespace database;
    m_db->query(Query::createQuery<Query::INSERT>(Model::Character::TABLE, m_db)
                .value(Model::Character::NAME, infos.name));

    events::WorkerThread::newWork(this, &Maker::updateCharacterList);
    if (infos.type == CharacterInformations::NPC || infos.type == CharacterInformations::VENDOR)
    {
        Model::NPC::Type npcType;
        switch (infos.type)
        {
        case CharacterInformations::NPC:
            npcType = Model::NPC::NPC;
            break;
        case CharacterInformations::VENDOR:
            npcType = Model::NPC::VENDOR;
            break;
        default:
            npcType = Model::NPC::NPC;
        }
        m_db->query(Query::createQuery<Query::INSERT>(Model::NPC::TABLE, m_db)
                    .value(Model::NPC::NAME, infos.name)
                    .value(Model::NPC::TYPE, std::to_string(npcType)));
    }

    m_db->query(Query::createQuery<Query::INSERT>(Model::Position::TABLE, m_db)
                .value(Model::Position::FK_CHARACTER, infos.name)
                .value(Model::Position::X, std::to_string(infos.position.x()))
                .value(Model::Position::Y, std::to_string(infos.position.y()))
                .value(Model::Position::Z, std::to_string(infos.position.z())));



    return true;
}

bool Maker::saveCharacter(const Maker::CharacterInformations &current, const Maker::CharacterInformations &previous)
{
    using namespace database;
    if (current.name != previous.name)
    {
        m_db->query(Query::createQuery<Query::UPDATE>(Model::Character::TABLE, m_db)
                    .where(Model::Character::NAME, Query::EQUAL, previous.name)
                    .set(Model::Character::NAME, current.name));
        if (previous.isNPC())
        {
            m_db->query(Query::createQuery<Query::UPDATE>(Model::NPC::TABLE, m_db)
                        .where(Model::NPC::NAME, Query::EQUAL, previous.name)
                        .set(Model::NPC::NAME, current.name));
        }
        events::WorkerThread::newWork(this, &Maker::updateCharacterList);
    }
    if (current.type != previous.type)
    {
        if (current.isNPC() && previous.isNPC())
        {
            m_db->query(Query::createQuery<Query::UPDATE>(Model::NPC::TABLE, m_db)
                        .where(Model::NPC::NAME, Query::EQUAL, current.name)
                        .set(Model::NPC::TYPE, std::to_string(current.type)));
        }
        else if (!current.isNPC() && previous.isNPC()) // No NPC anymore
        {
            m_db->query(Query::createQuery<Query::DELETE>(Model::NPC::TABLE, m_db)
                        .where(Model::NPC::NAME, Query::EQUAL, current.name));
        }
        else if (current.isNPC() && !previous.isNPC())
        {
            Model::NPC::Type npcType;
            switch (current.type)
            {
            case CharacterInformations::NPC:
                npcType = Model::NPC::NPC;
                break;
            case CharacterInformations::VENDOR:
                npcType = Model::NPC::VENDOR;
                break;
            default:
                npcType = Model::NPC::NPC;
            }
            m_db->query(Query::createQuery<Query::INSERT>(Model::NPC::TABLE, m_db)
                        .value(Model::NPC::NAME, current.name)
                        .value(Model::NPC::TYPE, std::to_string(npcType)));

        }
    }
    if (previous.position != current.position)
    {
        m_db->query(Query::createQuery<Query::UPDATE>(Model::Position::TABLE, m_db)
                    .where(Model::Position::FK_CHARACTER, Query::EQUAL, current.name)
                    .set(Model::Position::X, std::to_string(current.position.x()))
                    .set(Model::Position::Y, std::to_string(current.position.y()))
                    .set(Model::Position::Z, std::to_string(current.position.z())));
    }

    return true;
}

std::vector<std::string> Maker::characterList() const
{
    return m_characterList;
}

bool Maker::getCharacterInformations(const std::string &name, Maker::CharacterInformations& out)
{
    CharacterInformations ret;
    using namespace database;
    auto result = m_db->query(Query::createQuery<Query::SELECT>(Model::Character::TABLE, m_db)
                              .where(Model::Character::NAME, Query::EQUAL, name));
    if (!Database::isQuerySuccessfull(result))
        return false;
    if (result.size() <= 1) // No result
        return false;

    out.name = name;

    result = m_db->query(Query::createQuery<Query::SELECT>(Model::NPC::TABLE, m_db)
                         .where(Model::NPC::NAME, Query::EQUAL, name)
                         .column(Model::NPC::TYPE));
    if (result.size() > 1)
    {
        Model::NPC::Type type = (Model::NPC::Type)std::stoi(result.at(1).at(Model::NPC::TYPE));
        switch (type) {
        case Model::NPC::VENDOR:
            out.type = CharacterInformations::VENDOR;
            break;
        default:
            out.type = CharacterInformations::NPC;
            break;
        }
    }

    result = m_db->query(Query::createQuery<Query::SELECT>(Model::Position::TABLE, m_db)
                         .where(Model::Position::FK_CHARACTER, Query::EQUAL, name));
    if (!Database::isQuerySuccessfull(result))
        return false;
    if (result.size() <= 1)
        return false;
    out.position = map::Position(std::atof(result.at(1).at(Model::Position::X).c_str()),
                                 std::atof(result.at(1).at(Model::Position::Y).c_str()),
                                 std::atof(result.at(1).at(Model::Position::Z).c_str()));
    LOG(INFO) << "Get information about " << name << " : OK";

    return true;
}

bool Maker::deleteCharacter(const std::string &name)
{
    using namespace database;
    m_db->query(Query::createQuery<Query::DELETE>(Model::NPC::TABLE, m_db)
                .where(Model::NPC::NAME, Query::EQUAL, name));
    m_db->query(Query::createQuery<Query::DELETE>(Model::Position::TABLE, m_db)
                .where(Model::Position::FK_CHARACTER, Query::EQUAL, name));
    if (Database::isQuerySuccessfull(m_db->query(Query::createQuery<Query::DELETE>(Model::Character::TABLE, m_db)
                                                 .where(Model::Character::NAME, Query::EQUAL, name))))
    {
        events::WorkerThread::newWork(this, &Maker::updateCharacterList);
        return true;
    }
    return false;

}

}

