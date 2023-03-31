/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * IOChrono.h
 *
 *  Created on: Mar 9, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_PROFILING_IOCHRONO_IOCHRONO_H_
#define ADIOS2_TOOLKIT_PROFILING_IOCHRONO_IOCHRONO_H_

/// \cond EXCLUDE_FROM_DOXYGEN
#include <unordered_map>
#include <vector>
/// \endcond

#include "adios2/common/ADIOSConfig.h"
#include "adios2/toolkit/profiling/iochrono/Timer.h"

namespace adios2
{
namespace profiling
{

/** Class used to track different timers */
class IOChrono
{

public:
    /**
     * Create timers for each process
     * <pre>
     * Key: process name
     * Value: see Timer class public API, use to track process time as a
     * chronometer with Resume() and Pause() functions
     * </pre>
     */
    std::unordered_map<std::string, Timer> m_Timers;

    /** Create byte tracking counter for each process*/
    std::unordered_map<std::string, size_t> m_Bytes;

    /** flag to determine if IOChrono object is being used */
    bool m_IsActive = false;

    IOChrono() = default;
    ~IOChrono() = default;

    /** Start existing process in m_Timers */
    void Start(const std::string process) noexcept;

    /**
     * Stop existing process in m_Timers
     * @throws std::invalid_argument if Start wasn't called
     * */
    void Stop(const std::string process);
};

} // end namespace profiling
} // end namespace adios

#endif /* ADIOS2_CORE_IOCHRONO_H_ */
