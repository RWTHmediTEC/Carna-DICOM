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


void Study::put( Series* series )
{
    pimpl->series.push_back( series );
}



}  // namespace Carna :: dicom

}  // namespace Carna
