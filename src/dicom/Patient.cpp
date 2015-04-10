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

#include <Carna/dicom/Patient.h>
#include <Carna/dicom/Study.h>
#include <algorithm>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// Patient :: Details
// ----------------------------------------------------------------------------------

struct Patient::Details
{
    std::vector< Study* > studies;
};



// ----------------------------------------------------------------------------------
// Patient
// ----------------------------------------------------------------------------------

Patient::Patient( const std::string& name )
    : pimpl( new Details() )
    , name( name )
{
}


Patient::~Patient()
{
    std::for_each( pimpl->studies.begin(), pimpl->studies.end(), std::default_delete< Study >() );
}


const std::vector< Study* >& Patient::studies() const
{
    return pimpl->studies;
}


void Patient::put( Study* study )
{
    pimpl->studies.push_back( study );
}



}  // namespace Carna :: dicom

}  // namespace Carna
