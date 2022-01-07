/*=============================================================================*/
// Copyright 2021-2022
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================
DebugRenderer2D.h: OpenGL immediate 2D debug renderer, used for drawing 2D shapes.
=============================================================================*/
#ifndef ELITE_DEBUG_RENDERER_2D_H
#define ELITE_DEBUG_RENDERER_2D_H
//--- Includes ---
#include "2DCamera/ECamera2D.h"
#include "ERenderingTypes.h"
#include "../EliteGeometry/EGeometry2DTypes.h"
namespace Elite
{
	template<typename Impl>
	class EDebugRenderer2D
	{
	public:
		//--- Constructor & Destructor ---
		EDebugRenderer2D() = default;
		~EDebugRenderer2D() = default;

		//--- Functions ---
		void Initialize(Camera2D* pActiveCamera);
		void Render();
		unsigned int LoadShadersToProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
		unsigned int LoadShadersToProgramFromEmbeddedSource(const char* vertexShader, const char* fragmentShader);
		Camera2D* GetActiveCamera() const { return m_pActiveCamera; }

		//--- User Functions ---
		void DrawPolygon(Elite::Polygon* polygon, const Color& color, float depth);
		void DrawPolygon(Elite::Polygon* polygon, const Color& color);
		void DrawPolygon(const Elite::Vector2* points, int count, const Color& color, float depth);
		void DrawSolidPolygon(Elite::Polygon* polygon, const Color& color, float depth, bool triangulate = false);
		void DrawSolidPolygon(const Elite::Polygon* polygon, const Color& color, float depth);
		void DrawSolidPolygon(const Elite::Vector2* points, int count, const Color& color, float depth, bool triangulate = false);
		void DrawCircle(const Elite::Vector2& center, float radius, const Color& color, float depth);
		void DrawSolidCircle(const Elite::Vector2& center, float radius, const Elite::Vector2& axis, const Color& color, float depth);
		void DrawSolidCircle(const Elite::Vector2& center, float radius, const Elite::Vector2& axis, const Color& color);
		void DrawSegment(const Elite::Vector2& p1, const Elite::Vector2& p2, const Color& color, float depth);
		void DrawSegment(const Elite::Vector2& p1, const Elite::Vector2& p2, const Color& color);
		void DrawDirection(const Elite::Vector2& p, const Elite::Vector2& dir, float length, const Color& color, float depth = 0.9f);
		void DrawTransform(const Elite::Vector2& p, const Elite::Vector2& xAxis, const Elite::Vector2& yAxis, float depth);
		void DrawPoint(const Elite::Vector2& p, float size, const Color& color, float depth = 0.9f);
		void DrawString(int x, int y, const char* string, ...) const;
		void DrawString(const Elite::Vector2& pw, const char* string, ...) const;

		inline float NextDepthSlice();

	protected:
		//General
		Camera2D* m_pActiveCamera = nullptr;
		float m_CurrDepthSlice = 0.f;

		//Containers of different INTERLEAVED primitiveTypes
		std::vector<Vertex> m_vPoints;
		std::vector<Vertex> m_vLines;
		std::vector<Vertex> m_vTriangles;

		//Functions
		void Shutdown();
	};
}
#endif