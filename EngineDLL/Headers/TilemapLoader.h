#pragma once
#include "SpriteSheet.h"

class ENGINEDLL_API TilemapLoader {
public:
	TilemapLoader(const char* fileDir, const SpriteSheet* tileset);
	~TilemapLoader();

private:
	bool CheckFormat(const char* fileDir);
	SpriteSheet m_tileset;
	unsigned int m_width;
	unsigned int m_height;
	int* m_ids;
};
