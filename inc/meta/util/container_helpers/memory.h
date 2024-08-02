//
// Created by matt on 8/2/24.
//
#pragma once
#include <memory>

namespace meta
{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique_struct(Args&&... args)
    {
        return std::unique_ptr<T>(new T{std::forward<Args>(args)...});
    }
}
