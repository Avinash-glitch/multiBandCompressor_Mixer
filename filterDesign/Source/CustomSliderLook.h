#pragma once

#include <JuceHeader.h> // This already includes everything you need

class CustomSliderLook : public juce::LookAndFeel_V4
{
public:
    CustomSliderLook()
    {
        thumbImage = juce::ImageCache::getFromMemory(BinaryData::sliderknob_png, BinaryData::sliderknob_pngSize);
    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/,
        const juce::Slider::SliderStyle style, juce::Slider&) override
    {
        if (style == juce::Slider::LinearVertical)
        {
            int centerX = x + width / 2;
            g.setColour(juce::Colours::ghostwhite);
            g.fillRect(centerX - 2, y, 4, height);
            const int knobWidth = 24;
            const int knobHeight = 24;

            int knobX = x + (width - knobWidth) / 2;
            int knobY = static_cast<int>(sliderPos - knobHeight / 2);

            g.drawImage(thumbImage, knobX, knobY, knobWidth, knobHeight, 0, 0,
                thumbImage.getWidth(), thumbImage.getHeight());
            
        }
    }

private:
    juce::Image thumbImage;
};

