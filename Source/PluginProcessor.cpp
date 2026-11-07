#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

JoweMutantFXAudioProcessor::JoweMutantFXAudioProcessor()
 : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                   .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
   apvts(*this, nullptr, "PARAMETERS", makeLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout JoweMutantFXAudioProcessor::makeLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    auto add = [&layout](const char* id, const char* title, float min, float max, float initial, float step = 0.001f)
    { layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{id, 1}, title, juce::NormalisableRange<float>(min,max,step), initial)); };
    add("mix", "Wet Mix", 0, 1, 0.75f);
    add("repeat", "Stutter", 0, 1, 0.65f);
    add("rate", "Repeat Rate", 1, 16, 8, 1);
    add("gate", "Rhythmic Gate", 0, 1, 0.35f);
    add("crush", "Bit Crush", 0, 1, 0.25f);
    add("drive", "Drive", 0, 1, 0.2f);
    add("tone", "Tone", 0, 1, 0.75f);
    add("output", "Output Gain", -18, 6, 0, 0.1f);
    return layout;
}

void JoweMutantFXAudioProcessor::prepareToPlay(double sr, int)
{
    sampleRateHz = sr > 0 ? sr : 44100.0;
    historySize = juce::jmax(4096, (int)(sampleRateHz * 2.0));
    history.assign(2, std::vector<float>((size_t)historySize, 0.0f));
    writeIndex = 0;
    loopStart = loopPosition = lastLoopLength = sampleCounter = holdCounter = 0;
    previousHold[0] = previousHold[1] = lowPass[0] = lowPass[1] = 0.0f;
    smoothedWet = apvts.getRawParameterValue("mix")->load();
    smoothedGate = 1.0f;
}
void JoweMutantFXAudioProcessor::releaseResources() {}

void JoweMutantFXAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int n = buffer.getNumSamples();
    const int channels = juce::jmin(2, buffer.getNumChannels());
    for (int c = getTotalNumInputChannels(); c < buffer.getNumChannels(); ++c) buffer.clear(c, 0, n);
    if (historySize == 0 || channels == 0) return;

    const float mix = apvts.getRawParameterValue("mix")->load();
    const float repeat = apvts.getRawParameterValue("repeat")->load();
    const int rate = juce::jlimit(1, 16, (int)std::round(apvts.getRawParameterValue("rate")->load()));
    const float gate = apvts.getRawParameterValue("gate")->load();
    const float crush = apvts.getRawParameterValue("crush")->load();
    const float drive = apvts.getRawParameterValue("drive")->load();
    const float tone = apvts.getRawParameterValue("tone")->load();
    const float gain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("output")->load());

    double bpm = 120.0;
    if (auto* playHead = getPlayHead())
        if (auto pos = playHead->getPosition())
            if (auto v = pos->getBpm()) bpm = juce::jlimit(30.0, 300.0, *v);
    const int beat = juce::jmax(1, (int)std::round(sampleRateHz * 60.0 / bpm));
    const int loopLength = juce::jlimit(32, historySize / 2, beat / rate);
    const int gateLength = juce::jmax(16, beat / 4);
    const int holdLength = 1 + (int)(crush * crush * 48.0f);
    const float bits = 16.0f - crush * 12.0f;
    const float levels = std::pow(2.0f, bits - 1.0f);
    const float cutoffHz = 400.0f + tone * tone * 19400.0f;
    const float lpCoefficient = std::exp(-juce::MathConstants<float>::twoPi * cutoffHz / (float)sampleRateHz);
    const float saturation = 1.0f + drive * 14.0f;
    const float norm = std::tanh(saturation);
    const float smoothing = std::exp(-1.0f / (0.007f * (float)sampleRateHz));

    if (lastLoopLength != loopLength)
    {
        lastLoopLength = loopLength;
        loopPosition = 0;
        loopStart = (writeIndex - loopLength + historySize) % historySize;
    }
    for (int i = 0; i < n; ++i)
    {
        // Re-capture a fresh, fully written slice for each loop; never allocate in the audio callback.
        if (loopPosition >= loopLength)
        {
            loopPosition = 0;
            loopStart = (writeIndex - loopLength + historySize) % historySize;
        }
        const int readIndex = (loopStart + loopPosition) % historySize;
        const float gateTarget = (sampleCounter % (2 * gateLength) < gateLength) ? 1.0f : (1.0f - gate);
        smoothedGate = smoothing * smoothedGate + (1.0f - smoothing) * gateTarget;
        smoothedWet = smoothing * smoothedWet + (1.0f - smoothing) * mix;
        for (int c = 0; c < channels; ++c)
        {
            const float dry = buffer.getSample(c, i);
            const float loopSample = history[(size_t)c][(size_t)readIndex];
            history[(size_t)c][(size_t)writeIndex] = dry;
            float wet = dry * (1.0f - repeat) + loopSample * repeat;
            if (holdCounter == 0) previousHold[c] = std::round(wet * levels) / levels;
            wet = previousHold[c];
            wet = std::tanh(wet * saturation) / norm;
            lowPass[c] = (1.0f - lpCoefficient) * wet + lpCoefficient * lowPass[c];
            wet = lowPass[c] * smoothedGate;
            const float out = (dry * (1.0f - smoothedWet) + wet * smoothedWet) * gain;
            buffer.setSample(c, i, juce::jlimit(-1.0f, 1.0f, out));
        }
        if (++holdCounter >= holdLength) holdCounter = 0;
        ++sampleCounter;
        ++loopPosition;
        if (++writeIndex >= historySize) writeIndex = 0;
    }
}

void JoweMutantFXAudioProcessor::getStateInformation(juce::MemoryBlock& dest)
{
    if (auto xml = apvts.copyState().createXml()) copyXmlToBinary(*xml, dest);
}
void JoweMutantFXAudioProcessor::setStateInformation(const void* data, int size)
{
    if (auto xml = getXmlFromBinary(data, size))
        if (xml->hasTagName(apvts.state.getType())) apvts.replaceState(juce::ValueTree::fromXml(*xml));
}
juce::AudioProcessorEditor* JoweMutantFXAudioProcessor::createEditor()
{
    return new JoweMutantFXAudioProcessorEditor(*this);
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new JoweMutantFXAudioProcessor(); }
