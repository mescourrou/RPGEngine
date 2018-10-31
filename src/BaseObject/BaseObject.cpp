#include "BaseObject.hpp"

BaseObject::BaseObject(BaseObject* parent) noexcept : m_parent(parent)
{
    if (m_parent)
        m_parent->addChild(this);
}


BaseObject::~BaseObject() noexcept
{
    if (m_parent)
        m_parent->removeChild(this);
    for (auto it = m_children.begin(); it != m_children.end(); )
    {
        (*it)->setParent(nullptr);
    }
}

BaseObject *BaseObject::parent() const noexcept
{
    return m_parent;
}

void BaseObject::setParent(BaseObject *parent) noexcept
{
    if (m_parent)
        m_parent->removeChild(this);
    m_parent = parent;
    if (m_parent)
        m_parent->addChild(this);
}

void BaseObject::addChild(BaseObject *child) noexcept
{
    if (child == nullptr)
        return;
    if (std::count(m_children.begin(), m_children.end(), child) == 0)
    {
        m_children.push_back(child);
    }
}

void BaseObject::removeChild(BaseObject *child) noexcept
{
    if (!child)
        return;
    if (std::count(m_children.begin(), m_children.end(), child) != 0)
    {
        m_children.remove(child);
    }
}
