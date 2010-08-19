cmake_minimum_required(VERSION 2.6)

PROJECT(SeedTool)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

ADD_EXECUTABLE(itksdt seedtool.cxx)
ADD_EXECUTABLE(itksdt2 seedtool2.cxx)
ADD_EXECUTABLE(itksdt3 seedtool3.cxx)


TARGET_LINK_LIBRARIES(itksdt
 vtkHybrid)
TARGET_LINK_LIBRARIES(itksdt2
 vtkHybrid)
TARGET_LINK_LIBRARIES(itksdt3
 vtkHybrid)
