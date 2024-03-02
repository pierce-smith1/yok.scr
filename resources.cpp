#include "resources.h"

Bitmap *load_bitmap(BitmapName name) {
	static std::map<BitmapName, Bitmap*> bitmap_cache;

	if (bitmap_cache.find(name) != bitmap_cache.end()) {
		return bitmap_cache.at(name);
	}

	HANDLE bitmap_resource = LoadImage(
		GetModuleHandle(NULL), 
		MAKEINTRESOURCE(BITMAP_RESOURCE_IDS.at(name)),
		IMAGE_BITMAP,
		BITMAP_WH,
		BITMAP_WH,
		LR_CREATEDIBSECTION
	);

	BITMAP bitmap;
	GetObject(bitmap_resource, sizeof(BITMAP), &bitmap);

	Bitmap *new_bitmap = new Bitmap((GLubyte *) bitmap.bmBits);
	bitmap_cache[name] = new_bitmap;

	DeleteObject(bitmap_resource);

	return new_bitmap;
}