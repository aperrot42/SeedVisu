cmake_minimum_required(VERSION 2.6)
 
PROJECT(SeedTool)
 
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

ADD_EXECUTABLE(itksdt seedtool.cxx)
 
TARGET_LINK_LIBRARIES(itksdt
 vtkHybrid)
