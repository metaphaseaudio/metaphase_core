//
// Created by Matt on 8/8/2021.
//

#pragma once

namespace CommandIDs
{
    static const int open_file            = 0x30000;
    static const int close_file           = 0x30001;
    static const int audio_settings       = 0x30200;
    static const int spectrogram_settings = 0x30300;
    static const int about                = 0x30400;
}

namespace AppState
{
    static const std::string OPEN_FILES = "open_files";
    static const std::string RECENT_FILES = "recent_files";
    static const std::string MAIN_WINDOW_POSITION = "main_window_position";
}