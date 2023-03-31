/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * helloHDF5Reader.cpp
 *
 *  Created on: Jan 24, 2018
 *      Author: Junmin Gu
 */

#include <ios>      //std::ios_base::failure
#include <iostream> //std::cout
#include <mpi.h>
#include <stdexcept> //std::invalid_argument std::exception
#include <vector>

#include <adios2.h>

template <class T>
void ReadData(adios2::IO h5IO, adios2::Engine &h5Reader,
              const std::string &name)
{
    adios2::Variable<T> var = h5IO.InquireVariable<T>(name);

    if (var)
    {
        int nDims = var.Shape().size();
        size_t totalSize = 1;
        for (int i = 0; i < nDims; i++)
        {
            totalSize *= var.Shape()[i];
        }
        std::vector<T> myValues(totalSize);
        // myFloats.data is pre-allocated
        h5Reader.Get<T>(var, myValues.data(), adios2::Mode::Sync);

        // std::cout << "\tValues of "<<name<<": ";
        std::cout << "\tPeek Values: ";

        if (totalSize < 20)
        { // print all
            for (const auto number : myValues)
            {
                std::cout << number << " ";
            }
        }
        else
        {
            size_t counter = 0;
            for (const auto number : myValues)
            {
                if ((counter < 5) || (counter > totalSize - 5))
                {
                    std::cout << number << " ";
                }
                else if (counter == 5)
                {
                    std::cout << " ......  ";
                }
                counter++;
            }
        }
        std::cout << "\n";
    }
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /** Application variable */
    const std::size_t Nx = 10;
    std::vector<float> myFloats(Nx);
    std::vector<int> myInts(Nx);

    const char *filename = "myVector.h5";
    if (argc > 1)
    {
        filename = argv[1];
    }

    std::cout << " Using file: " << filename << std::endl;
    try
    {
        /** ADIOS class factory of IO class objects */
        adios2::ADIOS adios(MPI_COMM_WORLD);

        /*** IO class object: settings and factory of Settings: Variables,
         * Parameters, Transports, and Execution: Engines */
        adios2::IO h5IO = adios.DeclareIO("ReadHDF5");

        h5IO.SetEngine("HDF5");

        /** Engine derived class, spawned to start IO operations */
        adios2::Engine h5Reader = h5IO.Open(filename, adios2::Mode::Read);

        const std::map<std::string, adios2::Params> variables =
            h5IO.AvailableVariables();

        for (const auto variablePair : variables)
        {
            std::cout << "Name: " << variablePair.first;
            std::cout << std::endl;

            for (const auto &parameter : variablePair.second)
            {
                std::cout << "\t" << parameter.first << ": " << parameter.second
                          << "\n";
                if (parameter.second == "double")
                {
                    ReadData<double>(h5IO, h5Reader, variablePair.first);
                }
                else if (parameter.second == "float")
                {
                    ReadData<float>(h5IO, h5Reader, variablePair.first);
                }
                else if (parameter.second == "unsigned int")
                {
                    ReadData<unsigned int>(h5IO, h5Reader, variablePair.first);
                }
                else if (parameter.second == "int")
                {
                    ReadData<int>(h5IO, h5Reader, variablePair.first);
                }
                //... add more types if needed
            }
        }

        const std::map<std::string, adios2::Params> attributes =
            h5IO.AvailableAttributes();

        for (const auto attrPair : attributes)
        {
            std::cout << "AttrName: " << attrPair.first;
            std::cout << std::endl;

            for (const auto &parameter : attrPair.second)
            {
                std::cout << "\t" << parameter.first << ": " << parameter.second
                          << "\n";
                if (parameter.second == "double")
                {
                    // ReadData<double>(h5IO, h5Reader, variablePair.first);
                }
                else if (parameter.second == "float")
                {
                    // ReadData<float>(h5IO, h5Reader, variablePair.first);
                }
                else if (parameter.second == "unsigned int")
                {
                    // ReadData<unsigned int>(h5IO, h5Reader,
                    // variablePair.first);
                }
                else if (parameter.second == "int")
                {
                    // ReadData<int>(h5IO, h5Reader, variablePair.first);
                }
                //... add more types if needed
            }
        }

        h5Reader.Close();
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Invalid argument exception, STOPPING PROGRAM from rank "
                  << rank << "\n";
        std::cout << e.what() << "\n";
    }
    catch (std::ios_base::failure &e)
    {
        std::cout << "IO System base failure exception, STOPPING PROGRAM "
                     "from rank "
                  << rank << "\n";
        std::cout << e.what() << "\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Exception, STOPPING PROGRAM from rank " << rank << "\n";
        std::cout << e.what() << "\n";
    }

    MPI_Finalize();
    return 0;
}
