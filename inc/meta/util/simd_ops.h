//
// created by mzapp 4/2/2020
//
#pragma once
#include <JuceHeader.h>
#include <stdint.h>


namespace meta
{	
	struct SIMDOps32
	{
		using Type = float;
		using ParallelType = juce::dsp::SIMDNativeOps<float>::vSIMDType;;
		using IntegerType = juce::dsp::SIMDNativeOps<float>::vSIMDType;;
		enum { numParallel = 4 };

		static forcedinline ParallelType load_one(Type v)                    noexcept { return _mm_load1_ps(&v); }
		static forcedinline ParallelType load_aligned(const Type* v)         noexcept { return _mm_load_ps(v); }
		static forcedinline ParallelType load_unaligned(const Type* v)       noexcept { return _mm_loadu_ps(v); }

		static forcedinline void store_aligned(Type* dest, ParallelType a)   noexcept { _mm_store_ps(dest, a); }
		static forcedinline void store_unaligned(Type* dest, ParallelType a) noexcept { _mm_storeu_ps(dest, a); }

		static forcedinline ParallelType mul(ParallelType a, ParallelType b) noexcept { return _mm_mul_ps(a, b); }
		static forcedinline ParallelType div(ParallelType a, ParallelType b) noexcept { return _mm_div_ps(a, b); }
		static forcedinline ParallelType hadd(ParallelType a, ParallelType b) noexcept { return _mm_hadd_ps(a, b); }

        static forcedinline ParallelType max(ParallelType a, ParallelType b) noexcept { return _mm_max_ps(a, b); }

		static forcedinline ParallelType cmpeq(ParallelType a, ParallelType b)  noexcept { return _mm_cmpeq_ps(a, b); }
		static forcedinline int movemask(ParallelType a) noexcept { return _mm_movemask_ps(a); }
	};

	struct SIMDOps64
	{
		using Type = double;
		using ParallelType = juce::dsp::SIMDNativeOps<double>::vSIMDType;;
		using IntegerType = juce::dsp::SIMDNativeOps<double>::vSIMDType;;
		enum { numParallel = 4 };

		static forcedinline ParallelType load_one(Type v)              noexcept { return _mm_load1_pd(&v); }
		
		static forcedinline ParallelType load_aligned(const Type* v)   noexcept { return _mm_load_pd(v); }
		static forcedinline ParallelType load_unaligned(const Type* v) noexcept { return _mm_loadu_pd(v); }

		static forcedinline void store_aligned(Type* dest, ParallelType a)   noexcept { _mm_store_pd(dest, a); }
		static forcedinline void store_unaligned(Type* dest, ParallelType a) noexcept { _mm_storeu_pd(dest, a); }

		static forcedinline ParallelType div(ParallelType a, ParallelType b) noexcept { return _mm_div_pd(a, b); }
	};



	template <typename T>
	struct simd
	{
        template<int typeSize> struct ModeType    { using Mode = SIMDOps32; };
        template<>             struct ModeType<8> { using Mode = SIMDOps64; };

        inline static bool isAligned(const void* p) noexcept { return (((std::int64_t)p) & 15) == 0; }
		inline static void inc_ptr(T*& ptr) noexcept { ptr += (16 / sizeof(*ptr)); }


		template <typename Op, typename VOp>
		inline static void do_op_src_const(const Op& op, const VOp& vop, T* dst, const T& src, long n)
		{
			using Mode = ModeType<sizeof(*dst)>::Mode;
			const int nSIMDOps = n / Mode::numParallel;

			const auto dstLoad = (isAligned(dst)) ? Mode::load_aligned : Mode::load_unaligned;
			const auto dstStore = (isAligned(dst)) ? Mode::store_aligned : Mode::store_unaligned;
			const Mode::ParallelType s = Mode::load_one(src);

			for (int i = 0; i < nSIMDOps; i++)
			{				
				const Mode::ParallelType d = dstLoad(dst);
				dstStore(dst, vop(d, s));
				dst += (16 / sizeof(*dst));
			}

			n &= (Mode::numParallel - 1);
			if (n == 0) return;
			for (int i = 0; i < n; ++i) { op(dst[i], src); }
		}


		template <typename Op, typename VOp>
		inline static void do_op_src_dst(const Op& op, const VOp& vop, T* dst, const T* src, long n)
		{
			using Mode = ModeType<sizeof(*dst)>::Mode;
			const int nSIMDOps = n / Mode::numParallel;

			const auto srcLoad  = (isAligned(src)) ? Mode::load_aligned : Mode::load_unaligned;
			const auto dstLoad  = (isAligned(dst)) ? Mode::load_aligned : Mode::load_unaligned;
			const auto dstStore = (isAligned(dst)) ? Mode::store_aligned : Mode::store_unaligned;

			for (int i = 0; i < nSIMDOps; i++)
			{
				const Mode::ParallelType s = srcLoad(src);
				const Mode::ParallelType d = dstLoad(dst);
				dstStore(dst, vop(d, s));
				dst += (16 / sizeof(*dst)); src += (16 / sizeof(*dst));
			}

			n &= (Mode::numParallel - 1);
			if (n == 0) return;
			for (int i = 0; i < n; ++i) { op(dst[i], src[i]); }
		}


		template <typename Op, typename VOp>
		inline static void do_op_src_src_dst(const Op& op, const VOp& vop, T* dst, const T* srca, const T* srcb, long n)
		{
			using Mode = ModeType<sizeof(*dst)>::Mode;
			const int nSIMDOps = n / Mode::numParallel;

			const auto srcaLoad = (isAligned(srca)) ? Mode::load_aligned : Mode::load_unaligned;
			const auto srcbLoad = (isAligned(srcb)) ? Mode::load_aligned : Mode::load_unaligned;
			const auto dstStore = (isAligned(dst)) ? Mode::store_aligned : Mode::store_unaligned;

			for (int i = 0; i < nSIMDOps; i++)
			{
				const Mode::ParallelType sa = srcaLoad(srca);
				const Mode::ParallelType sb = srcbLoad(srcb);
				dstStore(dst, vop(sa, sb));
				dst += (16 / sizeof(*dst)); srca += (16 / sizeof(*dst)); srcb += (16 / sizeof(*dst));
			}

			n &= (Mode::numParallel - 1);
			if (n == 0) return;
			for (int i = 0; i < n; ++i) { op(dst[i], srca[i], srcb[i]); }
		}

		///////////////////////////////////////////////////////////////////////

		static void div(T* dst, const T& src, size_t n)
		{ do_op_src_const([](T& x, const T& y) { x /= y; }, ModeType<sizeof(*dst)>::Mode::div, dst, src, n); }

		static void div(T* dst, const T* src, size_t n)
		{ do_op_src_dst([](T& x, const T& y) { x /= y; }, ModeType<sizeof(*dst)>::Mode::div, dst, src, n); }


		static void div(T* dst, const T* srca, const T* srcb, size_t n)
		{ 
			do_op_src_src_dst([](T& x, const T& y, const T& z) { x = y / z; },
				              ModeType<sizeof(*dst)>::Mode::div,
							  dst, srca, srcb, n); 
		}


		inline static bool eq(const T* srca, const T* srcb, long n)
		{
			using Mode = ModeType<sizeof(*srca)>::Mode;
			const int nSIMDOps = n / Mode::numParallel;

			const auto srcaLoad = (isAligned(srca)) ? Mode::load_aligned : Mode::load_unaligned;
			const auto srcbLoad = (isAligned(srcb)) ? Mode::load_aligned : Mode::load_unaligned;
			
			for (int i = 0; i < nSIMDOps; i++)
			{
				const Mode::ParallelType sa = srcaLoad(srca);
				const Mode::ParallelType sb = srcbLoad(srcb);
				const auto mask = Mode::movemask(Mode::cmpeq(sa, sb));
				if (mask != 0xF) { return false; }
				srca += (16 / sizeof(*srca)); srcb += (16 / sizeof(*srcb));
			}

			n &= (Mode::numParallel - 1);
			if (n == 0) return true;
			for (int i = 0; i < n; ++i) { if (srca[i] != srcb[i]) { return false; } }
			return true;
		}


		inline static T accumulate(const T* src, long n)
		{
			using Mode = ModeType<sizeof(*src)>::Mode;
			const int nSIMDOps = n / Mode::numParallel;

			T tmp[4] = { 0, 0, 0, 0 };
			const auto srcLoad = (isAligned(src)) ? Mode::load_aligned : Mode::load_unaligned;
			const auto tmpStore = (isAligned(tmp)) ? Mode::store_aligned : Mode::store_unaligned;

			Mode::ParallelType running_sum = Mode::load_one(0);
			for (int i = 0; i < nSIMDOps; i++)
			{
				const Mode::ParallelType s = srcLoad(src);
				running_sum = Mode::hadd(running_sum, s);
				src += (16 / sizeof(*src));
			}

			tmpStore(tmp, running_sum);
			n &= (Mode::numParallel - 1);
			T sum = tmp[0] + tmp[1] + tmp[2] + tmp[3];

			if (n == 0) return sum;
			for (int i = 0; i < n; ++i) { sum += src[i]; }
			return sum;
		}


		static void pow(T* out, const T* in, size_t n, int64_t exponent)
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