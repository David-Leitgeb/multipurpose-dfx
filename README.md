# Multipurpose Digital Audio Effect: Combining Different Techniques Within a Single Device
This repository contains supplementary material for the EEICT 2025 paper Multipurpose Digital Audio Effect: Combining Different Techniques Within a Single Device.

### Abstract
This paper introduces an audio effect (FX) plug-in module capable of processing the input signal with several common types of audio manipulation within up to 30 individual bands. The advantage of the proposed implementation lies mostly in its versatility, with potential applications ranging from simple delay up to spatial simulations, such as reverb. Unlike the existing commercial solutions that typically split the input signal into a fixed set of frequency bands, the proposed FX processes the input signal in parallel across all bands, with each band applying optional filtering and other processing independently. This paper includes a basic classification of the proposed FX, a brief overview of similar already available software solutions, description of the methods used during the implementation process and a series of experiments conducted using the suggested implementation. The proposed FX was implemented using the C++ programming language and the JUCE framework.

### Resources
- [Listening page](https://david-leitgeb.github.io/multipurpose-dfx)

### Graphical user interface
![Graphical interface of the proposed FX plug-in](/docs/png/fig_plugin_gui.png)


The source code for the proposed FX plug-in is available in the ```source-code``` directory. The plug-in is also available in the VST3 format in the same directory.

