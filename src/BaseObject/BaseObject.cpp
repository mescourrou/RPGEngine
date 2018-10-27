#include "BaseObject.hpp"

BaseObject::BaseObject(BaseObject* parent) : m_parent(parent)
{
    if (m_parent)
        m_parent->addChild(this);
}


BaseObject::~BaseObject()
{
    if (m_parent)
        m_parent->removeChild(this);
    for (std::list<BaseObject*>::iterator it = m_children.begin(); it != m_children.end(); )
    {
        (*it)->setParent(nullptr);
    }
}

BaseObject *BaseObject::parent() const
{
    return m_parent;
}

void BaseObject::setParent(BaseObject *parent)
{
    if (m_parent)
        m_parent->removeChild(this);
    m_parent = parent;
    if (m_parent)
        m_parent->addChild(this);
}

void BaseObject::addChild(BaseObject *child)
{
    if (child == nullptr)
        return;
    if (std::count(m_children.begin(), m_children.end(), child) == 0)
    {
        m_children.push_back(child);
    }
}

void BaseObject::removeChild(BaseObject *child)
{
    if (!child)
        return;
    if (std::count(m_children.begin(), m_children.end(), child) != 0)
    {
        m_children.remove(child);
    }
}
