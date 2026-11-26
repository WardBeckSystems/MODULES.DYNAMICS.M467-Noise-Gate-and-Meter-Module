cmake_minimum_required(VERSION 3.22)
project(WBS_M467_Noise_Gate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(WBS_M467_Noise_Gate STATIC
    PluginProcessor.hpp
    PluginProcessor.cpp
    PluginEditor.hpp
    PluginEditor.cpp
)

target_link_libraries(WBS_M467_Noise_Gate PUBLIC wbs_core)
target_include_directories(WBS_M467_Noise_Gate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
