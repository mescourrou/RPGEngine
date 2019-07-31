#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <map>

class PerformanceTimer
{
    struct infos {
        double average;
        unsigned int count;
        double min;
        double max;
    };

public:
    PerformanceTimer(const char* name) : m_name(name) {
        m_clock = std::chrono::high_resolution_clock::now();
    }

    ~PerformanceTimer() {
        auto diff = std::chrono::high_resolution_clock::now() - m_clock;
        double value = diff.count() * 0.001;

        if (m_averages.find(m_name) == m_averages.end())
        {
            m_averages[m_name] = infos{value, 1, value, value};
        }
        else
        {
            m_averages[m_name].average = (m_averages[m_name].average * m_averages[m_name].count + diff.count()*0.001)/++m_averages[m_name].count;
            if (value > m_averages[m_name].max) m_averages[m_name].max = value;
            if (value < m_averages[m_name].min) m_averages[m_name].min = value;
        }
        printf("PERF %15s : %10.3f us | avg : %10.3f us | min : %10.3f us | max : %10.3f us\n",
               m_name, value, m_averages[m_name].average, m_averages[m_name].min, m_averages[m_name].max);
    }

protected:
    const char* m_name;
    static inline std::map<const char*, infos> m_averages;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_clock;
};

