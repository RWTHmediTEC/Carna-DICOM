#include <Carna/dicom/DICOMController.h>
#include <Carna/qt/Application.h>
#include <Carna/qt/Display.h>
#include <Carna/qt/FrameRendererFactory.h>
#include <Carna/presets/DRRStage.h>
#include <Carna/presets/CameraShowcaseControl.h>
#include <Carna/presets/PerspectiveControl.h>
#include <Carna/base/Node.h>
#include <Carna/base/Camera.h>
#include "Demo.h"



// ----------------------------------------------------------------------------------
// Demo
// ----------------------------------------------------------------------------------

Demo::Demo( Carna::dicom::DICOMController& dicomLoader )
    : dicomLoader( dicomLoader )
{
}


void Demo::load()
{
    using namespace Carna;
    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;
    
    /* Define a scene.
     */
    gridHelper.reset( dicomLoader.takeLoadedVolumeGrid() );
    root.reset( new base::Node() );
    base::Camera* const cam = new base::Camera();
    cam->localTransform = base::math::translation4f( 0, 0, 350.f );
    root->attachChild( cam );
    root->attachChild( gridHelper->createNode( GEOMETRY_TYPE_VOLUMETRIC, dicomLoader.loadedVolumeGridSpacing() ) );

    /* Define DRR renderer.
     */
    qt::FrameRendererFactory* const frFactory = new qt::FrameRendererFactory();
    frFactory->appendStage( new presets::DRRStage( GEOMETRY_TYPE_VOLUMETRIC ) );
    display.reset( new qt::Display( frFactory ) );
    display->setCamera( *cam );
    display->setCameraControl( new base::Composition< base::CameraControl >( new presets::CameraShowcaseControl() ) );
    display->setProjectionControl( new base::Composition< base::ProjectionControl >( new presets::PerspectiveControl() ) );

    /* Start rendering.
     */
    display->show();
    dicomLoader.close();
}



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

int main( int argc, char** argv )
{
    Carna::qt::Application app( argc, argv );

    Carna::dicom::DICOMController dicomLoader;
    dicomLoader.show();

    Demo demo( dicomLoader );
    QObject::connect( &dicomLoader, SIGNAL( volumeGridLoaded() ), &demo, SLOT( load() ) );

    return QApplication::exec();
}
