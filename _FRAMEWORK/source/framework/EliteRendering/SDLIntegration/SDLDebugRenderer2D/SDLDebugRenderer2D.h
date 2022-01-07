/*=============================================================================*/
// Copyright 2021-2022
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================
DebugRenderer2D.h: OpenGL immediate 2D debug renderer, used for drawing 2D shapes.
=============================================================================*/
#ifndef ELITE_SDL_RENDERER_2D_H
#define ELITE_SDL_RENDERER_2D_H

//--- Includes ---
#include "../../EDebugRenderer2D.h"
#include "../../ERenderingTypes.h"
#include "../../../EliteGeometry/EGeometry2DTypes.h"
#include "../../Shaders.h"

namespace Elite
{
	class SDLDebugRenderer2D final : public EDebugRenderer2D<SDLDebugRenderer2D>, public ESingleton<SDLDebugRenderer2D>
	{
	public:
		//--- Constructor & Destructor ---
		SDLDebugRenderer2D() = default;
		~SDLDebugRenderer2D() { Shutdown(); };

		//--- Functions ---
		void Initialize(Camera2D* pActiveCamera);
		void Render();
		unsigned int LoadShadersToProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
		unsigned int LoadShadersToProgramFromEmbeddedSource(const char* vertexShader, const char* fragmentShader);

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

	private:
		//--- Datamembers ---
		//PROGRAM, VERTEX & ATTRIBUTE DATA
		unsigned int m_programID = 0;
		int m_projectionUniform = 0;
		int m_positionAttribute = 0;
		int m_colorAttribute = 1;
		int m_sizeAttribute = 2;
		unsigned int m_vaoId = 0;
		unsigned int m_bufferIDs[1];

		//Functions
		void Shutdown();
	};
}
#endif