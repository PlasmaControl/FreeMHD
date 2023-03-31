/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * FileDescriptor.cpp file I/O using POSIX I/O library
 *
 *  Created on: Oct 6, 2016
 *      Author: William F Godoy godoywf@ornl.gov
 */
#include "FilePOSIX.h"

#include <cstdio>      // remove
#include <cstring>     // strerror
#include <errno.h>     // errno
#include <fcntl.h>     // open
#include <stddef.h>    // write output
#include <sys/stat.h>  // open, fstat
#include <sys/types.h> // open
#include <unistd.h>    // write, close

/// \cond EXCLUDE_FROM_DOXYGEN
#include <ios> //std::ios_base::failure
/// \endcond

namespace adios2
{
namespace transport
{

FilePOSIX::FilePOSIX(helper::Comm const &comm)
: Transport("File", "POSIX", comm)
{
}

FilePOSIX::~FilePOSIX()
{
    if (m_IsOpen)
    {
        close(m_FileDescriptor);
    }
}

void FilePOSIX::WaitForOpen()
{
    if (m_IsOpening)
    {
        if (m_OpenFuture.valid())
        {
            m_FileDescriptor = m_OpenFuture.get();
        }
        m_IsOpening = false;
        CheckFile("couldn't open file " + m_Name + ", in call to POSIX open");
        m_IsOpen = true;
    }
}

void FilePOSIX::Open(const std::string &name, const Mode openMode,
                     const bool async)
{
    auto lf_AsyncOpenWrite = [&](const std::string &name) -> int {
        ProfilerStart("open");
        errno = 0;
        int FD = open(m_Name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
        m_Errno = errno;
        ProfilerStop("open");
        return FD;
    };

    m_Name = name;
    CheckName();
    m_OpenMode = openMode;
    switch (m_OpenMode)
    {

    case (Mode::Write):
        if (async)
        {
            m_IsOpening = true;
            m_OpenFuture =
                std::async(std::launch::async, lf_AsyncOpenWrite, name);
        }
        else
        {
            ProfilerStart("open");
            errno = 0;
            m_FileDescriptor =
                open(m_Name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
            m_Errno = errno;
            ProfilerStop("open");
        }
        break;

    case (Mode::Append):
        ProfilerStart("open");
        errno = 0;
        // m_FileDescriptor = open(m_Name.c_str(), O_RDWR);
        m_FileDescriptor = open(m_Name.c_str(), O_RDWR | O_CREAT, 0777);
        lseek(m_FileDescriptor, 0, SEEK_END);
        m_Errno = errno;
        ProfilerStop("open");
        break;

    case (Mode::Read):
        ProfilerStart("open");
        errno = 0;
        m_FileDescriptor = open(m_Name.c_str(), O_RDONLY);
        m_Errno = errno;
        ProfilerStop("open");
        break;

    default:
        CheckFile("unknown open mode for file " + m_Name +
                  ", in call to POSIX open");
    }

    if (!m_IsOpening)
    {
        CheckFile("couldn't open file " + m_Name + ", in call to POSIX open");
        m_IsOpen = true;
    }
}

void FilePOSIX::Write(const char *buffer, size_t size, size_t start)
{
    auto lf_Write = [&](const char *buffer, size_t size) {
        while (size > 0)
        {
            ProfilerStart("write");
            errno = 0;
            const auto writtenSize = write(m_FileDescriptor, buffer, size);
            m_Errno = errno;
            ProfilerStop("write");

            if (writtenSize == -1)
            {
                if (errno == EINTR)
                {
                    continue;
                }

                throw std::ios_base::failure(
                    "ERROR: couldn't write to file " + m_Name +
                    ", in call to POSIX Write" + SysErrMsg());
            }

            buffer += writtenSize;
            size -= writtenSize;
        }
    };

    WaitForOpen();
    if (start != MaxSizeT)
    {
        errno = 0;
        const auto newPosition = lseek(m_FileDescriptor, start, SEEK_SET);
        m_Errno = errno;

        if (static_cast<size_t>(newPosition) != start)
        {
            throw std::ios_base::failure(
                "ERROR: couldn't move to start position " +
                std::to_string(start) + " in file " + m_Name +
                ", in call to POSIX lseek" + SysErrMsg());
        }
    }

    if (size > DefaultMaxFileBatchSize)
    {
        const size_t batches = size / DefaultMaxFileBatchSize;
        const size_t remainder = size % DefaultMaxFileBatchSize;

        size_t position = 0;
        for (size_t b = 0; b < batches; ++b)
        {
            lf_Write(&buffer[position], DefaultMaxFileBatchSize);
            position += DefaultMaxFileBatchSize;
        }
        lf_Write(&buffer[position], remainder);
    }
    else
    {
        lf_Write(buffer, size);
    }
}

void FilePOSIX::Read(char *buffer, size_t size, size_t start)
{
    auto lf_Read = [&](char *buffer, size_t size) {
        while (size > 0)
        {
            ProfilerStart("read");
            errno = 0;
            const auto readSize = read(m_FileDescriptor, buffer, size);
            m_Errno = errno;
            ProfilerStop("read");

            if (readSize == -1)
            {
                if (errno == EINTR)
                {
                    continue;
                }

                throw std::ios_base::failure(
                    "ERROR: couldn't read from file " + m_Name +
                    ", in call to POSIX IO read" + SysErrMsg());
            }

            buffer += readSize;
            size -= readSize;
        }
    };

    WaitForOpen();

    if (start != MaxSizeT)
    {
        errno = 0;
        const auto newPosition = lseek(m_FileDescriptor, start, SEEK_SET);
        m_Errno = errno;

        if (static_cast<size_t>(newPosition) != start)
        {
            throw std::ios_base::failure(
                "ERROR: couldn't move to start position " +
                std::to_string(start) + " in file " + m_Name +
                ", in call to POSIX lseek" + SysErrMsg());
        }
    }

    if (size > DefaultMaxFileBatchSize)
    {
        const size_t batches = size / DefaultMaxFileBatchSize;
        const size_t remainder = size % DefaultMaxFileBatchSize;

        size_t position = 0;
        for (size_t b = 0; b < batches; ++b)
        {
            lf_Read(&buffer[position], DefaultMaxFileBatchSize);
            position += DefaultMaxFileBatchSize;
        }
        lf_Read(&buffer[position], remainder);
    }
    else
    {
        lf_Read(buffer, size);
    }
}

size_t FilePOSIX::GetSize()
{
    struct stat fileStat;
    WaitForOpen();
    errno = 0;
    if (fstat(m_FileDescriptor, &fileStat) == -1)
    {
        m_Errno = errno;
        throw std::ios_base::failure("ERROR: couldn't get size of file " +
                                     m_Name + SysErrMsg());
    }
    m_Errno = errno;
    return static_cast<size_t>(fileStat.st_size);
}

void FilePOSIX::Flush() {}

void FilePOSIX::Close()
{
    WaitForOpen();
    ProfilerStart("close");
    errno = 0;
    const int status = close(m_FileDescriptor);
    m_Errno = errno;
    ProfilerStop("close");

    if (status == -1)
    {
        throw std::ios_base::failure("ERROR: couldn't close file " + m_Name +
                                     ", in call to POSIX IO close" +
                                     SysErrMsg());
    }

    m_IsOpen = false;
}

void FilePOSIX::Delete()
{
    WaitForOpen();
    if (m_IsOpen)
    {
        Close();
    }
    std::remove(m_Name.c_str());
}

void FilePOSIX::CheckFile(const std::string hint) const
{
    if (m_FileDescriptor == -1)
    {
        throw std::ios_base::failure("ERROR: " + hint + SysErrMsg());
    }
}

std::string FilePOSIX::SysErrMsg() const
{
    return std::string(": errno = " + std::to_string(m_Errno) + ": " +
                       strerror(m_Errno));
}

void FilePOSIX::SeekToEnd()
{
    WaitForOpen();
    errno = 0;
    const int status = lseek(m_FileDescriptor, 0, SEEK_END);
    m_Errno = 0;
    if (status == -1)
    {
        throw std::ios_base::failure(
            "ERROR: couldn't seek to the end of file " + m_Name +
            ", in call to POSIX IO lseek" + SysErrMsg());
    }
}

void FilePOSIX::SeekToBegin()
{
    WaitForOpen();
    errno = 0;
    const int status = lseek(m_FileDescriptor, 0, SEEK_SET);
    m_Errno = errno;
    if (status == -1)
    {
        throw std::ios_base::failure(
            "ERROR: couldn't seek to the begin of file " + m_Name +
            ", in call to POSIX IO lseek" + SysErrMsg());
    }
}

} // end namespace transport
} // end namespace adios2
