cmake_minimum_required(VERSION 3.10)


################################################################################
# Project name
#
#  many targets will be created:
#   myProject::warnings
#   myProject:
################################################################################
project( vkb
        VERSION 1.0.0.0
        LANGUAGES CXX)

################################################################################


set(CMAKE_CXX_STANDARD_REQUIRED ON)



################################################################################

# Extra targets.  adds coverage::coverage, warning::all, warning:error
SET(PROJECT_TARGETS_PREFIX ${PROJECT_NAME})

include(.cmake/extras.cmake)

option( ${PROJECT_NAME}_ENABLE_COVERAGE "Enable Coverage. After build, execute make coverage"        TRUE)
option( ${PROJECT_NAME}_ENABLE_WARNINGS "Enable Strict Warnings" TRUE)
option( ${PROJECT_NAME}_ENABLE_TESTING  "Enable Unit Tests" FALSE)

################################################################################



################################################################################
add_library(vkb INTERFACE)
add_library(vkb::vkb ALIAS vkb)
target_include_directories(vkb INTERFACE "include")
target_compile_features(   vkb INTERFACE cxx_std_17)
################################################################################



if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")

    if( ${PROJECT_NAME}_ENABLE_TESTING )

        ################################################################################
        # If the conan file exists, include it
        ################################################################################
        if( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake )
            include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
            conan_basic_setup(NO_OUTPUT_DIRS TARGETS)
        endif()
        ################################################################################




        ################################################################################
        # Build the SDLVulkanWindow as a library so that we can use it for the
        # examples/unit tests
        ################################################################################
        find_package(Vulkan)
        add_library(SDLVulkanWindow  third_party/SDLVulkanWindow/SDLVulkanWindow_INIT.cpp third_party/SDLVulkanWindow/SDLVulkanWindow_USAGE.cpp)
        add_library(SDLVulkanWindow::SDLVulkanWindow ALIAS SDLVulkanWindow )
        target_include_directories(SDLVulkanWindow PUBLIC "third_party/SDLVulkanWindow")
        target_link_libraries(SDLVulkanWindow PUBLIC Vulkan::Vulkan CONAN_PKG::sdl2)
        ################################################################################


        enable_testing()
        add_subdirectory(test)
    endif()

endif()



return()
