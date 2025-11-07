find_package(Git)

if (NOT IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../vendor/lps")
    message(STATUS "Cloning lps...")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} clone https://github.com/87flowers/lps
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../vendor")
endif()

file(READ "${CMAKE_CURRENT_LIST_DIR}/../vendor/lps_commit.txt" LPS_COMMIT)
string(STRIP ${LPS_COMMIT} LPS_COMMIT)

message(STATUS "Checking out lps commit ${LPS_COMMIT}...")

execute_process(
    COMMAND ${GIT_EXECUTABLE} show ${LPS_COMMIT}
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../vendor/lps"
    RESULT_VARIABLE COMMIT_NOT_PRESENT
    OUTPUT_QUIET
    ERROR_QUIET)

if (COMMIT_NOT_PRESENT)
    message(STATUS "Fetching from lps upstream...")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} fetch
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../vendor/lps"
        RESULT_VARIABLE FETCH_FAILED)

    if (FETCH_FAILED)
        message(FATAL_ERROR "Could not fetch lps from upstream.")
    endif()
endif ()

execute_process(
    COMMAND ${GIT_EXECUTABLE} -c advice.detachedHead=false checkout ${LPS_COMMIT}
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../vendor/lps"
    RESULT_VARIABLE CHECKOUT_FAILED)

if (CHECKOUT_FAILED)
    message(FATAL_ERROR "Could not checkout lps commit ${LPS_COMMIT}")
endif()
