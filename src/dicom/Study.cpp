/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/dicom/Study.h>
#include <Carna/dicom/Series.h>
#include <algorithm>
#include <map>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// Study :: Details
// ----------------------------------------------------------------------------------

struct Study::Details
{
    std::vector< Series* > series;
    std::map< std::string, Series* > seriesByName;
};



// ----------------------------------------------------------------------------------
// Study
// ----------------------------------------------------------------------------------

Study::Study( const std::string& name )
    : pimpl( new Details() )
    , name( name )
{
}


Study::~Study()
{
    std::for_each( pimpl->series.begin(), pimpl->series.end(), std::default_delete< Series >() );
}


const std::vector< Series* >& Study::series() const
{
    return pimpl->series;
}


Series& Study::series( const std::string& name )
{
    const auto itr = pimpl->seriesByName.find( name );
    if( itr == pimpl->seriesByName.end() )
    {
        Series* const series = new Series( name );
        pimpl->seriesByName[ name ] = series;
        pimpl->series.push_back( series );
        return *series;
    }
    else
    {
        return *itr->second;
    }
}



}  // namespace Carna :: dicom

}  // namespace Carna
