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

#include <Carna/dicom/Patient.h>
#include <Carna/dicom/Study.h>
#include <algorithm>
#include <map>

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
    std::map< std::string, Study* > studyByName;
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


void Patient::take( Study* study )
{
    CARNA_ASSERT( pimpl->studyByName.find( study->name ) == pimpl->studyByName.end() );
    pimpl->studies.push_back( study );
    pimpl->studyByName[ name ] = study;
}


Study& Patient::study( const std::string& name )
{
    const auto itr = pimpl->studyByName.find( name );
    if( itr == pimpl->studyByName.end() )
    {
        Study* const study = new Study( name );
        pimpl->studyByName[ name ] = study;
        pimpl->studies.push_back( study );
        return *study;
    }
    else
    {
        return *itr->second;
    }
}



}  // namespace Carna :: dicom

}  // namespace Carna
