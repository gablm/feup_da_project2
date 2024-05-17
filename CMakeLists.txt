cmake_minimum_required(VERSION 3.22.1)
project(da_project2)

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
endif (UNIX)

add_executable(da_project2
	src/main.cpp
	src/graph/graph.cpp
	src/manager/loader.cpp
	src/manager/manager.cpp
	src/manager/tspCaller.cpp
	src/manager/heuristics/backtrack.cpp
	src/manager/heuristics/triagApprox.cpp
	src/manager/heuristics/other.cpp
	src/manager/heuristics/realworld.cpp
	src/manager/heuristics/utils.cpp
	src/ui/mainMenu.cpp
	src/ui/selectorMenu.cpp
	src/ui/resultMenu.cpp
)

file(COPY datasets DESTINATION ${CMAKE_BINARY_DIR})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
