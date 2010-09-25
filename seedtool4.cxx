#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkImageResample.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
//#include <vtkVolumeTextureMapper2D.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkParticleReader.h>
#include <vtkGlyph3DMapper.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <sstream>

int main(int argc, char* argv [] )
{

  if ( argc < 3 )
    {
    std::cerr << "Missing Parameters: "
              << argv[0] << std::endl
              << "seedfile(*.txt) image(itkvtkimage) opacity:min(double)-max(double) color:min(double)-max(double)"
              << std::endl;
    return EXIT_FAILURE;
    }



  std::cout << "Opening " << argv[1] << std::endl;

  std::string inputFileName = argv[1];

  vtkSmartPointer<vtkParticleReader> reader =
    vtkSmartPointer<vtkParticleReader>::New();
  reader->SetFileName(inputFileName.c_str());
  reader->Update();
  // create the glyph (a sphere), very low res
  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetRadius(1.);
  sphereSource->SetThetaResolution(3);
  sphereSource->SetPhiResolution(6);

  // create the glyph mapper
  vtkSmartPointer<vtkGlyph3DMapper> glyph =
    vtkSmartPointer<vtkGlyph3DMapper>::New();
  glyph->SetInputConnection(reader->GetOutputPort());
  glyph->SetSourceConnection(sphereSource->GetOutputPort());
  glyph->SetColorModeToMapScalars();
  glyph->SetScaleModeToNoDataScaling();

  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  //Create an actor
  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(glyph);
  renderer->AddActor(actor);


  // read image dataset
  vtkSmartPointer<vtkImageReader2Factory> r_factory =
    vtkSmartPointer<vtkImageReader2Factory>::New();
  vtkSmartPointer<vtkImageReader2> imgReader =
    r_factory->CreateImageReader2(argv[2]);

  imgReader->SetFileName(argv[2]);
  imgReader->Update();



//  vtkSmartPointer<vtkImageResample> resample =
//      vtkSmartPointer<vtkImageResample>::New();
//     {
//  resample->SetInputConnection( imgReader->GetOutputPort() );
//  resample->SetAxisMagnificationFactor(0, 1.);
//  resample->SetAxisMagnificationFactor(1, 1.);
//  resample->SetAxisMagnificationFactor(2, 1.);
//  }



  vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
  cast->SetInputConnection(imgReader->GetOutputPort());
  cast->SetOutputScalarTypeToUnsignedChar();
  cast->Update();




//  vtkSmartPointer<vtkLookupTable> cmap =
//    vtkSmartPointer<vtkLookupTable>::New();
//  cmap->SetNumberOfColors( VTK_UNSIGNED_CHAR_MAX + 1 );
//  cmap->SetTableRange( VTK_UNSIGNED_CHAR_MIN, VTK_UNSIGNED_CHAR_MAX );

//  double value;
//  for (i=0;i<256;++i)
//    {
//    value = i/m_scalarRange[1];
//    cmap->SetTableValue(i,value,value,value,1.0);

//    }

// Create transfer function mapping scalar value to opacity
  vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction =
    vtkSmartPointer<vtkPiecewiseFunction>::New();

  if (argc>=5)
    {
    int minop = atoi(argv[3]);
    int maxop = atoi(argv[4]);
    opacityTransferFunction->AddPoint(minop, 0.);
    opacityTransferFunction->AddPoint(maxop, 1.0);
    }
  else
    {
    opacityTransferFunction->AddPoint(10, 0.);
    opacityTransferFunction->AddPoint(150, 1.0);
    }
  opacityTransferFunction->AddPoint(255, 1.0);

// Create transfer function mapping scalar value to color
  vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction =
        vtkSmartPointer<vtkColorTransferFunction>::New();

  if (argc==7)
    {
    int mincol = atoi(argv[5]);
    int maxcol = atoi(argv[6]);
    colorTransferFunction->AddRGBPoint(mincol,0.0,0.0,0.0);
    colorTransferFunction->AddRGBPoint	(maxcol,1.0,1.0,1.0);
    }
  else
    {
    colorTransferFunction->AddRGBPoint(0.0,0.0,0.0,0.0);
    colorTransferFunction->AddRGBPoint	(200,1.0,1.0,1.0);
    }
  colorTransferFunction->AddRGBPoint(0.0,0.0,0.0,0.0);
  colorTransferFunction->AddRGBPoint	(200,1.0,1.0,1.0);

// The property describes how the data will look
    vtkSmartPointer<vtkVolumeProperty> volumeProperty =
  vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty->SetColor( colorTransferFunction );
  volumeProperty->SetScalarOpacity(opacityTransferFunction );
  volumeProperty->SetInterpolationTypeToLinear();;


vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper =
      vtkSmartPointer<vtkSmartVolumeMapper> ::New();
volumeMapper->SetInputConnection( cast->GetOutputPort() );
volumeMapper->SetInterpolationModeToNearestNeighbor();
//volumeMapper->SetRequestedRenderMode(volumeMapper->GPURenderMode);

  // The volume holds the mapper and the property and
  // can be used to position/orient the volume
  vtkSmartPointer<vtkVolume> volume =
      vtkSmartPointer<vtkVolume>::New();
  volume->SetMapper	( volumeMapper );
  volume->SetProperty( volumeProperty );


  // render and display the window
  renderer->AddVolume(volume);


  renderer->SetBackground(0., 0., 0.); // Background color green
  renderer->ResetCamera();
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();
  return EXIT_SUCCESS;
}
