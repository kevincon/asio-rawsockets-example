project(asio-rawsockets-example)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++11)

find_package(Boost REQUIRED system)


add_executable(example
    ${CMAKE_CURRENT_SOURCE_DIR}/src/example.cpp
)
target_include_directories(example PUBLIC ${Boost_INCLUDE_DIR}
)
target_link_libraries(example PUBLIC ${Boost_LIBRARIES}     
)
