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

// .NAME CosmoHaloFinderP - find halos within a cosmology data file in parallel
//
// .SECTION Description
// CosmoHaloFinderP takes a series of data files containing .cosmo data
// along with parameters defining the box size for the data and for
// determining halos within the particle data.  It distributes the data
// across processors including a healthy dead zone of particles belonging
// to neighbor processors.  By definition all halos can be determined
// completely for any processor because of this dead zone.  The serial
// halo finder is called on each processor.
//
// Halos returned from the serial halo finder either contain all particles
// interior to this processor (ALIVE), all particles completely in the dead
// zone (DEAD) or a combination (MIXED).
//
// If mixed halos are shared with only one neighbor the rule followed is if
// the halo is in the upper planes of the processor (high values of x,y,z)
// then this processor will keep that halo as alive.  If the halo is in the
// low planes it is given up as dead, with the understanding that the
// adjacent processors will claim it as alive.  When more than two processors
// claim a halo the information is sent to the MASTER processor which
// determines which processor can claim that halo and the other two give
// it up.
//

#ifndef CosmoHaloFinderP_h
#define CosmoHaloFinderP_h

#include "vtkCosmoHaloFinderModule.h"

#include "Definition.h"


#include "CosmoHaloFinder.h"
#include "CosmoHalo.h"

#include <cassert>
#include <string>
#include <vector>

namespace cosmotk {


class VTKCOSMOHALOFINDER_EXPORT CosmoHaloFinderP {
public:
  CosmoHaloFinderP();
  ~CosmoHaloFinderP();

  // Special methods to release memory because results are used in analysis
  void clearHaloTag();
  void clearHaloStart();
  void clearHaloList();
  void clearHaloSize();

  // Set parameters for serial halo finder which does the work
  void setParameters(
        const string& outName,  // Base name of output halo files
        POSVEL_T rL,            // Box size of the physical problem
        POSVEL_T deadSize,      // Dead size used to normalize for non periodic
        long np,                // Number of particles in the problem
        int pmin,               // Minimum number of particles in a halo
        POSVEL_T bb,            // Normalized distance between particles
                                // which define a single halo
        int nmin = 1);          // The minimum number of neighbors for linking

  // Execute the serial halo finder for this processor
  void executeHaloFinder();

  // Collect the halo information from the serial halo finder
  // Save the mixed halos so as to determine which processor owns them
  void collectHalos(bool clearTag = true);
  void buildHaloStructure();
  void processMixedHalos();

  // Write the particles with mass field containing halo tags
  void writeTaggedParticles(int hmin, float ss, int minPerHalo,
                            bool writeAllTags, bool writeAll,
                            bool clearTag = true);

  // Set alive particle vectors which were created elsewhere
  void setParticles(long count,
                    POSVEL_T* xLoc,
                    POSVEL_T* yLoc,
                    POSVEL_T* zLoc,
                    POSVEL_T* xVel,
                    POSVEL_T* yVel,
                    POSVEL_T* zVel,
                    POTENTIAL_T* potential,
                    ID_T* id,
                    MASK_T* mask,
                    STATUS_T* state);

  // Return information needed by halo center finder
  int getNumberOfHalos()        { return (int)this->halos.size(); }
  int* getHalos()               { return &this->halos[0]; }
  int* getHaloCount()           { return &this->haloCount[0]; }
  int* getHaloList()            { return this->haloList; }

  // Description
  // Returns the halo ID of the ith halo in this process
  ID_T getHaloID(const int i)
  {
    assert("pre: i is out-of-bounds" && (i >= 0) && (i < this->getNumberOfHalos() ) );
    int p = this->halos[ i ];
    return( this->tag[ this->haloTag[p] ] );
  }

  inline ID_T getHaloIDForParticle(const long p)
  {
    assert("pre: p is out-of-bounds" && (p >= 0) && (p < this->particleCount));
    return 
        (this->haloSize[this->haloTag[p]] < this->pmin)
        ? -1: this->tag[this->haloTag[p]];
  }

private:
  int    myProc;                // My processor number
  int    numProc;               // Total number of processors

  int    layoutSize[DIMENSION]; // Decomposition of processors
  int    layoutPos[DIMENSION];  // Position of this processor in decomposition

  string outFile;               // File of particles written by this processor

  CosmoHaloFinder haloFinder;   // Serial halo finder for this processor

  POSVEL_T boxSize;             // Physical box size of the data set
  POSVEL_T deadSize;            // Border size for dead particles
  long    np;                   // Number of particles in the problem
  int    pmin;                  // Minimum number of particles in a halo
  POSVEL_T bb;                  // Minimum normalized distance between
                                // particles in a halo
  int    nmin;                  // The minimum number of neighbors for linking

  long   particleCount;         // Running index used to store data
                                // Ends up as the number of alive plus dead

  int    neighbor[NUM_OF_NEIGHBORS];    // Neighbor processor ids
  int    deadParticle[NUM_OF_NEIGHBORS];// Number of neighbor dead particles
  int    deadHalo[NUM_OF_NEIGHBORS];    // Number of neighbor mixed halos

  POSVEL_T* xx;                 // X location for particles on this processor
  POSVEL_T* yy;                 // Y location for particles on this processor
  POSVEL_T* zz;                 // Z location for particles on this processor
  POSVEL_T* vx;                 // X velocity for particles on this processor
  POSVEL_T* vy;                 // Y velocity for particles on this processor
  POSVEL_T* vz;                 // Z velocity for particles on this processor
  POTENTIAL_T* pot;             // Particle potential
  ID_T* tag;                    // Id tag for particles on this processor
  MASK_T* mask;                 // Particle information

  STATUS_T* status;             // Particle is ALIVE or labeled with neighbor
                                // processor index where it is ALIVE

  int* haloTag;                 // From serial halo finder, the index of the
                                // first particle in a halo

  int* haloSize;                // From serial halo finder, the size of a halo
                                // where the first particle has the actual size
                                // and other member particles have size=0
  int* haloAliveSize;

  int numberOfAliveHalos;       // Number of alive or valid halos
  int numberOfDeadHalos;        // Number of dead halos
  int numberOfMixedHalos;       // Number of halos with both alive and dead
  int numberOfHaloParticles;    // Number of particles in all VALID halos

  vector<CosmoHalo*> myMixedHalos;      // Mixed halos on this processor

  vector<int> halos;            // First particle index into haloList
  vector<int> haloCount;        // Size of each halo

  int* haloList;                // Indices of next particle in halo
  int* haloStart;               // Index of first particle in halo
                                // Chain is built backwards but using these two
                                // arrays, all particle indices for a halo
                                // can be found
};

} // END namespace cosmotk

#endif
