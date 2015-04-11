/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/dicom/Series.h>
#include <Carna/dicom/SeriesElement.h>
#include <Carna/base/CarnaException.h>
#include <climits>
#include <algorithm>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// Series :: Details
// ----------------------------------------------------------------------------------

struct Series::Details
{
    Details();
    const static double SPACING_DIRTY;

    OrderedElements elements;
    double zSpacing;
};


const double Series::Details::SPACING_DIRTY = -std::numeric_limits< double >::infinity();


Series::Details::Details()
    : zSpacing( SPACING_DIRTY )
{
}



// ----------------------------------------------------------------------------------
// Series
// ----------------------------------------------------------------------------------

Series::Series( const std::string& name )
    : pimpl( new Details() )
    , name( name )
{
}


Series::~Series()
{
    std::for_each( pimpl->elements.begin(), pimpl->elements.end(), std::default_delete< SeriesElement >() );
}


bool Series::IsCloser::operator()( SeriesElement* e1, SeriesElement* e2 ) const
{
    return e1->zPosition != e2->zPosition ? e1->zPosition < e2->zPosition : e1->fileName < e2->fileName;
}


const Series::OrderedElements& Series::elements() const
{
    return pimpl->elements;
}


void Series::take( SeriesElement* element )
{
    const std::size_t previousSize = pimpl->elements.size();
    pimpl->elements.insert( element );
    if( previousSize != pimpl->elements.size() )
    {
        element->putInto( *this );
        pimpl->zSpacing = Details::SPACING_DIRTY;
    }
}


double Series::spacingZ() const
{
    CARNA_ASSERT( pimpl->elements.size() >= MINIMUM_ELEMENTS_COUNT );
    if( pimpl->zSpacing == Details::SPACING_DIRTY )
    {
        auto itr = pimpl->elements.begin();
        pimpl->zSpacing  = ( **itr ).zPosition;
        pimpl->zSpacing -= ( **( ++itr ) ).zPosition;
        pimpl->zSpacing *= -1;
    }
    return pimpl->zSpacing;
}


const SeriesElement& Series::centralElement() const
{
    CARNA_ASSERT( pimpl->elements.size() >= MINIMUM_ELEMENTS_COUNT );

    const auto& last = **( --pimpl->elements.end() );
    const auto& first = **( pimpl->elements.begin() );

    const double z_max = last.zPosition;
    const double z_min = first.zPosition;

    CARNA_ASSERT( z_max >= z_min );

    const double z_mid = z_min + ( z_max - z_min ) / 2;

    double min_z_mid_distance = std::numeric_limits< double >::max();
    SeriesElement* mid_closest_element = nullptr;
    for( auto current_itr = pimpl->elements.begin(); current_itr != pimpl->elements.end(); ++current_itr )
    {
        const double z_mid_distance = std::abs( ( **current_itr ).zPosition - z_mid );
        if( z_mid_distance > min_z_mid_distance )
        {
            break;
        }
        else
        {
            mid_closest_element = *current_itr;
            min_z_mid_distance = z_mid_distance;
        }
    }

    return *mid_closest_element;
}


bool Series::contains( const std::string& fileName ) const
{
    for( auto itr = pimpl->elements.begin(); itr != pimpl->elements.end(); ++itr )
    {
        if( ( **itr ).fileName == fileName )
        {
            return true;
        }
    }
    return false;
}



}  // namespace Carna :: dicom

}  // namespace Carna
