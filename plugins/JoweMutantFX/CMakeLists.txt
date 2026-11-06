# JOWE MUTANT FX
# VST3 Plugin - Windows
# Developed by Jowe Beats

cmake_minimum_required(VERSION 3.22)

juce_add_plugin(JoweMutantFX
    COMPANY_NAME "Jowe Beats"
    PLUGIN_MANUFACTURER_CODE Jowe
    PLUGIN_CODE JmtF
    FORMATS VST3
    PRODUCT_NAME "JOWE MUTANT FX"
    VST3_CATEGORIES Fx
    COPY_PLUGIN_AFTER_BUILD OFF
)

juce_generate_juce_header(JoweMutantFX)

target_sources(JoweMutantFX
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/FXMicroBank.h
        Source/FXSequencer.h
        Source/GlitchEngine.h
)

target_compile_features(JoweMutantFX
    PRIVATE cxx_std_20
)

target_link_libraries(JoweMutantFX
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
)

target_compile_definitions(JoweMutantFX
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)
