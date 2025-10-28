find_package(Git)

if(Git_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --date=format:%Y%m%d --format=%cd-%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --format=%H
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    set(GIT_VERSION "nodate-nohash")
    set(GIT_HASH "nohash")
endif()

configure_file(${INPUT_FILE} ${OUTPUT_FILE})
