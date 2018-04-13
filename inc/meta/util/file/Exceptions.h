//
// Created by mzapp on 4/10/18.
//
//    Exceptions to be thrown in the case of errors in file handling.
//

#pragma once
#include <JuceHeader.h>

namespace meta
{
    /**
     * Generic File Exception. Contains the file what generated the error, and
     * a short description of the failure.
     *
     * While one should avoid using this exception to throw, as it is almost
     * always going to be more appropriate to use one of the more-specific
     * read/write exceptions, this exception type can be used to catch all
     * exceptions related to file errors.
     */
    class FileException
        : public std::runtime_error
    {
    public:
        FileException(const juce::File& f, const std::string& what)
            : std::runtime_error(what)
            , file(f)
        {}

        juce::File file;
    };

    /// A File Exception generated when a requested file does not exist.
    class FileExistsException
        : public FileException
    {
    public:
        FileExistsException(const juce::File& f, const std::string& what)
                : FileException(f, what)
        {}
    };

    /// A File Exception generated upon a failure to read a file.
    class FileReadException
        : public FileException
    {
    public:
        FileReadException(const juce::File& f, const std::string& what)
            : FileException(f, what)
        {}
    };

    /// A File Exception generated upon a failure to write a file.
    class FileWriteException
        : public FileException
    {
    public:
        FileWriteException(const juce::File& f, const std::string& what)
            : FileException(f, what)
        {}
    };
}