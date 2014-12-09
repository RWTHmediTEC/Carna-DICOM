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

#ifndef DICOMCONTROLLER_H_6014714286
#define DICOMCONTROLLER_H_6014714286

/** \file   DicomController.h
  * \brief  Defines \ref Carna::dicom::DicomController.
  */

#include <Carna/Carna.h>
#include <Carna/base/noncopyable.h>
#include <QWidget>

class QDoubleSpinBox;
class QPushButton;
class QThread;
class QLabel;

namespace Carna
{

namespace dicom
{

class DicomManager;
class DicomExtractionSettings;
class SeriesView;
class SeriesLoadingRequest;



// ----------------------------------------------------------------------------------
// DicomController
// ----------------------------------------------------------------------------------

/** \brief  Visual component for managing DICOM series and selecting for loading.
  *
  * \image html DicomController01.png
  *
  * Usage example:
  *
  * \code
  * #include <QObject>
  * #include <Carna/dicom/DicomController.h>
  * #include <Carna/dicom/SeriesLoadingRequest.h>
  * #include <Carna/base/model/Scene.h>
  * #include <Carna/base/model/SceneFactory.h>
  *
  * using Carna::dicom::DicomController;
  * using Carna::dicom::SeriesLoadingRequest;
  * using Carna::base::model::Scene;
  * using Carna::base::model::SceneFactory;
  *
  * class Loader : public QObject
  * {
  *
  *     Q_OBJECT
  *
  * public:
  *
  *     Loader( QWidget* dialogParent = nullptr )
  *         : modelFactory( new SceneFactory( dialogParent ) )
  *     {
  *         DicomController* controller = new DicomController();
  *         controller->show();
  *         
  *         connect( controller
  *                , SIGNAL( const Carna::dicom::SeriesLoadingRequest& )
  *                , this
  *                , SLOT( load( const Carna::dicom::SeriesLoadingRequest& ) ) );
  *     }
  *
  * signals:
  *
  *     void modelLoaded( const Scene* );
  *
  * private slots:
  *
  *     void load( const SeriesLoadingRequest& request )
  *     {
  *         const Scene* model = CarmodelFactory.createFromRequest( request );
  *         if( model != nullptr )
  *         {
  *             emit modelLoaded( model );
  *         }
  *     }
  *
  * private:
  *
  *     std::unique_ptr< SceneFactory > modelFactory;
  *
  * } // Loader
  * \endcode
  *
  * You might also want put the \c DicomController into a \c QDialog.
  *
  * \author Leonid Kostrykin
  * \date   5.10.12 - 8.10.12
  */
class CARNA_DICOM_LIB DicomController : public QWidget
{

    NON_COPYABLE

    Q_OBJECT

public:

    /** \brief  Instantiates.
      */
    DicomController();

    /** \brief  Releases acquired resources.
      */
    ~DicomController();


signals:

    /** \cond   0
      */
    void openDirectory( const QString& );
    void openIndex( const QString& );
    void saveIndex( const QString& );
    void extractSeries( const Carna::dicom::DicomExtractionSettings& );
    /** \endcond
      */
    
    /** \brief  The user has intended to load some DICOM series.
      */
    void seriesLoadingRequested( const Carna::dicom::SeriesLoadingRequest& );


private:

    SeriesView* const seriesView;

    QLabel* const laSpacingZ;
    QDoubleSpinBox* const sbSpacingZ;

    QPushButton* const buSaveIndex;
    QPushButton* const buExtract;
    QPushButton* const buLoad;

    QThread* const managerThread;
    DicomManager* const manager;


private slots:

    void openDirectory();

    void openIndex();

    void saveIndex();

    void seriesLoaded();

    void seriesLoadingFailed( const QString& );

    void seriesLoadingAborted();

    void closeSeries();

    void indexSavingFailed( const QString& );

    void extractionFailed( const QString& );

    void selectionChanged();

    void seriesSelected( const Carna::dicom::Series& );

    void seriesDoubleClicked( const Carna::dicom::Series& );

    void extractSeries();

    void loadSeries();

}; // DicomController



}  // namespace Carna :: dicom

}  // namespace Carna

#endif // DICOMCONTROLLER_H_6014714286
