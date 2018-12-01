#include "../Headers/TilemapLoader.h"
#include <iostream>
#include <fstream>
#define MAX_WIDTH 200

TilemapLoader::TilemapLoader(const char* fileDir, const SpriteSheet* tileset) : m_tileset(*tileset), m_width(0), m_height(0) {
	if (!CheckFormat(fileDir)) {
		printf("Error Loading Tilemap: Invalid format\n");
		return;
	} 

	std::ifstream file;
	file.open(fileDir, std::ios::in);

	if (!file.is_open()) {
		printf("Error Loading Tilemap: Unable to open file\n");
		return;
	}

	char buffer[MAX_WIDTH];
	file.getline(buffer, MAX_WIDTH);
	int iter = 0;
	m_width++; //since we use ',' to count the tiles of a row
				//we have to take into account the first tile
	while (*(buffer + iter) != '\0') {
		if (*(buffer + iter) == ',') {
			m_width++;
		}
		iter++;
	}
	file.clear();
	file.seekg(0, std::ios::beg);
	file.getline(buffer, MAX_WIDTH);
	while (!file.eof()) {
		m_height++;
		file.getline(buffer, MAX_WIDTH);
	}
	file.clear();
	file.seekg(0, std::ios::beg);
	m_ids = new int[m_width * m_height];
	int id = 0;
	file.getline(buffer, MAX_WIDTH);
	while (!file.eof()) {
		char* ptr = buffer;
		iter = 0; //iter = the number of characters from the previous ',' until the next one
		for (int i = 0; i < m_width;) {
			if (*(ptr + iter) == ',' || *(ptr + iter) == '\0') {
				if (*ptr == '-') { //if the value is negative there is no tile
					m_ids[id] = -1;
				} else {
					int newID = 0; //accumulator
					int decimal = 1; //multiplies the value of the character 
									 //to take into account it's position (1, 10, 100, etc)
					for (int j = 0; j < iter; j++) {
						newID += (*(ptr + iter - 1 - j) - '0') * decimal; //read number from right to left
																		// - '0' to convert it from ASCII to int
						decimal *= 10;
					}
					m_ids[id] = newID;
				}
				i++;
				id++;
				ptr = ptr + iter + 1;
				iter = 0;
			} else {
				iter++;
			}
		}
		file.getline(buffer, MAX_WIDTH);
	}
}

TilemapLoader::~TilemapLoader() {
	delete[] m_ids;
}

bool TilemapLoader::CheckFormat(const char * fileDir) {
	//check if it is .csv
	//get to the end of the direction
	int iter = 0;
	while (*(fileDir + iter) != '\0') {
		iter++;
	}

	if (*(fileDir + --iter) != 'v') {
		return false;
	} else if (*(fileDir + --iter) != 's') {
		return false;
	} else if (*(fileDir + --iter) != 'c') {
		return false;
	} else if (*(fileDir + --iter) != '.') {
		return false;
	} else {
		return true;
	}
}
