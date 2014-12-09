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

#ifndef UINT16VOLUMECOMPUTATIONWORKER_H_6014714286
#define UINT16VOLUMECOMPUTATIONWORKER_H_6014714286

#include <QThread>
#include <vector>
#include <Carna/base/model/UInt16VolumeBaseWorker.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/CarnaException.h>

namespace Carna
{

namespace dicom
{

class DicomSeries;



// ----------------------------------------------------------------------------------
// UInt16VolumeComputationWorker
// ----------------------------------------------------------------------------------

class UInt16VolumeComputationWorker : public Carna::base::model::UInt16VolumeBaseWorker
{

    Q_OBJECT

public:

    UInt16VolumeComputationWorker
        ( const DicomSeries& dicomSeries
        , const base::Vector3ui& size
        , base::model::UInt16VolumeBaseWorker::DestinationBuffer& dst );


public slots:

    virtual void run() override;


private:

    const DicomSeries& dicomSeries;
    const base::Vector3ui size;

}; // UInt16VolumeComputationWorker



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // UINT16VOLUMECOMPUTATIONWORKER_H_6014714286
