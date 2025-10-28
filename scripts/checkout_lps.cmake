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
    COMMAND ${GIT_EXECUTABLE} -c advice.detachedHead=false checkout ${LPS_COMMIT}
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../vendor/lps")
