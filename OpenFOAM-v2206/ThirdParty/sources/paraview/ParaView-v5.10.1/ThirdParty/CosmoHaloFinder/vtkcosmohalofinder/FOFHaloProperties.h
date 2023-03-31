/*=========================================================================

Copyright (c) 2007, Los Alamos National Security, LLC

All rights reserved.

Copyright 2007. Los Alamos National Security, LLC.
This software was produced under U.S. Government contract DE-AC52-06NA25396
for Los Alamos National Laboratory (LANL), which is operated by
Los Alamos National Security, LLC for the U.S. Department of Energy.
The U.S. Government has rights to use, reproduce, and distribute this software.
NEITHER THE GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY, LLC MAKES ANY WARRANTY,
EXPRESS OR IMPLIED, OR ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.
If software is modified to produce derivative works, such modified software
should be clearly marked, so as not to confuse it with the version available
from LANL.

Additionally, redistribution and use in source and binary forms, with or
without modification, are permitted provided that the following conditions
are met:
-   Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
-   Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
-   Neither the name of Los Alamos National Security, LLC, Los Alamos National
    Laboratory, LANL, the U.S. Government, nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS NATIONAL SECURITY, LLC OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

// .NAME FOFHaloProperties - calculate properties of all FOF halos
//
// FOFHaloProperties takes data from CosmoHaloFinderP about individual halos
// and data from all particles and calculates properties.
//

#ifndef FOFHaloProperties_h
#define FOFHaloProperties_h

#include "vtkCosmoHaloFinderModule.h"

#include "Definition.h"

#include "ChainingMesh.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

namespace cosmotk {


class VTKCOSMOHALOFINDER_EXPORT FOFHaloProperties {

public:
  FOFHaloProperties();
  ~FOFHaloProperties();

  // Set parameters for sizes of the dead/alive space
  void setParameters(const string& outName,  // Base name of output halo files
                     POSVEL_T rL,            // Physical box size
                     POSVEL_T deadSize,      // Dead size
                     POSVEL_T bb);           // Inter particle distance

  // Set alive particle vectors which were created elsewhere
  void setParticles(long count,
                    POSVEL_T* xLoc,
                    POSVEL_T* yLoc,
                    POSVEL_T* zLoc,
                    POSVEL_T* xVel,
                    POSVEL_T* yVel,
                    POSVEL_T* zVel,
                    POSVEL_T* pmass,
                    POTENTIAL_T* potential,
                    ID_T* id,
                    MASK_T* mask,
                    STATUS_T* state);

  void setParticles(long count,
                    POSVEL_T* xLoc,
                    POSVEL_T* yLoc,
                    POSVEL_T* zLoc,
                    POSVEL_T* xVel,
                    POSVEL_T* yVel,
                    POSVEL_T* zVel,
                    POSVEL_T* pmass,
                    ID_T* id);

  // Set the halo information from the FOF halo finder
  void setHalos(int  numberOfHalos,// Number of halos found
                int* halos,        // Index into haloList of first particle
                int* haloCount,    // Number of particles in the matching halo
                int* haloList);    // Chain of indices of all particles in halo

  // Find the halo centers (minimum potential) finding minimum of array
  void FOFHaloCenterMinimumPotential(vector<int>* haloCenter);

  // Find the mass of each halo by accumulating individual particle masses
  void FOFHaloMass(vector<POSVEL_T>* haloMass);

  // Find the average position of FOF halo particles
  void FOFPosition(vector<POSVEL_T>* xPos,
                   vector<POSVEL_T>* yPos,
                   vector<POSVEL_T>* zPos);

  // Find the center of mass of FOF halo particles
  void FOFCenterOfMass(vector<POSVEL_T>* xCofMass,
                       vector<POSVEL_T>* yCofMass,
                       vector<POSVEL_T>* zCofMass);

  // Find the average velocity of FOF halo particles
  void FOFVelocity(vector<POSVEL_T>* xVel,
                   vector<POSVEL_T>* yVel,
                   vector<POSVEL_T>* zVel);

  // Find the velocity dispersion of FOF halos
  void FOFVelocityDispersion(vector<POSVEL_T>* xVel,
                             vector<POSVEL_T>* yVel,
                             vector<POSVEL_T>* zVel,
                             vector<POSVEL_T>* velDisp);

  // Kahan summation of floating point numbers to reduce roundoff error
  POSVEL_T KahanSummation(int halo, POSVEL_T* data);
  POSVEL_T KahanSummation2(int halo, POSVEL_T* data1, POSVEL_T* data2);

  // Dot product
  POSVEL_T dotProduct(POSVEL_T x, POSVEL_T y, POSVEL_T z);

  // Incremental mean, possibly needed for very large halos
  POSVEL_T incrementalMean(int halo, POSVEL_T* data);

  // Extract locations and tags for all particles in a halo
  void extractLocation(int halo,
                       int* actualIndx,
                       POSVEL_T* xLocHalo,
                       POSVEL_T* yLocHalo,
                       POSVEL_T* zLocHalo,
                       ID_T* tag);

  void extractInformation(int halo,
                          int* actualIndx,
                          POSVEL_T* xLocHalo,
                          POSVEL_T* yLocHalo,
                          POSVEL_T* zVelHalo,
                          POSVEL_T* xVelHalo,
                          POSVEL_T* yVelHalo,
                          POSVEL_T* zLocHalo,
                          POSVEL_T* pmass,
                          ID_T* tag);

  // Print information about halos for debugging and selection
  void FOFHaloCatalog(vector<int>* haloCenter,
                      vector<POSVEL_T>* haloMass,
                      vector<POSVEL_T>* xVel,
                      vector<POSVEL_T>* yVel,
                      vector<POSVEL_T>* zVel);

  void printHaloSizes(int minSize);
  void printLocations(int haloIndex);
  void printBoundingBox(int haloIndex);

private:
  int    myProc;                // My processor number
  int    numProc;               // Total number of processors

  string outFile;               // File of particles written by this processor

  POSVEL_T boxSize;             // Physical box size of the data set
  POSVEL_T deadSize;            // Border size for dead particles
  POSVEL_T bb;                  // Interparticle distance for halos

  long   particleCount;         // Total particles on this processor

  POSVEL_T* xx;                 // X location for particles on this processor
  POSVEL_T* yy;                 // Y location for particles on this processor
  POSVEL_T* zz;                 // Z location for particles on this processor
  POSVEL_T* vx;                 // X velocity for particles on this processor
  POSVEL_T* vy;                 // Y velocity for particles on this processor
  POSVEL_T* vz;                 // Z velocity for particles on this processor
  POSVEL_T* mass;               // mass of particles on this processor
  POTENTIAL_T* pot;             // Particle potential
  ID_T* tag;                    // Id tag for particles on this processor
  MASK_T* mask;                 // Particle information
  STATUS_T* status;             // Particle is ALIVE or labeled with neighbor
                                // processor index where it is ALIVE

  // Information about halos from FOF halo finder
  int  numberOfHalos;           // Number of halos found
  int* halos;                   // First particle index into haloList
  int* haloCount;               // Size of each halo
  int* haloList;                // Indices of next particle in halo
};

} // END namespace cosmotk
#endif
