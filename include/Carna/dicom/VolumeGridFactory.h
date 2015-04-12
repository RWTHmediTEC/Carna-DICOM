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

#ifndef VOLUMEGRIDFACTORY_H_3294808493
#define VOLUMEGRIDFACTORY_H_3294808493

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/helpers/VolumeGridHelper.h>

/** \file   VolumeGridFactory.h
  * \brief  Defines \ref Carna::dicom::VolumeGridFactoryBase
  *             and \ref Carna::dicom::VolumeGridFactory.
  */

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// VolumeGridFactoryBase
// ----------------------------------------------------------------------------------

/** \brief
  * Defines type-parameters-independent \ref VolumeGridFactory base interface.
  *
  * \author Leonid Kostrykin
  * \date   9.4.15
  */
class CARNADICOM_LIB VolumeGridFactoryBase
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    VolumeGridFactoryBase();

    virtual ~VolumeGridFactoryBase();
    
    void setMaximumSegmentBytesize( std::size_t maximumSegmentBytesize );
    
    std::size_t maximumSegmentBytesize() const;
    
    /** \brief
      * \todo Write.
      *
      * \returns
      *     an `helpers::VolumeGridHelperBase` object with data loaded from \a series
      *     or `nullptr` if \a series was empty.
      */
    helpers::VolumeGridHelperBase* loadSeries( const Series& series );
    
    base::math::Vector3f spacing() const;

protected:

    virtual helpers::VolumeGridHelperBase* create( const base::math::Vector3ui& nativeResolution ) = 0;
    
    virtual void setHUVoxel( const base::math::Vector3ui& location, base::HUV huv ) = 0;

}; // VolumeGridFactoryBase



// ----------------------------------------------------------------------------------
// VolumeGridFactory
// ----------------------------------------------------------------------------------

/** \brief
  * Instantiates `helpers::VolumeGridHelper` objects from \ref Series "DICOM series".
  *
  * \author Leonid Kostrykin
  * \date   9.4.15
  */
template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
class VolumeGridFactory : public VolumeGridFactoryBase
{

    helpers::VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >* helper;

public:
    
    helpers::VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >* loadSeries( const Series& series );

protected:

    virtual helpers::VolumeGridHelperBase* create( const base::math::Vector3ui& nativeResolution ) override;
    
    virtual void setHUVoxel( const base::math::Vector3ui& location, base::HUV huv ) override;

}; // VolumeGridFactory


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
helpers::VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >* VolumeGridFactory< SegmentHUVolumeType, SegmentNormalsVolumeType >
    ::loadSeries( const Series& series )
{
    VolumeGridFactoryBase::loadSeries( series );
    return helper;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
helpers::VolumeGridHelperBase* VolumeGridFactory< SegmentHUVolumeType, SegmentNormalsVolumeType >
    ::create( const base::math::Vector3ui& nativeResolution )
{
    helper = new helpers::VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >( nativeResolution, maximumSegmentBytesize() );
    return helper;
}


template< typename SegmentHUVolumeType, typename SegmentNormalsVolumeType >
void VolumeGridFactory< SegmentHUVolumeType, SegmentNormalsVolumeType >
    ::setHUVoxel( const base::math::Vector3ui& location, base::HUV huv )
{
    helper->grid().setVoxel< base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::HUVSelector >( location, huv );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // VOLUMEGRIDFACTORY_H_3294808493
