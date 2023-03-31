/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * IOHDF5.cpp: HDF5-specific IO engine factory
 */

#include "IO.h"

#include "adios2/engine/hdf5/HDF5ReaderP.h"
#include "adios2/engine/hdf5/HDF5WriterP.h"
#if H5_VERSION_GE(1, 11, 0)
#include "adios2/engine/mixer/HDFMixer.h"
#endif

namespace adios2
{
namespace core
{

namespace
{

template <typename T>
std::shared_ptr<Engine> MakeEngineHDF5(IO &io, const std::string &name,
                                       const Mode mode, helper::Comm comm)
{
#ifndef H5_HAVE_PARALLEL
    if (comm.IsMPI())
    {
        throw std::invalid_argument("A serial HDF5 engine cannot be used "
                                    "with a communicator that is MPI-based.");
    }
#endif
    return IO::MakeEngine<T>(io, name, mode, std::move(comm));
}

} // end anonymous namespace

IO::EngineFactoryEntry IO_MakeEngine_HDFMixer()
{
#if H5_VERSION_GE(1, 11, 0)
    return IO::EngineFactoryEntry{MakeEngineHDF5<engine::HDF5ReaderP>,
                                  MakeEngineHDF5<engine::HDFMixer>};
#else
    return IO::NoEngineEntry("ERROR: update HDF5 >= 1.11 to support VDS.");
#endif
}

IO::EngineFactoryEntry IO_MakeEngine_HDF5()
{
    return IO::EngineFactoryEntry{MakeEngineHDF5<engine::HDF5ReaderP>,
                                  MakeEngineHDF5<engine::HDF5WriterP>};
}

} // end namespace core
} // end namespace adios2
