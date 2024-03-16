#pragma once

#include "graphics.h"
#include "resourcew.h"
#include "spritecontrol.h"

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
	lkyoy,
	lkyoyapprove,
	fnyoy,
	cvyoy,
	_BITMAP_COUNT,
};

const static std::map<BitmapName, int> BITMAP_RESOURCE_IDS({
	{ cvjoy, IDB_BITMAP1 },
	{ fn, IDB_BITMAP2 },
	{ fnplead, IDB_BITMAP3 },
	{ lk, IDB_BITMAP4 },
	{ lkconcern, IDB_BITMAP5 },
	{ lkcool, IDB_BITMAP6 },
	{ lkexhausted, IDB_BITMAP7 },
	{ lkhusk, IDB_BITMAP8 },
	{ lkjoy, IDB_BITMAP9 },
	{ lkmoyai, IDB_BITMAP10 },
	{ lksix, IDB_BITMAP11 },
	{ lkthink, IDB_BITMAP12 },
	{ lkthumbsup, IDB_BITMAP13 },
	{ lkunamused, IDB_BITMAP14 },
	{ lkxd, IDB_BITMAP15 },
	{ nx, IDB_BITMAP16 },
	{ vx, IDB_BITMAP17 },
	{ lkyoy, IDB_BITMAP18 },
	{ lkyoyapprove, IDB_BITMAP19 },
	{ fnyoy, IDB_BITMAP20 },
	{ cvyoy, IDB_BITMAP21 },
});

Bitmap *load_bitmap(BitmapName name);

