#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/qt/CarnaQt.h>
#include <Carna/qt/Display.h>
#include <Carna/qt/DRRControl.h>
#include <Carna/helpers/VolumeGridHelper.h>
#include <Carna/base/Node.h>
#include <QObject>
#include <memory>

// ----------------------------------------------------------------------------------
// Require CarnaQt to be enabled
// ----------------------------------------------------------------------------------

#if !CARNAQT_ENABLED
#error This demo requires CarnaQt to be enabled!
#endif

// ----------------------------------------------------------------------------------
// Demo
// ----------------------------------------------------------------------------------

class Demo : public QObject
{

    Q_OBJECT

    Carna::dicom::DICOMBrowser& dicomLoader;
    std::unique_ptr< Carna::helpers::VolumeGridHelperBase > gridHelper;
    std::unique_ptr< Carna::qt::Display > display;
    std::unique_ptr< Carna::qt::DRRControl > control;
    std::unique_ptr< Carna::base::Node > root;

public:

    Demo( Carna::dicom::DICOMBrowser& dicomLoader );

public slots:

    void load();

}; // Demo
