#pragma once

#include <JuceHeader.h>

#include "FXMicroBank.h"
#include "FXSequencer.h"
#include "GlitchEngine.h"

namespace JoweMutant
{
class PluginProcessor final
    : public juce::AudioProcessor
{
public:
    PluginProcessor();

    void prepareToPlay(
        double sampleRate,
        int samplesPerBlock) override;

    void releaseResources() override;

    void processBlock(
        juce::AudioBuffer<float>&,
        juce::MidiBuffer&) override;

    const juce::String getName() const override
    {
        return "JOWE MUTANT FX";
    }

    bool acceptsMidi() const override
    {
        return false;
    }

    bool producesMidi() const override
    {
        return false;
    }

    double getTailLengthSeconds() const override
    {
        return 0.0;
    }

    int getNumPrograms() override
    {
        return 1;
    }

    int getCurrentProgram() override
    {
        return 0;
    }

    void setCurrentProgram(int) override {}

    const juce::String getProgramName(int) override
    {
        return {};
    }

    void changeProgramName(
        int,
        const juce::String&) override {}

    bool hasEditor() const override
    {
        return true;
    }

    juce::AudioProcessorEditor*
        createEditor() override;

    void getStateInformation(
        juce::MemoryBlock&) override;

    void setStateInformation(
        const void*,
        int) override;

private:
    FXMicroBank microBank;
    FXSequencer sequencer;

    GlitchEngine glitchLeft;
    GlitchEngine glitchRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        PluginProcessor)
};
}
