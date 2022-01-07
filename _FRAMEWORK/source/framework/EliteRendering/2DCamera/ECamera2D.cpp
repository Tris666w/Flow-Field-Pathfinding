//=== General Includes ===
#include "stdafx.h"
#include "ECamera2D.h"

//=== Constructor & Destructor
Camera2D::Camera2D(unsigned int width, unsigned int height):
	m_width(width), m_height(height)
{
	m_center = Elite::ZeroVector2;
	m_zoom = 20.0f;
};

//=== Base Functions ===
void Camera2D::Update()
{
	if (INPUTMANAGER->IsMouseButtonDown(Elite::InputMouseButton::eRight) && !m_isMoveLocked)
	{
		m_isRightMouseDown = true;
		auto mouseData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eRight);
		m_lastPosition = ConvertScreenToWorld(Elite::Vector2((float)mouseData.X, (float)mouseData.Y));
	}
	if (INPUTMANAGER->IsMouseMoving() && m_isRightMouseDown && !m_isMoveLocked)
	{
		auto mouseData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseMotion);
		auto pos = ConvertScreenToWorld(Elite::Vector2(float(mouseData.X), float(mouseData.Y)));
		const Elite::Vector2 difference = pos - m_lastPosition;
		m_center -= difference;
	}
	if (INPUTMANAGER->IsMouseButtonUp(Elite::InputMouseButton::eRight))
		m_isRightMouseDown = false;

	if (INPUTMANAGER->IsMouseScrolling() && !m_isZoomLocked)
	{
		auto mouseData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseWheel);
		if (mouseData.Y > 0)
			m_zoom /= 1.1f;
		else if(mouseData.Y < 0)
			m_zoom *= 1.1f;
	}
}

Elite::Vector2 Camera2D::ConvertScreenToWorld(const Elite::Vector2& screenPoint) const
{
	const auto w = float(m_width);
	const auto h = float(m_height);
	const auto u = screenPoint.x / w;
	const auto v = (h - screenPoint.y) / h;

	const auto ratio = w / h;
	Elite::Vector2 extents(ratio, 1.0f);
	extents *= m_zoom;

	const Elite::Vector2 lower = m_center - extents;
	const Elite::Vector2 upper = m_center + extents;

	Elite::Vector2 pw = {};
	pw.x = (1.0f - u) * lower.x + u * upper.x;
	pw.y = (1.0f - v) * lower.y + v * upper.y;
	return pw;
}

Elite::Vector2 Camera2D::ConvertWorldToScreen(const Elite::Vector2& worldPoint) const
{
	const auto w = float(m_width);
	const auto h = float(m_height);
	const auto ratio = w / h;
	Elite::Vector2 extents(ratio, 1.0f);
	extents *= m_zoom;

	const Elite::Vector2 lower = m_center - extents;
	const Elite::Vector2 upper = m_center + extents;

	const auto u = (worldPoint.x - lower.x) / (upper.x - lower.x);
	const auto v = (worldPoint.y - lower.y) / (upper.y - lower.y);

	Elite::Vector2 ps;
	ps.x = u * w;
	ps.y = (1.0f - v) * h;
	return ps;
}

// Convert from world coordinates to normalized device coordinates.
// http://www.songho.ca/opengl/gl_projectionmatrix.html
void Camera2D::BuildProjectionMatrix(float* m, float zBias) const
{
	const auto w = float(m_width);
	const auto h = float(m_height);
	const auto ratio = w / h;
	Elite::Vector2 extents(ratio, 1.0f);
	extents *= m_zoom;

	const Elite::Vector2 lower = m_center - extents;
	const Elite::Vector2 upper = m_center + extents;

	m[0] = 2.0f / (upper.x - lower.x);
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = 2.0f / (upper.y - lower.y);
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;

	m[12] = -(upper.x + lower.x) / (upper.x - lower.x);
	m[13] = -(upper.y + lower.y) / (upper.y - lower.y);
	m[14] = zBias;
	m[15] = 1.0f;
}
