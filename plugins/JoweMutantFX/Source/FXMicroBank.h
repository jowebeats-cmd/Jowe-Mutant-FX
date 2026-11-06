#pragma once

#include <array>
#include <string>

namespace JoweMutant
{
    enum class FXType
    {
        Glitch,
        Pitch,
        Filter,
        Reverse,
        Delay,
        Pan
    };

    struct MicroPreset
    {
        std::string name = "Init";

        float rate = 0.5f;
        float depth = 0.5f;
        float length = 0.5f;
        float grain = 0.5f;
        float drive = 0.0f;
    };

    class FXMicroBank
    {
    public:
        static constexpr int presetCount = 8;

        MicroPreset& getPreset(int index)
        {
            return presets.at(index);
        }

        const MicroPreset& getPreset(int index) const
        {
            return presets.at(index);
        }

    private:
        std::array<MicroPreset, presetCount> presets{};
    };
}
