#pragma once

// Project
#include <Character.hpp>
#include <BaseObject.hpp>

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
    NPC(std::string name, std::shared_ptr<config::Context> context);
    /// @brief Default constructor
    ~NPC() override = default;

    bool loadFromDatabase(std::shared_ptr<database::Database> db) override;

    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);
    static bool createDatabaseModel(std::shared_ptr<database::Database> db);

    virtual void updatePosition();
  protected:
    std::list<map::Position>
    m_path;                    ///< List of points to follow
    map::Position* m_targettedPosition = nullptr;       ///< Current point targetted

};

} // namespace character
