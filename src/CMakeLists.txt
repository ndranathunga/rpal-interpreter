file(GLOB_RECURSE SOURCES "*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_library(rpal_interpreter_lib ${SOURCES})

target_include_directories(rpal_interpreter_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)


add_executable(rpal_interpreter main.cpp)

target_link_libraries(rpal_interpreter rpal_interpreter_lib)

target_include_directories(rpal_interpreter PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
