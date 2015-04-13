#include <Carna/dicom/CarnaDICOM.h>
#include <Carna/qt/CarnaQt.h>
#include <QObject>
#include <memory>



// ----------------------------------------------------------------------------------
// Demo
// ----------------------------------------------------------------------------------

class Demo : public QObject
{

    Q_OBJECT

    Carna::dicom::DICOMController& dicomLoader;
    std::unique_ptr< Carna::helpers::VolumeGridHelperBase > gridHelper;
    std::unique_ptr< Carna::qt::Display > display;
	std::unique_ptr< Carna::qt::DRRControl > control;
    std::unique_ptr< Carna::base::Node > root;

public:

    Demo( Carna::dicom::DICOMController& dicomLoader );

public slots:

    void load();

}; // Demo
