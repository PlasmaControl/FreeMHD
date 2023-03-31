/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | www.cfmesh.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
     Copyright (C) 2014-2017 Creative Fields, Ltd.
     Copyright (C) 2018-2021 OpenCFD Ltd.
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
    Foam::Module::DynList

Description
    Similar to DynamicList but uses a small fixed list internally to
    improve multi-thread performance.

SourceFiles
    DynListI.H
    DynList.C

\*---------------------------------------------------------------------------*/

#ifndef DynList_H
#define DynList_H

#include "List.H"
#include "FixedList.H"
#include <type_traits>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{

// Forward declarations

template<class T, int SizeMin> class DynList;

template<class T, int SizeMin>
Ostream& operator<<
(
    Ostream& is,
    const DynList<T, SizeMin>& list
);

template<class T, int SizeMin>
Istream& operator>>
(
    Istream& is,
    DynList<T, SizeMin>& list
);


/*---------------------------------------------------------------------------*\
                           Class DynList Declaration
\*---------------------------------------------------------------------------*/

template<class T, int SizeMin = 16>
class DynList
:
    public UList<T>
{
    static_assert(SizeMin > 0, "Invalid min size parameter");

    // Private data

        //- Statically allocated list (used for short lists)
        FixedList<T,SizeMin> shortList_;

        //- List allocated from heap (for longer lists)
        List<T> heapList_;

        //- The capacity (allocated size) of the underlying list.
        label capacity_;


    // Private Member Functions

        //- Verify that size() is within capacity()
        inline void checkAllocation() const;

public:

    // Constructors

        //- Construct null
        inline DynList();

        //- Construct given size
        //  This is inconsistent with DynamicList, which sets the reserve size.
        explicit inline DynList(const label nElem);

        //- Construct given integer size
        //  This is inconsistent with DynamicList, which sets the reserve size.
        #if WM_LABEL_SIZE == 64
        explicit inline DynList(const int32_t nElem)
        :
            DynList(label(nElem))
        {}
        #endif

        //- Construct with given size and value for all elements.
        inline DynList(const label nElem, const T& val);

        //- Construct with given size initializing all elements to zero
        inline DynList(const label nElem, const zero);

        //- Copy construct
        inline DynList(const DynList<T, SizeMin>& list);

        //- Construct from UList. Size set to UList size.
        //  Also constructs from DynList with different sizing parameters.
        explicit inline DynList(const UList<T>& list);

        //- Construct from other ListType
        template<class ListType>
        inline DynList(const ListType& list);

        //- Construct from Istream. Size set to size of list read.
        explicit DynList(Istream& is);


    // Member Functions

    // Access

        //- Normal lower capacity limit - the SizeMin template parameter
        inline label min_size() const;

        //- Size of the underlying storage.
        inline label capacity() const;


    // Edit

        //- Alter the size of the underlying storage.
        //  The addressed size will be truncated if needed to fit, but will
        //  remain otherwise untouched.
        //  Use this or reserve() in combination with append().
        inline void setCapacity(const label nElem);

        //- Alter addressable list size.
        //  New space will be allocated if required.
        //  Use this to resize the list prior to using the operator[] for
        //  setting values (as per List usage).
        inline void setSize(const label nElem);

        //- Alter addressable list size and fill new space with constant.
        inline void setSize(const label nElem, const T& val);

        //- Alter addressable list size.
        //  New space will be allocated if required.
        //  Use this to resize the list prior to using the operator[] for
        //  setting values (as per List usage).
        inline void resize(const label nElem);

        //- Alter addressable list size and fill new space with constant.
        inline void resize(const label nElem, const T& val);

        //- Reserve allocation space for at least this size.
        //  Never shrinks the allocated size, use setCapacity() for that.
        inline void reserve(const label nElem);

        //- Clear the addressed list, i.e. set the size to zero.
        //  Allocated size does not change
        inline void clear();

        //- Clear the list and delete storage.
        inline void clearStorage();

        //- Expand the addressable size to fit the allocated capacity.
        //  Returns the previous addressable size.
        inline label expandStorage();

        //- Shrink the allocated space to the number of elements used.
        //  Returns a reference to the DynList.
        inline DynList<T, SizeMin>& shrink();


    // Member Operators

        //- Append an element at the end of the list
        inline void append(const T& val);

        //- Append an element at the end of the list if it is not yet
        //- present in the list (takes linear time)
        inline void appendUniq(const T& val);

        //- Remove and return the last element. Fatal on an empty list.
        inline T remove();

        //- Remove and return the specified element. Fatal on an empty list.
        //  The place of the removed element is swapped with the last one
        //  in the list, which changes the ordering.
        inline T removeElement(const label i);

        //- Return non-const access to an element, resizing list if necessary
        inline T& newElmt(const label i);

        //- Return non-const access to an element, resizing list if necessary
        inline T& operator()(const label i);

        //- Assignment of all addressed entries to the given value
        inline void operator=(const T& val);

        //- Copy list
        inline void operator=(const DynList<T, SizeMin>& list);

//        //- Copy of another list
//        inline void operator=(const UList<T>& list);


    // IOstream operators

        //- Write DynList to Ostream.
        friend Ostream& operator<< <T, SizeMin>
        (
            Ostream& os,
            const DynList<T, SizeMin>& list
        );

        //- Read from Istream, discarding contents of existing DynList.
        friend Istream& operator>> <T, SizeMin>
        (
            Istream& is,
            DynList<T, SizeMin>& list
        );


    // Housekeeping

        //- Same as appendUniq()
        void appendIfNotIn(const label rowI, const label val)
        {
            this->appendUniq(rowI, val);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "DynListI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "DynList.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
