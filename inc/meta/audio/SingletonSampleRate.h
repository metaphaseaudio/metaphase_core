//
// Created by mzapp on 4/11/18.
//
// To avoid passing around, or keeping multiple copies of an application's
// sample rate, use this singleton.
//
// Be careful, singletons are a bad practice and this could lead to unexpected
// behaviour when running multiple copies of the metaphase core library.
//

#pragma once

namespace meta
{
    template <typename NumericType>
    class SingletonSampleRate
    {
    public:
        static SingletonSampleRate& getInstance()
        {
            static SingletonSampleRate instance;
            return instance;
        }

        static NumericType& getValue()
            { return getInstance().value; }

        static void setValue(NumericType x)
            { getInstance().value = x; }

        SingletonSampleRate(const SingletonSampleRate&) = delete;
        SingletonSampleRate& operator=(const SingletonSampleRate&) = delete;

    private:
        SingletonSampleRate() : value(44100) {}
        NumericType value;
    };
}