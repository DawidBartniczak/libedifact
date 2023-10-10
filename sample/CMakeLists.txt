project(edisample VERSION 0.0.1 LANGUAGES C)

file(GLOB_RECURSE libedifact_SAMPLE CONFIGURE_DEPENDS "*.c" "*.h")
add_executable(edisample ${libedifact_SAMPLE})

target_link_libraries(edisample libedifact)