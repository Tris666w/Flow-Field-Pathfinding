/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EMat22.h: Row Major Matrix 2x2 struct
// | x1 , y1 |
// | x2 , y2 |
// Info: Row Major Matrix for cache coherency, even though this is a small piece of data, 
// it is a good practice. 
/*=============================================================================*/
#ifndef ELITE_MATH_MATRIX22
#define	ELITE_MATH_MATRIX22

namespace Elite {

	#define IdentityMat22 Mat22();

	//Matrix 2x2
	struct Mat22
	{
		//=== Datamembers ===
		Vector2 r[2] = { {1.f,0.f},{0.f, 1.f} };

		//=== Constructors ===
		Mat22() {};
		Mat22(float x1, float y1, float x2, float y2)
		{ r[0] = Vector2(x1, y1); r[1] = Vector2(x2, y2); };
		Mat22(const Vector2& r1, const Vector2& r2)
		{ r[0] = r1; r[1] = r2;	};

		//=== Matrix Conversions Functions ===
#ifdef USE_BOX2D
		//From row-major to column-major
		Mat22& operator=(const b2Mat22& m)
		{
			r[0][0] = m.ex.x; r[0][1] = m.ex.y;
			r[1][0] = m.ey.x; r[1][1] = m.ey.y;
			return *this;
		}
		operator b2Mat22() const
		{
			return b2Mat22(r[0][0], r[1][0], r[0][1], r[1][1]);
		}
#endif

		//=== Arithmetic Operators ===
		inline auto operator+(const Mat22& m) const
		{ return Mat22(r[0] + m.r[0], r[1] + m.r[1]); }
		inline auto operator-(const Mat22& m) const
		{ return Mat22(r[0] - m.r[0], r[1] - m.r[1]); }
		inline auto operator*(float scale) const
		{ return Mat22(r[0] * scale, r[1] * scale);	}
		inline auto operator*(const Mat22& m) const
		{
			//The rows of the first matrix are multiplied by the columns of the second one
			Mat22 res = {};
			for(auto row = 0; row < 2; ++row)
			{
				for(auto col = 0; col < 2; ++col)
				{
					res.r[row][col] = r[row][0] * m.r[0][col] +
						r[row][1] * m.r[1][col];
				}
			}
			return res;
		}
		inline auto operator*(const Vector2& v) const
		{
			//Same principle as mat22 * mat22, but with a "column" Vector2...
			return Vector2(
				r[0][0] * v.x + r[0][1] * v.y,
				r[1][0] * v.x + r[1][1] * v.y);
		}

		//=== Compound Assignment Operators ===
		inline auto& operator+=(const Mat22& m)
		{ r[0] += m.r[0]; r[1] += m.r[1]; return *this;	}
		inline auto& operator-=(const Mat22& m)
		{ r[0] -= m.r[0]; r[1] -= m.r[1]; return *this;	}
		inline auto& operator*=(float scale)
		{ r[0] *= scale; r[1] *= scale; return *this; }
		inline auto& operator*=(const Mat22& m)
		{ auto result = *this * m; return *this = result; }

		//=== Internal Vector Functions ===
		void SetIdentity()
		{ r[0] = Vector2( 1.f, 0.f); r[1] = Vector2(0.f, 1.f); }
		auto Determinant() const //Formula: x1*y2 - y1*x2
		{ return r[0][0] * r[1][1] - r[0][1] * r[1][0];	}
		auto Inverse() const
		{
			auto det = Determinant();
			if (AreEqual(det, 0.f)) //We cannot get inverse because determinant is zero, return identity matrix instead
				return IdentityMat22;

			det = 1.f / det;
			auto m00 = det * r[1][1], m01 = det * (-r[0][1]);
			auto m10 = det * (-r[1][0]), m11 = det * r[0][0];
			return Mat22(m00, m01, m10, m11);
		}
	};

	//=== Global Matrix Functions ===
	inline auto GetDeterminant(const Mat22& m)
	{ return m.Determinant(); }

	inline auto GetInverse(const Mat22& m)
	{ return m.Inverse(); }
}
#endif
