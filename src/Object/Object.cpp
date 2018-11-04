#include "Object.hpp"

/**
 * @brief Construct an object
 * @param name Name of the object
 */
object::Object::Object(std::string name) : m_name(std::move(name))
{

}


/**
 * @brief Print the object into the stream
 * @param stream Stream into write on
 * @param object Object to print
 * @return Modified stream
 */
std::ostream &object::operator<<(std::ostream &stream, const Object& object)
{
    stream << "Object : " << object.name() << std::endl;
    stream << object.description() << std::endl;

    return stream;
}

