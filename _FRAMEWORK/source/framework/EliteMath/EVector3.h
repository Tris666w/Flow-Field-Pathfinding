/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EVector2.h: Vector3D struct
/*=============================================================================*/
#ifndef ELITE_MATH_VECTOR3
#define	ELITE_MATH_VECTOR3
namespace Elite {
	
	#define ZeroVector3 Vector3()
	#define UnitVector3 Vector3(1.f,1.f,1.f)

	//Vector 3D
	struct Vector3
	{
		//=== Datamembers ===
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		//=== Constructors ===
		Vector3(){};
		Vector3(float _x, float _y, float _z):x(_x), y(_y), z(_z) {};
		explicit Vector3(const Vector2 v, float _z = 0.f):x(v.x), y(v.y), z(_z) {};
		//Vector3(const Vector3& other); //Copy Constructor
		//Vector3& operator=(const Vector3& other); //Copy Assignment Operator

		//=== Vector Conversions Functions ===
#ifdef USE_BOX2D
		explicit Vector3(const b2Vec3& v) : x(v.x), y(v.y), z(v.z) {};
		Vector3& operator=(const b2Vec3& v) { x = v.x; y = v.y; z = v.z; return *this; }
		explicit operator b2Vec3() const
		{ return b2Vec3(x, y, z); };
#endif

		//=== Arithmetic Operators ===
		inline auto operator+(const Vector3& v) const
		{ return Vector3(x + v.x, y + v.y, z + v.z); }
		inline auto operator-(const Vector3& v) const
		{ return Vector3(x - v.x, y - v.y, z - v.z); }
		inline auto operator*(float scale) const
		{ return Vector3(x * scale, y * scale, z * scale); }
		inline auto operator/(float scale) const
		{
			const auto revScale = 1.0f / scale;
			return Vector3(x * revScale, y * revScale, z * revScale);
		}

		//=== Compound Assignment Operators === //auto& for type deduction
		inline auto& operator+=(const Vector3& v)
		{ x += v.x; y += v.y; z += v.z; return *this; }
		inline auto& operator-=(const Vector3& v)
		{ x -= v.x; y -= v.y; z -= v.z; return *this; }
		inline auto& operator*=(float scale)
		{ x *= scale; y *= scale; z *= scale; return *this; }
		inline auto& operator/=(float scale)
		{
			const auto revScale = 1.0f / scale;
			x *= revScale; y *= revScale; z *= revScale; return *this;
		}

		//=== Relational Operators ===
		inline auto operator==(const Vector3& v) const /*Check if both components are equal*/
		{ return AreEqual(x, v.x) && AreEqual(y, v.y) && AreEqual(z, v.z); }
		inline auto operator!=(const Vector3& v) const /*Check if one or both components are NOT equal*/
		{ return !(*this == v);	}

		//=== Member Access Operators ===
		inline float operator[](unsigned int i) const
		{ return ((i == 0) ? x : y); }
		inline float& operator[](unsigned int i)
		{ return ((i == 0) ? x : y); }

		//=== Internal Vector Functions ===
		inline auto Dot(const Vector3& v) const
		{ return x * v.x + y * v.y + z * v.z; }

		inline auto Cross(const Vector3& v) const
		{ 
			return Vector3(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x);
		}

		inline auto GetAbs() const
		{ return Vector3(abs(x), abs(y), abs(z)); }

		inline auto SqrtMagnitude() const
		{ return x*x + y*y + z*z;	}

		inline auto Magnitude() const
		{ return sqrtf(SqrtMagnitude()); }

		inline void Normalize()
		{
			auto m = Magnitude();
			if (AreEqual(m, 0.f))
			{
				*this = ZeroVector3;
				return;
			}

			auto invM = 1.f / m;
			x *= invM;
			y *= invM;
			z *= invM;
		}
		
		inline Vector3 GetNormalized() const /*! Returns a normalized copy of this vector. This vector does not change.*/
		{
			auto v = Vector3(*this);
			v.Normalize();
			return v;
		}

		inline auto DistanceSquared(const Vector3& v) const
		{ return Square(v.x - x) + Square(v.y - y) + Square(v.z - z); }

		inline auto Distance(const Vector3& v) const
		{ return sqrtf(DistanceSquared(v)); }

		inline auto Project(const Vector3& v) const
		{ return v * ((*this).Dot(v) / v.Dot(v)); }

		inline auto Reject(const Vector3& v) const
		{ return *this - (*this).Project(v);}
	};

	//=== Global Vector Operators ===
#pragma region GlobalVectorOperators
	inline auto operator * (float s, const Vector3& v)
	{ return Vector3(s * v.x, s * v.y, s * v.z); }
#pragma endregion //GlobalVectorOperators

	//=== Global Vector Functions ===
#pragma region GlobalVectorFunctions
	inline auto Dot(const Vector3& v1, const Vector3& v2)
	{ return v1.Dot(v2); }

	inline auto Cross(const Vector3& v1, const Vector3& v2)
	{ return v1.Cross(v2); }

	inline auto GetAbs(const Vector3& v)
	{ return v.GetAbs(); }

	inline void Abs(Vector3& v) /*! Make absolute Vector3 of this Vector3 */
	{ v = v.GetAbs(); }

	inline void Normalize(Vector3& v)
	{ v.Normalize(); }

	inline auto GetNormalized(Vector3& v)
	{ return v.GetNormalized(); }

	inline auto DistanceSquared(const Vector3& v1, const Vector3& v2)
	{ return v1.DistanceSquared(v2);	}

	inline auto Distance(const Vector3& v1, const Vector3& v2)
	{ return v1.Distance(v2); }

	inline auto Project(const Vector3& v1, const Vector3& v2)
	{ return v1.Project(v2); }

	inline auto Reject(const Vector3& v1, const Vector3& v2)
	{ return v1.Reject(v2);	}
#pragma endregion //GlobalVectorFunctions
}
#endif