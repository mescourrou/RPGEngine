#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <map>

/**
 * @brief Start a timer at its creation and stop it at it destruction
 *
 * Prints the time in micro-seconds, the average, the maximum and the minimum.
 * The average is calculated in O(1)
 */
class PerformanceTimer
{
    /**
     * @brief Informations stored in the timer list
     */
    struct infos {
        double average;     ///< Current average
        unsigned int count; ///< Sample count, used to calculate the average
        double min;         ///< Minimum
        double max;         ///< Maximum
    };

public:
    /**
     * @brief Create athe PerformanceTimer and start the timer
     * @param name Name of the PerformanceTimer, used to create the PerformanceTimer::infos
     */
    PerformanceTimer(const char* name, bool printInfos = true) : m_name(name), m_printInfos(printInfos) {
        m_clock = std::chrono::high_resolution_clock::now();
    }

    /**
     * @brief Destruct the PerformanceTimer : stops the timer, calculates the informations and print them.
     */
    ~PerformanceTimer() {
        auto diff = std::chrono::high_resolution_clock::now() - m_clock;
        double value = diff.count() * 0.001;
        if (m_printInfos)
        {
            if (m_informations.find(m_name) == m_informations.end())
            {
                m_informations[m_name] = infos{value, 1, value, value};
            }
            else
            {
                // Calculation method for the average.
                //    a + b + c
                //   ----------- is the previous average
                //        3
                //
                //           a + b + c
                //          ----------- * 3 + d
                //               3
                //   avg = ----------------------
                //                   4
                //
                m_informations[m_name].average = (m_informations[m_name].average * m_informations[m_name].count + diff.count()*0.001)/++m_informations[m_name].count;
                if (value > m_informations[m_name].max) m_informations[m_name].max = value;
                if (value < m_informations[m_name].min) m_informations[m_name].min = value;
            }
            // Print the informations
            printf("PERF %15s : %10.3f us | avg : %10.3f us | min : %10.3f us | max : %10.3f us\n",
                   m_name, value, m_informations[m_name].average, m_informations[m_name].min, m_informations[m_name].max);
        }
        else
            printf("PERF %15s : %10.3f us\n", m_name, value);
    }

protected:
    const char* m_name;         ///< Name of the timer
    bool m_printInfos = true; ///< Calculate and print the additionnal informations
    static inline std::map<const char*, infos> m_informations;              ///< Informations
    std::chrono::time_point<std::chrono::high_resolution_clock> m_clock;    ///< Chrono
};

