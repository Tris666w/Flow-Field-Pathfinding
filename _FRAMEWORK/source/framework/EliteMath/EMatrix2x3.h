#pragma once
#include <vector>

struct Matrix2x3
{
	// -------------------------
	// Constructors 
	// -------------------------
	// Default constructor results in a unity matrix
	explicit Matrix2x3( Elite::Vector2 dirX = Elite::Vector2{ 1, 0 }, Elite::Vector2 dirY = Elite::Vector2{ 0, 1 }, Elite::Vector2 origTrans = Elite::Vector2{ 0, 0 } );
	// Constructor, using floats, all required
	explicit Matrix2x3( float e1X, float e1Y, float e2X, float e2Y, float oX, float oY );

	// -------------------------
	// General Methods 
	// -------------------------
	// Elite::Vector2 vTransformed = mat.Transform(v);
	Elite::Vector2 Transform( const Elite::Vector2& v ) const;

	// Calculate the determinant
	float Determinant( ) const;

	// Calculate the inverse matrix
	Matrix2x3 Inverse( ) const;

	// Are two matrices equal within a threshold?	
	// mat1.Equals(mat2)
	bool Equals( const Matrix2x3& other, float epsilon = 0.001f ) const;

	// Creates a string containing a text representation of the values of the matrix
	std::string ToString( ) const;

	// Converts this matrix into a Identity matrix
	void SetAsIdentity( );
	// Converts this matrix into a Rotate matrix
	void SetAsRotate( float degrees );
	// Converts this matrix into a Translation matrix
	void SetAsTranslate( float tx, float ty );
	// Converts this matrix into a Translation matrix
	void SetAsTranslate( Elite::Vector2 pt );
	// Converts this matrix into a Scale matrix
	void SetAsScale( float sx, float sy );
	// Converts this matrix into a Scale matrix
	void SetAsScale( float s );

	// -------------------------------------------
	// Static Matrix2x3 object creation methods 
	// -------------------------------------------
	// Instantiate a rotation matrix: 
	// Matrix matRot = Matrix::Rotation(45.0f);
	static Matrix2x3 CreateRotationMatrix( float degrees );
	// Instantiate an identity matrix: 
	// Matrix2x3 matId = Matrix2x3::Identity();
	static Matrix2x3 CreateIdentityMatrix( );
	// Instantiate a scale matrix: 
	// Matrix matScale = Matrix::Scaling(2.0f);
	static Matrix2x3 CreateScalingMatrix( float scale );
	// Instantiate a scale matrix: 
	// Matrix matScale = Matrix::Scaling(2.0f,-3.0f);
	static Matrix2x3 CreateScalingMatrix( float scaleX, float scaleY );
	// Instantiate a scale matrix: 
	// Matrix matScale = Matrix::Scaling( Elite::Vector2(2.0f,-3.0f) );
	static Matrix2x3 CreateScalingMatrix( Elite::Vector2 scaleVector );
	// Instantiate a translation matrix: 
	// Matrix matTrans = Matrix::Translation( Elite::Vector2(2.0f,3.0f) );
	static Matrix2x3 CreateTranslationMatrix( Elite::Vector2 origin );
	// Instantiate a translation matrix: 
	// Matrix matTrans = Matrix::Translation(2.0f, 3.0f);
	static Matrix2x3 CreateTranslationMatrix( float tx, float ty );

	// -------------------------
	// Datamembers
	// -------------------------
	Elite::Vector2 dirX;	// The first matrix vector (the "x-axis"), 1st column
	Elite::Vector2 dirY;	// The second matrix vector (the "y-axis"), second column
	Elite::Vector2 orig; 	// The origin of  the coordinate matrix (the "translation"), third column
};

// -------------------------
// Operators 
// -------------------------
// Are two matrices exactly equal?				
// mat1 == mat2
bool operator==( const Matrix2x3& lhs, const Matrix2x3& rhs );
// Are two matrices exactly unequal?			
// mat1 != mat2
bool operator!=( const Matrix2x3& lhs, const Matrix2x3& rhs );
// Multiply matrices
// Matrix2x3 matProduct {mat1 * mat2};
Matrix2x3 operator*( const Matrix2x3& lhs, const Matrix2x3& rhs );
// Send matrix to output stream
// std::cout << mat;
std::ostream& operator<<( std::ostream& os, const Matrix2x3& matrix );