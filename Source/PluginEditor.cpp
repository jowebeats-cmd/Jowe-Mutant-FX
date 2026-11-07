#include "PluginEditor.h"
JoweMutantFXAudioProcessorEditor::JoweMutantFXAudioProcessorEditor(JoweMutantFXAudioProcessor& p)
  : AudioProcessorEditor(&p), processor(p)
{
    const char* ids[] = {"mix","repeat","rate","gate","crush","drive","tone","output"};
    const char* labels[] = {"MIX","STUTTER","RATE","GATE","CRUSH","DRIVE","TONE","OUTPUT"};
    for (size_t i = 0; i < controls.size(); ++i)
    {
        auto& d = controls[i];
        d.slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        d.slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 24);
        d.slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour::fromRGB(242, 83, 174));
        d.slider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
        d.slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(225,235,248));
        d.slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        d.label.setText(labels[i], juce::dontSendNotification);
        d.label.setJustificationType(juce::Justification::centred);
        d.label.setColour(juce::Label::textColourId, juce::Colour::fromRGB(152, 205, 224));
        addAndMakeVisible(d.slider);
        addAndMakeVisible(d.label);
        d.attach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.apvts, ids[i], d.slider);
    }
    setSize(760, 420);
}
void JoweMutantFXAudioProcessorEditor::paint(juce::Graphics& g)
{
    auto b = getLocalBounds().toFloat();
    g.setGradientFill(juce::ColourGradient(juce::Colour::fromRGB(13, 19, 34), 0, 0,
                                          juce::Colour::fromRGB(41, 15, 54), b.getRight(), b.getBottom(), false));
    g.fillAll();
    g.setColour(juce::Colour::fromRGB(85, 225, 240));
    g.fillRoundedRectangle(30.0f, 30.0f, 5.0f, 55.0f, 2.5f);
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions(35.0f).withStyle("Bold")));
    g.drawText("JOWE MUTANT FX", 50, 24, 620, 52, juce::Justification::centredLeft);
    g.setColour(juce::Colour::fromRGB(242,83,174));
    g.setFont(juce::Font(juce::FontOptions(14.0f)));
    g.drawText("GLITCH ENGINE  //  V1.0", 52, 77, 620, 28, juce::Justification::centredLeft);
    g.setColour(juce::Colour::fromRGB(60, 74, 99));
    g.drawRoundedRectangle(24.0f, 125.0f, (float)getWidth()-48.0f, 258.0f, 16.0f, 1.5f);
    g.setColour(juce::Colour::fromRGB(120, 145, 169));
    g.setFont(juce::Font(juce::FontOptions(12.0f)));
    g.drawText("JOWE BEATS  •  BPM-SYNCED STUTTER / GATE / CRUSH / SATURATION", 20, 390, getWidth()-40, 24, juce::Justification::centred);
}
void JoweMutantFXAudioProcessorEditor::resized()
{
    const int left = 38, top = 147, cellW = (getWidth()-76)/4, cellH = 112;
    for (int i = 0; i < (int)controls.size(); ++i)
    {
        const int x = left+(i%4)*cellW, y = top+(i/4)*cellH;
        controls[(size_t)i].label.setBounds(x, y, cellW, 22);
        controls[(size_t)i].slider.setBounds(x+17, y+22, cellW-34, 88);
    }
}
