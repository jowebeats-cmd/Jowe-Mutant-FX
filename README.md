# JOWE MUTANT FX — Glitch Engine v1.0

JUCE 8 VST3 project for Windows, built with GitHub Actions.

## Effects
- **Stutter**: repeats a captured slice of incoming audio, host BPM synchronized.
- **Rate**: slice duration from 1 beat to 1/16 beat.
- **Gate**: rhythmic attenuation in sixteenth-note pulses.
- **Crush**: bit-depth and sample-rate reduction.
- **Drive**: normalized soft saturation.
- **Tone**: one-pole low-pass filter.
- **Mix**: dry/wet with click-reducing smoothing.
- **Output**: post effect level adjustment.

## GitHub build
Upload all project files to the root of the repository, retaining `.github/workflows/build.yml`. GitHub Actions -> Build JOWE MUTANT FX VST3 -> Run workflow. Download `JOWE-MUTANT-FX-Windows-VST3` from Artifacts after build succeeds.

Copy the extracted `JOWE MUTANT FX.vst3` folder to `C:\Program Files\Common Files\VST3\` then rescan plugins in Bitwig or Ableton. Remove the previous version first and restart the DAW.

## Test
1. Place the VST3 on an audio track with a looping drum beat.
2. Set MIX = 1, STUTTER = 1, RATE = 16; verify audible repeats.
3. Set STUTTER = 0, CRUSH = 1, DRIVE = 0.7; verify distortion.
4. Set MIX = 0; verify dry input passes unchanged at OUTPUT = 0 dB (except for hard limiting if over 0 dBFS).

This is a new self-contained replacement project, not a patch of the previous Audio Plugin Coder fork. Build and runtime behavior must be verified in GitHub Actions and your DAW; this package has not been compiled here.
