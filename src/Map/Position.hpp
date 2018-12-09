#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Map.hpp>
#include <Vector.hpp>
#include <Database.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace map
{

#ifdef RPG_BUILD_TEST
class PositionTest;
#endif

class Position : public BaseObject
{
public:
    class PositionException : BaseException
    {
    public:
        PositionException(const std::string& w, const Errors& code = BaseException::UNKNOWN) noexcept : BaseException(w, code) {}
    };
#ifdef RPG_BUILD_TEST
    friend class map::PositionTest;
#endif
public:
    Position() = default;
    Position(double x, double y, double z = 0);
    Position(std::shared_ptr<Map> map) : m_map(map) {}
    Position(std::shared_ptr<Map> map, double x, double y, double z = 0);
    ~Position() override = default;

    bool loadFromDatabase(std::shared_ptr<database::Database> db, const std::string& characterName);

    const std::shared_ptr<Map> map() const noexcept { return m_map; }
    void setMap(std::shared_ptr<Map> map) { m_map = map; }
    double& x() noexcept { return m_position.x(); }
    double& y() noexcept { return m_position.y(); }
    double& z() noexcept { return m_position.z(); }
    double x() const noexcept { return m_position.x(); }
    double y() const noexcept { return m_position.y(); }
    double z()  const noexcept{ return m_position.z(); }

    void move(const Vector<3>& move);

    double distanceTo(const Position& other) const;
    double operator-(const Position& other) { return distanceTo(other); }

    Position& operator+=(const Vector<3>& m) { move(m); return *this; }
    bool operator==(const Position& other);
    bool operator!=(const Position& other);

    std::string className() const noexcept override { return "Position"; }
private:
    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);
    std::shared_ptr<Map> m_map;
    Vector<3> m_position;
};

} // namespace map

