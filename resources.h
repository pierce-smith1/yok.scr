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

const static std::map<PaletteName, Palette> PALETTES({
	{ autumn, {{ 0,   0,   0,   0   }, 
	           { 219, 131, 19,  255 }, 
	           { 190, 146, 117, 255 }, 
	           { 144, 74,  8,   255 }, 
	           { 87,  77,  60,  255 }, 
	           { 144, 74,  8,   255 }, 
	           { 243, 230, 233, 255 }, 
	           { 0,   0,   0,   255 }}
	},
	{ ascent, {{ 0,   0,   0,   0   }, 
	           { 25,  99,  196, 255 }, 
	           { 25,  99,  196, 255 }, 
	           { 11,  64,  125, 255 }, 
	           { 140, 150, 221, 255 }, 
	           { 140, 150, 221, 255 }, 
	           { 255, 255, 255, 255 }, 
	           { 80,  107, 188, 255 }}
	},
	{ azul, {{ 0,   0,   0,   0   }, 
	         { 26,  173, 217, 255 }, 
	         { 161, 208, 229, 255 }, 
	         { 41,  109, 158, 255 }, 
	         { 77,  141, 185, 255 }, 
	         { 41,  109, 158, 255 }, 
	         { 255, 255, 255, 255 }, 
	         { 16,  48,  80,  255 }}
	},
	{ bliss, {{ 0,   0,   0,   0   }, 
	          { 115, 152, 30,  255 }, 
	          { 115, 152, 30,  255 }, 
	          { 61,  83,  23,  255 }, 
	          { 106, 150, 242, 255 }, 
	          { 40,  36,  56,  255 }, 
	          { 234, 242, 255, 255 }, 
	          { 59,  115, 238, 255 }}
	},
	{ crystal, {{ 0,   0,   0,   0   }, 
	            { 56,  57,  158, 255 }, 
	            { 81,  183, 207, 255 },
	            { 28,  99,  216, 255 }, 
	            { 67,  74,  217, 255 }, 
	            { 10,  26,  74,  255 }, 
	            { 234, 242, 255, 255 }, 
	            { 10,  26,  74,  255 }}
	},
	{ follow, {{ 0,   0,   0,   0   }, 
	           { 222, 90,  41,  255 }, 
	           { 208, 116, 45,  255 }, 
	           { 10,  26,  74,  255 }, 
	           { 192, 152, 43,  255 }, 
	           { 10,  26,  74,  255 }, 
	           { 234, 242, 255, 255 }, 
	           { 159, 137, 78,  255 }}
	},
	{ friend_, {{ 0,   0,   0,   0   }, 
	            { 160, 141, 119, 255 }, 
	            { 188, 171, 150, 255 }, 
	            { 67,  58,  45,  255 }, 
	            { 67,  58,  45,  255 }, 
	            { 25,  23,  19,  255 }, 
	            { 227, 221, 213, 255 },
	            { 25,  23,  19,  255 }}
	},
	{ home, {{ 0,   0,   0,   0   }, 
	         { 162, 97,  119, 255 }, 
	         { 125, 90,  124, 255 }, 
	         { 89,  59,  66,  255 }, 
	         { 126, 62,  76,  255 }, 
	         { 65,  73,  198, 255 }, 
	         { 255, 225, 231, 255 }, 
	         { 89,  59,  66,  255 }}
	},
	{ moonflower, {{ 0,   0,   0,   0   }, 
	               { 234, 74,  113, 255 }, 
	               { 246, 100, 204, 255 }, 
	               { 180, 29,  61,  255 }, 
	               { 255, 255, 255, 255 },
	               { 246, 100, 204, 255 }, 
	               { 255, 255, 255, 255 }, 
	               { 249, 183, 12,  255 }}
	},
	{ peace, {{ 0,   0,   0,   0   }, 
	          { 221, 253, 254, 255 }, 
	          { 255, 255, 255, 255 }, 
	          { 155, 219, 222, 255 }, 
	          { 186, 241, 240, 255 }, 
	          { 175, 220, 233, 255 },
	          { 221, 253, 254, 255 }, 
	          { 155, 219, 222, 255 }}
	},
	{ power, {{ 0,   0,   0,   0   }, 
	          { 254, 160, 248, 255 }, 
	          { 255, 195, 200, 255 },
	          { 254, 10,  180, 255 }, 
	          { 254, 10,  180, 255 }, 
	          { 255, 0,   197, 255 }, 
	          { 255, 195, 200, 255 }, 
	          { 254, 160, 248, 255 }}
	},
	{ purpleflower, {{ 0,   0,   0,   0   }, 
                     { 121, 75,  124, 255 }, 
                     { 162, 124, 184, 255 }, 
                     { 70,  31,  62,  255 },
                     { 44,  158, 173, 255 }, 
                     { 20,  106, 114, 255 }, 
                     { 219, 255, 255, 255 }, 
                     { 20,  106, 114, 255 }}
	},
	{ radiance, {{ 0,   0,   0,   0   }, 
                 { 55,  44,  36,  255 }, 
                 { 108, 102, 92,  255 }, 
                 { 0,   0,   0,   255 }, 
                 { 181, 166, 146, 255 }, 
                 { 55,  44,  36,  255 }, 
                 { 218, 195, 177, 255 }, 
                 { 146, 137, 122, 255 }}
	},
	{ redmoondesert, {{ 0,   0,   0,   0   }, 
                      { 137, 40,  11,  255 }, 
                      { 205, 83,  41,  255 }, 
                      { 62,  17,  9,   255 }, 
                      { 94,  139, 220, 255 },
                      { 205, 83,  41,  255 }, 
                      { 206, 219, 255, 255 }, 
                      { 21,  54,  122, 255 }}
	},
	{ ripple, {{ 0,   0,    0,   0   }, 
               { 10,  164, 245, 255 }, 
               { 0,   189, 247, 255 }, 
               { 0,   87,  218, 255 }, 
               { 0,   77,  163, 255 }, 
               { 0,   189, 247, 255 }, 
               { 204, 204, 255, 255 }, 
               { 0,   49,  95,  255 }}
	},
	{ stonehenge, {{ 0,   0,   0,   0   }, 
                   { 98,  126, 196, 255 }, 
                   { 166, 181, 226, 255 }, 
                   { 67,  88,  154, 255 }, 
                   { 67,  77,  53,  255 }, 
                   { 148, 142, 133, 255 }, 
                   { 238, 238, 255, 255 }, 
                   { 24,  43,  17,  255 }}
	},
	{ tulips, {{ 0,   0,   0,   0   }, 
               { 211, 158, 14,  255 }, 
               { 183, 192, 230, 255 }, 
               { 123, 85,  5,   255 }, 
               { 39,  46,  18,  255 }, 
               { 81,  107, 220, 255 }, 
               { 255, 255, 238, 255 }, 
               { 28,  35,  40,  255 }}
	},
	{ vortecspace, {{ 0,   0,   0,   0   }, 
                    { 2,   156, 254, 255 }, 
                    { 2,   64,  222, 255 }, 
                    { 12,  31,  136, 255 }, 
                    { 2,   156, 254, 255 }, 
                    { 2,   156, 254, 255 }, 
                    { 12,  20,  42,  255 }, 
                    { 12,  31,  136, 255 }}
	},
	{ wind, {{ 0,   0,   0,   0   }, 
             { 203, 125, 84,  255 }, 
             { 243, 139, 65,  255 }, 
             { 107, 76,  48,  255 }, 
             { 172, 138, 157, 255 }, 
             { 243, 139, 65,  255 }, 
             { 255, 238, 221, 255 }, 
             { 85,  105, 173, 255 }}
	},
	{ windowsxp, {{ 0,   0,   0,   0   }, 
                  { 103, 157, 74,  255 }, 
                  { 91,  166, 49,  255 }, 
                  { 68,  99,  51,  255 }, 
                  { 66,  119, 37,  255 }, 
                  { 123, 150, 249, 255 },
                  { 255, 238, 255, 255 }, 
                  { 68,  99,  51,  255 }}
	},

	{ aemil, {{0,   0,   0,   0  },
			  {86,  235, 142, 255},
			  {132, 245, 195, 255},
			  {29,  149, 80,  255},
			  {226, 154, 86,  255},
			  {225, 106, 114, 255},
			  {223, 249, 235, 255},
			  {150, 99,  54,  255}}
	},
	{ chasnah, {{0,   0,   0,   0  },
				{111, 49,  221, 255},
				{147, 45,  227, 255},
				{58,  18,  162, 255},
				{227, 14,  254, 255},
				{226, 39,  255, 255},
				{246, 245, 244, 255},
				{233, 89,  245, 255}}
	},
	{ dejil, {{0,   0,   0,   0  },
			  {240, 240, 240, 255},
			  {255, 255, 255, 255},
			  {190, 190, 190, 255},
			  {198, 191, 160, 255},
			  {240, 240, 240, 255},
			  {240, 240, 240, 255},
			  {171, 146, 94,  255}}
	},
	{ dzune, {{0,   0,   0,   0  },
			  {229, 97,  97,  255},
			  {199, 86,  83,  255},
			  {171, 50,  50,  255},
			  {75,  17,  17,  255},
			  {227, 134, 99,  255},
			  {250, 236, 236, 255},
			  {55,  8,   8,   255}}
	},
	{ ellai, {{0,   0,   0,   0  },
			  {151, 204, 114, 255},
			  {202, 236, 177, 255},
			  {51,  133, 39,  255},
			  {114, 72,  45,  255},
			  {240, 79,  42,  255},
			  {255, 255, 255, 255},
			  {69,  22,  14,  255}}
	},
	{ evjar, {{0,   0,   0,   0  },
			  {108, 39,  23,  255},
			  {139, 66,  49,  255},
			  {47,  28,  49,  255},
			  {34,  14,  50,  255},
			  {240, 79,  42,  255},
			  {255, 255, 255, 255},
			  {14,  0,   25,  255}}
	},
	{ fruit, {{0,   0,   0,   0  },
	 		  {240, 95,  124, 255},
	 		  {237, 120, 144, 255},
	 		  {211, 64,  94,  255},
	 		  {77,  167, 150, 255},
	 		  {247, 229, 112, 255},
	 		  {222, 173, 91,  255},
	 		  {235, 222, 110, 255}}
	},
	{ gimeljoy, {{0,   0,   0,   0  },
				 {255, 222, 196, 255},
				 {250, 196, 153, 255},
				 {255, 179, 118, 255},
				 {231, 176, 133, 255},
				 {0,   0,   0,   255},
				 {255, 161, 84,  255},
				 {206, 145, 97,  255}}
	},
	{ gimelsad, {{0,   0,   0,   0  },
				 {196, 231, 255, 255},
				 {129, 192, 236, 255},
				 {95,  167, 218, 255},
				 {99,  160, 203, 255},
				 {0,   0,   0,   255},
				 {55,  171, 252, 255},
				 {61,  128, 175, 255}}
	},
	{ gimelYOO, {{0,   0,   0,   0  },
				 {249, 196, 255, 255},
				 {244, 146, 255, 255},
				 {240, 108, 255, 255},
				 {221, 132, 231, 255},
				 {0,   0,   0,   255},
				 {237, 68,  255, 255},
				 {199, 103, 209, 255}}
	},
	{ jaela, {{0,   0,   0,   0  },
	 		  {248, 183, 101, 255},
	 		  {246, 213, 142, 255},
	 		  {169, 90,  46,  255},
	 		  {246, 88,  91,  255},
	 		  {232, 85,  51,  255},
	 		  {255, 255, 255, 255},
	 		  {181, 40,  43,  255}}
	},
	{ jergh, {{0,   0,   0,   0  },
	 		  {231, 231, 231, 255},
	 		  {243, 243, 235, 255},
	 		  {180, 188, 209, 255},
	 		  {121, 197, 105, 255},
	 		  {208, 138, 183, 255},
	 		  {240, 240, 240, 255},
	 		  {207, 144, 184, 255}}
	},
	{ kirii, {{0,   0,   0,   0  },
	 		  {74,  212, 191, 255},
	 		  {68,  224, 209, 255},
	 		  {44,  145, 151, 255},
	 		  {119, 128, 127, 255},
	 		  {100, 131, 188, 255},
	 		  {216, 255, 249, 255},
	 		  {82,  83,  83,  255}}
	},
	{ kraza, {{0,   0,   0,   0  },
	 		  {89,  60,  43,  255},
	 		  {119, 83,  54,  255},
	 		  {59,  38,  32,  255},
	 		  {255, 99,  101, 255},
	 		  {255, 255, 247, 255},
	 		  {129, 206, 58,  255},
	 		  {185, 58,  114, 255}}
	},
	{ llema, {{0,   0,   0,   0  },
	 		  {78,  174, 139, 255},
	 		  {132, 193, 156, 255},
	 		  {61,  145, 119, 255},
	 		  {28,  52,  67,  255},
	 		  {242, 162, 127, 255},
	 		  {255, 239, 225, 255},
	 		  {22,  36,  50,  255}}
	},
	{ lotus, {{0,   0,   0,   0  },
	 		  {240, 37,  85,  255},
	 		  {242, 82,  120, 255},
	 		  {124, 27,  61,  255},
	 		  {244, 159, 80,  255},
	 		  {228, 109, 51,  255},
	 		  {243, 230, 233, 255},
	 		  {171, 94,  57,  255}}
	},
	{ loxxe, {{0,   0,   0,   0  },
	 		  {36,  57,  102, 255},
	 		  {49,  72,  122, 255},
	 		  {22,  39,  77,  255},
	 		  {18,  34,  64,  255},
	 		  {84,  110, 120, 255},
	 		  {164, 178, 182, 255},
	 		  {11,  22,  44,  255}}
	},
	{ meazs, {{0,   0,   0,   0  },
	 		  {234, 130, 65,  255},
	 		  {243, 171, 88,  255},
	 		  {158, 59,  31,  255},
	 		  {234, 177, 226, 255},
	 		  {205, 64,  221, 255},
	 		  {255, 255, 255, 255},
	 		  {185, 106, 194, 255}}
	},
	{ metis, {{0,   0,   0,   0  },
	 		  {80,  193, 105, 255},
	 		  {114, 211, 135, 255},
	 		  {27,  112, 63,  255},
	 		  {116, 42,  125, 255},
	 		  {240, 79,  42,  255},
	 		  {255, 255, 255, 255},
	 		  {41,  6,   59,  255}}
	},
	{ nachi, {{0,   0,   0,   0  },
	 		  {255, 84,  84,  255},
	 		  {214, 56,  56,  255},
	 		  {0,   255, 255, 255},
	 		  {0,   255, 255, 255},
	 		  {186, 0,   0,   255},
	 		  {255, 255, 255, 255},
	 		  {0,   201, 201, 255}}
	},
	{ oom, {{0,   0,   0,   0  },
	 		{6,   0,   132, 255},
	 		{0,   0,   255, 255},
	 		{4,   0,   77,  255},
	 		{255, 255, 255, 255},
	 		{244, 0,   6,   255},
	 		{255, 255, 255, 255},
	 		{0,   0,   255, 255}}
	},
	{ romal, {{0,   0,   0,   0  },
	 		  {90,  49,  120, 255},
	 		  {130, 81,  151, 255},
	 		  {63,  31,  86,  255},
	 		  {172, 172, 172, 255},
	 		  {120, 90,  225, 255},
	 		  {255, 255, 255, 255},
	 		  {101, 91,  107, 255}}
	},
	{ sillh, {{0,   0,   0,   0  },
	 		  {248, 221, 209, 255},
	 		  {255, 238, 231, 255},
	 		  {191, 146, 127, 255},
	 		  {243, 111, 89,  255},
	 		  {215, 78,  55,  255},
	 		  {255, 255, 255, 255},
	 		  {198, 63,  59,  255}}
	},
	{ vette, {{0,   0,   0,   0  },
	 		  {170, 176, 207, 255},
	 		  {112, 127, 202, 255},
	 		  {116, 124, 168, 255},
	 		  {123, 125, 137, 255},
	 		  {121, 138, 219, 255},
	 		  {213, 220, 255, 255},
	 		  {77,  80,  100, 255}}
	},
	{ yette, {{0,   0,   0,   0  },
	 		  {69,  176, 238, 255},
	 		  {96,  193, 249, 255},
	 		  {35,  140, 201, 255},
	 		  {195, 196, 231, 255},
	 		  {68,  167, 216, 255},
	 		  {232, 249, 255, 255},
	 		  {133, 135, 193, 255}}
	},
	{ zehal, {{0,   0,   0,   0  },
	 		  {234, 179, 117, 255},
	 		  {232, 198, 133, 255},
	 		  {173, 116, 85,  255},
	 		  {114, 71,  49,  255},
	 		  {243, 162, 150, 255},
	 		  {251, 251, 215, 255},
	 		  {79,  45,  27,  255}}
	},
	{ zoog, {{0,   0,   0,   0  },
	 		 {159, 41,  54,  255},
	 		 {199, 69,  69,  255},
	 		 {133, 26,  38,  255},
	 		 {42,  8,   8,   255},
	 		 {162, 8,   20,  255},
	 		 {0,   0,   0,   255},
	 		 {28,  4,   7,   255}}
	}}
);