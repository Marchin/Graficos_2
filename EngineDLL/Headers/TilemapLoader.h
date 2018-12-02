#pragma once
#include "Export.h"

class ENGINEDLL_API TilemapLoader {
public:
	TilemapLoader(const char* fileDir, int** matrix, unsigned int* width, unsigned int* height);
	~TilemapLoader();

private:
	bool CheckFormat(const char* fileDir);
	unsigned int m_width;
	unsigned int m_height;
};
