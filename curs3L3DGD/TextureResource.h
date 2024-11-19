#pragma once
#include <iostream>

using namespace std;

enum TType { T_2D, T_CUBE };

struct TextureResource
{
	string path;
	int type;
	int min_filter;
	int mag_filter;
	int wrap_s;
	int wrap_t;
};