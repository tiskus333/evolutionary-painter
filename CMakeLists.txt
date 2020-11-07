cmake_minimum_required(VERSION 3.1)

project(evolutionary-painter)

add_compile_options(-Wall -O2)

file(GLOB TARGET_SRC "./src/*.cpp" )
##set build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

#set(SFML_ROOT "C:/SFML/") #in case if SFML location is not standard
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories(
${CMAKE_HOME_DIRECTORY}/include
${SFML_INCLUDE_DIR}
)


add_executable(evolutionary-painter ${TARGET_SRC})
target_link_libraries(evolutionary-painter sfml-graphics sfml-audio sfml-window sfml-system)
