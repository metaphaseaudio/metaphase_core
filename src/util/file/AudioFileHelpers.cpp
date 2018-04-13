//
// Created by mzapp on 4/10/18.
//

#include <meta/util/file/AudioFileHelpers.h>
#include <meta/util/file/Exceptions.h>

juce::AudioFormatWriter*
meta::AudioFileHelpers::createWriter
(juce::File &f, double sampleRate, int numChans, int bps
, const juce::StringPairArray &metadata, int quality)
{
    if (!f.existsAsFile()) { throw meta::FileWriteException(f, "Target file does not exist"); }

    juce::AudioFormatManager manager;
    manager.registerBasicFormats();

    std::unique_ptr<juce::FileOutputStream> outStream(f.createOutputStream());

    for (int i = 0; i < manager.getNumKnownFormats(); i++)
    {
        auto format = manager.getKnownFormat(i);
        if (format->canHandleFile(f))
        {
            return format->createWriterFor
                    (outStream.release(), sampleRate, numChans, bps, metadata, quality);
        };
    };

    throw meta::FileWriteException(f, "No recognized default format");
}
