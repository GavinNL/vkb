# Create a static library for Catch2s main so that we can reduce
# compiling time. Each unit test will link to this
cmake_minimum_required(VERSION 3.13)

get_filename_component(folder_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" folder_name ${folder_name})

message("*****************************************************")
message("Examples:")
message("*****************************************************")
# Find all files named unit-*.cpp
file(GLOB files "*.cpp")
foreach(file ${files})

    get_filename_component(file_basename ${file} NAME_WE)

    set(UNIT_EXE_NAME  ${file_basename} )

    add_executable( ${UNIT_EXE_NAME} ${file} )
    target_compile_definitions( ${UNIT_EXE_NAME} PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
    target_compile_features( ${UNIT_EXE_NAME}
                                PUBLIC
                                    cxx_std_17)

    target_link_libraries( ${UNIT_EXE_NAME} 
                                PUBLIC 
                                    ${PROJECT_NAME}-catchmain 
                                    ${PROJECT_NAME}::warnings 
                                    CONAN_PKG::sdl
                                    CONAN_PKG::fmt
                                    Vulkan::Vulkan
                                    CONAN_PKG::spirv-cross
                                    CONAN_PKG::vulkan-memory-allocator
                                    vkb::vkb
                                    vkw::vkw)


    message("  ${UNIT_EXE_NAME} ")
endforeach()
