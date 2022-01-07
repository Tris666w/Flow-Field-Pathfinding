#pragma once

#include "framework/EliteRendering/ERenderingTypes.h"

//Colors
const Elite::Color DEFAULT_NODE_COLOR{ 0.7f, 0.7f, 0.7f };
const Elite::Color DEFAULT_CONNECTION_COLOR{ .2f, .2f, .2f };

const Elite::Color HIGHLIGHTED_NODE_COLOR{ 0.f, 0.8f, 0.1f };
const Elite::Color START_NODE_COLOR{ 0.f, 1.0f, 0.0f };
const Elite::Color END_NODE_COLOR{ 1.f, 0.f, 0.f };

const Elite::Color GROUND_NODE_COLOR{ 0.8f, 0.8f, 0.8f};
const Elite::Color MUD_NODE_COLOR{ 0.4f, 0.2f, 0.f };
const Elite::Color WATER_NODE_COLOR{ 0.5f, 0.9f, 0.9f };
const Elite::Color WALL_NODE_COLOR{ 0.f, 0.f, 0.f };


//Radius
const float DEFAULT_NODE_RADIUS{ 3.f };