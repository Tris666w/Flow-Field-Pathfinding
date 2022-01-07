/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// EMathUtilities.h: Utility class containing a bunch of commonely used functionality (not custom-type specific)
/*=============================================================================*/
#ifndef ELITE_MATH_UTILITIES
#define ELITE_MATH_UTILITIES
//Standard C++ includes
#include <cstdlib>
#include <cfloat>
#include <type_traits>

namespace Elite {
	/* --- CONSTANTS --- */
	#define E_PI	3.14159265358979323846
	#define E_PI_2	1.57079632679489661923
	#define E_PI_4	0.785398163397448309616

	/* --- FUNCTIONS --- */
	/*! Comparing two values (preferably float or doubles) and see if they are equal. You can change the precision (by default: epsilon)*/
	template<typename T, typename = std::enable_if<std::is_pod<T>::value>>
	constexpr bool AreEqual(T a, T b, float precision = FLT_EPSILON)
	{
		if (abs(a - b) > precision)
			return false;
		return true;
	}
	/*! An accurate inverse square root*/
	inline float InvSqrt(float f) //sqrtf not defined as constexpr
	{
		return 1.0f / sqrtf(f);
	}
	/*! An fast inverse square root, not fully accurate. Implementation based on Quake III Arena*/
	/*! Reference: https://betterexplained.com/articles/understanding-quakes-fast-inverse-square-root/ */
	inline float InvSqrtFst(float f)
	{
		const float xHalf = 0.5f * f;
		int i = *reinterpret_cast<int*>(&f);
		i = 0x5f3759df - (i >> 1);
		f = *reinterpret_cast<float*>(&i);
		f = f*(1.5f - xHalf*f*f);
		return f;
	}
	/*! Function to square a number */
	template<typename T, typename = std::enable_if<std::is_pod<T>::value>>
	constexpr auto Square(T v)
	{
		return v*v;
	}
	/*! Function to convert degrees to radians */
	constexpr float ToRadians(const float angle)
	{
		return angle * (static_cast<float>(E_PI) / 180.f);
	}
	/*! Function to convert radians to degrees */
	constexpr float ToDegrees(const float angle)
	{
		return angle * (180.f / static_cast<float>(E_PI));
	}
	/*! Template function to clamp between a minimum and a maximum value -> in STD since c++17 */
	template<typename T>
	constexpr T Clamp(const T a, T min, T max)
	{
		if (a < min)
			return min;

		if (a > max)
			return max;

		return a;
	}

	/*! Template function to clamp between a minimum and a maximum value*/
	template<typename T>
	constexpr T ClampRef(T& a, T min, T max)
	{
		if (a < min)
			a = min;

		if (a > max)
			a = max;

		return a;
	}

	/*! Random Integer */
	inline int randomInt(int max = 1)
	{ return rand() % max; }

	/*! Random Float */
	inline float randomFloat(float max = 1.f)
	{ return max * (float(rand()) / RAND_MAX); }

	/*! Random Float */
	inline float randomFloat(float min, float max)
	{
		auto range = max - min;
		return (range * (float(rand()) / RAND_MAX)) + min;
	}

	/*! Random Binomial Float */
	inline float randomBinomial(float max = 1.f)
	{ return randomFloat(max) - randomFloat(max); }

	/*! Linear Interpolation */
	/*inline float Lerp(float v0, float v1, float t)
	{ return (1 - t) * v0 + t * v1;	}*/
	template<typename T>
	inline T Lerp(T v0, T v1, float t)
	{ return (1 - t) * v0 + t * v1;	}

	/*! Smooth Step */
	inline float smoothStep(float edge0, float edge1, float x)
	{
		// Scale, bias and saturate x to 0..1 range
		x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
		// Evaluate polynomial
		return x * x * (3 - 2 * x);
	}

	/*! Sign Function*/
	template <typename T>  int sign(T val) 
	{ return (T(0) < val) - (val < T(0)); }
}
#endif