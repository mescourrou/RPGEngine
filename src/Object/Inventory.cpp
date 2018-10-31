#include "Inventory.hpp"


std::shared_ptr<object::Object> object::Inventory::get(unsigned int index) const
{
    if (index < m_inventory.size())
    {
        unsigned int i = 0;
        for (auto object : m_inventory)
        {
            if (i == index)
                return object;

            i++;
        }
    }
    return {};
}

std::shared_ptr<object::Object> object::Inventory::get(const std::string &objectName) const
{
    auto objectIt = std::find_if(m_inventory.begin(), m_inventory.end(),
                                 [objectName](std::shared_ptr<Object> object) -> bool { if (object) return object->name() == objectName; return false; });
    if (objectIt != m_inventory.end())
        return *objectIt;
    return {};
}

std::shared_ptr<object::Object> object::Inventory::pop(unsigned int index)
{
    std::shared_ptr<object::Object> ret = get(index);
    if (ret)
    {
        m_inventory.remove(ret);
    }
    return ret;
}

std::shared_ptr<object::Object> object::Inventory::pop(const std::string &objectName)
{
    std::shared_ptr<object::Object> ret = get(objectName);
    if (ret)
    {
        m_inventory.remove(ret);
    }
    return ret;
}

void object::Inventory::push(const std::shared_ptr<object::Object>& newObject)
{
    if (newObject)
        m_inventory.emplace_back(newObject);
}

