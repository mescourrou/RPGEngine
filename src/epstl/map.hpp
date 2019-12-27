#pragma once

#include "container.hpp"
#include "math.hpp"

#include <stdexcept>

namespace epstl
{

/**
 * @brief Key based map
 */
template <typename key_t, typename item_t>
class map : public container
{
    /**
     * @brief Map tree node
     */
    struct node_t
    {
        key_t key;
        node_t* left_node = nullptr;
        node_t* right_node = nullptr;
        node_t* parent = nullptr;

        item_t payload;
    };

  public:
    /**
     * @brief Default constructor
     */
    map() = default;
    /**
     * @brief Create a map with the given less operator
     * @param less_operator Less (<) operator to use
     */
    map(bool (*less_operator)(const key_t& k1, const key_t& k2)) :
        m_less_operator(less_operator) {}

    ~map() override;

    size_t size() const noexcept override;

    item_t& operator[](const key_t& key);
    bool insert(key_t key, item_t item);

    /**
     * @brief Get the height of the map tree
     * @return
     */
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

    /// Less (<) operator to use
    bool (*m_less_operator)(const key_t& k1, const key_t& k2) = &less<key_t, key_t>;
    epstl::size_t m_size = 0; ///< Size of the map

    node_t* m_root = nullptr; ///< Root node of the tree
};

/**
 * @brief Destructor which free the contained values
 * @todo Destructor to implement
 */
template<typename key_t, typename item_t>
map<key_t, item_t>::~map()
{

}

/**
 * @brief Get the size of the map : number of items inside
 */
template <typename key_t, typename item_t>
size_t map<key_t, item_t>::size() const noexcept
{
    return m_size;
}

/**
 * @brief Get the value at the given key
 * @todo To implement
 */
template<typename key_t, typename item_t>
item_t& map<key_t, item_t>::operator[](const key_t& key)
{

}

/**
 * @brief Insert the item at the given key
 * @param key Key of the item
 * @param item Item to insert
 * @return Return true if the insertion was successful
 */
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
    // Equilibrate the tree
    short diff = height(m_root->left_node) - height(m_root->right_node);
    if (diff < -1)
        leftRotate(m_root);
    else if (diff > 1)
        rightRotate(m_root);
    return true;
}

/**
 * @brief Get a const pointer on the item at the given key
 * @param key Key to look for
 * @return Const pointer on the value or nullptr if the key was not found
 */
template<typename key_t, typename item_t>
const item_t* map<key_t, item_t>::at(const key_t& key) const noexcept
{
    node_t* node = search(key);
    if (node)
        return &node->payload;

    return nullptr;
}

/**
 * @brief Get a mutable pointer on the item at the given key
 * @param key Key to look for
 * @return Mutable pointer on the value or nullptr if the key was not found
 */
template<typename key_t, typename item_t>
item_t* map<key_t, item_t>::at(const key_t& key) noexcept
{
    node_t* node = search(key);
    if (node)
        return &node->payload;

    return nullptr;
}

/**
 * @brief Compute the height of the tree with the given root
 * @param root Root of the tree where to compute the height
 */
template<typename key_t, typename item_t>
size_t map<key_t, item_t>::height(node_t* root) const noexcept
{
    if (!root)
        return 0;
    return epstl::max(height(root->left_node), height(root->right_node)) + 1;
}

/**
 * @brief Recusive version of insert
 * @param current_node Root of the tree to insert into
 * @param key Key of the item
 * @param item Item to insert
 * @return Return true if the item was inserted
 */
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

/**
 * @brief Search for the key and return the node found
 * @param key Key to look for
 * @return Pointer on the note. Null if not found
 */
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

/**
 * @brief Do a left rotation on the given node
 * @param node Node to rotate
 */
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

/**
 * @brief Do a right rotation on the given node
 * @param node Node to rotate
 */
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
