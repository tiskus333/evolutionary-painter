##
##author Paweł Kotiuk
##

cmake_minimum_required(VERSION 3.1)

project(evolutionary-painter)

add_compile_options(-Wall -O2)

file(GLOB TARGET_SRC "src/*.cpp" )
##set build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

#set(SFML_ROOT "C:/SFML/") #in case if SFML location is not standard
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
find_package(PythonLibs 3.5 REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)

include_directories(
${CMAKE_HOME_DIRECTORY}/include
${SFML_INCLUDE_DIR}
)


# Without this, any build libraries automatically have names "lib{x}.so"
set(CMAKE_SHARED_MODULE_PREFIX "")

list(REMOVE_ITEM TARGET_SRC "${CMAKE_SOURCE_DIR}/src/bindings.cpp")

# Add a shared module - modules are intended to be imported at runtime.
# - This is where you add the source files
add_library(bindings MODULE ${TARGET_SRC} ${CMAKE_SOURCE_DIR}/src/bindings.cpp)

# Set up the libraries and header search paths for this target
target_link_libraries(bindings ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} sfml-graphics sfml-audio sfml-window sfml-system)
target_include_directories(bindings PRIVATE ${PYTHON_INCLUDE_DIRS})


add_executable(evolutionary-painter ${TARGET_SRC} ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(evolutionary-painter sfml-graphics sfml-audio sfml-window sfml-system)
