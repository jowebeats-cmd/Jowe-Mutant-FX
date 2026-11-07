cmake_minimum_required(VERSION 3.22)
project(JoweMutantFX VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(JUCE GIT_REPOSITORY https://github.com/juce-framework/JUCE.git GIT_TAG 8.0.10 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)
juce_add_plugin(JoweMutantFX
 COMPANY_NAME "Jowe Beats"
 BUNDLE_ID "com.jowebeats.mutantfx"
 PLUGIN_MANUFACTURER_CODE Jowe
 PLUGIN_CODE Jmfx
 FORMATS VST3
 PRODUCT_NAME "JOWE MUTANT FX"
 IS_SYNTH FALSE
 NEEDS_MIDI_INPUT FALSE
 NEEDS_MIDI_OUTPUT FALSE
 IS_MIDI_EFFECT FALSE
 COPY_PLUGIN_AFTER_BUILD FALSE)
juce_generate_juce_header(JoweMutantFX)
target_sources(JoweMutantFX PRIVATE Source/PluginProcessor.cpp Source/PluginEditor.cpp)
target_compile_definitions(JoweMutantFX PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(JoweMutantFX PRIVATE juce::juce_audio_utils juce::juce_dsp juce::juce_gui_extra juce::juce_recommended_config_flags juce::juce_recommended_warning_flags juce::juce_recommended_lto_flags)
