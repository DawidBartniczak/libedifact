project(libedifact VERSION 0.0.1 LANGUAGES C)

file(GLOB_RECURSE libedifact_SRC CONFIGURE_DEPENDS "*.c" "*.h")
add_library(libedifact SHARED ${libedifact_SRC})