#pragma once

#include "container.hpp"

#include <stdexcept>

namespace epstl
{

template <typename key_t, typename item_t>
class map : public container
{
    struct node_t
    {
        key_t key;
        node_t* left_node = nullptr;
        node_t* right_node = nullptr;
        node_t* parent = nullptr;

        item_t payload;
    };

  public:
    map() = default;
    map(bool (*less_operator)(const key_t& k1, const key_t& k2)) :
        m_less_operator(less_operator) {}
    ~map() override;

    size_t size() const noexcept override;

    item_t& operator[](const key_t& key);
    bool insert(key_t key, item_t item);
    epstl::size_t height() const noexcept
    {
        return height(m_root);
    }

    const item_t* at(const key_t& key) const noexcept;
    item_t* at(const key_t& key) noexcept;

  private:
    bool insert_recursive(node_t* current_node, key_t& key, item_t& item) noexcept;
    epstl::size_t height(node_t* root) const noexcept;
    auto search(const key_t& key) const noexcept -> map<key_t, item_t>::node_t*;

    void leftRotate(node_t* node) noexcept;
    void rightRotate(node_t* node) noexcept;
    bool (*m_less_operator)(const key_t& k1, const key_t& k2) = &less<key_t, key_t>;
    epstl::size_t m_size = 0;

    node_t* m_root = nullptr;
};

template<typename key_t, typename item_t>
map<key_t, item_t>::~map()
{

}

template <typename key_t, typename item_t>
size_t map<key_t, item_t>::size() const noexcept
{
    return m_size;
}

template<typename key_t, typename item_t>
item_t& map<key_t, item_t>::operator[](const key_t& key)
{

}

template<typename key_t, typename item_t>
bool map<key_t, item_t>::insert(key_t key, item_t item)
{
    if (!m_root)
    {
        node_t* new_node = new node_t;
        new_node->key = std::move(key);
        new_node->payload = std::move(item);
        m_root = new_node;
        return true;
    }

    if (!insert_recursive(m_root, key, item))
        return false;
    short diff = height(m_root->left_node) - height(m_root->right_node);
    if (diff < -1)
        leftRotate(m_root);
    else if (diff > 1)
        rightRotate(m_root);
    return true;
}

template<typename key_t, typename item_t>
const item_t* map<key_t, item_t>::at(const key_t& key) const noexcept
{
    node_t* node = search(key);
    if (node)
        return &node->payload;

    return nullptr;
}

template<typename key_t, typename item_t>
item_t* map<key_t, item_t>::at(const key_t& key) noexcept
{
    node_t* node = search(key);
    if (node)
        return &node->payload;

    return nullptr;
}

template<typename key_t, typename item_t>
size_t map<key_t, item_t>::height(node_t* root) const noexcept
{
    if (!root)
        return 0;
    return std::max(height(root->left_node), height(root->right_node)) + 1;
}

template<typename key_t, typename item_t>
bool map<key_t, item_t>::insert_recursive(node_t* current_node, key_t& key,
        item_t& item) noexcept
{
    if (current_node->key == key)
        return false;
    if (m_less_operator(key, current_node->key))
    {
        if (current_node->left_node)
            current_node = current_node->left_node;
        else
        {
            node_t* new_node = new node_t;
            new_node->key = std::move(key);
            new_node->payload = std::move(item);
            new_node->parent = current_node;
            current_node->left_node = new_node;
            return true;
        }
    }
    else
    {
        if (current_node->right_node)
            current_node = current_node->right_node;
        else
        {
            node_t* new_node = new node_t;
            new_node->key = std::move(key);
            new_node->payload = std::move(item);
            new_node->parent = current_node;
            current_node->right_node = new_node;
            return true;
        }
    }
    if (!insert_recursive(current_node, key, item))
        return false;

    short diff = height(current_node->left_node) - height(current_node->right_node);
    if (diff < -1)
        leftRotate(current_node);
    else if (diff > 1)
        rightRotate(current_node);
    return true;

}

template<typename key_t, typename item_t>
auto map<key_t, item_t>::search(const key_t& key) const noexcept ->
map<key_t, item_t>::node_t*
{
    node_t* current_node = m_root;
    while (current_node)
    {
        if (current_node->key == key)
            return current_node;
        if (m_less_operator(key, current_node->key))
            current_node = current_node->left_node;
        else
            current_node = current_node->right_node;
    }
    return nullptr;
}

template<typename key_t, typename item_t>
void map<key_t, item_t>::leftRotate(map::node_t* node) noexcept
{
    if (!node || !node->right_node)
        return;
    node_t* pivot = node->right_node;

    node->right_node = pivot->left_node;
    if (node->right_node)
        node->right_node->parent = node;

    pivot->left_node = node;

    node_t* node_parent = node->parent;
    if (node->parent)
    {
        if (node->parent->left_node == node)
            node->parent->left_node = pivot;
        else
            node->parent->right_node = pivot;
    }
    else
        m_root = pivot;
    node->parent = pivot;
    pivot->parent = node_parent;
}

template<typename key_t, typename item_t>
void map<key_t, item_t>::rightRotate(map::node_t* node) noexcept
{
    if (!node || !node->left_node)
        return;
    node_t* pivot = node->left_node;

    node->left_node = pivot->right_node;
    if (node->left_node)
        node->left_node->parent = node;

    pivot->right_node = node;

    node_t* node_parent = node->parent;
    if (node->parent)
    {
        if (node->parent->left_node == node)
            node->parent->left_node = pivot;
        else
            node->parent->right_node = pivot;
    }
    else
        m_root = pivot;
    node->parent = pivot;
    pivot->parent = node_parent;
}

} // namespace epstl
