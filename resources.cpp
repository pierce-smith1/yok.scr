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

	GLubyte *data = new GLubyte[BITMAP_WH * BITMAP_WH];


	for (size_t y = 0; y < BITMAP_WH; y++) {
		for (size_t x = 0; x < BITMAP_WH; x++) {
			int i_source = y * BITMAP_WH + x;
			int i_target = (BITMAP_WH - y - 1) + BITMAP_WH * x;
	
			data[i_target] = ((GLubyte *) bitmap.bmBits)[i_source];
		}
	}	 
	// ------------------------------------------------------------
	//                      GET ROTATED
	//                         Idiot

	Bitmap *new_bitmap = new Bitmap(data);
	bitmap_cache[name] = new_bitmap;

	DeleteObject(bitmap_resource);

	return new_bitmap;
}