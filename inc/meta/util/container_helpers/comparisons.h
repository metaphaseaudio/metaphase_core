//
// Created by Matt on 7/6/2021.
//

#pragma once

namespace meta
{
    template <class iterator>
    iterator argmax(iterator& begin, const iterator& end)
    {
        iterator max = begin;
        for (auto itr = begin; itr < end; itr++) { max = *itr > *max ? itr : max; }
        return max;
    }

    template <typename T>
    T* argmax(T* begin, size_t n)
    {
        T* max = begin;
        for (auto i = 0; i < n; i++) { max = *(begin + i) > *max ? begin + i : max; }
        return max;
    }
}