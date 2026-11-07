#pragma once
#include "PluginProcessor.h"
#include <array>
class JoweMutantFXAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit JoweMutantFXAudioProcessorEditor(JoweMutantFXAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    JoweMutantFXAudioProcessor& processor;
    struct Dial
    {
        juce::Slider slider;
        juce::Label label;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attach;
    };
    std::array<Dial, 8> controls;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JoweMutantFXAudioProcessorEditor)
};
