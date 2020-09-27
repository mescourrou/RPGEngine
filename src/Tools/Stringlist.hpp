#pragma once

#include <string>
#include <vector>
#include <string.h>
#include <cassert>
#include <set>

/**
 * @brief Array of c-strings
 *
 * The template parameter is the size of each allocation : one by one, two by two, ...
 */
template <short BATCH>
class stringlist_batch
{
  public:
    stringlist_batch();
    explicit stringlist_batch(const char* const* list, size_t len);
    explicit stringlist_batch(const char** list, size_t len);
    explicit stringlist_batch(std::vector<std::string> list);
    explicit stringlist_batch(std::vector<const char*> list);
    ~stringlist_batch();

    stringlist_batch(const stringlist_batch<BATCH>& copy) noexcept;
    stringlist_batch(stringlist_batch<BATCH>&& move) noexcept :
        m_data(move.m_data), m_size(move.m_size), m_allocated(move.m_allocated)
    {
        move.m_data = nullptr;
        move.m_size = 0;
        move.m_allocated = 0;
    }

    stringlist_batch& operator=(const std::vector<std::string>& copy);
    stringlist_batch& operator=(const std::set<std::string>& copy);
    stringlist_batch& operator=(const stringlist_batch<BATCH>& copy) noexcept;
    stringlist_batch& operator=(stringlist_batch<BATCH>&& move) noexcept
    {
        m_data = move.m_data;
        m_size = move.m_size;
        m_allocated = move.m_allocated;
        move.m_data = nullptr;
        move.m_size = 0;
        move.m_allocated = 0;
        return *this;
    }

    size_t size() const;
    const char* const* data() const;
    char** data();
    std::vector<std::string> toVectorString() const;
    std::string getStr(size_t i) const;
    const char* get(size_t i) const;
    char* get(size_t i);
    void clear();
    bool remove(const std::string& str);
    bool remove(size_t i);
    void push_back(const std::string& str);
    void push_back(const char* str);
  private:
    static void copy(char** dest, size_t lenDest, char** origin, size_t lenOrigin);
    char** m_data = nullptr;    ///< String array
    size_t m_size = 0;          ///< Number of strings
    size_t m_allocated = 0;     ///< Number of allocated char*

};

typedef stringlist_batch<5> stringlist;

/**
 * @brief Create the list
 */
template<short BATCH>
stringlist_batch<BATCH>::stringlist_batch() : m_size(0), m_allocated(0)
{

}

/**
 * @brief Create a list from a const char* const* list.
 *
 * Allocate the memory and *copy* the list
 *
 * @param list List to copy
 * @param len Length of the list (number of c-strings)
 */
template <short BATCH>
stringlist_batch<BATCH>::stringlist_batch(const char* const* list,
        size_t len) : m_size(len)
{
    m_allocated = (len / BATCH + 1) * BATCH;
    m_data = new char* [m_allocated];
    for (size_t i = 0; i < len; i++)
    {
        size_t slen = strlen(list[i]);
        m_data[i] = new char[slen];
        strcpy(m_data[i], list[i]);
    }
}

/**
 * @brief Create a list from a const char** list.
 *
 * Allocate the memory and *copy* the list
 * @param list List to copy
 * @param len Number of strings on the list
 */
template <short BATCH>
stringlist_batch<BATCH>::stringlist_batch(const char** list,
        size_t len) : m_size(len)
{
    m_allocated = (len / BATCH + 1) * BATCH;
    m_data = new char* [m_allocated];
    for (size_t i = 0; i < len; i++)
    {
        size_t slen = strlen(list[i]);
        m_data[i] = new char[slen];
        strcpy(m_data[i], list[i]);
    }
}

/**
 * @brief Create a list from a string vector
 *
 * Allocate the memory and *copy* the list
 *
 * @param list List to copy
 */
template <short BATCH>
stringlist_batch<BATCH>::stringlist_batch(std::vector<std::string> list) :
    m_size(list.size())
{
    m_allocated = (list.size() / BATCH + 1) * BATCH;
    m_data = new char* [m_allocated];
    for (size_t i = 0; i < list.size(); i++)
    {
        size_t slen = list[i].size();
        m_data[i] = new char[slen];
        strcpy(m_data[i], list[i].c_str());
    }
}

/**
 * @brief Create a list from a c-string vector.
 *
 * Allocate the memory and *copy* the list
 * @param list List to copy
 */
template <short BATCH>
stringlist_batch<BATCH>::stringlist_batch(std::vector<const char*> list) :
    m_size(list.size())
{
    m_allocated = (list.size() / BATCH + 1) * BATCH;
    m_data = new char* [m_allocated];
    for (size_t i = 0; i < list.size(); i++)
    {
        size_t slen = strlen(list[i]);
        m_data[i] = new char[slen];
        strcpy(m_data[i], list[i]);
    }
}

/**
 * @brief Free the memory, by calling clear
 */
template <short BATCH>
stringlist_batch<BATCH>::~stringlist_batch()
{
    clear();
}

template<short BATCH>
stringlist_batch<BATCH>::stringlist_batch(const stringlist_batch<BATCH>& copy)
noexcept :
    m_allocated(copy.m_allocated), m_size(copy.m_size)
{
    m_data = new char* [m_allocated];
    for (size_t i = 0; i < m_size; i++)
    {
        size_t slen = strlen(copy.m_data[i]);
        m_data[i] = new char[slen];
        strcpy(m_data[i], copy.m_data[i]);
    }
}

/**
 * @brief Clear the list and copy the data
 * @brief copy List to copy
 */
template<short BATCH>
stringlist_batch<BATCH>& stringlist_batch<BATCH>::operator=
(const std::vector<std::string>& copy)
{
    clear();
    m_allocated = (copy.size() / BATCH + 1) * BATCH;
    m_data = new char* [m_allocated];
    for (size_t i = 0; i < copy.size(); i++)
    {
        size_t slen = copy[i].size();
        m_data[i] = new char[slen];
        strcpy(m_data[i], copy[i].c_str());
    }
    m_size = copy.size();
    return *this;

}

/**
 * @brief Clear the list and copy the data
 * @param copy List to copy
 */
template<short BATCH>
stringlist_batch<BATCH>& stringlist_batch<BATCH>::operator=
(const std::set<std::string>& copy)
{
    clear();
    m_allocated = (copy.size() / BATCH + 1) * BATCH;
    m_data = new char* [m_allocated];
    size_t i = 0;
    for (auto& s : copy)
    {
        size_t slen = s.size();
        m_data[i] = new char[slen];
        strcpy(m_data[i], s.c_str());
        i++;
    }
    m_size = copy.size();
    return *this;
}

template<short BATCH>
stringlist_batch<BATCH>& stringlist_batch<BATCH>::operator=(const
        stringlist_batch<BATCH>& copy) noexcept
{
    clear();
    m_allocated = copy.m_allocated;
    m_size = copy.m_size;
    m_data = new char* [m_allocated];
    for (size_t i = 0; i < m_size; i++)
    {
        size_t slen = strlen(copy.m_data[i]);
        m_data[i] = new char[slen];
        strcpy(m_data[i], copy.m_data[i]);
    }
    return *this;
}

/**
 * @brief Size of the list
 */
template <short BATCH>
size_t stringlist_batch<BATCH>::size() const
{
    return m_size;
}

/**
 * @brief Get the const data pointer
 */
template <short BATCH>
const char* const* stringlist_batch<BATCH>::data() const
{
    return m_data;
}

/**
 * @brief Get the data pointer
 */
template <short BATCH>
char** stringlist_batch<BATCH>::data()
{
    return m_data;
}

/**
 * @brief Convert to string vector.
 * @warning Create and allocate a new vector
 */
template<short BATCH>
std::vector<std::string> stringlist_batch<BATCH>::toVectorString() const
{
    std::vector<std::string> ret;
    ret.reserve(m_size);
    for (unsigned int i = 0; i < m_size; i++)
    {
        ret.push_back(m_data[i]);
    }
    return ret;
}

/**
 * @brief Get the string at the given index (create the string)
 * @param i Index to get
 */
template<short BATCH>
std::string stringlist_batch<BATCH>::getStr(size_t i) const
{
    assert(i < m_size);
    return std::string(m_data[i]);
}

/**
 * @brief Get the pointer on the c-string at the given index
 * @param i Index to get
 */
template<short BATCH>
const char* stringlist_batch<BATCH>::get(size_t i) const
{
    assert(i < m_size);
    return m_data[i];
}

/**
 * @brief Get the modifiable pointer of the c-string at the given index
 * @param i Index to get
 */
template<short BATCH>
char* stringlist_batch<BATCH>::get(size_t i)
{
    assert(i < m_size);
    return m_data[i];
}

/**
 * @brief Clear the data
 */
template <short BATCH>
void stringlist_batch<BATCH>::clear()
{
    if (m_data)
    {
        for (size_t i = 0; i < m_size; i++)
        {
            delete[] m_data[i];
        }
        delete[] m_data;
    }
    m_allocated = 0;
    m_size = 0;
}

/**
 * @brief Remove the given string. Shift all the followings
 *
 * Complexity : O(n)
 *
 * @param str String to search for and remove
 */
template<short BATCH>
bool stringlist_batch<BATCH>::remove(const std::string& str)
{
    bool shiftPhase = false;
    for (size_t i = 0; i < m_size; i++)
    {
        if (strcmp(m_data[i], str.c_str()) == 0)
        {
            shiftPhase = true;
            delete m_data[i];
        }
        if (shiftPhase && i + 1 < m_size)
            m_data[i] = m_data[i + 1];
    }
    if (shiftPhase)
    {
        m_size--;
    }

    // If we could find the string, shift phase was set to true
    return shiftPhase;

}

/**
 * @brief Remove the item at the given index
 *
 * Complexity : Worst case O(n), Best case O(1)
 *
 * @param i Index to get
 */
template<short BATCH>
bool stringlist_batch<BATCH>::remove(size_t i)
{
    assert(i < m_size);
    delete m_data[i];
    for (; i < m_size - 1; i++)
    {
        m_data[i] = m_data[i + 1];
    }
    m_size--;

    return true;

}

/**
 * @brief Push the string at the end of the list
 * Allocate the memory by BATCH if needed
 *
 * @param str String to add
 */
template <short BATCH>
void stringlist_batch<BATCH>::push_back(const std::string& str)
{
    if (m_size >= m_allocated)
    {
        do
        {
            m_allocated += BATCH;
        } while (m_size >= m_allocated);

        char** newOne = new char* [m_allocated];
        copy(newOne, m_size + 1, m_data, m_size);
        clear();

        m_data = newOne;
    }
    m_data[m_size] = new char[str.size()];
    strcpy(m_data[m_size], str.c_str());
    m_size++;
}

/**
 * @brief Push the string at the end of the list
 * Allocate the memory by BATCH if needed
 *
 * @param str String to add
 */
template <short BATCH>
void stringlist_batch<BATCH>::push_back(const char* str)
{
    if (m_size >= m_allocated)
    {
        do
        {
            m_allocated += BATCH;
        } while (m_size >= m_allocated);

        char** newOne = new char* [m_allocated];
        copy(newOne, m_size + 1, m_data, m_size);
        clear();

        m_data = newOne;
    }
    m_data[m_size] = new char[strlen(str)];
    strcpy(m_data[m_size], str);
    m_size++;
}

/**
 * @brief Utility function to copy c-string arrays
 *
 * @param dest c-string destination
 * @param lenDest Size of the destination
 * @param origin c-string origin
 * @param lenOrigin Size of the origin
 */
template <short BATCH>
void stringlist_batch<BATCH>::copy(char** dest, size_t lenDest, char** origin,
                                   size_t lenOrigin)
{
    assert(lenDest >= lenOrigin);
    for (size_t i = 0; i < lenOrigin; i++)
    {
        strcpy(dest[i], origin[i]);
    }
}
