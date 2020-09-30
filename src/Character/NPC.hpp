#pragma once

// Project
#include <Character.hpp>
#include <BaseObject.hpp>
#include <Dialogue.hpp>

namespace character
{

/**
 * @brief Class managing all Non Playable Character
 */
class NPC : public Character
{
    DECLARE_BASEOBJECT(NPC)
  public:
    NPC() = delete;
    NPC(const std::string& name, std::shared_ptr<config::Context> context);
    /// @brief Default constructor
    ~NPC() override = default;

    bool loadFromDatabase(std::shared_ptr<databaseTools::Database> db) override;

    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    static bool createDatabaseModel(std::shared_ptr<databaseTools::Database> db);

    virtual void updatePosition();

  private:
    static bool verifyNPCModel(std::shared_ptr<databaseTools::Database> db);
    static bool verifyNPCPathModel(std::shared_ptr<databaseTools::Database> db);
    std::list<map::Position> m_path; ///< List of points to follow
    map::Position* m_targettedPosition = nullptr; ///< Current point targetted

    std::vector<quest::Dialogue> m_dialogues;
};

} // namespace character
