#pragma once

#include <string>
#include <set>
#include <vector>
#include <array>

#include "context.h"
#include "common.h"
#include "resourcew.h"

constexpr static unsigned int BITMAP_WH = 128;

class BitmapData : public Identifiable<std::array<GLubyte, BITMAP_WH * BITMAP_WH>> {
public:
	BitmapData(const std::initializer_list<GLubyte> &i_list);
	BitmapData(const GLubyte *data);
};

enum class BitmapGroup {
	Yokin,
	Impostor,
	YoyImpostor,
};

struct Bitmaps {
	struct Definition {
		auto operator<=>(const Definition &other) const = default;

		std::wstring name;
		int resource_id;
		BitmapGroup group;
		BitmapData *data;
	};

	static HANDLE load_raw_resource(int resource_id);
	static BitmapData *load(int resource_id);

	inline const static Definition Cvjoy = {
		.name = L"cvjoy",
		.resource_id = IDB_BITMAP1,
		.group = BitmapGroup::Impostor,
		.data = load(IDB_BITMAP1),
	};

	inline const static Definition Fn = {
		.name = L"fn",
		.resource_id = IDB_BITMAP2,
		.group = BitmapGroup::Impostor,
		.data = load(IDB_BITMAP2),
	};

	inline const static Definition Fnplead = {
		.name = L"fnplead",
		.resource_id = IDB_BITMAP3,
		.group = BitmapGroup::Impostor,
		.data = load(IDB_BITMAP3),
	};

	inline const static Definition Lk = {
		.name = L"lk",
		.resource_id = IDB_BITMAP4,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP4),
	};

	inline const static Definition Lkconcern = {
		.name = L"lkconcern",
		.resource_id = IDB_BITMAP5,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP5),
	};

	inline const static Definition Lkcool = {
		.name = L"lkcool",
		.resource_id = IDB_BITMAP6,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP6),
	};

	inline const static Definition Lkexhausted = {
		.name = L"lkexhausted",
		.resource_id = IDB_BITMAP7,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP7),
	};

	inline const static Definition Lkhusk = {
		.name = L"lkhusk",
		.resource_id = IDB_BITMAP8,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP8),
	};

	inline const static Definition Lkjoy = {
		.name = L"lkjoy",
		.resource_id = IDB_BITMAP9,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP9),
	};

	inline const static Definition Lkmoyai = {
		.name = L"lkmoyai",
		.resource_id = IDB_BITMAP10,
		.group = BitmapGroup::Impostor,
		.data = load(IDB_BITMAP10),
	};

	inline const static Definition Lksix = {
		.name = L"lksix",
		.resource_id = IDB_BITMAP11,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP11),
	};

	inline const static Definition Lkthink = {
		.name = L"lkthink",
		.resource_id = IDB_BITMAP12,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP12),
	};

	inline const static Definition Lkthumbsup = {
		.name = L"lkthumbsup",
		.resource_id = IDB_BITMAP13,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP13),
	};

	inline const static Definition Lkunamused = {
		.name = L"lkunamumsed",
		.resource_id = IDB_BITMAP14,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP14),
	};

	inline const static Definition Lkxd = {
		.name = L"lkxd",
		.resource_id = IDB_BITMAP15,
		.group = BitmapGroup::Yokin,
		.data = load(IDB_BITMAP15),
	};

	inline const static Definition Nx = {
		.name = L"nx",
		.resource_id = IDB_BITMAP16,
		.group = BitmapGroup::Impostor,
		.data = load(IDB_BITMAP16),
	};

	inline const static Definition Vx = {
		.name = L"vx",
		.resource_id = IDB_BITMAP17,
		.group = BitmapGroup::Impostor,
		.data = load(IDB_BITMAP17),
	};

	inline const static Definition Lkyoy = {
		.name = L"lkyoy",
		.resource_id = IDB_BITMAP18,
		.group = BitmapGroup::YoyImpostor,
		.data = load(IDB_BITMAP18),
	};

	inline const static Definition Lkyoyapprove = {
		.name = L"lkyoyapprove",
		.resource_id = IDB_BITMAP19,
		.group = BitmapGroup::YoyImpostor,
		.data = load(IDB_BITMAP19),
	};

	inline const static Definition Fnyoy = {
		.name = L"fnyoy",
		.resource_id = IDB_BITMAP20,
		.group = BitmapGroup::YoyImpostor,
		.data = load(IDB_BITMAP20),
	};

	inline const static Definition Cvyoy = {
		.name = L"cvyoy",
		.resource_id = IDB_BITMAP21,
		.group = BitmapGroup::YoyImpostor,
		.data = load(IDB_BITMAP21),
	};

	inline const static std::set<Definition> All = {
		Cvjoy,
		Fn,
		Fnplead,
		Lk,
		Lkconcern,
		Lkcool,
		Lkexhausted,
		Lkhusk,
		Lkjoy,
		Lkmoyai,
		Lksix,
		Lkthink,
		Lkthumbsup,
		Lkunamused,
		Lkxd,
		Nx,
		Vx,
		Lkyoy,
		Lkyoyapprove,
		Fnyoy,
		Cvyoy,
	};

	static std::vector<Definition> bitmaps_of_group(BitmapGroup group);
};

