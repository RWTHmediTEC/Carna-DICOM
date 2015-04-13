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

#ifndef SERIESVIEW_H_3294808493
#define SERIESVIEW_H_3294808493
#if !CARNAQT_DISABLED

/** \file   SeriesView.h
  * \brief  Defines \ref Carna::dicom::SeriesView.
  */

#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/base/noncopyable.h>
#include <QFrame>
#include <deque>
#include <set>

class QVBoxLayout;

namespace Carna
{

namespace dicom
{



// ----------------------------------------------------------------------------------
// SeriesView
// ----------------------------------------------------------------------------------

class SeriesView : public QFrame
{

    NON_COPYABLE
    Q_OBJECT

public:

    /** \brief
      * Instantiates.
      */
    explicit SeriesView( QWidget* parent = nullptr );

    /** \brief
      * Deletes.
      */
    virtual ~SeriesView();

    const std::set< const Series* >& getSelectedSeries() const;

public slots:

    void addPatient( const Carna::dicom::Patient& );

    void clear();

signals:

    void selectionChanged();

    void seriesSelected( const Carna::dicom::Series& );

    void seriesUnselected( const Carna::dicom::Series& );

    void seriesDoubleClicked( const Carna::dicom::Series& );

private:

    QVBoxLayout* container;

    QWidget* const containerWidget;

    bool rebuildRequired;

    std::deque< const Patient* > patients;

    std::set< const Series* > selectedSeries;

    std::set< ToggleSeriesPreview* > selectedSeriesPreviews;

private slots:

    void rebuild();

    void scheduleRebuild();

    void seriesToggled( Carna::dicom::ToggleSeriesPreview& );

    void seriesDoubleClicked( Carna::dicom::ToggleSeriesPreview& );

}; // SeriesView



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // CARNAQT_DISABLED
#endif // SERIESVIEW_H_3294808493
