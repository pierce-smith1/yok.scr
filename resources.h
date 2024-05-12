#pragma once

#include "graphics.h"
#include "resourcew.h"

enum BitmapName {
	cvjoy,
	fn,
	fnplead,
	lk,
	lkconcern,
	lkcool,
	lkexhausted,
	lkhusk,
	lkjoy,
	lkmoyai,
	lksix,
	lkthink,
	lkthumbsup,
	lkunamused,
	lkxd,
	nx,
	vx,
	_BITMAP_COUNT,
};

typedef std::map<BitmapName, int> BitmapResourceIds;
BitmapResourceIds get_bitmap_resource_ids();
static std::map<BitmapName, int> BITMAP_RESOURCE_IDS = get_bitmap_resource_ids();

enum PaletteName {
	autumn,
	ascent,
	azul,
	bliss,
	crystal,
	follow,
	friend_,
	home,
	moonflower,
	peace,
	power,
	purpleflower,
	radiance,
	redmoondesert,
	ripple,
	stonehenge,
	tulips,
	vortecspace,
	wind,
	windowsxp,

	aemil,
	chasnah,
	dejil,
	dzune,
	ellai,
	evjar,
	fruit,
	gimeljoy,
	gimelsad,
	gimelYOO,
	jaela,
	jergh,
	kirii,
	kraza,
	llema,
	lotus,
	loxxe,
	meazs,
	metis,
	nachi,
	oom,
	romal,
	sillh,
	vette,
	yette,
	zehal,
	zoog,

	_PALETTE_COUNT,
};

Bitmap *load_bitmap(BitmapName name);

typedef std::map<PaletteName, Palette> Palettes;
Palettes get_palettes();
static Palettes PALETTES = get_palettes();
