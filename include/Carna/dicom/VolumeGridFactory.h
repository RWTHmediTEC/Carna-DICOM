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
#include <Carna/base/BufferedHUVolume.h>

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

    /** \brief
      * Instantiates.
      */
    VolumeGridFactoryBase();

    /** \brief
      * Deletes.
      */
    virtual ~VolumeGridFactoryBase();

    /** \brief
      * Defines interface that is notified about the \ref loadSeries progress.
      */
    struct CARNADICOM_LIB Progress
    {
        /** \brief
          * Deletes.
          */
        virtual ~Progress();

        /** \brief
          * Notifies that the total number of DICOM images to be processed has been
          * determined.
          */
        virtual void setTotalSlicesCount( unsigned int ) = 0;

        /** \brief
          * Notifies that the number of DICOM images processed so far has changed.
          */
        virtual void setProcessedSlicesCount( unsigned int ) = 0;
    };
    
    /** \brief
      * Uses this setting on the created `helpers::VolumeGridHelper` when
      * \ref loadSeries is called.
      */
    void setMaximumSegmentBytesize( std::size_t maximumSegmentBytesize );
    
    /** \brief
      * Tells the value that will be used for the created `helpers::VolumeGridHelper`
      * when \ref loadSeries is called.
      */
    std::size_t maximumSegmentBytesize() const;
    
    /** \brief
      * Craetes and returns new ``helpers::VolumeGridHelper` object whose resolution
      * and data adapts \a series.
      *
      * \returns
      *     an `helpers::VolumeGridHelperBase` object with data loaded from \a series
      *     or `nullptr` if \a series was empty.
      */
    helpers::VolumeGridHelperBase* loadSeries( const Series& series, Progress& progress );

    /** \overload
      */
    helpers::VolumeGridHelperBase* loadSeries( const Series& series );
    
    /** \brief
      * Tells the spacing of the \ref loadSeries "series loaded" last. The return
      * value of this method is undefined if \ref loadSeries was not called yet.
      */
    const base::math::Vector3f& spacing() const;

protected:

    /** \brief
      * Instantiates new `helpers::VolumeGridHelper` object from \a nativeResolution.
      */
    virtual helpers::VolumeGridHelperBase* create( const base::math::Vector3ui& nativeResolution ) = 0;
    
    /** \brief
      * Updates the HUV at \a location to \a huv on the `helpers::VolumeGridHelper`
      * object \ref create "created last".
      */
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
    
    /** \copydoc VolumeGridFactoryBase::loadSeries(const Series&)
      */
    helpers::VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >* loadSeries( const Series& series );
    
    /** \copydoc VolumeGridFactoryBase::loadSeries(const Series&, Progress&)
      */
    helpers::VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >* loadSeries( const Series& series, Progress& progress );

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
helpers::VolumeGridHelper< SegmentHUVolumeType, SegmentNormalsVolumeType >* VolumeGridFactory< SegmentHUVolumeType, SegmentNormalsVolumeType >
    ::loadSeries( const Series& series, VolumeGridFactoryBase::Progress& progress )
{
    VolumeGridFactoryBase::loadSeries( series, progress );
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
    helper->grid().template setVoxel
        < typename base::VolumeGrid< SegmentHUVolumeType, SegmentNormalsVolumeType >::HUVSelector >( location, huv );
}



}  // namespace Carna :: base

}  // namespace Carna

#endif // VOLUMEGRIDFACTORY_H_3294808493
