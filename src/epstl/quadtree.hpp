#pragma once

#include <initializer_list>
#include <ostream>
#include <functional>

#include "container.hpp"
#include "exception.hpp"
#include "math.hpp"
#include "pair.hpp"

namespace epstl
{

enum behaviour_t
{
    quadtree_no_replace = 1,
    quadtree_multithread = 1 << 1
};

template<typename key_t, typename item_t>
class quadtree : public container
{
  private:
    struct position_t
    {
        key_t x = 0;
        key_t y = 0;
    };

    struct rect_bound_t
    {
        key_t left = 0;
        key_t right = 0;
        key_t top = 0;
        key_t bottom = 0;

        position_t center;

        bool isInside(key_t x, key_t y) const
        {
            return x >= left && x < right && y >= bottom && y < top;
        }
    };

    struct quadrant_t
    {
        item_t data;
        position_t data_position;
        rect_bound_t bound;
        quadrant_t* ne = nullptr;
        quadrant_t* nw = nullptr;
        quadrant_t* sw = nullptr;
        quadrant_t* se = nullptr;
        quadrant_t* parent = nullptr;
    };

  public:
    explicit quadtree(key_t center_x, key_t center_y, key_t width, key_t height) :
        m_width(width), m_height(height), m_center{center_x, center_y}, m_default_value{} {}
    explicit quadtree(key_t width, key_t height) :
        quadtree<key_t, item_t>(0, 0, width, height) {}
    explicit quadtree(key_t center_x, key_t center_y, key_t width, key_t height,
                      const item_t& default_value) :
        quadtree(center_x, center_y, width, height)
    {
        m_default_value = default_value;
    }
    explicit quadtree(const quadtree& copy);
    explicit quadtree(quadtree&& move);
    ~quadtree() override;

    quadtree& operator=(const quadtree& copy);
    quadtree& operator=(quadtree&& move);

    size_t size() const noexcept override
    {
        return m_size;
    }

    size_t depth() const noexcept
    {
        return m_depth;
    }

    size_t insert(key_t x, key_t y, const item_t& item);

    const item_t* at(key_t x, key_t y) const;
    item_t* at(key_t x, key_t y);

    bool get(key_t x, key_t y, item_t& ret) const;
    bool get(key_t x, key_t y, item_t& ret);

    void set_behaviour_flag(uint8_t flag)
    {
        m_behaviour_flag = flag;
    }

    void print(std::ostream& stream) const
    {
        stream << "Root:\n";
        print_quadrant(stream, m_root, 0);
    }

    bool find(const item_t& item, epstl::pair<key_t>& keys,
                            std::function<bool(const item_t&, const item_t&)> criterion
                            = [](const item_t& i1, const item_t& i2){return i1 == i2;}) const;

  private:
    static quadrant_t* clone_quadrant(const quadrant_t* quadrant);
    static void free_quadrant(quadrant_t* quadrant);
    size_t insert_quadrant(quadrant_t* quadrant, key_t x, key_t y,
                           const item_t& item);
    static quadrant_t** select_quadrant(quadrant_t* quadrant, key_t x, key_t y);
    void create_quadrants(quadrant_t* parent);
    static item_t* get_value(quadrant_t* quadrant, key_t x, key_t y);
    static void print_quadrant(std::ostream& stream, quadrant_t* quadrant,
                               uint32_t shifts);
    static void shift_stream(std::ostream& stream, uint32_t shifts,
                             const char* separator);

    bool find_quadrant(quadrant_t* quadrant, const item_t& item, epstl::pair<key_t>& keys,
                                   std::function<bool(const item_t&, const item_t&)> criterion) const;

    quadrant_t* m_root = nullptr;
    size_t m_size = 0;
    size_t m_depth = 0;
    item_t m_default_value;
    key_t m_height;
    key_t m_width;
    position_t m_center;

    uint8_t m_behaviour_flag = 0;
};

template<typename key_t, typename item_t>
quadtree<key_t, item_t>::quadtree(const quadtree<key_t, item_t>& copy)
{
    m_root = clone_quadrant(copy.m_root);
    m_root->parent = nullptr;
    m_size = copy.m_size;
}

template<typename key_t, typename item_t>
quadtree<key_t, item_t>::quadtree(quadtree<key_t, item_t>&& move)
{
    m_root = move.m_root;
    m_size = move.m_size;
    move.m_root = nullptr;
    move.m_size = 0;
}

template<typename key_t, typename item_t>
quadtree<key_t, item_t>::~quadtree()
{
    free_quadrant(m_root);
}

template<typename key_t, typename item_t>
quadtree<key_t, item_t>& quadtree<key_t, item_t>::operator=
(const quadtree<key_t, item_t>& copy)
{
    m_root = clone_quadrant(copy.m_root);
    m_root->parent = nullptr;
    m_size = copy.m_size;
    return *this;
}

template<typename key_t, typename item_t>
size_t quadtree<key_t, item_t>::insert(key_t x, key_t y, const item_t& item)
{
    if (!m_root)
    {
        m_root = new quadrant_t;
        m_root->bound.left = m_center.x - m_width / 2.;
        m_root->bound.right = m_center.x + m_width / 2.;
        m_root->bound.bottom = m_center.y - m_height / 2.;
        m_root->bound.top = m_center.y + m_height / 2.;
        m_root->bound.center = m_center;
        m_root->data = item;
        m_root->data_position.x = x;
        m_root->data_position.y = y;
        m_size++;
    }
    else
    {
        size_t branch_depth = insert_quadrant(m_root, x, y, item);
        m_depth = epstl::max(branch_depth, m_depth);
    }

    return m_size;
}

template<typename key_t, typename item_t>
const item_t* quadtree<key_t, item_t>::at(key_t x, key_t y) const
{
    return get_value(m_root, x, y);

}

template<typename key_t, typename item_t>
item_t* quadtree<key_t, item_t>::at(key_t x, key_t y)
{
    return get_value(m_root, x, y);
}

template<typename key_t, typename item_t>
bool quadtree<key_t, item_t>::get(key_t x, key_t y, item_t& ret) const
{
    const item_t* value = get_value(m_root, x, y);
    if (!value)
        return false;
    ret = *value;
    return true;
}

template<typename key_t, typename item_t>
bool quadtree<key_t, item_t>::get(key_t x, key_t y, item_t& ret)
{
    item_t* value = get_value(m_root, x, y);
    if (!value)
        return false;
    ret = *value;
    return true;
}

template<typename key_t, typename item_t>
bool quadtree<key_t, item_t>::find(const item_t& item, epstl::pair<key_t>& keys, std::function<bool (const item_t&, const item_t&)> criterion) const
{
    return find_quadrant(m_root, item, keys, criterion);
}

template<typename key_t, typename item_t>
quadtree<key_t, item_t>& quadtree<key_t, item_t>::operator=
(quadtree<key_t, item_t>&& move)
{
    m_root = move.m_root;
    m_size = move.m_size;
    move.m_root = nullptr;
    move.m_size = 0;
    return *this;
}

template<typename key_t, typename item_t>
typename quadtree<key_t, item_t>::quadrant_t*
quadtree<key_t, item_t>::clone_quadrant(const quadrant_t* quadrant)
{
    if (quadrant)
    {
        quadrant_t* clone = new quadrant_t;
        if (clone->ne = clone_quadrant(quadrant->ne))
            clone->ne->parent = clone;
        if (clone->nw = clone_quadrant(quadrant->nw))
            clone->nw->parent = clone;
        if (clone->sw = clone_quadrant(quadrant->sw))
            clone->sw->parent = clone;
        if (clone->se = clone_quadrant(quadrant->se))
            clone->se->parent = clone;
        clone->bound = quadrant->bound;
        clone->data = quadrant->data;
    }
    else
    {
        return nullptr;
    }
}

template<typename key_t, typename item_t>
void quadtree<key_t, item_t>::free_quadrant(quadrant_t* quadrant)
{
    if (quadrant)
    {
        free_quadrant(quadrant->ne);
        free_quadrant(quadrant->nw);
        free_quadrant(quadrant->sw);
        free_quadrant(quadrant->se);

        delete quadrant;
    }
}

template<typename key_t, typename item_t>
size_t quadtree<key_t, item_t>::insert_quadrant(quadrant_t* quadrant, key_t x,
        key_t y, const item_t& item)
{
    if (!quadrant)
        throw epstl::implementation_exception("insertion in a null quadrant");
    if (!quadrant->bound.isInside(x, y))
        return 0;
    if (quadrant->ne) // If there is a quadrant division
    {
        return insert_quadrant(quadrant->ne, x, y, item)
               + insert_quadrant(quadrant->nw, x, y, item)
               + insert_quadrant(quadrant->sw, x, y, item)
               + insert_quadrant(quadrant->se, x, y, item)
               + 1;
    }

    if (quadrant->data == m_default_value)
    {
        quadrant->data = item;
        quadrant->data_position.x = x;
        quadrant->data_position.y = y;
        m_size++;
        return 0;
    }

    if (quadrant->data_position.x != x || quadrant->data_position.y != y)
    {
        // Division
        create_quadrants(quadrant);

        insert_quadrant(quadrant->ne, quadrant->data_position.x,
                        quadrant->data_position.y, quadrant->data);
        insert_quadrant(quadrant->nw, quadrant->data_position.x,
                        quadrant->data_position.y, quadrant->data);
        insert_quadrant(quadrant->sw, quadrant->data_position.x,
                        quadrant->data_position.y, quadrant->data);
        insert_quadrant(quadrant->se, quadrant->data_position.x,
                        quadrant->data_position.y, quadrant->data);
        m_size--;

        return insert_quadrant(quadrant->ne, x, y, item)
               + insert_quadrant(quadrant->nw, x, y, item)
               + insert_quadrant(quadrant->sw, x, y, item)
               + insert_quadrant(quadrant->se, x, y, item)
               + 1;

    }
    else
    {
        if (!m_behaviour_flag & quadtree_no_replace)
            quadrant->data = item;
    }

    return 0;

}

template<typename key_t, typename item_t>
typename quadtree<key_t, item_t>::quadrant_t**
quadtree<key_t, item_t>::select_quadrant(quadrant_t* quadrant, key_t x, key_t y)
{
    if (!quadrant->bound.isInside(x, y))
        return nullptr;
    if (x >= quadrant->bound.center.x && y >= quadrant->bound.center.y)
        return &quadrant->ne;
    if (x < quadrant->bound.center.x && y >= quadrant->bound.center.y)
        return &quadrant->nw;
    if (x >= quadrant->bound.center.x && y < quadrant->bound.center.y)
        return &quadrant->se;
    if (x < quadrant->bound.center.x && y < quadrant->bound.center.y)
        return &quadrant->sw;
    return nullptr;

}

/**
 *
 *  @todo What happens if we can't devide by 2 ? Ex: parent quadrant is already 1x1 and key_t is int.
 */
template<typename key_t, typename item_t>
void quadtree<key_t, item_t>::create_quadrants(quadrant_t* parent)
{
    quadrant_t* ne = new quadrant_t;
    ne->parent = parent;
    ne->data = m_default_value;
    ne->bound.left = parent->bound.center.x;
    ne->bound.right = parent->bound.right;
    ne->bound.top = parent->bound.top;
    ne->bound.bottom = parent->bound.center.y;
    ne->bound.center.x = (ne->bound.left + ne->bound.right) / 2.;
    ne->bound.center.y = (ne->bound.top + ne->bound.bottom) / 2.;
    parent->ne = ne;

    quadrant_t* nw = new quadrant_t;
    nw->parent = parent;
    nw->data = m_default_value;
    nw->bound.left = parent->bound.left;
    nw->bound.right = parent->bound.center.x;
    nw->bound.top = parent->bound.top;
    nw->bound.bottom = parent->bound.center.y;
    nw->bound.center.x = (nw->bound.left + nw->bound.right) / 2.;
    nw->bound.center.y = (nw->bound.top + nw->bound.bottom) / 2.;
    parent->nw = nw;

    quadrant_t* sw = new quadrant_t;
    sw->parent = parent;
    sw->data = m_default_value;
    sw->bound.left = parent->bound.left;
    sw->bound.right = parent->bound.center.x;
    sw->bound.top = parent->bound.center.y;
    sw->bound.bottom = parent->bound.bottom;
    sw->bound.center.x = (sw->bound.left + sw->bound.right) / 2.;
    sw->bound.center.y = (sw->bound.top + sw->bound.bottom) / 2.;
    parent->sw = sw;

    quadrant_t* se = new quadrant_t;
    se->parent = parent;
    se->data = m_default_value;
    se->bound.left = parent->bound.center.x;
    se->bound.right = parent->bound.right;
    se->bound.top = parent->bound.center.y;
    se->bound.bottom = parent->bound.bottom;
    se->bound.center.x = (se->bound.left + se->bound.right) / 2.;
    se->bound.center.y = (se->bound.top + se->bound.bottom) / 2.;
    parent->se = se;

}

template<typename key_t, typename item_t>
item_t* quadtree<key_t, item_t>::get_value(quadrant_t* quadrant, key_t x,
        key_t y)
{
    if (!quadrant->bound.isInside(x, y))
        return nullptr;
    if (quadrant->ne)
    {
        quadrant_t** selected_quadrant = select_quadrant(quadrant, x, y);
        if (!selected_quadrant)
            return nullptr;
        return get_value(*selected_quadrant, x, y);
    }
    else
    {
        return &quadrant->data;
    }
}

template<typename key_t, typename item_t>
void quadtree<key_t, item_t>::print_quadrant(std::ostream& stream,
        quadrant_t* quadrant, uint32_t shifts)
{
    if (quadrant)
    {
        shift_stream(stream, shifts, "| ");
        stream << "[ " << quadrant->bound.left << ", " << quadrant->bound.right <<
               " ], [ " << quadrant->bound.bottom << ", " << quadrant->bound.top << " ]\n";
        if (quadrant->ne)
        {
            shift_stream(stream, shifts, "| ");
            stream << "NE : \n";
            print_quadrant(stream, quadrant->ne, shifts + 1);
            shift_stream(stream, shifts, "| ");
            stream << "NW : \n";
            print_quadrant(stream, quadrant->nw, shifts + 1);
            shift_stream(stream, shifts, "| ");
            stream << "SW : \n";
            print_quadrant(stream, quadrant->sw, shifts + 1);
            shift_stream(stream, shifts, "| ");
            stream << "SE : \n";
            print_quadrant(stream, quadrant->se, shifts + 1);
            shift_stream(stream, shifts, "| ");
            stream << "-\n";
        }
        else
        {
            shift_stream(stream, shifts, "| ");
            stream << "Data : " << quadrant->data << "\n";
            shift_stream(stream, shifts, "| ");
            stream << "Data position : " << quadrant->data_position.x << ", " <<
                   quadrant->data_position.y << "\n";
        }

    }
}

template<typename key_t, typename item_t>
void quadtree<key_t, item_t>::shift_stream(std::ostream& stream,
        uint32_t shifts, const char* separator)
{
    for (uint32_t i = 0; i < shifts; i++)
    {
        stream << separator;
    }
}

template<typename key_t, typename item_t>
bool quadtree<key_t, item_t>::find_quadrant(quadrant_t* quadrant, const item_t& item, epstl::pair<key_t>& keys,
                                            std::function<bool (const item_t&, const item_t&)> criterion) const
{
    if (!quadrant)
        return false;
    if (quadrant->ne)
    {
        if (m_behaviour_flag & epstl::quadtree_multithread)
        {
            // TODO
            return find_quadrant(quadrant->ne, item, keys, criterion) ||
                    find_quadrant(quadrant->nw, item, keys, criterion) ||
                    find_quadrant(quadrant->sw, item, keys, criterion) ||
                    find_quadrant(quadrant->se, item, keys, criterion);
        }
        else
        {
            return find_quadrant(quadrant->ne, item, keys, criterion) ||
                    find_quadrant(quadrant->nw, item, keys, criterion) ||
                    find_quadrant(quadrant->sw, item, keys, criterion) ||
                    find_quadrant(quadrant->se, item, keys, criterion);
        }
    }
    else if (quadrant->data == item)
    {
        keys.first = quadrant->data_position.x;
        keys.second = quadrant->data_position.y;
        return true;
    }
    else
    {
        return false;
    }
}


} // namespace epstl
