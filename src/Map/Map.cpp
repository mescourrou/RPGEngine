#include "Map.hpp"

#include <glog/logging.h>
#include <VerbosityLevels.hpp>
/**
 * @brief Constructor of the map
 * @param name Name of the map. Must match the database
 */
map::Map::Map(const std::string &name) : m_name(name)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;

}

