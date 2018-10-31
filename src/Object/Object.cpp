#include "Object.hpp"

object::Object::Object(std::string name) : m_name(std::move(name))
{

}


std::ostream &object::operator<<(std::ostream &stream, const Object& object)
{
    stream << "Object : " << object.name() << std::endl;
    stream << object.description() << std::endl;

    return stream;
}

