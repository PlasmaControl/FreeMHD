/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | www.cfmesh.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2017 Creative Fields, Ltd.
-------------------------------------------------------------------------------
Author
     Franjo Juretic (franjo.juretic@c-fields.com)

License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::Module::meshSubset

Description
    A class containing labels of elements

SourceFiles
    meshSubsetI.H

\*---------------------------------------------------------------------------*/

#ifndef meshSubset_H
#define meshSubset_H

#include "label.H"
#include "HashSet.H"
#include "VRWGraph.H"

#include <set>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
class Ostream;

namespace Module
{
class VRWGraph;
class meshSubset;
inline Ostream& operator<<(Ostream&, const meshSubset&);
inline Istream& operator>>(Istream&, meshSubset&);

/*---------------------------------------------------------------------------*\
                        class meshSubset Declaration
\*---------------------------------------------------------------------------*/

class meshSubset
{
    // Private data

        //- name of the given subset
        word name_;

        //- type of subset
        label type_;

        //- labels of elements
        //HashSet<label> data_;
        std::set<label> data_;


public:

    // Enumerators

        enum subsetType_
        {
            UNKNOWN = 0,
            CELLSUBSET = 1,
            FACESUBSET = 2,
            POINTSUBSET = 4,
            FEATUREEDGESUBSET = 8
        };


    // Constructors

        //- Null constructor
        inline meshSubset();

        //- Construct from name. Creates an empty subset
        inline meshSubset(const word& name, const subsetType_&);

        //- Construct from name and element labels
        template<class ListType>
        inline meshSubset
        (
            const word& name,
            const subsetType_&,
            const ListType& elements
        );

        //- Copy construct
        inline meshSubset(const meshSubset&);

        //- Construct from Istream
        inline meshSubset(Istream&);


    //- Destructor
    ~meshSubset() = default;


    // Member Functions

        // Access

            //- Return name
            inline const word& name() const;

            //- Type of subset
            inline label type() const;

            //- elements contained in the subset
            template<class ListType>
            inline void containedElements(ListType&) const;


        // Edit

            //- add element label to subset
            inline void addElement(const label);

            //- remove element from subset
            inline void removeElement(const label);

            //- update subset after mesh modification
            //- the list contains new labels of mesh elements
            //- removed elements have a negative label
            template<class ListType>
            inline void updateSubset(const ListType&);

            //- update subset after modification
            //- this modifier is used in case when elements are decomposed
            inline void updateSubset(const VRWGraph&);


        // Searches

            //- find if the element exists in the subset
            inline bool found(const label) const;


    // operators

        inline void operator=(const meshSubset&);
        inline bool operator==(const meshSubset&) const;
        inline bool operator!=(const meshSubset&) const;


    // IOstream operators

        inline friend Ostream& operator<<(Ostream&, const meshSubset&);
        inline friend Istream& operator>>(Istream&, meshSubset&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "meshSubsetI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
