set(PROJECT_TARGETS_PREFIX ${PROJECT_NAME})

message("*****************************************************")
message("EXTRA TARGETS:")
message("*****************************************************")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

    add_library( ${PROJECT_TARGETS_PREFIX}_coverage INTERFACE)
    add_library( ${PROJECT_TARGETS_PREFIX}::coverage ALIAS ${PROJECT_TARGETS_PREFIX}_coverage)
    target_compile_options(${PROJECT_TARGETS_PREFIX}_coverage
                                INTERFACE
                                    --coverage -g -O0 -fprofile-arcs -ftest-coverage)

    target_link_libraries( ${PROJECT_TARGETS_PREFIX}_coverage
                            INTERFACE --coverage -g -O0 -fprofile-arcs -ftest-coverage)


    add_custom_target(coverage
        COMMAND rm -rf coverage
        COMMAND mkdir -p coverage
        COMMAND ${CMAKE_MAKE_PROGRAM} test
        COMMAND gcovr . -r ${CMAKE_SOURCE_DIR} --html-details --html -o coverage/index.html -e ${CMAKE_SOURCE_DIR}/test/third_party;
        COMMAND gcovr . -r ${CMAKE_SOURCE_DIR} --xml -o coverage/report.xml -e ${CMAKE_SOURCE_DIR}/test/third_party;
        COMMAND gcovr . -r ${CMAKE_SOURCE_DIR} -o coverage/report.txt -e ${CMAKE_SOURCE_DIR}/test/third_party;
        COMMAND cat coverage/report.txt
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}  # Need separate command for this line
    )

    message("New Target: ${PROJECT_TARGETS_PREFIX}::coverage")

endif()


add_library(${PROJECT_TARGETS_PREFIX}_warnings INTERFACE)
add_library(${PROJECT_TARGETS_PREFIX}::warnings ALIAS ${PROJECT_TARGETS_PREFIX}_warnings)

target_compile_options(${PROJECT_TARGETS_PREFIX}_warnings INTERFACE -Wall -Wextra -Wpedantic)


add_library(${PROJECT_TARGETS_PREFIX}_warnings_error INTERFACE)
add_library(${PROJECT_TARGETS_PREFIX}::error ALIAS ${PROJECT_TARGETS_PREFIX}_warnings_error)
target_compile_options(${PROJECT_TARGETS_PREFIX}_warnings_error INTERFACE -Werror)

message("New Target: ${PROJECT_TARGETS_PREFIX}::warnings")
message("New Target: ${PROJECT_TARGETS_PREFIX}::error")
message("*****************************************************")
