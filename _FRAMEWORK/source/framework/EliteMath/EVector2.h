/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EVector2.h: Vector2D struct
/*=============================================================================*/
#ifndef ELITE_MATH_VECTOR2
#define	ELITE_MATH_VECTOR2
namespace Elite
{
#define ZeroVector2 Vector2()
#define UnitVector2 Vector2(1.f,1.f)

	//Vector 2D
	struct Vector2
	{
		//=== Datamembers ===
		float x = 0.0f;
		float y = 0.0f;

		//=== Constructors ===
		Vector2() = default;
		Vector2(float _x, float _y) :x(_x), y(_y) {};

		//=== Vector Conversions Functions ===
#ifdef USE_BOX2D
		explicit Vector2(const b2Vec2& v) : x(v.x), y(v.y) {};
		Vector2& operator=(const b2Vec2& v) { x = v.x; y = v.y; return *this; }
		operator b2Vec2() const
		{return {x, y};};
#endif

		//=== Arithmetic Operators ===
		inline auto operator-(const Vector2& v) const
		{ return Vector2(x - v.x, y - v.y);	}
		inline auto operator-() const
		{ return Vector2(-x, -y); }
		inline auto operator*(float scale) const
		{ return Vector2(x * scale, y * scale);	}
		inline auto operator/(float scale) const
		{
			const auto revScale = 1.0f / scale;
			return Vector2(x * revScale, y * revScale);
		}

		//=== Compound Assignment Operators === //auto& for type deduction
		inline auto& operator+=(const Vector2& v)
		{ x += v.x; y += v.y; return *this;	}
		inline auto& operator-=(const Vector2& v)
		{ x -= v.x; y -= v.y; return *this;	}
		inline auto& operator*=(float scale)
		{ x *= scale; y *= scale; return *this;	}
		inline auto& operator/=(float scale)
		{
			const auto revScale = 1.0f / scale;
			x *= revScale; y *= revScale; return *this;
		}



		//=== Relational Operators ===
		inline auto operator==(const Vector2& v) const /*Check if both components are equal*/
		{ return AreEqual(x, v.x) && AreEqual(y, v.y);	}
		inline auto operator!=(const Vector2& v) const /*Check if one or both components are NOT equal*/
		{ return !(*this == v);	}

		//=== Member Access Operators ===
		inline float operator[](unsigned int i) const
		{
			return ((i == 0) ? x : y);
			//if (i >= 0 && i < 2)
			//	return ((i == 0) ? x : y);
			//throw; /*TODO : specify error thrown;*/
		}
		inline float& operator[](unsigned int i)
		{
			return ((i == 0) ? x : y);
			//if (i >= 0 && i < 2)
			//	return ((i == 0) ? x : y);
			//throw; /*TODO : specify error thrown;*/
		}

		//=== Internal Vector Functions ===
		inline auto Dot(const Vector2& v) const
		{ return x * v.x + y * v.y;	}

		inline auto Cross(const Vector2& v) const
		{ return x * v.y - y * v.x;	}

		inline auto GetAbs() const
		{ return Vector2(abs(x), abs(y)); }

		inline auto MagnitudeSquared() const
		{ return x*x + y*y;	}

		inline auto Magnitude() const
		{ return sqrtf(MagnitudeSquared()); }

		inline float Normalize()
		{
			auto m = Magnitude();
			if (AreEqual(m, 0.f))
			{
				*this = ZeroVector2;
				return 0.f;
			}

			auto invM = 1.f / m;
			x *= invM;
			y *= invM;

			return m;
		}

		inline Vector2 GetNormalized() const /*! Returns a normalized copy of this vector. This vector does not change.*/
		{
			auto v = Vector2(*this);
			v.Normalize();
			return v;
		}

		inline auto DistanceSquared(const Vector2& v) const
		{ return Square(v.x - x) + Square(v.y - y);	}

		inline auto Distance(const Vector2& v) const
		{ return sqrtf(DistanceSquared(v)); }

		inline auto Clamp(float max)
		{
			auto scale = max / Magnitude();
			scale = scale < 1.f ? scale : 1.f;
			return *this * scale;
		}

		inline auto AngleWith(const Elite::Vector2& other)
		{
			return atan2(x * other.y - other.x * y, x * other.x + y * other.y);
		}
	};

	//=== Global Vector Operators ===
#pragma region GlobalVectorOperators
	inline auto operator+(const Vector2& v, const Vector2& v2)
	{ return Vector2(v.x + v2.x, v.y + v2.y); }

	inline auto operator* (float s, const Vector2& v)
	{ return Vector2(s * v.x, s * v.y); }

	inline auto operator*(const Vector2& a, const Vector2& b)
	{ return Vector2(a.x*b.x, a.y*b.y); }

	inline auto operator/ (float s, const Vector2& v)
	{
		const auto revScale = 1.0f / s;
		return Vector2(revScale * v.x, revScale * v.y);
	}

	inline ostream& operator<<(ostream& os, const Vector2& rhs)
	{
		os << "(" << rhs.x << ", " << rhs.y << " )";
		return os;
	}
#pragma endregion //GlobalVectorOperators

	//=== Global Vector Functions ===
#pragma region GlobalVectorFunctions
	inline auto Dot(const Vector2& v1, const Vector2& v2)
	{ return v1.Dot(v2); }

	inline auto Cross(const Vector2& v1, const Vector2& v2)
	{ return v1.Cross(v2); }

	inline auto GetAbs(const Vector2& v)
	{ return v.GetAbs(); }

	inline void Abs(Vector2& v) /*! Make absolute Vector2 of this Vector2 */
	{ v = v.GetAbs(); }

	inline void Normalize(Vector2& v)
	{ v.Normalize(); }

	inline auto GetNormalized(const Vector2& v)
	{ return v.GetNormalized(); }

	inline auto DistanceSquared(const Vector2& v1, const Vector2& v2)
	{ return v1.DistanceSquared(v2); }

	inline auto Distance(const Vector2& v1, const Vector2& v2)
	{ return v1.Distance(v2); }

	inline auto Clamp(const Vector2& v, float max)
	{
		auto scale = max / v.Magnitude();
		scale = scale < 1.f ? scale : 1.f;
		return v * scale;
	}
#pragma endregion //GlobalVectorFunctions

#pragma region ExtraFunctions
	/*! Random Vector2 */
	inline Vector2 randomVector2(float max = 1.f)
	{
		return{ randomBinomial(max),randomBinomial(max) };
	}
	inline Vector2 randomVector2(float min, float max)
	{
		return{ randomFloat(min, max),randomFloat(min, max) };
	}
	/*! Orientation to a Vector2 */
	inline Vector2 OrientationToVector(float orientation)
	{
		orientation -= static_cast<float>(E_PI_2);
		return Vector2(cos(orientation), sin(orientation));
	}
	/*! Get orientation from an a velocity vector */
	inline float GetOrientationFromVelocity(const Elite::Vector2& velocity)
	{
		if (velocity.Magnitude() == 0)
			return 0.f;

		return atan2f(velocity.x, -velocity.y);
	}

	/*! Get Angle Between 2 vectors*/
	inline float AngleBetween(const Elite::Vector2& v1, const Elite::Vector2& v2) {
		float x = v1.Dot(v2);
		float y = v1.Cross(v2);
		return atan2(y, x);
	}

	/*Calculate the middle of a vector between 2 points*/
	inline Vector2 CalculateMiddle(const Elite::Vector2& v1, const Elite::Vector2& v2)
	{
		Vector2 result{};
		result.x = (v1.x + v2.x) / 2.f;
		result.y = (v1.y + v2.y) / 2.f;
		return result;
	}
#pragma endregion //ExtraFunctions
}
#endif