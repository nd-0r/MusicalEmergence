cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)

project(synchrony_tests)

set(CMAKE_CXX_STANDARD 14)

list(APPEND SOURCE_FILES ../Source/Particle.cpp
                         ../Source/ParticleManager.cpp
                         ../Source/Utilities.h)

add_executable(test test_main.cpp test_plugin.cpp ${SOURCE_FILES})

add_compile_definitions(DEBUG=1 JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED)

target_include_directories(test PUBLIC ../JuceLibraryCode
                                       /Applications/JUCE/modules
                                       /Applications/JUCE/modules/juce_audio_plugin_client
                                       /usr/local/include/catch2
                                       /usr/local/include/vmmlib)