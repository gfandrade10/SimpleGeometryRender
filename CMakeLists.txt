cmake_minimum_required(VERSION 3.16)
project(SimpleGeo)
set(CMAKE_CXX_STANDARD 20)
set(CDIR ${CMAKE_SOURCE_DIR})
set(INC ${CDIR}/include)
set(LIBS ${CDIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBS})
set(CMAKE_MAKE_PROGRAM "C:/mingw64/bin/mingw32-make.exe")
set(IMGUI_DIR "F:/Codes/conan_data/p/imgui6d92dd284f976/s/src")

add_subdirectory(src)

find_package(fmt REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    fmt::fmt
    Threads::Threads
    OpenGL::GL
    glfw
    glad::glad
    imgui::imgui
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INC}
    ${IMGUI_DIR}/backends
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -O2 -Wall -Wpedantic
)

if(DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g)
endif()
