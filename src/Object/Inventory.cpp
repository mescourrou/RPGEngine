#include "Inventory.hpp"

/**
 * @brief Get a pointer on the wanted object, but keep it on the inventory
 * @param index Number of the object (start with 0)
 * @return Pointer on the object
 */
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

/**
 * @brief Get a pointer on the wanted object, but keep it on the inventory
 * @param objectName Name of the object wanted
 * @return Pointer on the object
 */
std::shared_ptr<object::Object> object::Inventory::get(const std::string &objectName) const
{
    auto objectIt = std::find_if(m_inventory.begin(), m_inventory.end(),
                                 [objectName](std::shared_ptr<Object> object) -> bool { if (object) return object->name() == objectName; return false; });
    if (objectIt != m_inventory.end())
        return *objectIt;
    return {};
}

/**
 * @brief Remove the object and return a pointer on it
 * @param index Number of the object
 * @return Pointer on the removed object
 */
std::shared_ptr<object::Object> object::Inventory::pop(unsigned int index)
{
    std::shared_ptr<object::Object> ret = get(index);
    if (ret)
    {
        m_inventory.remove(ret);
    }
    return ret;
}

/**
 * @brief Remove the object and return a pointer on it
 * @param objectName Name of the object
 * @return Pointer on the removed object
 */
std::shared_ptr<object::Object> object::Inventory::pop(const std::string &objectName)
{
    std::shared_ptr<object::Object> ret = get(objectName);
    if (ret)
    {
        m_inventory.remove(ret);
    }
    return ret;
}

/**
 * @brief Add a new object on the inventory
 * @param newObject Smart pointer of the new object
 */
void object::Inventory::push(const std::shared_ptr<object::Object>& newObject)
{
    if (newObject)
        m_inventory.emplace_back(newObject);
}

