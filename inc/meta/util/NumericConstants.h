//
// Created by Matt Zapp on 4/9/2018.
//
//    Various numeric constants. Ok right now it's just float PI.
//

#pragma once
namespace meta
{
    template <typename NumericType>
    struct NumericConstants
    {};


    template <>
    struct NumericConstants<float>
    { const float PI = 3.14159f; };
}