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
# Sources
# ---------------------------
    set(DOSM_SOURCES
            src/dosmEntity.cpp
            src/dosmParticle.cpp
            src/dosmMetric.cpp
            src/dosmLawLJ.cpp 
            src/dosmLawLJP.cpp
            src/dosmLawLJPNB.cpp
        #     src/dosmLawLJPNB_OMP.cpp
            src/dosmLawVV.cpp 
            src/dosmSocketPublisher.cpp
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
            )

    target_link_libraries(dosm
            PUBLIC Eigen3::Eigen
            PUBLIC Kokkos::kokkos
            )

    target_compile_definitions(dosm 
            PUBLIC DOSM_ENABLE_LOG
            PUBLIC DOSM_MATH_EIGEN
            )

# ---------------------------
# Executable
# ---------------------------
add_executable(${APP_NAME} src/main.cpp)

    target_link_libraries(${APP_NAME}
            PRIVATE ${PROJECT_NAME}
            )





