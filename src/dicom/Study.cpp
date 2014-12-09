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

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// Study
// ----------------------------------------------------------------------------------

Study::Study( const std::string& name )
    : name( name )
{
}


Study::~Study()
{
    std::for_each( series.begin(), series.end(), std::default_delete< Series >() );
}


const std::deque< Series* >& Study::getSeries() const
{
    return series;
}


void Study::put( Series* series )
{
    this->series.push_back( series );
}



}  // namespace Carna :: dicom

}  // namespace Carna
