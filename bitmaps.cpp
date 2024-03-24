#include <map>
#include <algorithm>

#include <Windows.h>
#include <gl/GL.h>

#include "bitmaps.h"
#include <iterator>

HANDLE Bitmaps::load_raw_resource(int resource_id) {
	HANDLE bitmap_resource = LoadImage(
		GetModuleHandle(NULL), 
		MAKEINTRESOURCE(resource_id),
		IMAGE_BITMAP,
		BITMAP_WH,
		BITMAP_WH,
		LR_CREATEDIBSECTION
	);

	return bitmap_resource;
}

BitmapData *Bitmaps::load(int resource_id) {
	static std::map<int, BitmapData *> bitmap_cache;

	if (bitmap_cache.find(resource_id) != bitmap_cache.end()) {
		return bitmap_cache.at(resource_id);
	}

	HANDLE bitmap_resource = load_raw_resource(resource_id);

	BITMAP bitmap_header;
	GetObject(bitmap_resource, sizeof(BITMAP), &bitmap_header);

	BitmapData *new_bitmap = new BitmapData((GLubyte *) bitmap_header.bmBits);
	bitmap_cache[resource_id] = new_bitmap;

	DeleteObject(bitmap_resource);

	return new_bitmap;
}

std::vector<Bitmaps::Definition> Bitmaps::bitmaps_of_group(BitmapGroup group) {
	std::vector<Bitmaps::Definition> bitmaps;
	std::copy_if(Bitmaps::All.begin(), Bitmaps::All.end(), std::back_inserter(bitmaps), [&](const Bitmaps::Definition &bitmap) {
		return bitmap.group == group;
	});

	return bitmaps;
}