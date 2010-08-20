#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeTextureMapper2D.h>
#include <vtkvolumeProperty.h>
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

  if ( argc < 2 )
    {
    std::cerr << "Missing Parameters: "
              << argv[0] << std::endl
              << "seedfile(*.txt)"
              << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Opening " << argv[1] << std::endl;

  std::string inputFileName = argv[1];

  vtkSmartPointer<vtkParticleReader> reader =
    vtkSmartPointer<vtkParticleReader>::New();
  reader->SetFileName(inputFileName.c_str());
  reader->Update();
  // create the glyph (a sphere)
  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetRadius(1.);

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

  vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
  cast->SetInput(imgReader->GetOutput());
  cast->SetOutputScalarTypeToUnsignedChar();
  cast->Update();

  vtkSmartPointer<vtkImageData> m_image = imgReader->GetOutput();
  /*
  double m_scalarRange[2];
  image->GetScalarRange(m_scalarRange);
  */
/*
  vtkSmartPointer<vtkLookupTable> cmap =
    vtkSmartPointer<vtkLookupTable>::New();
  cmap->SetNumberOfColors( VTK_UNSIGNED_CHAR_MAX + 1 );
  cmap->SetTableRange( VTK_UNSIGNED_CHAR_MIN, VTK_UNSIGNED_CHAR_MAX );

  double value;
  for (i=0;i<256;++i)
    {
    value = i/m_scalarRange[1];
    cmap->SetTableValue(i,value,value,value,1.0);

    }
*/
// Create transfer function mapping scalar value to opacity
  vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction =
    vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacityTransferFunction->AddPoint(40, VTK_UNSIGNED_CHAR_MIN);
  opacityTransferFunction->AddPoint(VTK_UNSIGNED_CHAR_MAX, 1.0);

// Create transfer function mapping scalar value to color
  vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction =
        vtkSmartPointer<vtkColorTransferFunction>::New();
  colorTransferFunction->AddRGBPoint(0.0,0.0,0.0,0.0);
  colorTransferFunction->AddRGBPoint	(255.0,1.0,1.0,1.0);

// The property describes how the data will look
    vtkSmartPointer<vtkVolumeProperty> volumeProperty =
  vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty->SetColor( colorTransferFunction );
  volumeProperty->SetScalarOpacity(opacityTransferFunction );
  volumeProperty->SetInterpolationTypeToLinear();


  vtkSmartPointer<vtkVolumeTextureMapper2D> volumeMapper =
      vtkSmartPointer<vtkVolumeTextureMapper2D> ::New();
  volumeMapper->SetInput( cast->GetOutput() );


  // The volume holds the mapper and the property and
  // can be used to position/orient the volume
  vtkSmartPointer<vtkVolume> volume =
      vtkSmartPointer<vtkVolume>::New();
  volume->SetMapper	( volumeMapper );
  volume->SetProperty( volumeProperty );




  // render and display the window

  renderer->AddVolume(volume);
  renderer->SetBackground(1, 0.4, 0.7); // Background color green
  renderer->ResetCamera();
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
