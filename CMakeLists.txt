cmake_minimum_required (VERSION 3.8)

project (MikrotikPlus)

# Set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Locate files
file(GLOB MikrotikPlus_HEADERS include/MikrotikPlus/*.hpp)
file(GLOB MikrotikPlus_SOURCES src/*.cpp)

# Set up target
add_library(MikrotikPlus ${MikrotikPlus_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/MikrotikPlus)

# Installation configuration
install(FILES ${MikrotikPlus_HEADERS} DESTINATION include)
install(TARGETS MikrotikPlus
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)