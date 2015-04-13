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

#include <Carna/dicom/CarnaDICOM.h>
#if !CARNAQT_DISABLED

#include <Carna/dicom/AsyncVolumeGridFactory.h>
#include <Carna/dicom/VolumeGridFactory.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// AsyncVolumeGridFactory :: Details
// ----------------------------------------------------------------------------------

struct AsyncVolumeGridFactory::Details : public VolumeGridFactoryBase::Progress
{
    VolumeGridFactory< base::HUVolumeUInt16, base::NormalMap3DInt8 > factoryWithNormals;
    VolumeGridFactory< base::HUVolumeUInt16, void > factoryWithoutNormals;
    helpers::VolumeGridHelperBase* result;
    const Series* series;
    bool normals;

    const std::unique_ptr< base::math::Vector3f > spacing;

    Details( AsyncVolumeGridFactory& self );
    AsyncVolumeGridFactory& self;

    virtual void setTotalSlicesCount( unsigned int ) override;
    virtual void setProcessedSlicesCount( unsigned int ) override;
};


AsyncVolumeGridFactory::Details::Details( AsyncVolumeGridFactory& self )
    : result( nullptr )
    , series( nullptr )
    , normals( true )
    , spacing( new base::math::Vector3f() )
    , self( self )
{
}


void AsyncVolumeGridFactory::Details::setTotalSlicesCount( unsigned int slices )
{
    self.workAmountChanged( static_cast< int >( slices ) );
}


void AsyncVolumeGridFactory::Details::setProcessedSlicesCount( unsigned int slices )
{
    self.workAmountDone( static_cast< int >( slices ) );
}



// ----------------------------------------------------------------------------------
// AsyncVolumeGridFactory
// ----------------------------------------------------------------------------------

AsyncVolumeGridFactory::AsyncVolumeGridFactory()
    : pimpl( new Details( *this ) )
{
}


AsyncVolumeGridFactory::~AsyncVolumeGridFactory()
{
    reset();
}


Carna::helpers::VolumeGridHelperBase* AsyncVolumeGridFactory::takeLoadedVolumeGrid()
{
    Carna::helpers::VolumeGridHelperBase* const result = pimpl->result;
    CARNA_ASSERT( result != nullptr );
    pimpl->result = nullptr;
    return result;
}


const base::math::Vector3f& AsyncVolumeGridFactory::spacing() const
{
    return *pimpl->spacing;
}


void AsyncVolumeGridFactory::reset()
{
    Carna::helpers::VolumeGridHelperBase* const result = pimpl->result;
    if( result != nullptr )
    {
        delete result;
        pimpl->result = nullptr;
    }
}


void AsyncVolumeGridFactory::setSeries( const Series& series )
{
    pimpl->series = &series;
}


void AsyncVolumeGridFactory::setNormals( bool normals )
{
    pimpl->normals = normals;
}


void AsyncVolumeGridFactory::load()
{
    CARNA_ASSERT( pimpl->series != nullptr );
    reset();
    if( pimpl->normals )
    {
        helpers::VolumeGridHelper< base::HUVolumeUInt16, base::NormalMap3DInt8 >* const result
            = pimpl->factoryWithNormals.loadSeries( *pimpl->series, *pimpl );
        pimpl->result = result;
        *pimpl->spacing = pimpl->factoryWithNormals.spacing();
        emit workAmountChanged( 0 );
        emit workAmountDone( 0 );
        emit workHintChanged( QString( "Computing normals..." ) );
        result->computeNormals();
    }
    else
    {
        pimpl->result = pimpl->factoryWithoutNormals.loadSeries( *pimpl->series, *pimpl );
        *pimpl->spacing = pimpl->factoryWithoutNormals.spacing();
    }
    emit finished();
}



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
