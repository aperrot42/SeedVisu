#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkParticleReader.h>
#include <vtkGlyph3D.h>
#include <vtkLookupTable.h>

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

    vtkSmartPointer<vtkSphereSource> sphereSource =
      vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(1.);
    vtkSmartPointer<vtkGlyph3D> glyph =
      vtkSmartPointer<vtkGlyph3D>::New();
    glyph->SetInput(reader->GetOutput());
    glyph->SetSource(sphereSource->GetOutput());
    glyph->SetScaleModeToDataScalingOff();
    glyph->SetColorModeToColorByScalar();


    //define LookUp Table

    //Create a renderer, render window, and interactor

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

    //Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(glyph->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  renderer->AddActor(actor);





  // render and display the window
  renderer->SetBackground(1, 0.4, 0.7); // Background color green
  renderer->ResetCamera();
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
