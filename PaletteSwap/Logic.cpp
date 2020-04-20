#include <cstdlib>
#include <cmath>
#include <vector>
#include <regex>
#include "stdafx.h"
#include "Logic.h"
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
	BITMAPINFOHEADER *infoheader;
	RGSSRGBA *firstRow;
	RGSSRGBA *lastRow;
} RGSSBMINFO;

typedef struct {
	DWORD unk1;
	DWORD unk2;
	RGSSBMINFO *bminfo;
} BITMAPSTRUCT;

typedef struct {
	DWORD flags;
	DWORD klass;
	void(*dmark) (void*);
	void(*dfree) (void*);
	BITMAPSTRUCT *bm;
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


PALETTE parsePalette(char* _input)
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
	PALETTE palette = { parsedCsv.at(0), parsedCsv.at(1), parsedCsv.at(2), parsedCsv.at(3) };
	return palette;
};

RGSSApi bool Colorize(unsigned int object, char* palette)
{
	#pragma warning (disable:4312)
	RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object << 1))->bm->bminfo;
	#pragma warning (default:4312)

	long width, height;
	RGSSRGBA *row;
	long x, y;
	int red, green, blue;
	if (!bitmap) {
		return false;
	}

	width = bitmap->infoheader->biWidth;
	height = bitmap->infoheader->biHeight;

	row = bitmap->lastRow;

	PALETTE parsedPalette = parsePalette(palette);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			red = row->red;
			green = row->green;
			blue = row->blue;

			if (y == 30) {
				cout << red << "," << green << "," << blue;
			}

			if ((red == 75) && (green = 75) && (blue == 75)) {
				red = parsedPalette.border.red;
				green = parsedPalette.border.green;
				blue = parsedPalette.border.blue;
			}
			else if ((red == 105) && (green = 105) && (blue == 105)) {
				red = parsedPalette.shadow.red;
				green = parsedPalette.shadow.green;
				blue = parsedPalette.shadow.blue;
			}
			else if ((red == 135) && (green = 135) && (blue == 135)) {
				red = parsedPalette.midtone.red;
				green = parsedPalette.midtone.green;
				blue = parsedPalette.midtone.blue;
			}
			else if ((red == 175) && (green = 175) && (blue == 175)) {
				red = parsedPalette.highlight.red;
				green = parsedPalette.highlight.green;
				blue = parsedPalette.highlight.blue;
			}
			else {
				row++;
				continue;
			}
			row->red = (unsigned char)min(255, max(0, red));
			row->green = (unsigned char)min(255, max(0, green));
			row->blue = (unsigned char)min(255, max(0, blue));
			row++;
		}
	}
	return true;
}