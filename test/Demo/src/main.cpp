#include <Carna/dicom/DICOMController.h>
#include <Carna/qt/Application.h>



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

int main( int argc, char** argv )
{
    Carna::qt::Application app( argc, argv );

    Carna::dicom::DICOMController ctrl;
    ctrl.show();

    return QApplication::exec();
}
