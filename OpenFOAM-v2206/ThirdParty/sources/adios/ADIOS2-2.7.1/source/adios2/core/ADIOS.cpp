/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * ADIOS.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "ADIOS.h"

#include <algorithm> // std::transform
#include <fstream>
#include <ios> //std::ios_base::failure

#include "adios2/core/IO.h"
#include "adios2/helper/adiosCommDummy.h"
#include "adios2/helper/adiosFunctions.h" //InquireKey, BroadcastFile
#include <adios2sys/SystemTools.hxx>

// OPERATORS

// compress
#ifdef ADIOS2_HAVE_BZIP2
#include "adios2/operator/compress/CompressBZIP2.h"
#endif

#ifdef ADIOS2_HAVE_ZFP
#include "adios2/operator/compress/CompressZFP.h"
#endif

#ifdef ADIOS2_HAVE_SZ
#include "adios2/operator/compress/CompressSZ.h"
#endif

#ifdef ADIOS2_HAVE_MGARD
#include "adios2/operator/compress/CompressMGARD.h"
#endif

#ifdef ADIOS2_HAVE_BZIP2
#include "adios2/operator/compress/CompressBZIP2.h"
#endif

#ifdef ADIOS2_HAVE_PNG
#include "adios2/operator/compress/CompressPNG.h"
#endif

#ifdef ADIOS2_HAVE_BLOSC
#include "adios2/operator/compress/CompressBlosc.h"
#endif

// callbacks
#include "adios2/operator/callback/Signature1.h"
#include "adios2/operator/callback/Signature2.h"

namespace adios2
{
namespace core
{

ADIOS::ADIOS(const std::string configFile, helper::Comm comm,
             const std::string hostLanguage)
: m_ConfigFile(configFile), m_HostLanguage(hostLanguage),
  m_Comm(std::move(comm))
{
    if (!configFile.empty())
    {
        if (!adios2sys::SystemTools::FileExists(configFile))
        {
            throw std::logic_error("Config file " + configFile +
                                   " passed to ADIOS does not exist.");
        }
        if (helper::EndsWith(configFile, ".xml"))
        {
            XMLInit(configFile);
        }
        else if (helper::EndsWith(configFile, ".yaml") ||
                 helper::EndsWith(configFile, ".yml"))
        {
            YAMLInit(configFile);
        }
    }
}

ADIOS::ADIOS(const std::string configFile, const std::string hostLanguage)
: ADIOS(configFile, helper::CommDummy(), hostLanguage)
{
}

ADIOS::ADIOS(helper::Comm comm, const std::string hostLanguage)
: ADIOS("", std::move(comm), hostLanguage)
{
}

ADIOS::ADIOS(const std::string hostLanguage)
: ADIOS("", helper::CommDummy(), hostLanguage)
{
}

ADIOS::~ADIOS() = default;

IO &ADIOS::DeclareIO(const std::string name)
{
    auto itIO = m_IOs.find(name);

    if (itIO != m_IOs.end())
    {
        IO &io = itIO->second;

        if (!io.IsDeclared()) // exists from config xml
        {
            io.SetDeclared();
            return io;
        }
        else
        {
            throw std::invalid_argument(
                "ERROR: IO with name " + name +
                " previously declared with DeclareIO, name must be "
                "unique,"
                " in call to DeclareIO\n");
        }
    }

    auto ioPair = m_IOs.emplace(
        std::piecewise_construct, std::forward_as_tuple(name),
        std::forward_as_tuple(*this, name, false, m_HostLanguage));
    IO &io = ioPair.first->second;
    io.SetDeclared();
    return io;
}

IO &ADIOS::AtIO(const std::string name)
{
    auto itIO = m_IOs.find(name);

    if (itIO == m_IOs.end())
    {
        throw std::invalid_argument("ERROR: IO with name " + name +
                                    " was not declared, did you previously "
                                    "call DeclareIO?, in call to AtIO\n");
    }
    else
    {
        if (!itIO->second.IsDeclared())
        {
            throw std::invalid_argument("ERROR: IO with name " + name +
                                        " was not declared, did you previously "
                                        "call DeclareIO ?, in call to AtIO\n");
        }
    }

    return itIO->second;
}

void ADIOS::FlushAll()
{
    for (auto &ioPair : m_IOs)
    {
        ioPair.second.FlushAll();
    }
}

Operator &ADIOS::DefineOperator(const std::string &name, const std::string type,
                                const Params &parameters)
{
    auto lf_ErrorMessage = [](const std::string type) -> std::string {
        return "ERROR: this version of ADIOS2 didn't compile with the " + type +
               " library, when parsing config file in ADIOS constructor or in "
               "call to ADIOS::DefineOperator";
    };

    std::shared_ptr<Operator> operatorPtr;

    CheckOperator(name);
    const std::string typeLowerCase = helper::LowerCase(type);

    if (typeLowerCase == "bzip2")
    {
#ifdef ADIOS2_HAVE_BZIP2
        auto itPair = m_Operators.emplace(
            name, std::make_shared<compress::CompressBZIP2>(parameters));
        operatorPtr = itPair.first->second;
#else
        throw std::invalid_argument(lf_ErrorMessage("BZip2"));
#endif
    }
    else if (typeLowerCase == "zfp")
    {
#ifdef ADIOS2_HAVE_ZFP
        auto itPair = m_Operators.emplace(
            name, std::make_shared<compress::CompressZFP>(parameters));
        operatorPtr = itPair.first->second;
#else
        throw std::invalid_argument(lf_ErrorMessage("ZFP"));
#endif
    }
    else if (typeLowerCase == "sz")
    {
#ifdef ADIOS2_HAVE_SZ
        auto itPair = m_Operators.emplace(
            name, std::make_shared<compress::CompressSZ>(parameters));
        operatorPtr = itPair.first->second;
#else
        throw std::invalid_argument(lf_ErrorMessage("SZ"));
#endif
    }
    else if (typeLowerCase == "mgard")
    {
#ifdef ADIOS2_HAVE_MGARD
        auto itPair = m_Operators.emplace(
            name, std::make_shared<compress::CompressMGARD>(parameters));
        operatorPtr = itPair.first->second;
#else
        throw std::invalid_argument(lf_ErrorMessage("MGARD"));
#endif
    }
    else if (typeLowerCase == "png")
    {
#ifdef ADIOS2_HAVE_PNG
        auto itPair = m_Operators.emplace(
            name, std::make_shared<compress::CompressPNG>(parameters));
        operatorPtr = itPair.first->second;
#else
        throw std::invalid_argument(lf_ErrorMessage("PNG"));
#endif
    }
    else if (typeLowerCase == "blosc")
    {
#ifdef ADIOS2_HAVE_BLOSC
        auto itPair = m_Operators.emplace(
            name, std::make_shared<compress::CompressBlosc>(parameters));
        operatorPtr = itPair.first->second;
#else
        throw std::invalid_argument(lf_ErrorMessage("Blosc"));
#endif
    }
    else
    {
        throw std::invalid_argument(
            "ERROR: Operator " + name + " of type " + type +
            " is not supported by ADIOS2, in call to DefineOperator\n");
    }

    if (!operatorPtr)
    {
        throw std::invalid_argument(
            "ERROR: Operator " + name + " of type " + type +
            " couldn't be defined, in call to DefineOperator\n");
    }

    return *operatorPtr.get();
}

Operator *ADIOS::InquireOperator(const std::string &name) noexcept
{
    std::shared_ptr<Operator> *op = helper::InquireKey(name, m_Operators);
    if (op == nullptr)
    {
        return nullptr;
    }
    return op->get();
}

#define declare_type(T)                                                        \
    Operator &ADIOS::DefineCallBack(                                           \
        const std::string name,                                                \
        const std::function<void(const T *, const std::string &,               \
                                 const std::string &, const std::string &,     \
                                 const size_t, const Dims &, const Dims &,     \
                                 const Dims &)> &function,                     \
        const Params &parameters)                                              \
    {                                                                          \
        CheckOperator(name);                                                   \
        std::shared_ptr<Operator> callbackOperator =                           \
            std::make_shared<callback::Signature1>(function, parameters);      \
                                                                               \
        auto itPair = m_Operators.emplace(name, std::move(callbackOperator));  \
        return *itPair.first->second;                                          \
    }

ADIOS2_FOREACH_STDTYPE_1ARG(declare_type)
#undef declare_type

Operator &ADIOS::DefineCallBack(
    const std::string name,
    const std::function<void(void *, const std::string &, const std::string &,
                             const std::string &, const size_t, const Dims &,
                             const Dims &, const Dims &)> &function,
    const Params &parameters)
{
    CheckOperator(name);
    std::shared_ptr<Operator> callbackOperator =
        std::make_shared<callback::Signature2>(function, parameters);

    auto itPair = m_Operators.emplace(name, std::move(callbackOperator));
    return *itPair.first->second;
}

bool ADIOS::RemoveIO(const std::string name)
{
    if (m_IOs.erase(name) == 1)
    {
        return true;
    }

    return false;
}

void ADIOS::RemoveAllIOs() noexcept { m_IOs.clear(); }

// PRIVATE FUNCTIONS
void ADIOS::CheckOperator(const std::string name) const
{
    if (m_Operators.count(name) == 1)
    {
        throw std::invalid_argument(
            "ERROR: Operator with name " + name +
            ", is already defined in either config file "
            "or with call to DefineOperator, name must "
            "be unique, in call to DefineOperator\n");
    }
}

void ADIOS::XMLInit(const std::string &configFileXML)
{
    helper::ParseConfigXML(*this, configFileXML, m_IOs, m_Operators);
}

void ADIOS::YAMLInit(const std::string &configFileYAML)
{
    helper::ParseConfigYAML(*this, configFileYAML, m_IOs, m_Operators);
}

} // end namespace core
} // end namespace adios2
