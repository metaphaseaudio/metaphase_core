//
// Created by Matt on 4/22/2023.
//
#include "meta/util/StringHelpers.h"

std::vector<std::string> meta::StringHelpers::split(const std::string& str, const std::string& delimiter)
{
    std::string s = str;
    std::vector<std::string> rv;
    size_t pos;

    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        auto token = s.substr(0, pos);
        rv.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    if (s.size()) { rv.push_back(s); }
    return rv;
}

std::string meta::StringHelpers::join(const std::vector<std::string>& in, const std::string& delimiter)
{
    std::string rv;
    for (auto itr = std::begin(in); itr != std::end(in); itr++)
    {
        rv += *itr;
        if (itr != std::end(in) - 1) { rv += delimiter; }
    }

    return rv;
}
