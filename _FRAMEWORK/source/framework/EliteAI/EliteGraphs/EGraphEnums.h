#pragma once

enum
{
	invalid_node_index = -1
};


enum class TerrainType : uint_fast8_t
{
	Ground = 1,
	Mud = 3,
	Water = 5,

	Wall = 255
};