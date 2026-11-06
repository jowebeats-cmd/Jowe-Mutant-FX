#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

namespace JoweMutant
{
    class GlitchEngine
    {
    public:
        void prepare(double newSampleRate)
        {
            sampleRate = std::max(1.0, newSampleRate);

            // Reserva memória antes do processamento.
            const int maxSamples =
                static_cast<int>(sampleRate * 2.0) + 1;

            buffer.assign(maxSamples, 0.0f);
            reset();
            updateLength();
        }

        void reset()
        {
            position = 0;
            captured = false;

            std::fill(
                buffer.begin(),
                buffer.end(),
                0.0f
            );
        }

        void setBpm(double newBpm)
        {
            bpm = std::clamp(newBpm, 20.0, 300.0);
            updateLength();
        }

        void setDivision(int denominator)
        {
            if (denominator == 4 ||
                denominator == 8 ||
                denominator == 16 ||
                denominator == 32 ||
                denominator == 64)
            {
                division = denominator;
                updateLength();
            }
        }

        void setEnabled(bool state)
        {
            if (enabled != state)
            {
                enabled = state;
                reset();
            }
        }

        float processSample(float input)
        {
            if (!enabled || buffer.empty())
                return input;

            if (!captured)
            {
                buffer[position] = input;
                ++position;

                if (position >= length)
                {
                    position = 0;
                    captured = true;
                }

                return input;
            }

            const float output = buffer[position];

            ++position;

            if (position >= length)
                position = 0;

            return output;
        }

    private:
        void updateLength()
        {
            const double secondsPerBeat = 60.0 / bpm;

            const double duration =
                secondsPerBeat * (4.0 / division);

            const int requested =
                static_cast<int>(duration * sampleRate);

            const int capacity =
                std::max(1, static_cast<int>(buffer.size()));

            length = std::clamp(requested, 1, capacity);

            reset();
        }

        std::vector<float> buffer;

        double sampleRate = 44100.0;
        double bpm = 120.0;

        int division = 16;
        int length = 1;
        int position = 0;

        bool enabled = false;
        bool captured = false;
    };
}
