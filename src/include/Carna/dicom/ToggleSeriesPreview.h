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

#ifndef TOGGLESERIESPREVIEW_H_6014714286
#define TOGGLESERIESPREVIEW_H_6014714286

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

    /** \brief  Instantiates.
      */
    explicit ToggleSeriesPreview( QWidget* parent = nullptr );

    /** \brief  Releases acquired resources.
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

#endif // TOGGLESERIESPREVIEW_H_6014714286
