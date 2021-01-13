//
// Created by mzapp on 4/10/18.
//
//    There is a lot of wrote action needed when reading or writing audio
//    files.  These classes can be used in situations where the default generic
//    operations are acceptable.
//

#pragma once
#include <juce_audio_formats/juce_audio_formats.h>

namespace meta
{
    class AudioFileHelpers
    {
    public:
        /**
         * Creates a juce::AudioFormatWriter for the type specified by the
         * specified file's extension.  The user is responsible for taking
         * ownership of this object.
         *
         * Not all parameters may have an effect on the generated writer.
         *
         * @param f - The file to use.
         * @param sampleRate - the sample rate to use in the generated file.
         * @param chanCount - the number of channels to use in the generated file.
         * @param bps - the bit-depth of the generated file.
         * @param metadata - any metadata to use.
         * @param quality - the quality value of the encoder.
         * @return a pointer to the generated juce::AudioFormatWriter
         */
        static juce::AudioFormatWriter* createWriter
                (juce::File& f, double sampleRate=48000, int chanCount=2
                , int bps=24, const juce::StringPairArray& metadata=NULL, int quality=0);
    };

}
