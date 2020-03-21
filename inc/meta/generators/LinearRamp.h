//
// Created by Matt Zapp on 3/18/2020.
//
#pragma once
#include <exception>

namespace meta 
{
	class LinearRamp
	{
	public:
		LinearRamp(size_t start = 0, size_t end = 1, size_t ticks = 0)
			: m_Start(start)
			, m_End(end)
			, m_Ticks(ticks)
			, m_Remaining(ticks)
		{};

		void setStart(size_t start) { m_Start = start; };
		void setEnd(size_t end) { m_End = end; };
		void setTicks(size_t ticks)
		{
			m_Ticks = ticks; 
			m_Remaining = std::min(m_Ticks, m_Remaining);
		}

		void reset() { m_Remaining = m_Ticks; }

		float tick() 
		{
			if (m_Remaining > 0 && m_Ticks > 0) 
			{ 
				auto f = float(m_Remaining--) / float(m_Ticks);
				return (m_Start * f) + (m_End * (1.0f - f));
			}

			return m_End;
		}

		float getProgress() { return 1.0f - (float(m_Remaining) / float(m_Ticks)); }

	private:
		size_t m_Start;
		size_t m_End;
		size_t m_Ticks;
		size_t m_Remaining;
	};
}
