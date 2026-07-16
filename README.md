# krakendsp
Audio multi-effects library

...or beginnings of it.

Main idea is to have a set of audio-effects that implement the MultiFX virtual class, so they can easily be used in different projects.
At first there will be separate Juce-projects to create VST-plugins from them, but another main goal is to create a physical multi-effects units for modular synthesizers using Electrosmith Daisy ( https://electro-smith.com/products/daisy-seed ).


TO DO:

Distortion:
- implement Overdrive type
- implement Metal Zone type
- implement EQ/Tonestack
- Save and load all settings in vst

Tremolo:
- More settings
- Update JUCE-project
- Make JUCE-project save settings
  
Delay:
- Add stereo function
- Create other delay types
  - Low pass filter for delayed signal
  - Modulated delay
  - Multi-tap delay
- Make JUCE-project save settings

Chorus:
- Very basic
- Add filter and more settings
- Make JUCE-project save settings
  
Biquad Filter:
- Fix normalization/denormalization confusion of controls
- create JUCE-project
  
Cabinet Modeler:
- Currently more of a proof-of-concept
- Extremely simplified IR-convolution, should be remade with FFT

LFO:
- Check if vertical & hortical distortions actually work as intended
- Define what is actually intended
- Add interpolation on sine lookup-table
- Add more

RingBuffer:
- Add multiple read heads

Effects to add:
- Bitcrusher
- Compressor (requires Envelope follower and ADSR envelope)
- Gate/Expander (requires Envelope follower and ADSR envelope)
- Filters
- Reverb
- Convolution reverb

Utilities to add:
- ADSR envelope
- Envelope follower

Other:
- Preprocessor configuration to build version for pure 64-bit processors to use double precision floating point numbers or for slower embedded processor to use single precision floating point numbers
- Proper documentation