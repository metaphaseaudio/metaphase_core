//
// Created by Matt on 4/22/2023.
//

#pragma once
#include <string>
#include <vector>

namespace meta
{
    class StringHelpers
    {
    public:
        static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
        static std::string join(const std::vector<std::string>& in, const std::string& delimiter);
    };
}
