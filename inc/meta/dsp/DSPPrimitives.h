//
// Created by Matt on 3/10/2025.
//

#pragma once
#include <array>
#include <cstdint>
#include "meta/util/BitMask.h"
#include "meta/util/math.h"
#include <juce_core/juce_core.h>

namespace meta {
    template<typename T, size_t... Reservations>
    class SharedMemoryBlock
    {
    public:
        static constexpr size_t requested_reservations = (Reservations + ...);
        static constexpr size_t MASK = bitfill<ceillog2(requested_reservations)>::value;

        class Accumulator
        {
        public:
            Accumulator(std::array<T, MASK + 1>& buffer, int32_t offset)
                : r_Buffer(buffer)
                , m_Offset(offset)
                , m_Accum(0)
            {}
            template <size_t N>
            inline size_t getBufferSize() { return GetNth<N, Reservations...>::value; }
            inline void setValue(T value) { m_Accum = value; }
            inline void accumulate(T value) { m_Accum += value; }
            inline void scaleValue(float scale) { m_Accum *= scale; }
            [[ nodiscard ]] inline T getValue() const { return m_Accum; }

            template <size_t N>
            inline void writeToBufferThenScaleBy(int32_t offset, float scale)
            {
                constexpr auto length = GetNth<N, Reservations...>::value;
                constexpr auto base = SumToNth<N, Reservations...>::value - length;

                jassert(offset < length);
                jassert(offset >= 0);

                r_Buffer[(base + offset + m_Offset) & MASK] = m_Accum;
                m_Accum *= scale;
            }

            template <size_t N>
            inline void processAllPass(float coeff, float size=1.0f)
            {
                jassert(size >= 0.1f);
                jassert(size <= 1.0f);

                constexpr auto length = GetNth<N, Reservations...>::value;
                constexpr auto base = SumToNth<N, Reservations...>::value - length;
                const auto readIndex = (m_Offset + base + static_cast<size_t>(length * size) - 1) & MASK;
                const auto writeIndex = (m_Offset + base) & MASK;

                const T delayedOut = r_Buffer[readIndex];
                const T input = delayedOut * -coeff + m_Accum;
                const T output = delayedOut + input * coeff;
                r_Buffer[writeIndex] = input;
                m_Accum = output;
            }

            template <size_t N>
            inline void interpolateAndAccumulateWithScale(float offset, float scale)
            {
                constexpr auto length = GetNth<N, Reservations...>::value;
                constexpr auto base = SumToNth<N, Reservations...>::value - length;

                jassert(offset < length);
                jassert(offset >= 0);

                auto [i, f] = make_integral_and_fractional(offset);
                float a = r_Buffer[(m_Offset + i + base) & MASK];
                float b = r_Buffer[(m_Offset + i + base + 1) & MASK];
                float x = Interpolate<T>::linear(a, b, f);
                m_Accum += x * scale;
            }

            template <size_t N>
            inline void accumulateFromBuffer(size_t offset)
            {
                constexpr auto length = GetNth<N, Reservations...>::value;
                constexpr auto base = SumToNth<N, Reservations...>::value - length;

                jassert(offset < length);
                jassert(offset >= 0);

                m_Accum += r_Buffer[(m_Offset + offset + base) & MASK];
            }

            template <size_t N>
            inline void accumulateFromBufferWithScale(size_t offset, float scale)
            {
                constexpr auto length = GetNth<N, Reservations...>::value;
                constexpr auto base = SumToNth<N, Reservations...>::value - length;

                jassert(offset < length);
                jassert(offset >= 0);

                m_Accum += r_Buffer[(m_Offset + offset + base) & MASK] * scale;
            }

            inline void lowPass(float& state, float coefficient)
            {
                state += coefficient * (m_Accum - state);
                m_Accum = state;
            }

            inline void highPass(float& state, float coefficient)
            {
                state += coefficient * (m_Accum - state);
                m_Accum -= state;
            }

        private:
            std::array<T, MASK + 1>& r_Buffer;
            int32_t m_Offset;
            T m_Accum;

        };

        Accumulator next()
        {
            --m_WriteIndex;
            if (m_WriteIndex < 0)
            { m_WriteIndex += MASK + 1; }
            return Accumulator(m_Buffer, m_WriteIndex);
        }

    private:
        int32_t m_WriteIndex = 0;
        std::array<T, MASK + 1> m_Buffer{0};
    };
}
