#include "HerbSp.hpp"

#include "core/logger/LogHelper.hpp"
#include "game/backend/Self.hpp"
#include "game/backend/ScriptMgr.hpp"
#include "game/rdr/Natives.hpp"

namespace YimMenu::HerbSpawner
{
	bool isTreeOrchid(Hash asset)
	{
		switch (asset)
		{
		case "COMPOSITE_LOOTABLE_ORCHID_LADY_NIGHT_DEF"_J:
		case "COMPOSITE_LOOTABLE_ORCHID_GHOST_DEF"_J:
		case "COMPOSITE_LOOTABLE_ORCHID_RAT_TAIL_DEF"_J:
		case "COMPOSITE_LOOTABLE_ORCHID_SPIDER_DEF"_J:
		case "COMPOSITE_LOOTABLE_ORCHID_CIGAR_DEF"_J: return true;

		default: return false;
		}
	}

	bool SpawnHerbComposite(Hash asset)
	{
		auto ped = Self::GetPed();

		if (!ped)
			return false;

		TASK::_REQUEST_HERB_COMPOSITE_ASSET(asset);

		for (int i = 0; i < 100; ++i)
		{
			if (TASK::ARE_COMPOSITE_LOOTABLE_ENTITY_DEF_ASSETS_LOADED(asset))
				break;

			ScriptMgr::Yield(10ms);
		}

		if (!TASK::ARE_COMPOSITE_LOOTABLE_ENTITY_DEF_ASSETS_LOADED(asset))
		{
			LOG(INFO) << "Timed out loading herb composite asset.";
			return false;
		}

		

		auto pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped.GetHandle(), 0.0f, 1.5f, 0.0f);

		Any extraData = 0;
		int groundSetting = 0;
		float headingRadians = ENTITY::GET_ENTITY_HEADING(ped.GetHandle()) * 0.01745329f;

		if (isTreeOrchid(asset)) {
			pos.z += 1.0f;
			groundSetting |= 2;
		}
		
		int compositeId = TASK::_CREATE_HERB_COMPOSITES(asset, pos.x, pos.y, pos.z, headingRadians, groundSetting, &extraData, -1);

		LOG(INFO) << "Created herb composite id: " << compositeId;

		return compositeId != 0;
	}
}