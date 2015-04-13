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

#ifndef SERIESPREVIEW_H_3294808493
#define SERIESPREVIEW_H_3294808493
#if !CARNAQT_DISABLED

/** \file   SeriesPreview.h
  * \brief  Defines \ref Carna::dicom::SeriesPreview.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QFrame>

class QLabel;
class QImage;

namespace Carna
{

namespace dicom
{

class Series;



// ----------------------------------------------------------------------------------
// SeriesPreview
// ----------------------------------------------------------------------------------

class SeriesPreview : public QFrame
{

    NON_COPYABLE
    Q_OBJECT

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    explicit SeriesPreview( QWidget* parent = nullptr );

    /** \brief
      * Releases acquired resources.
      */
    virtual ~SeriesPreview();

    void setSeries( const Series& );

    const Series& series() const;

    bool hasSeries() const;

}; // SeriesPreview



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
#endif // SERIESPREVIEW_H_3294808493
