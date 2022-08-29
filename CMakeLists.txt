cmake_minimum_required(VERSION 3.0.0)
project(inp VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(include)

add_executable(
  inp 
  src/main.c 
  src/install.c 
  src/create-package.c 
  src/utils/common.c 
  src/utils/pair.c
)

target_link_libraries(inp -larchive)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
