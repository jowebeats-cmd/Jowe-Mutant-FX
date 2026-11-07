#pragma once
#include <JuceHeader.h>
#include <vector>
class JoweMutantFXAudioProcessor final : public juce::AudioProcessor
{
public:
    JoweMutantFXAudioProcessor();
    void prepareToPlay(double, int) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "JOWE MUTANT FX"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;
    juce::AudioProcessorValueTreeState apvts;
    static juce::AudioProcessorValueTreeState::ParameterLayout makeLayout();
private:
    double sampleRateHz = 44100.0;
    std::vector<std::vector<float>> history;
    int writeIndex = 0;
    int historySize = 0;
    int lastLoopLength = 0;
    int loopStart = 0;
    int loopPosition = 0;
    int sampleCounter = 0;
    int holdCounter = 0;
    float previousHold[2]{};
    float lowPass[2]{};
    float smoothedWet = 0.0f;
    float smoothedGate = 1.0f;
    float inputPeak = 0.0f;
    juce::Random random;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JoweMutantFXAudioProcessor)
};
