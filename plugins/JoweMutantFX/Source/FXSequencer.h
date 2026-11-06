#pragma once

#include <array>
#include <cstddef>

namespace JoweMutant
{
    class FXSequencer
    {
    public:
        static constexpr int maxSteps = 32;
        static constexpr int patternCount = 8;
        static constexpr int effectCount = 6;
        static constexpr int presetCount = 8;

        struct Step
        {
            // -1 = efeito desligado
            //  0 a 7 = micro preset A a H
            int microPreset = -1;
        };

        struct Pattern
        {
            std::array<
                std::array<Step, maxSteps>,
                effectCount
            > lanes{};

            int length = 16;
        };

        bool selectPattern(int index)
        {
            if (index < 0 || index >= patternCount)
                return false;

            currentPattern = index;
            return true;
        }

        int getCurrentPattern() const
        {
            return currentPattern;
        }

        bool setStep(int effect, int step, int preset)
        {
            if (effect < 0 || effect >= effectCount)
                return false;

            if (step < 0 || step >= maxSteps)
                return false;

            if (preset < -1 || preset >= presetCount)
                return false;

            patterns[currentPattern]
                .lanes[effect][step]
                .microPreset = preset;

            return true;
        }

        int getStep(int effect, int step) const
        {
            if (effect < 0 || effect >= effectCount)
                return -1;

            if (step < 0 || step >= maxSteps)
                return -1;

            return patterns[currentPattern]
                .lanes[effect][step]
                .microPreset;
        }

        bool setPatternLength(int length)
        {
            if (length != 16 && length != 32)
                return false;

            patterns[currentPattern].length = length;
            return true;
        }

        int getPatternLength() const
        {
            return patterns[currentPattern].length;
        }

    private:
        std::array<Pattern, patternCount> patterns{};
        int currentPattern = 0;
    };
}
