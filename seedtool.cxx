#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

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

  // each read line goes in this temporary variable
  std::string line;
  // cool C++ way to parse strings
  std::stringstream sStream;
  // we create a vector of size 3 containing zeros
  std::vector<double> pt (3,0.);
  // float for the confidence value (should be in ]0;1])
  float fConfidence;

  std::ifstream seedFile (argv[1]);
  if ( !(seedFile.is_open() ) )
    {
    std::cerr << "Unable to open file";
    return EXIT_FAILURE;
    }

  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  //We go through the file
  while (! seedFile.eof() )
    {
    getline (seedFile,line);
    if (line != "") // if line contains something interresting
      {
      sStream.clear();
      sStream.str("");
      sStream.str(line);
      // we read each line of the file, hooping that it contains:
      //confidence [space] Xposition [space] Yposition [space] Zposition
      sStream >> fConfidence
              >> pt[0]
              >> pt[1]
              >> pt[2];

      //Create a sphere
      vtkSmartPointer<vtkSphereSource> sphereSource =
        vtkSmartPointer<vtkSphereSource>::New();
      sphereSource->SetCenter(pt[0], pt[1], pt[2]);
      sphereSource->SetRadius(2.0);

      //Create a mapper and actor
      vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
      mapper->SetInputConnection(sphereSource->GetOutputPort());

      vtkSmartPointer<vtkActor> actor =
        vtkSmartPointer<vtkActor>::New();
      actor->SetMapper(mapper);
      renderer->AddActor(actor);

      std::cout <<" creating sphere at " << std::endl
                << " X= "<< pt[0] << std::endl
                << " Y= "<< pt[1] << std::endl
                << " Z= "<< pt[2] << std::endl
                << " Conf= "<< fConfidence <<std::endl;
      }
    }
  seedFile.close();


  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);


  // render and display the window
  renderer->SetBackground(.3, .6, .3); // Background color green
  renderer->ResetCamera();
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
