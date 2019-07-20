#pragma once

#include <string>
#include <vector>
#include <string.h>
#include <cassert>

template <short BATCH>
class stringlist_batch
{
public:
    stringlist_batch();
    stringlist_batch(const char* const* list, size_t len);
    stringlist_batch(const char** list, size_t len);
    stringlist_batch(std::vector<std::string> list);
    stringlist_batch(std::vector<const char*> list);
    ~stringlist_batch();

    stringlist_batch& operator=(const std::vector<std::string>& copy);

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
    char** m_data = nullptr;
    size_t m_size = 0;
    size_t m_allocated = 0;

};

typedef stringlist_batch<5> stringlist;

template<short BATCH>
stringlist_batch<BATCH>::stringlist_batch() : m_size(0), m_allocated(0)
{

}

template <short BATCH>
stringlist_batch<BATCH>::stringlist_batch(const char * const *list, size_t len) : m_size(len)
{
    m_allocated = (len/BATCH + 1)*BATCH;
    m_data = new char*[m_allocated];
    for (size_t i = 0; i < len; i++)
    {
        size_t slen = strlen(list[i]);
        m_data[i] = new char[slen];
        strcpy(m_data[i], list[i]);
    }
}

template <short BATCH>
stringlist_batch<BATCH>::stringlist_batch(const char** list, size_t len) : m_size(len)
{
    m_allocated = (len/BATCH + 1)*BATCH;
    m_data = new char*[m_allocated];
    for (size_t i = 0; i < len; i++)
    {
        size_t slen = strlen(list[i]);
        m_data[i] = new char[slen];
        strcpy(m_data[i], list[i]);
    }
}

template <short BATCH>
stringlist_batch<BATCH>::stringlist_batch(std::vector<std::string> list) : m_size(list.size())
{
    m_allocated = (list.size()/BATCH + 1)*BATCH;
    m_data = new char*[m_allocated];
    for (size_t i = 0; i < list.size(); i++)
    {
        size_t slen = list[i].size();
        m_data[i] = new char[slen];
        strcpy(m_data[i], list[i].c_str());
    }
}

template <short BATCH>
stringlist_batch<BATCH>::stringlist_batch(std::vector<const char*> list) : m_size(list.size())
{
    m_allocated = (list.size()/BATCH + 1)*BATCH;
    m_data = new char*[m_allocated];
    for (size_t i = 0; i < list.size(); i++)
    {
        size_t slen = strlen(list[i]);
        m_data[i] = new char[slen];
        strcpy(m_data[i], list[i]);
    }
}

template <short BATCH>
stringlist_batch<BATCH>::~stringlist_batch()
{
    clear();
}

template<short BATCH>
stringlist_batch<BATCH> &stringlist_batch<BATCH>::operator=(const std::vector<std::string> &copy)
{
    clear();
    m_allocated = (copy.size()/BATCH + 1)*BATCH;
    m_data = new char*[m_allocated];
    for (size_t i = 0; i < copy.size(); i++)
    {
        size_t slen = copy[i].size();
        m_data[i] = new char[slen];
        strcpy(m_data[i], copy[i].c_str());
    }
    m_size = copy.size();
    return *this;

}

template <short BATCH>
size_t stringlist_batch<BATCH>::size() const
{
    return m_size;
}

template <short BATCH>
const char* const* stringlist_batch<BATCH>::data() const
{
    return m_data;
}

template <short BATCH>
char **stringlist_batch<BATCH>::data()
{
    return m_data;
}

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

template<short BATCH>
std::string stringlist_batch<BATCH>::getStr(size_t i) const
{
    assert(i < m_size);
    return std::string(m_data[i]);
}

template<short BATCH>
const char *stringlist_batch<BATCH>::get(size_t i) const
{
    assert(i < m_size);
    return m_data[i];
}

template<short BATCH>
char *stringlist_batch<BATCH>::get(size_t i)
{
    assert(i < m_size);
    return m_data[i];
}

template <short BATCH>
void stringlist_batch<BATCH>::clear()
{
    for (size_t i = 0; i < m_size; i++) {
        delete m_data[i];
    }
    delete[] m_data;
    m_allocated = 0;
    m_size = 0;
}

template<short BATCH>
bool stringlist_batch<BATCH>::remove(const std::string &str)
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
            m_data[i] = m_data[i+1];
    }
    if (shiftPhase)
    {
        m_size--;
    }

    // If we could find the string, shift phase was set to true
    return shiftPhase;

}

template<short BATCH>
bool stringlist_batch<BATCH>::remove(size_t i)
{
    assert(i < m_size);
    delete m_data[i];
    for (; i < m_size - 1; i++)
    {
        m_data[i] = m_data[i+1];
    }
    m_size--;

    return true;

}

template <short BATCH>
void stringlist_batch<BATCH>::push_back(const std::string &str)
{
    if (m_size >= m_allocated)
    {
        do {
            m_allocated += BATCH;
        } while(m_size >= m_allocated);

        char** newOne = new char*[m_allocated];
        copy(newOne, m_size+1, m_data, m_size);
        clear();

        m_data = newOne;
    }
    m_data[m_size] = new char[str.size()];
    strcpy(m_data[m_size], str.c_str());
    m_size++;
}

template <short BATCH>
void stringlist_batch<BATCH>::push_back(const char *str)
{
    if (m_size >= m_allocated)
    {
        do {
            m_allocated += BATCH;
        } while(m_size >= m_allocated);

        char** newOne = new char*[m_allocated];
        copy(newOne, m_size+1, m_data, m_size);
        clear();

        m_data = newOne;
    }
    m_data[m_size] = new char[strlen(str)];
    strcpy(m_data[m_size], str);
    m_size++;
}

template <short BATCH>
void stringlist_batch<BATCH>::copy(char **dest, size_t lenDest, char **origin, size_t lenOrigin)
{
    assert(lenDest >= lenOrigin);
    for (size_t i = 0; i < lenOrigin; i++)
    {
        strcpy(dest[i], origin[i]);
    }
}
