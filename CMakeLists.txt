cmake_minimum_required (VERSION 3.8)

project ("QBCPPCraftDemo")

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_GENERATOR_PLATFORM x64)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Add source to this project's executable.
add_executable (QBCPPCraftDemo "src/QBCPPCraftDemo.cpp")
