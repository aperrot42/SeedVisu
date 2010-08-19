#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkParticleReader.h>
#include <vtkGlyph3DMapper.h>

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

  // render and display the window
  renderer->SetBackground(1, 0.4, 0.7); // Background color green
  renderer->ResetCamera();
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
