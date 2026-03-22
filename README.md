# krakendsp
Audio multi-effects library

...or beginnings of it.

Main idea is to have a set of audio-effects that implement the MultiFX virtual class, so they can easily be used in different projects.
At first there will be separate Juce-projects to create VST-plugins from them, but another main goal is to create a physical multi-effects units for modular synthesizers using Electrosmith Daisy ( https://electro-smith.com/products/daisy-seed ).

Currently it has only simple distortion effect.

TO DO:
Distortion:
- implement Overdrive type
- implement Metal Zone type (requires filter effects)
- implement oversampling

Effects to add:
- Tremolo (requires LFO)
- Bitcrusher
- Chorus (requires LFO and Delay)
- Compressor (requires Envelope follower and ADSR envelope)
- Delay (requires Ringbuffer)
- Gate/Expander (requires Envelope follower and ADSR envelope)
- Filters
- Reverb
- Convolution reverb and/or convolution based guitar cabinet modeler

Utilities to add:
- LFO
- ADSR envelope
- Envelope follower
- Ringbuffer

Other:
- Preprocessor configuration to build version for pure 64-bit processors to use double precision floating point numbers or for slower embedded processor to use single precision floating point numbers