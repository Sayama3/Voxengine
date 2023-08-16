cmake_minimum_required(VERSION 3.24)

# This is a platform dependent that include the right folder depending on the platform.
# The fallback is currently "other".

# TODO: add renderer dependent choice.
target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_OPENGL)
add_subdirectory(OpenGL)

# TODO: add platform dependent.
target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC VXM_DEFAULT)
add_subdirectory(other)