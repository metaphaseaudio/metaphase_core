//
// Created by Matt on 5/3/2022.
//

#pragma once

#include <functional>

namespace meta
{
    template <int tickCount>
    class Countdown
    {
    public:
        explicit Countdown(std::function<void()> onZero = [](){}, int startingCount=tickCount)
            : onZero(std::move(onZero))
        { sync(startingCount); }

        void sync(int newCount=tickCount)
        {
            assert(newCount <= tickCount);
            assert(newCount > 0);
            count = tickCount;
        }

        int tick()
        {
            --count;

            if (count == 0) {
                count = tickCount;
                onZero();
                return 0;
            }

            return count;
        }

        std::function<void()> onZero;

    private:
        int count;
    };
}