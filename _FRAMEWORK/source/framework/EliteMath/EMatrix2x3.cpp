#include "stdafx.h"
#include <cmath>
#include <string>
#include <iostream>
#include "EMatrix2x3.h"


Matrix2x3::Matrix2x3(Elite::Vector2 dirX, Elite::Vector2 dirY, Elite::Vector2 orig)
	: dirX{ dirX }, dirY{ dirY }, orig{ orig }
{}

Matrix2x3::Matrix2x3(float e1X, float e1Y, float e2X, float e2Y, float oX, float oY) 
	: Matrix2x3{ Elite::Vector2{e1X, e1Y}, Elite::Vector2{e2X, e2Y}, Elite::Vector2{oX, oY} }
{}

Elite::Vector2 Matrix2x3::Transform(const Elite::Vector2& vector) const
{
	return Elite::Vector2{ vector.x * dirX + vector.y * dirY } + orig;
}

float Matrix2x3::Determinant() const
{
	return dirX.x * dirY.y - dirX.y * dirY.x;
}

Matrix2x3 Matrix2x3::Inverse() const
{
	//Calculate Determinant
	float det = Determinant();
	
	//1)calculate matrix of minors
	//2)Use the alternating law of signs to produce the matrix of cofactors 
	//3)Transpose
	//4)the inverse matrix is 1/Determinant * the resulting matrix
	return Matrix2x3{
		Elite::Vector2(+dirY.y, -dirX.y) / det,
		Elite::Vector2(-dirY.x, +dirX.x) / det,
		Elite::Vector2(dirY.x * orig.y - dirY.y * orig.x, -(dirX.x * orig.y - dirX.y * orig.x)) / det
	};
}

bool Matrix2x3::Equals(const Matrix2x3& other, float epsilon ) const
{
	return (dirX == other.dirX) && 
		(dirY == other.dirY) && 
		(orig == other.orig);
}

std::string Matrix2x3::ToString() const
{
	return std::string( "Matrix2x3( x( ")  + 
		std::to_string(dirX.x) + ", " + std::to_string( dirX.y ) 
		+ " ), y( " + std::to_string( dirY.x ) + ", " + std::to_string( dirY.y )
		+ " ), orig( " + std::to_string( orig.x ) + ", " + std::to_string( orig.y ) +  " )  )";
}

void Matrix2x3::SetAsIdentity()
{
	dirX = Elite::Vector2{1, 0};
	dirY = Elite::Vector2{0, 1};
	orig = Elite::Vector2{0, 0};
}

void Matrix2x3::SetAsRotate(float degrees)
{
	float radians = degrees * 3.1415926535f / 180;
	dirX = Elite::Vector2{ cos( radians ), sin( radians ) };
	dirY = Elite::Vector2{ -sin( radians ), cos( radians ) };
	orig = Elite::Vector2{ 0, 0 };
}
void Matrix2x3::SetAsTranslate(float tx, float ty)
{
	dirX = Elite::Vector2{ 1, 0 };
	dirY = Elite::Vector2{ 0, 1 };
	orig = Elite::Vector2{ tx, ty };
}

void Matrix2x3::SetAsTranslate(Elite::Vector2 pt)
{
	dirX = Elite::Vector2{ 1, 0 };
	dirY = Elite::Vector2{ 0, 1 };
	orig = Elite::Vector2{ pt.x, pt.y };
}

void Matrix2x3::SetAsScale(float scaleX, float scaleY)
{
	dirX = Elite::Vector2{ scaleX, 0 };
	dirY = Elite::Vector2{ 0, scaleY };
	orig = Elite::Vector2{ 0, 0 };
}

void Matrix2x3::SetAsScale(float scale)
{
	SetAsScale(scale, scale);
}

Matrix2x3 Matrix2x3::CreateRotationMatrix(float degrees)
{
	float radians = degrees * 3.1415926535f / 180;
	return Matrix2x3( Elite::Vector2{ cos( radians ), sin( radians ) }, Elite::Vector2{ -sin(radians), cos( radians ) }, Elite::Vector2{} );
}

Matrix2x3 Matrix2x3::CreateIdentityMatrix()
{
	return Matrix2x3( Elite::Vector2{ 1, 0 }, Elite::Vector2{ 0, 1 }, Elite::Vector2{} );
}

Matrix2x3 Matrix2x3::CreateScalingMatrix(float scale)
{
	return CreateScalingMatrix(scale, scale);
}

Matrix2x3 Matrix2x3::CreateScalingMatrix(Elite::Vector2 scaleVector)
{
	return CreateScalingMatrix(scaleVector.x, scaleVector.y);
}

Matrix2x3 Matrix2x3::CreateScalingMatrix(float scaleX, float scaleY)
{
	return Matrix2x3( Elite::Vector2{ scaleX, 0 }, Elite::Vector2{ 0, scaleY }, Elite::Vector2{} );
}

Matrix2x3 Matrix2x3::CreateTranslationMatrix(Elite::Vector2 origin)
{
	return Matrix2x3( Elite::Vector2{ 1, 0 }, Elite::Vector2{ 0, 1 }, origin );
}

Matrix2x3 Matrix2x3::CreateTranslationMatrix(float tx, float ty)
{
	return CreateTranslationMatrix( Elite::Vector2{ tx, ty } );
}

// Operator overloading functionality
bool operator==(const Matrix2x3& lhs, const Matrix2x3& rhs) 
{
	return lhs.Equals(rhs);
}

bool operator!=(const Matrix2x3& lhs, const Matrix2x3& rhs) 
{
	return !(lhs == rhs);
}

Matrix2x3 operator*(const Matrix2x3& lhs, const Matrix2x3& rhs)
{
	return Matrix2x3{
		Elite::Vector2{ rhs.dirX.x * lhs.dirX.x + rhs.dirX.y * lhs.dirY.x, rhs.dirX.x * lhs.dirX.y + rhs.dirX.y * lhs.dirY.y },
		Elite::Vector2{ rhs.dirY.x * lhs.dirX.x + rhs.dirY.y * lhs.dirY.x, rhs.dirY.x * lhs.dirX.y + rhs.dirY.y * lhs.dirY.y },
		Elite::Vector2{ rhs.orig.x * lhs.dirX.x + rhs.orig.y * lhs.dirY.x + lhs.orig.x, rhs.orig.x * lhs.dirX.y + rhs.orig.y * lhs.dirY.y + lhs.orig.y }
	};
}

std::ostream& operator<<(std::ostream& os, const Matrix2x3& matrix )
{
	os << matrix.ToString( );
	return os;
}

