#include "BaseObject.hpp"

/**
 * \brief Constructor of the BaseObject
 * \param parent Optionnal parent of the object
 */
BaseObject::BaseObject(BaseObject* parent) noexcept : m_parent(parent)
{
    if (m_parent)
        m_parent->addChild(this);
}

/**
 * @brief Destructor of Base object
 * Break the parent-child chain but doesn't delete children
 */
BaseObject::~BaseObject() noexcept
{
    if (m_parent)
        m_parent->removeChild(this);
    for (auto it = m_children.begin(); it != m_children.end(); )
    {
        (*it)->setParent(nullptr);
    }
}

/** @fn BaseObject *BaseObject::parent() const noexcept
 * @brief Get parent
 */
BaseObject *BaseObject::parent() const noexcept
{
    return m_parent;
}

/**
 * @brief Set the parent
 * @param parent New parent (can be nullptr)
 */
void BaseObject::setParent(BaseObject *parent) noexcept
{
    if (m_parent)
        m_parent->removeChild(this);
    m_parent = parent;
    if (m_parent)
        m_parent->addChild(this);
}

/**
 * @brief Add a child to the list
 * @param child New child. Unusefull with nullptr
 */
void BaseObject::addChild(BaseObject *child) noexcept
{
    if (child == nullptr)
        return;
    if (std::count(m_children.begin(), m_children.end(), child) == 0)
    {
        m_children.push_back(child);
    }
}

/**
 * @brief Remove the given child of the list
 * @param child To remove. Unusefull with nullptr
 */
void BaseObject::removeChild(BaseObject *child) noexcept
{
    if (!child)
        return;
    if (std::count(m_children.begin(), m_children.end(), child) != 0)
    {
        m_children.remove(child);
    }
}
