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
	src/ui/mainMenu.cpp
	src/ui/selectorMenu.cpp
)
