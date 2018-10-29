#include "Object.hpp"



std::ostream &object::operator<<(std::ostream &stream, const Object& object)
{
    stream << "Object : " << object.name() << std::endl;
    stream << object.description() << std::endl;

    return stream;
}
