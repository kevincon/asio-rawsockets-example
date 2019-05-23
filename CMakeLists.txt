project(asio-rawsockets-example)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++11)

find_package(Boost REQUIRED system)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

# Building server
add_executable(example
    ${CMAKE_CURRENT_SOURCE_DIR}/src/example.cpp
)

target_link_libraries(example PUBLIC ${Boost_LIBRARIES}     
)
