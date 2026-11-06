#include "PluginProcessor.h"

namespace JoweMutant
{

PluginProcessor::PluginProcessor()
    : juce::AudioProcessor(
        BusesProperties()
            .withInput(
                "Input",
                juce::AudioChannelSet::stereo(),
                true)
            .withOutput(
                "Output",
                juce::AudioChannelSet::stereo(),
                true))
{
}

void PluginProcessor::prepareToPlay(
    double sampleRate,
    int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);

    glitchLeft.prepare(sampleRate);
    glitchRight.prepare(sampleRate);

    // BPM temporario para o primeiro teste.
    // A sincronizacao com o Bitwig sera
    // implementada posteriormente.

    glitchLeft.setBpm(120.0);
    glitchRight.setBpm(120.0);

    glitchLeft.setDivision(16);
    glitchRight.setDivision(16);

    glitchLeft.setEnabled(true);
    glitchRight.setEnabled(true);
}

void PluginProcessor::releaseResources()
{
    glitchLeft.reset();
    glitchRight.reset();
}

void PluginProcessor::processBlock(
    juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* samples = buffer.getWritePointer(channel);

        auto& engine = (channel == 0)
            ? glitchLeft
            : glitchRight;

        for (int sample = 0; sample < numSamples; ++sample)
        {
            samples[sample] =
                engine.processSample(samples[sample]);
        }
    }
}

juce::AudioProcessorEditor*
PluginProcessor::createEditor()
{
    class TestEditor final : public juce::AudioProcessorEditor
    {
    public:
        explicit TestEditor(juce::AudioProcessor& processor)
            : juce::AudioProcessorEditor(processor)
        {
            setSize(600, 350);
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colour(0xff10121a));

            g.setColour(juce::Colours::orangered);
            g.setFont(32.0f);

            g.drawFittedText(
                "JOWE MUTANT FX",
                20, 40, 560, 60,
                juce::Justification::centred, 1
            );

            g.setColour(juce::Colours::cyan);
            g.setFont(20.0f);

            g.drawFittedText(
                "GLITCH ENGINE - V0.1",
                20, 130, 560, 40,
                juce::Justification::centred, 1
            );

            g.setColour(juce::Colours::white);
            g.setFont(16.0f);

            g.drawFittedText(
                "FIRST AUDIO TEST",
                20, 210, 560, 40,
                juce::Justification::centred, 1
            );
        }
    };

    return new TestEditor(*this);
}
}

void PluginProcessor::getStateInformation(
    juce::MemoryBlock& destData)
{
    juce::ignoreUnused(destData);
}

void PluginProcessor::setStateInformation(
    const void* data,
    int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
}

}

// Entrada utilizada pelo JUCE para criar o plugin.

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JoweMutant::PluginProcessor();
}
