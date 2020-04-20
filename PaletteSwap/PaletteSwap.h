#pragma once
#define RGSSApi extern "C" __declspec(dllexport)

RGSSApi bool Colorize(unsigned int object, char* og_palette, char* palette);