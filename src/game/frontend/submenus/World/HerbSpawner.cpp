#include "HerbSpawner.hpp"

#include "game/backend/FiberPool.hpp"
#include "game/rdr/Scripts.hpp"
#include "game/rdr/HerbSp.hpp"
#include <map>

#define HERB(id, name) {name, "COMPOSITE_LOOTABLE_" id "_DEF"_J}

namespace YimMenu::Submenus
{
	
	void RenderHerbSpawnerMenu()
	{
		static Hash selectedSpHerb = "COMPOSITE_LOOTABLE_ALASKAN_GINSENG_ROOT_DEF"_J;

		ImGui::PushID("herbspawn"_J);

		static const std::map<std::string, Hash> herbTranslations = {
		    HERB("ALASKAN_GINSENG_ROOT", "Alaskan Ginseng"),
		    HERB("AMERICAN_GINSENG_ROOT", "American Ginseng"),
		    HERB("BAY_BOLETE", "Bay Bolete"),
		    HERB("BLACK_BERRY", "Black Berry"),
		    HERB("BLACK_CURRANT", "Black Currant"),
		    HERB("BURDOCK_ROOT", "Burdock Root"),
		    HERB("CHANTERELLES", "Chanterelles"),
		    HERB("COMMON_BULRUSH", "Common Bulrush"),
		    HERB("CREEPING_THYME", "Creeping Thyme"),
		    HERB("DESERT_SAGE", "Desert Sage"),
		    HERB("ENGLISH_MACE", "English Mace"),
		    HERB("EVERGREEN_HUCKLEBERRY", "Evergreen Huckleberry"),
		    HERB("GOLDEN_CURRANT", "Golden Currant"),
		    HERB("HUMMINGBIRD_SAGE", "Hummingbird Sage"),
		    HERB("INDIAN_TOBACCO", "Indian Tobacco"),
		    HERB("MILKWEED", "Milkweed"),
		    HERB("OLEANDER_SAGE", "Oleander Sage"),
		    HERB("OREGANO", "Oregano"),
		    HERB("PARASOL_MUSHROOM", "Parasol Mushroom"),
		    HERB("PRAIRIE_POPPY", "Prairie Poppy"),
		    HERB("RAMS_HEAD", "Rams Head"),
		    HERB("RED_RASPBERRY", "Red Raspberry"),
		    HERB("RED_SAGE", "Red Sage"),
		    HERB("ORCHID_VANILLA", "Vanilla Flower"),
		    HERB("VIOLET_SNOWDROP", "Violet Snowdrop"),
		    HERB("WILD_CARROT", "Wild Carrots"),
		    HERB("WILD_FEVERFEW", "Wild Feverfew"),
		    HERB("WILD_MINT", "Wild Mint"),
		    HERB("WINTERGREEN_BERRY", "Wintergreen Berry"),
		    HERB("YARROW", "Yarrow"),
		    HERB("ORCHID_ACUNA_STAR", "Acuna's Star Orchid"),
		    HERB("ORCHID_CIGAR", "Cigar Orchid"),
		    HERB("ORCHID_CLAM_SHELL", "Clam Shell Orchid"),
		    HERB("ORCHID_DRAGONS", "Dragon's Mouth Orchid"),
		    HERB("ORCHID_GHOST", "Ghost Orchid"),
		    HERB("ORCHID_LADY_NIGHT", "Lady of the Night Orchid"),
		    HERB("ORCHID_LADY_SLIPPER", "Lady Slipper Orchid"),
		    HERB("ORCHID_MOCCASIN", "Moccasin Orchid"),
		    HERB("ORCHID_NIGHT_SCENTED", "Night Scented Orchid"),
		    HERB("ORCHID_QUEENS", "Queen's Orchid"),
		    HERB("ORCHID_RAT_TAIL", "Rat Tail Orchid"),
		    HERB("ORCHID_SPARROWS", "Sparrow's Egg Orchid"),
		    HERB("ORCHID_SPIDER", "Spider Orchid"),
		};

		const char* selectedLabel = "Select Herb";
		for (const auto& [translation, asset] : herbTranslations){
			if (asset == selectedSpHerb){
				selectedLabel = translation.c_str();
			}
		}

		if (ImGui::BeginCombo("Herbs", selectedLabel)){
			for (const auto& [translation, asset] : herbTranslations){
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