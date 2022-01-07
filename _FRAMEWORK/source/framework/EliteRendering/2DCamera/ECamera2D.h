/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// E2DCamera.h: 2D camera implementation in the engine.
/*=============================================================================*/
#ifndef ELITE_2D_CAMERA_H
#define ELITE_2D_CAMERA_H

class Camera2D final
{
public:
	//--- Constructor & Destructor ---
	explicit Camera2D(unsigned int width, unsigned int height);
	~Camera2D() = default;

	//--- Base Functions ---
	void Update();

	Elite::Vector2 ConvertScreenToWorld(const Elite::Vector2& screenPoint) const;
	Elite::Vector2 ConvertWorldToScreen(const Elite::Vector2& worldPoint) const;
	void BuildProjectionMatrix(float* m, float zBias) const;
	void SetZoom(float z) { m_zoom = z; }
	void SetCenter(Elite::Vector2 c) { m_center = c; }
	void SetZoomLocked(bool state) { m_isZoomLocked = state; }
	void SetMoveLocked(bool state) { m_isMoveLocked = state; }
	unsigned int GetWidth() const { return m_width; }
	unsigned int GetHeight() const { return m_height; }

private:
	//--- Datamembers ---
	Elite::Vector2 m_center = Elite::ZeroVector2;
	Elite::Vector2 m_lastPosition = Elite::ZeroVector2;

	float m_zoom = 0;

	unsigned int m_width = 0;
	unsigned int m_height = 0;

	bool m_isZoomLocked = false;
	bool m_isMoveLocked = false;
	bool m_isRightMouseDown = false;
};
#endif