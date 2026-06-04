#include "HerbSpawner.hpp"

#include "game/backend/FiberPool.hpp"
#include "game/rdr/Scripts.hpp"
#include "game/rdr/HerbSp.hpp"
#include <array>

namespace YimMenu::Submenus
{
	void RenderHerbSpawnerMenu()
	{
		static Hash selectedSpHerb = "COMPOSITE_LOOTABLE_ALASKAN_GINSENG_ROOT_DEF"_J;

		ImGui::PushID("herbspawn"_J);

		static const std::map<Hash, std::string> herbTranslations = {
		    {"COMPOSITE_LOOTABLE_ALASKAN_GINSENG_ROOT_DEF"_J, "Alaskan Ginseng"},
		    {"COMPOSITE_LOOTABLE_AMERICAN_GINSENG_ROOT_DEF"_J, "American Ginseng"},
		    {"COMPOSITE_LOOTABLE_BAY_BOLETE_DEF"_J, "Bay Bolete"},
		    {"COMPOSITE_LOOTABLE_BLACK_BERRY_DEF"_J, "Black Berry"},
		    {"COMPOSITE_LOOTABLE_BLACK_CURRANT_DEF"_J, "Black Currant"},
		    {"COMPOSITE_LOOTABLE_BURDOCK_ROOT_DEF"_J, "Burdock Root"},
		    {"COMPOSITE_LOOTABLE_CHANTERELLES_DEF"_J, "Chanterelles"},
		    {"COMPOSITE_LOOTABLE_COMMON_BULRUSH_DEF"_J, "Common Bulrush"},
		    {"COMPOSITE_LOOTABLE_CREEPING_THYME_DEF"_J, "Creeping Thyme"},
		    {"COMPOSITE_LOOTABLE_DESERT_SAGE_DEF"_J, "Desert Sage"},
		    {"COMPOSITE_LOOTABLE_ENGLISH_MACE_DEF"_J, "English Mace"},
		    {"COMPOSITE_LOOTABLE_EVERGREEN_HUCKLEBERRY_DEF"_J, "Evergreen Huckleberry"},
		    {"COMPOSITE_LOOTABLE_GOLDEN_CURRANT_DEF"_J, "Golden Currant"},
		    {"COMPOSITE_LOOTABLE_HUMMINGBIRD_SAGE_DEF"_J, "Hummingbird Sage"},
		    {"COMPOSITE_LOOTABLE_INDIAN_TOBACCO_DEF"_J, "Indian Tobacco"},
		    {"COMPOSITE_LOOTABLE_MILKWEED_DEF"_J, "Milkweed"},
		    {"COMPOSITE_LOOTABLE_OLEANDER_SAGE_DEF"_J, "Oleander Sage"},
		    {"COMPOSITE_LOOTABLE_OREGANO_DEF"_J, "Oregano"},
		    {"COMPOSITE_LOOTABLE_PARASOL_MUSHROOM_DEF"_J, "Parasol Mushroom"},
		    {"COMPOSITE_LOOTABLE_PRAIRIE_POPPY_DEF"_J, "Prairie Poppy"},
		    {"COMPOSITE_LOOTABLE_RAMS_HEAD_DEF"_J, "Rams Head"},
		    {"COMPOSITE_LOOTABLE_RED_RASPBERRY_DEF"_J, "Red Raspberry"},
		    {"COMPOSITE_LOOTABLE_RED_SAGE_DEF"_J, "Red Sage"},
		    {"COMPOSITE_LOOTABLE_ORCHID_VANILLA_DEF"_J, "Vanilla Flower"},
		    {"COMPOSITE_LOOTABLE_VIOLET_SNOWDROP_DEF"_J, "Violet Snowdrop"},
		    {"COMPOSITE_LOOTABLE_WILD_CARROT_DEF"_J, "Wild Carrots"},
		    {"COMPOSITE_LOOTABLE_WILD_FEVERFEW_DEF"_J, "Wild Feverfew"},
		    {"COMPOSITE_LOOTABLE_WILD_MINT_DEF"_J, "Wild Mint"},
		    {"COMPOSITE_LOOTABLE_WINTERGREEN_BERRY_DEF"_J, "Wintergreen Berry"},
		    {"COMPOSITE_LOOTABLE_YARROW_DEF"_J, "Yarrow"},
		    {"COMPOSITE_LOOTABLE_ORCHID_ACUNA_STAR_DEF"_J, "Acuna's Star Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_CIGAR_DEF"_J, "Cigar Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_CLAM_SHELL_DEF"_J, "Clam Shell Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_DRAGONS_DEF"_J, "Dragon's Mouth Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_GHOST_DEF"_J, "Ghost Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_LADY_NIGHT_DEF"_J, "Lady of the Night Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_LADY_SLIPPER_DEF"_J, "Lady Slipper Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_MOCCASIN_DEF"_J, "Moccasin Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_NIGHT_SCENTED_DEF"_J, "Night Scented Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_QUEENS_DEF"_J, "Queen's Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_RAT_TAIL_DEF"_J, "Rat Tail Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_SPARROWS_DEF"_J, "Sparrow's Egg Orchid"},
		    {"COMPOSITE_LOOTABLE_ORCHID_SPIDER_DEF"_J, "Spider Orchid"},
		};

		const auto selectedIt = herbTranslations.find(selectedSpHerb);
		const char* selectedLabel = selectedIt != herbTranslations.end() ? selectedIt->second.c_str() : "(Select Herb)";

		if (ImGui::BeginCombo("Herbs", selectedLabel))
		{
			for (const auto& [asset, translation] : herbTranslations)
			{
				const bool isSelected = asset == selectedSpHerb;
				if (ImGui::Selectable(translation.c_str(), isSelected))
				{
					selectedSpHerb = asset;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Spawn Selected"))
		{
			const Hash herbToSpawn = selectedSpHerb;

			FiberPool::Push([herbToSpawn] {
				HerbSpawner::SpawnHerbComposite(herbToSpawn);
			});
		}

		ImGui::PopID();
	}
}