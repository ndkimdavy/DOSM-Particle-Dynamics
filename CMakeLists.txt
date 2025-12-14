cmake_minimum_required(VERSION 3.15)
project(dosm LANGUAGES CXX)

set(APP_NAME dosmetric)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

# ---------------------------
# Kokkos
# ---------------------------
set(Kokkos_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(Kokkos_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
		kokkos
		GIT_REPOSITORY https://github.com/kokkos/kokkos.git
		GIT_TAG        5.0.0
		)
FetchContent_MakeAvailable(kokkos)

# ---------------------------
# Eigen
# ---------------------------
set(EIGEN_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(EIGEN_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
		eigen
		GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
		GIT_TAG        5.0.0
		)
FetchContent_MakeAvailable(eigen)

# ---------------------------
# Fastor
# ---------------------------
FetchContent_Declare(
		fastor
		GIT_REPOSITORY https://github.com/romeric/Fastor.git
		GIT_TAG        v0.1
		)
FetchContent_MakeAvailable(fastor)

# ---------------------------
# Raylib
# ---------------------------
set(EIGEN_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(EIGEN_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
		raylib
		GIT_REPOSITORY https://github.com/raysan5/raylib.git
		GIT_TAG        5.5
		)
FetchContent_MakeAvailable(raylib)

# ---------------------------
# Sources
# ---------------------------
set(DOSM_SOURCES
		src/dosmEntity.cpp
		src/dosmFactor.cpp
		src/dosmLawBasic.cpp
		src/dosmParallelKokkos.cpp
		src/dosmVisualRaylib.cpp
	)

# ---------------------------
# DOSM library
# ---------------------------
add_library(${PROJECT_NAME} STATIC ${DOSM_SOURCES})

target_include_directories(dosm
		PUBLIC
		${PROJECT_SOURCE_DIR}/include
		${PROJECT_SOURCE_DIR}/interface
		${PROJECT_SOURCE_DIR}/impl
		${fastor_SOURCE_DIR}
		)

target_link_libraries(dosm
		PUBLIC Eigen3::Eigen
		PUBLIC Kokkos::kokkos
		PUBLIC raylib
		)

target_compile_definitions(dosm 
		PUBLIC DOSM_MATH_EIGEN
		PUBLIC DOSM_ENABLE_LOG
		)

# ---------------------------
# Executable
# ---------------------------
add_executable(${APP_NAME} src/main.cpp)

target_link_libraries(${APP_NAME}
		PRIVATE ${PROJECT_NAME}
		)





