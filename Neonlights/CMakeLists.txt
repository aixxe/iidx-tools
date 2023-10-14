cmake_minimum_required(VERSION 3.23)
project(Neonlights VERSION 0.1.0.0 LANGUAGES CXX)

include(cmake/get_cpm.cmake)

# Embedded OS image probably won't have the redistributable installed, so just statically link it.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

CPMAddPackage(
    NAME              minhook
    GIT_TAG           4a455528f61b5a375b1f9d44e7d296d47f18bb18
    GITHUB_REPOSITORY TsudaKageyu/minhook
)

set(VERSIONS
    LDJ-003-2023090500
    LDJ-012-2023090500
)

foreach (VERSION ${VERSIONS})
    add_library(${PROJECT_NAME}.${VERSION} SHARED src/${VERSION}.cc)
    target_link_libraries(${PROJECT_NAME}.${VERSION} minhook)
    target_compile_features(${PROJECT_NAME}.${VERSION} PRIVATE cxx_std_17)
endforeach ()