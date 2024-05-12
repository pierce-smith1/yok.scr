#include "resources.h"

Bitmap *load_bitmap(BitmapName name) {
	static std::map<BitmapName, Bitmap *> bitmap_cache;

	if (bitmap_cache.find(name) != bitmap_cache.end()) {
		return bitmap_cache[name];
	}

	HANDLE bitmap_resource = LoadImage(
		GetModuleHandle(NULL), 
		MAKEINTRESOURCE(BITMAP_RESOURCE_IDS[name]),
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

BitmapResourceIds get_bitmap_resource_ids() {
	BitmapResourceIds ids;

	ids[cvjoy] = IDB_BITMAP1;
	ids[fn] = IDB_BITMAP2;
	ids[fnplead] = IDB_BITMAP3;
	ids[lk] = IDB_BITMAP4;
	ids[lkconcern] = IDB_BITMAP5;
	ids[lkcool] = IDB_BITMAP6;
	ids[lkexhausted] = IDB_BITMAP7;
	ids[lkhusk] = IDB_BITMAP8;
	ids[lkjoy] = IDB_BITMAP9;
	ids[lkmoyai] = IDB_BITMAP10;
	ids[lksix] = IDB_BITMAP11;
	ids[lkthink] = IDB_BITMAP12;
	ids[lkthumbsup] = IDB_BITMAP13;
	ids[lkunamused] = IDB_BITMAP14;
	ids[lkxd] = IDB_BITMAP15;
	ids[nx] = IDB_BITMAP16;
	ids[vx] = IDB_BITMAP17;

	return ids;
}

Palettes get_palettes() {
	Palettes palettes;

	palettes[autumn] = Palette(
		Color(0,   0,   0,   0),
		Color(219, 131, 19,  255),
		Color(190, 146, 117, 255),
		Color(144, 74,  8,   255),
		Color(87,  77,  60,  255),
		Color(144, 74,  8,   255),
		Color(243, 230, 233, 255),
		Color(0,   0,   0,   255));

	palettes[ascent] = Palette(
		Color(0,   0,   0,   0),
		Color(25,  99,  196, 255),
		Color(25,  99,  196, 255),
		Color(11,  64,  125, 255),
		Color(140, 150, 221, 255),
		Color(140, 150, 221, 255),
		Color(255, 255, 255, 255),
		Color(80,  107, 188, 255));

	palettes[azul] = Palette(
		Color(0,   0,   0,   0),
		Color(26,  173, 217, 255),
		Color(161, 208, 229, 255),
		Color(41,  109, 158, 255),
		Color(77,  141, 185, 255),
		Color(41,  109, 158, 255),
		Color(255, 255, 255, 255),
		Color(16,  48,  80,  255));

	palettes[bliss] = Palette(
		Color(0,   0,   0,   0),
		Color(115, 152, 30,  255),
		Color(115, 152, 30,  255),
		Color(61,  83,  23,  255),
		Color(106, 150, 242, 255),
		Color(40,  36,  56,  255),
		Color(234, 242, 255, 255),
		Color(59,  115, 238, 255));

	palettes[crystal] = Palette(
		Color(0,   0,   0,   0),
		Color(56,  57,  158, 255),
		Color(81,  183, 207, 255),
		Color(28,  99,  216, 255),
		Color(67,  74,  217, 255),
		Color(10,  26,  74,  255),
		Color(234, 242, 255, 255),
		Color(10,  26,  74,  255));

	palettes[follow] = Palette(
		Color(0,   0,   0,   0),
		Color(222, 90,  41,  255),
		Color(208, 116, 45,  255),
		Color(10,  26,  74,  255),
		Color(192, 152, 43,  255),
		Color(10,  26,  74,  255),
		Color(234, 242, 255, 255),
		Color(159, 137, 78,  255));

	palettes[friend_] = Palette(
		Color(0,   0,   0,   0),
		Color(160, 141, 119, 255),
		Color(188, 171, 150, 255),
		Color(67,  58,  45,  255),
		Color(67,  58,  45,  255),
		Color(25,  23,  19,  255),
		Color(227, 221, 213, 255),
		Color(25,  23,  19,  255));

	palettes[home] = Palette(
		Color(0,   0,   0,   0),
		Color(162, 97,  119, 255),
		Color(125, 90,  124, 255),
		Color(89,  59,  66,  255),
		Color(126, 62,  76,  255),
		Color(65,  73,  198, 255),
		Color(255, 225, 231, 255),
		Color(89,  59,  66,  255));

	palettes[moonflower] = Palette(
		Color(0,   0,   0,   0),
		Color(234, 74,  113, 255),
		Color(246, 100, 204, 255),
		Color(180, 29,  61,  255),
		Color(255, 255, 255, 255),
		Color(246, 100, 204, 255),
		Color(255, 255, 255, 255),
		Color(249, 183, 12,  255));

	palettes[peace] = Palette(
		Color(0,   0,   0,   0),
		Color(221, 253, 254, 255),
		Color(255, 255, 255, 255),
		Color(155, 219, 222, 255),
		Color(186, 241, 240, 255),
		Color(175, 220, 233, 255),
		Color(221, 253, 254, 255),
		Color(155, 219, 222, 255));

	palettes[power] = Palette(
		Color(0,   0,   0,   0),
		Color(254, 160, 248, 255),
		Color(255, 195, 200, 255),
		Color(254, 10,  180, 255),
		Color(254, 10,  180, 255),
		Color(255, 0,   197, 255),
		Color(255, 195, 200, 255),
		Color(254, 160, 248, 255));

	palettes[purpleflower] = Palette(
		Color(0,   0,   0,   0),
		Color(121, 75,  124, 255),
		Color(162, 124, 184, 255),
		Color(70,  31,  62,  255),
		Color(44,  158, 173, 255),
		Color(20,  106, 114, 255),
		Color(219, 255, 255, 255),
		Color(20,  106, 114, 255));

	palettes[radiance] = Palette(
		Color(0,   0,   0,   0),
		Color(55,  44,  36,  255),
		Color(108, 102, 92,  255),
		Color(0,   0,   0,   255),
		Color(181, 166, 146, 255),
		Color(55,  44,  36,  255),
		Color(218, 195, 177, 255),
		Color(146, 137, 122, 255));

	palettes[redmoondesert] = Palette(
		Color(0,   0,   0,   0),
		Color(137, 40,  11,  255),
		Color(205, 83,  41,  255),
		Color(62,  17,  9,   255),
		Color(94,  139, 220, 255),
		Color(205, 83,  41,  255),
		Color(206, 219, 255, 255),
		Color(21,  54,  122, 255));

	palettes[ripple] = Palette(
		Color(0,   0,    0,   0),
		Color(10,  164, 245, 255),
		Color(0,   189, 247, 255),
		Color(0,   87,  218, 255),
		Color(0,   77,  163, 255),
		Color(0,   189, 247, 255),
		Color(204, 204, 255, 255),
		Color(0,   49,  95,  255));

	palettes[stonehenge] = Palette(
		Color(0,   0,   0,   0),
		Color(98,  126, 196, 255),
		Color(166, 181, 226, 255),
		Color(67,  88,  154, 255),
		Color(67,  77,  53,  255),
		Color(148, 142, 133, 255),
		Color(238, 238, 255, 255),
		Color(24,  43,  17,  255));

	palettes[tulips] = Palette(
		Color(0,   0,   0,   0),
		Color(211, 158, 14,  255),
		Color(183, 192, 230, 255),
		Color(123, 85,  5,   255),
		Color(39,  46,  18,  255),
		Color(81,  107, 220, 255),
		Color(255, 255, 238, 255),
		Color(28,  35,  40,  255));

	palettes[vortecspace] = Palette(
		Color(0,   0,   0,   0),
		Color(2,   156, 254, 255),
		Color(2,   64,  222, 255),
		Color(12,  31,  136, 255),
		Color(2,   156, 254, 255),
		Color(2,   156, 254, 255),
		Color(12,  20,  42,  255),
		Color(12,  31,  136, 255));

	palettes[wind] = Palette(
		Color(0,   0,   0,   0),
		Color(203, 125, 84,  255),
		Color(243, 139, 65,  255),
		Color(107, 76,  48,  255),
		Color(172, 138, 157, 255),
		Color(243, 139, 65,  255),
		Color(255, 238, 221, 255),
		Color(85,  105, 173, 255));

	palettes[windowsxp] = Palette(
		Color(0,   0,   0,   0),
		Color(103, 157, 74,  255),
		Color(91,  166, 49,  255),
		Color(68,  99,  51,  255),
		Color(66,  119, 37,  255),
		Color(123, 150, 249, 255),
		Color(255, 238, 255, 255),
		Color(68,  99,  51,  255));

	palettes[aemil] = Palette(
		Color(0,   0,   0,   0),
		Color(86,  235, 142, 255),
		Color(132, 245, 195, 255),
		Color(29,  149, 80,  255),
		Color(226, 154, 86,  255),
		Color(225, 106, 114, 255),
		Color(223, 249, 235, 255),
		Color(150, 99,  54,  255));

	palettes[chasnah] = Palette(
		Color(0,   0,   0,   0),
		Color(111, 49,  221, 255),
		Color(147, 45,  227, 255),
		Color(58,  18,  162, 255),
		Color(227, 14,  254, 255),
		Color(226, 39,  255, 255),
		Color(246, 245, 244, 255),
		Color(233, 89,  245, 255));

	palettes[dejil] = Palette(
		Color(0,   0,   0,   0),
		Color(240, 240, 240, 255),
		Color(255, 255, 255, 255),
		Color(190, 190, 190, 255),
		Color(198, 191, 160, 255),
		Color(240, 240, 240, 255),
		Color(240, 240, 240, 255),
		Color(171, 146, 94,  255));

	palettes[dzune] = Palette(
		Color(0,   0,   0,   0),
		Color(229, 97,  97,  255),
		Color(199, 86,  83,  255),
		Color(171, 50,  50,  255),
		Color(75,  17,  17,  255),
		Color(227, 134, 99,  255),
		Color(250, 236, 236, 255),
		Color(55,  8,   8,   255));

	palettes[ellai] = Palette(
		Color(0,   0,   0,   0),
		Color(151, 204, 114, 255),
		Color(202, 236, 177, 255),
		Color(51,  133, 39,  255),
		Color(114, 72,  45,  255),
		Color(240, 79,  42,  255),
		Color(255, 255, 255, 255),
		Color(69,  22,  14,  255));

	palettes[evjar] = Palette(
		Color(0,   0,   0,   0),
		Color(108, 39,  23,  255),
		Color(139, 66,  49,  255),
		Color(47,  28,  49,  255),
		Color(34,  14,  50,  255),
		Color(240, 79,  42,  255),
		Color(255, 255, 255, 255),
		Color(14,  0,   25,  255));

	palettes[fruit] = Palette(
		Color(0,   0,   0,   0),
		Color(240, 95,  124, 255),
		Color(237, 120, 144, 255),
		Color(211, 64,  94,  255),
		Color(77,  167, 150, 255),
		Color(247, 229, 112, 255),
		Color(222, 173, 91,  255),
		Color(235, 222, 110, 255));

	palettes[gimeljoy] = Palette(
		Color(0,   0,   0,   0),
		Color(255, 222, 196, 255),
		Color(250, 196, 153, 255),
		Color(255, 179, 118, 255),
		Color(231, 176, 133, 255),
		Color(0,   0,   0,   255),
		Color(255, 161, 84,  255),
		Color(206, 145, 97,  255));

	palettes[gimelsad] = Palette(
		Color(0,   0,   0,   0),
		Color(196, 231, 255, 255),
		Color(129, 192, 236, 255),
		Color(95,  167, 218, 255),
		Color(99,  160, 203, 255),
		Color(0,   0,   0,   255),
		Color(55,  171, 252, 255),
		Color(61,  128, 175, 255));

	palettes[gimelYOO] = Palette(
		Color(0,   0,   0,   0),
		Color(249, 196, 255, 255),
		Color(244, 146, 255, 255),
		Color(240, 108, 255, 255),
		Color(221, 132, 231, 255),
		Color(0,   0,   0,   255),
		Color(237, 68,  255, 255),
		Color(199, 103, 209, 255));

	palettes[jaela] = Palette(
		Color(0,   0,   0,   0),
		Color(248, 183, 101, 255),
		Color(246, 213, 142, 255),
		Color(169, 90,  46,  255),
		Color(246, 88,  91,  255),
		Color(232, 85,  51,  255),
		Color(255, 255, 255, 255),
		Color(181, 40,  43,  255));

	palettes[jergh] = Palette(
		Color(0,   0,   0,   0),
		Color(231, 231, 231, 255),
		Color(243, 243, 235, 255),
		Color(180, 188, 209, 255),
		Color(121, 197, 105, 255),
		Color(208, 138, 183, 255),
		Color(240, 240, 240, 255),
		Color(207, 144, 184, 255));

	palettes[kirii] = Palette(
		Color(0,   0,   0,   0),
		Color(74,  212, 191, 255),
		Color(68,  224, 209, 255),
		Color(44,  145, 151, 255),
		Color(119, 128, 127, 255),
		Color(100, 131, 188, 255),
		Color(216, 255, 249, 255),
		Color(82,  83,  83,  255));

	palettes[kraza] = Palette(
		Color(0,   0,   0,   0),
		Color(89,  60,  43,  255),
		Color(119, 83,  54,  255),
		Color(59,  38,  32,  255),
		Color(255, 99,  101, 255),
		Color(255, 255, 247, 255),
		Color(129, 206, 58,  255),
		Color(185, 58,  114, 255));

	palettes[llema] = Palette(
		Color(0,   0,   0,   0),
		Color(78,  174, 139, 255),
		Color(132, 193, 156, 255),
		Color(61,  145, 119, 255),
		Color(28,  52,  67,  255),
		Color(242, 162, 127, 255),
		Color(255, 239, 225, 255),
		Color(22,  36,  50,  255));

	palettes[lotus] = Palette(
		Color(0,   0,   0,   0),
		Color(240, 37,  85,  255),
		Color(242, 82,  120, 255),
		Color(124, 27,  61,  255),
		Color(244, 159, 80,  255),
		Color(228, 109, 51,  255),
		Color(243, 230, 233, 255),
		Color(171, 94,  57,  255));

	palettes[loxxe] = Palette(
		Color(0,   0,   0,   0),
		Color(36,  57,  102, 255),
		Color(49,  72,  122, 255),
		Color(22,  39,  77,  255),
		Color(18,  34,  64,  255),
		Color(84,  110, 120, 255),
		Color(164, 178, 182, 255),
		Color(11,  22,  44,  255));

	palettes[meazs] = Palette(
		Color(0,   0,   0,   0),
		Color(234, 130, 65,  255),
		Color(243, 171, 88,  255),
		Color(158, 59,  31,  255),
		Color(234, 177, 226, 255),
		Color(205, 64,  221, 255),
		Color(255, 255, 255, 255),
		Color(185, 106, 194, 255));

	palettes[metis] = Palette(
		Color(0,   0,   0,   0),
		Color(80,  193, 105, 255),
		Color(114, 211, 135, 255),
		Color(27,  112, 63,  255),
		Color(116, 42,  125, 255),
		Color(240, 79,  42,  255),
		Color(255, 255, 255, 255),
		Color(41,  6,   59,  255));

	palettes[nachi] = Palette(
		Color(0,   0,   0,   0),
		Color(255, 84,  84,  255),
		Color(214, 56,  56,  255),
		Color(0,   255, 255, 255),
		Color(0,   255, 255, 255),
		Color(186, 0,   0,   255),
		Color(255, 255, 255, 255),
		Color(0,   201, 201, 255));

	palettes[oom] = Palette(
		Color(0,   0,   0,   0),
		Color(6,   0,   132, 255),
		Color(0,   0,   255, 255),
		Color(4,   0,   77,  255),
		Color(255, 255, 255, 255),
		Color(244, 0,   6,   255),
		Color(255, 255, 255, 255),
		Color(0,   0,   255, 255));

	palettes[romal] = Palette(
		Color(0,   0,   0,   0),
		Color(90,  49,  120, 255),
		Color(130, 81,  151, 255),
		Color(63,  31,  86,  255),
		Color(172, 172, 172, 255),
		Color(120, 90,  225, 255),
		Color(255, 255, 255, 255),
		Color(101, 91,  107, 255));

	palettes[sillh] = Palette(
		Color(0,   0,   0,   0),
		Color(248, 221, 209, 255),
		Color(255, 238, 231, 255),
		Color(191, 146, 127, 255),
		Color(243, 111, 89,  255),
		Color(215, 78,  55,  255),
		Color(255, 255, 255, 255),
		Color(198, 63,  59,  255));

	palettes[vette] = Palette(
		Color(0,   0,   0,   0),
		Color(170, 176, 207, 255),
		Color(112, 127, 202, 255),
		Color(116, 124, 168, 255),
		Color(123, 125, 137, 255),
		Color(121, 138, 219, 255),
		Color(213, 220, 255, 255),
		Color(77,  80,  100, 255));

	palettes[yette] = Palette(
		Color(0,   0,   0,   0),
		Color(69,  176, 238, 255),
		Color(96,  193, 249, 255),
		Color(35,  140, 201, 255),
		Color(195, 196, 231, 255),
		Color(68,  167, 216, 255),
		Color(232, 249, 255, 255),
		Color(133, 135, 193, 255));

	palettes[zehal] = Palette(
		Color(0,   0,   0,   0),
		Color(234, 179, 117, 255),
		Color(232, 198, 133, 255),
		Color(173, 116, 85,  255),
		Color(114, 71,  49,  255),
		Color(243, 162, 150, 255),
		Color(251, 251, 215, 255),
		Color(79,  45,  27,  255));

	palettes[zoog] = Palette(
		Color(0,   0,   0,   0),
		Color(159, 41,  54,  255),
		Color(199, 69,  69,  255),
		Color(133, 26,  38,  255),
		Color(42,  8,   8,   255),
		Color(162, 8,   20,  255),
		Color(0,   0,   0,   255),
		Color(28,  4,   7,   255));

	return palettes;
}
