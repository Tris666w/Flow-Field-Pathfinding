/*=============================================================================*/
// Copyright 2021-2022
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERenderingTypes.h: Helper class with "types" for rendering
/*=============================================================================*/
#ifndef ELITE_RENDERING_TYPES
#define ELITE_RENDERING_TYPES
#include "../EliteMath/EMath.h"
namespace Elite
{
	//Basic color
	struct Color final
	{
		//=== Datamembers ===
		float r = 0.f, g = 0.f, b = 0.f, a = 1.f;

		//=== Constructors ===
		Color() {}
		Color(float _r, float _g, float _b, float _a = 1.0f)
		{
			r = _r; g = _g; b = _b; a = _a;
		}

	};
	//Basic vertex
	struct Vertex final
	{
		/*
		* DEPTH
		* -1>CLOSE--0>CENTER--1>FAR [-1,1]
		* Immediate Draw Depth Layer> [-0.5,0.5]
		* Depth > [0.5,1.0] ==> Draw behind Debug Rendering (or Immediate Drawers)
		* Depth < [-1.0,-0.5] ==> Draw in front of Debug Rendering (or Immediate Drawers)
		*/
		//=== Datamembers ===
		Elite::Vector3 position = {};
		Color color = {};
		float size = 0.f;

		//=== Constructors ===
		Vertex(const Elite::Vector2& _position, float depth, const Color& _color, float _size = 1.0f) :
			position(_position.x, _position.y, depth), color(_color), size(_size)
		{}

		Vertex(const Elite::Vector2& _position, const Color& _color, float _size = 1.0f) :
			position(_position.x, _position.y, -0.5f), color(_color), size(_size)
		{}

		Vertex(const Elite::Vector3& _position, const Color& _color, float _size = 1.0f) :
			position(_position), color(_color), size(_size)
		{}

		Vertex() {}
	};
}
#endif