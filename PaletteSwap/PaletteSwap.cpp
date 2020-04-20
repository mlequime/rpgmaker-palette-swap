#include "stdafx.h"
#include <cstdlib>
#include <cmath>
#include <vector>
#include <regex>
#include "stdafx.h"
#include "PaletteSwap.h"

using namespace std;

typedef union {
	unsigned int pixel;
	struct {
		unsigned char blue;
		unsigned char green;
		unsigned char red;
		unsigned char alpha;
	};
} RGSSRGBA;

typedef struct {
	DWORD unk1;
	DWORD unk2;
	BITMAPINFOHEADER* infoheader;
	RGSSRGBA* firstRow;
	RGSSRGBA* lastRow;
} RGSSBMINFO;

typedef struct {
	DWORD unk1;
	DWORD unk2;
	RGSSBMINFO* bminfo;
} BITMAPSTRUCT;

typedef struct {
	DWORD flags;
	DWORD klass;
	void(*dmark) (void*);
	void(*dfree) (void*);
	BITMAPSTRUCT* bm;
} RGSSBITMAP;

typedef struct {
	int red;
	int green;
	int blue;
} COLOR;

typedef struct {
	COLOR border;
	COLOR shadow;
	COLOR midtone;
	COLOR highlight;
} PALETTE;


std::vector<COLOR> parsePalette(char* _input)
{
	std::string input(_input);
	std::stringstream  stream(input);
	std::string line;
	std::vector<COLOR> parsedCsv;
	while (std::getline(stream, line))
	{
		std::stringstream lineStream(line);
		std::string cell;
		std::vector<std::string> parsedRow;
		while (std::getline(lineStream, cell, ','))
		{
			parsedRow.push_back(cell);
		}

		COLOR color = { std::stoi(parsedRow.at(0)), std::stoi(parsedRow.at(1)), std::stoi(parsedRow.at(2)) };

		parsedCsv.push_back(color);
	}
	return parsedCsv;
};

RGSSApi bool Colorize(unsigned int object, char* og_palette, char* palette)
{
#pragma warning (disable:4312)
	RGSSBMINFO* bitmap = ((RGSSBITMAP*)(object << 1))->bm->bminfo;
#pragma warning (default:4312)

	long width, height;
	RGSSRGBA* row;
	long x, y, i;
	int red, green, blue;
	if (!bitmap) {
		return false;
	}

	width = bitmap->infoheader->biWidth;
	height = bitmap->infoheader->biHeight;

	row = bitmap->lastRow;

	std::vector<COLOR> ogPalette = parsePalette(og_palette);
	std::vector<COLOR> newPalette = parsePalette(palette);
	const int size = ogPalette.size();
	const int newSize = newPalette.size();

	int changeAt = -1;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			changeAt = -1;
			red = row->red;
			green = row->green;
			blue = row->blue;

			for (i = 0; i < size; i++) {
				COLOR curColor = ogPalette.at(i);
				if ((red == curColor.red) && (green == curColor.green) && (blue == curColor.blue)) {
					changeAt = i;
				}
			}
			if ((changeAt > -1) && (changeAt >= newSize == false)) {
				COLOR newColor = newPalette.at(changeAt);
				row->red = (unsigned char)min(255, max(0, newColor.red));
				row->green = (unsigned char)min(255, max(0, newColor.green));
				row->blue = (unsigned char)min(255, max(0, newColor.blue));
			}
			row++;
		}
	}
	return true;
}