#pragma once

// STL
#include <vector>
#include <thread>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace events
{

#ifdef RPG_BUILD_TEST
class EventTest;
#endif

template <typename ...Args>
class Event : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class events::EventTest;
#endif
public:
    Event() = default;
    ~Event() override = default;
	
    std::string className() const noexcept override { return "Event"; }

    void trigger(Args... arguments) const
    {
        for (auto& call : m_callList)
        {

        }
    }
    void subscribe(std::function<void(Args...)>);

private:
    std::vector<std::function<void(Args...)>> m_callList;
};


} // namespace events

