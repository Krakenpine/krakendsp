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

Tremolo:
- update JUCE-project
  
Delay:
- Create other delay types
  - Low pass filter for delayed signal
  - Modulated delay
  
Biquad Filter:
- Fix normalization/denormalization confusion of controls
- create JUCE-project

LFO:
- Check if vertical & hortical distortions actually work as intended
- Define what is actually intended
- Add more

Effects to add:
- Bitcrusher
- Chorus (requires LFO and Delay)
- Compressor (requires Envelope follower and ADSR envelope)
- Gate/Expander (requires Envelope follower and ADSR envelope)
- Filters
- Reverb
- Convolution reverb and/or convolution based guitar cabinet modeler

Utilities to add:
- ADSR envelope
- Envelope follower
- Ringbuffer

Other:
- Preprocessor configuration to build version for pure 64-bit processors to use double precision floating point numbers or for slower embedded processor to use single precision floating point numbers
- Proper documentation