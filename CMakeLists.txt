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





################################################################################

# Extra targets.  adds coverage::coverage, warning::all, warning:error
SET(PROJECT_TARGETS_PREFIX ${PROJECT_NAME})

include(.cmake/extras.cmake)

option( ${PROJECT_NAME}_ENABLE_COVERAGE "Enable Coverage. After build, execute make coverage"        TRUE)
option( ${PROJECT_NAME}_ENABLE_WARNINGS "Enable Strict Warnings" TRUE)

################################################################################



################################################################################
add_library(vkb INTERFACE)
add_library(vkb::vkb ALIAS vkb)
target_include_directories(vkb INTERFACE "include")
target_compile_features(   vkb INTERFACE cxx_std_17)
################################################################################


################################################################################
# Build the Library.
#  By default, the library name will be the ${PROJECT_NAME}
#
#  If you wish to add more libraries, make a copy of this section and
# change the values below.
################################################################################
include(GNUInstallDirs)
install(
   TARGETS
       vkb
   LIBRARY  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
   ARCHIVE  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
   RUNTIME  DESTINATION "${CMAKE_INSTALL_BINDIR}"
   INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  )

message("*****************************************************")
message("LIBRARIES:")
message("*****************************************************")
message("   ${outName} ")
message("*****************************************************")
################################################################################




if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")
    enable_testing()
    add_subdirectory(test)
endif()



#####################################################
# Macro get get all subdirectories
#####################################################
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()
#####################################################


# Loop through all the subdirectories in the /bin folder and
# compile each of the folders into an executable
#SUBDIRLIST(BIN_FOLDERS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#
#message("*****************************************************")
#message("EXECUTABLES:")
#message("*****************************************************")
#FOREACH(subdir ${BIN_FOLDERS})
#    set(bin_name ${subdir} )
#    file(GLOB_RECURSE srcFiles ${CMAKE_CURRENT_SOURCE_DIR}/bin/${subdir}/* )
#
#    add_executable( ${bin_name} ${srcFiles} )
#    target_link_libraries(${bin_name} PRIVATE NodeEditor::nodes QJForm::QJForm CONAN_PKG::sdl2 CONAN_PKG::nlohmann_json)
#
#    install(
#       TARGETS
#           ${bin_name}
#       LIBRARY  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#       ARCHIVE  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#       RUNTIME  DESTINATION "${CMAKE_INSTALL_BINDIR}"
#       INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
#      )
#
#      message("    ${bin_name}")
#
#ENDFOREACH()
#message("*****************************************************")


return()
