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

#ifndef ASYNCVOLUMEGRIDFACTORY_H_3294808493
#define ASYNCVOLUMEGRIDFACTORY_H_3294808493
#if !CARNAQT_DISABLED

/** \file   AsyncVolumeGridFactory.h
  * \brief  Defines \ref Carna::dicom::AsyncVolumeGridFactory.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/math.h>
#include <QObject>
#include <memory>
#include <vector>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// AsyncVolumeGridFactory
// ----------------------------------------------------------------------------------

class AsyncVolumeGridFactory : public QObject
{

    NON_COPYABLE
    Q_OBJECT

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    AsyncVolumeGridFactory();
    virtual ~AsyncVolumeGridFactory();

    Carna::helpers::VolumeGridHelperBase* takeLoadedVolumeGrid();
    const base::math::Vector3f& spacing() const;

    void setNormals( bool normals );
    void setSeries( const Series& series );

public slots:

    void reset();
    void load();

signals:

    void workAmountChanged( int );
    void workAmountDone( int );
    void workHintChanged( const QString& );
    void finished();

}; // AsyncVolumeGridFactory



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
#endif // ASYNCVOLUMEGRIDFACTORY_H_3294808493
