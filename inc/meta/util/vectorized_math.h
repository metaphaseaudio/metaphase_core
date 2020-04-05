//
// created by mzapp 4/2/2020
//
#pragma once

#include <JuceHeader.h>

namespace meta
{

	template <typename T>
	struct vecops
	{

		static constexpr void pow(T* in, T* out, size_t n, size_t exponent)
		{
			if (exponent == 0) { std::memset(out, 1, n); }
			else 
			{
				std::memcpy(out, in, sizeof(out) * n);
				for (auto i = exponent; --i > 0;) 
				{
					juce::FloatVectorOperations::multiply(out, out, in, n); 
				} 
			}
		}		
	};
}