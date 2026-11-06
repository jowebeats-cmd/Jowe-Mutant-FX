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
    return new juce::GenericAudioProcessorEditor(*this);
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
