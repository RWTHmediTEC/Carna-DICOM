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

#ifndef TOGGLESERIESPREVIEW_H_3294808493
#define TOGGLESERIESPREVIEW_H_3294808493
#if !CARNAQT_DISABLED

/** \file   ToggleSeriesPreview.h
  * \brief  Defines \ref Carna::dicom::ToggleSeriesPreview.
  */

#include <Carna/dicom/SeriesPreview.h>

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// ToggleSeriesPreview
// ----------------------------------------------------------------------------------

class ToggleSeriesPreview : public SeriesPreview
{

    Q_OBJECT

public:

    /** \brief
      * Instantiates.
      */
    explicit ToggleSeriesPreview( QWidget* parent = nullptr );

    /** \brief
      * Deletes.
      */
    virtual ~ToggleSeriesPreview();

    bool isToggled() const;

public slots:

    void setToggled( bool );

    void toggle();

signals:

    void toggled();

    void toggled( bool );

    void toggled( Carna::dicom::ToggleSeriesPreview& );

    void doubleClicked();

    void doubleClicked( Carna::dicom::ToggleSeriesPreview& );

protected:

    virtual void mousePressEvent( QMouseEvent* ) override;

    virtual void mouseDoubleClickEvent( QMouseEvent* ) override;

private:

    bool amToggled;

}; // ToggleSeriesPreview



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
#endif // TOGGLESERIESPREVIEW_H_3294808493
