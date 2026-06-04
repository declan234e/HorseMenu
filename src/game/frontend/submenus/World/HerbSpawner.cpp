#include "HerbSpawner.hpp"

#include "game/backend/FiberPool.hpp"
#include "game/rdr/Scripts.hpp"
#include "game/rdr/HerbSp.hpp"
#include <map>

#define HERB(id, name) {"COMPOSITE_LOOTABLE_" id "_DEF"_J, name}

namespace YimMenu::Submenus
{
	
	void RenderHerbSpawnerMenu()
	{
		static Hash selectedSpHerb = "COMPOSITE_LOOTABLE_ALASKAN_GINSENG_ROOT_DEF"_J;

		ImGui::PushID("herbspawn"_J);

		static const std::map<Hash, std::string> herbTranslations = {
		    HERB("ALASKAN_GINSENG_ROOT", "Alaskan Ginseng"),
		    HERB("AMERICAN_GINSENG_ROOT"_J, "American Ginseng"),
		    HERB("BAY_BOLETE"_J, "Bay Bolete"),
		    HERB("BLACK_BERRY"_J, "Black Berry"),
		    HERB("BLACK_CURRANT"_J, "Black Currant"),
		    HERB("BURDOCK_ROOT"_J, "Burdock Root"),
		    HERB("CHANTERELLES"_J, "Chanterelles"),
		    HERB("COMMON_BULRUSH"_J, "Common Bulrush"),
		    HERB("CREEPING_THYME"_J, "Creeping Thyme"),
		    HERB("DESERT_SAGE"_J, "Desert Sage"),
		    HERB("ENGLISH_MACE"_J, "English Mace"),
		    HERB("EVERGREEN_HUCKLEBERRY"_J, "Evergreen Huckleberry"),
		    HERB("GOLDEN_CURRANT"_J, "Golden Currant"),
		    HERB("HUMMINGBIRD_SAGE"_J, "Hummingbird Sage"),
		    HERB("INDIAN_TOBACCO"_J, "Indian Tobacco"),
		    HERB("MILKWEED"_J, "Milkweed"),
		    HERB("OLEANDER_SAGE"_J, "Oleander Sage"),
		    HERB("OREGANO"_J, "Oregano"),
		    HERB("PARASOL_MUSHROOM"_J, "Parasol Mushroom"),
		    HERB("PRAIRIE_POPPY"_J, "Prairie Poppy"),
		    HERB("RAMS_HEAD"_J, "Rams Head"),
		    HERB("RED_RASPBERRY"_J, "Red Raspberry"),
		    HERB("RED_SAGE"_J, "Red Sage"),
		    HERB("ORCHID_VANILLA"_J, "Vanilla Flower"),
		    HERB("VIOLET_SNOWDROP"_J, "Violet Snowdrop"),
		    HERB("WILD_CARROT"_J, "Wild Carrots"),
		    HERB("WILD_FEVERFEW"_J, "Wild Feverfew"),
		    HERB("WILD_MINT"_J, "Wild Mint"),
		    HERB("WINTERGREEN_BERRY"_J, "Wintergreen Berry"),
		    HERB("YARROW"_J, "Yarrow"),
		    HERB("ORCHID_ACUNA_STAR"_J, "Acuna's Star Orchid"),
		    HERB("ORCHID_CIGAR"_J, "Cigar Orchid"),
		    HERB("ORCHID_CLAM_SHELL"_J, "Clam Shell Orchid"),
		    HERB("ORCHID_DRAGONS"_J, "Dragon's Mouth Orchid"),
		    HERB("ORCHID_GHOST"_J, "Ghost Orchid"),
		    HERB("ORCHID_LADY_NIGHT"_J, "Lady of the Night Orchid"),
		    HERB("ORCHID_LADY_SLIPPER"_J, "Lady Slipper Orchid"),
		    HERB("ORCHID_MOCCASIN"_J, "Moccasin Orchid"),
		    HERB("ORCHID_NIGHT_SCENTED"_J, "Night Scented Orchid"),
		    HERB("ORCHID_QUEENS"_J, "Queen's Orchid"),
		    HERB("ORCHID_RAT_TAIL"_J, "Rat Tail Orchid"),
		    HERB("ORCHID_SPARROWS"_J, "Sparrow's Egg Orchid"),
		    HERB("ORCHID_SPIDER"_J, "Spider Orchid"),
		};

		const auto selectedIt = herbTranslations.find(selectedSpHerb);
		const char* selectedLabel = selectedIt != herbTranslations.end() ? selectedIt->second.c_str() : "(Select Herb)";

		if (ImGui::BeginCombo("Herbs", selectedLabel)){
			for (const auto& [asset, translation] : herbTranslations){
				const bool isSelected = asset == selectedSpHerb;
				if (ImGui::Selectable(translation.c_str(), isSelected)){
					selectedSpHerb = asset;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Spawn Selected")){
			const Hash herbToSpawn = selectedSpHerb;

			FiberPool::Push([herbToSpawn] {
				HerbSpawner::SpawnHerbComposite(herbToSpawn);
			});
		}

		ImGui::PopID();
	}
}