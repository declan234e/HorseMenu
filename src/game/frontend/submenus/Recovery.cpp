#include "Recovery.hpp"

#include "core/commands/BoolCommand.hpp"
#include "core/commands/Commands.hpp"
#include "game/backend/FiberPool.hpp"
#include "game/frontend/items/Items.hpp"
#include "game/rdr/ScriptFunction.hpp"
#include "game/rdr/Scripts.hpp"
#include "util/Rewards.hpp"
#include "util/HerbSpawner.hpp"
#include <array>

namespace YimMenu::Submenus
{

	Recovery::Recovery() :
	    Submenu::Submenu("Recovery")
	{
		auto recovery               = std::make_shared<Category>("Recovery");
		auto spawnCollectiblesGroup = std::make_shared<Group>("Spawn Collectibles");
		auto spawnHerbsGroup		= std::make_shared<Group>("Spawn Herbs");
		auto recoveryOptions        = std::make_shared<Group>("Options");

		static auto recoveryCommand = Commands::GetCommand<BoolCommand>("recoveryenabled"_J);

		spawnCollectiblesGroup->AddItem(std::make_shared<ImGuiItem>([=] {
			if (recoveryCommand->GetState())
			{
				static Rewards::eRewardType selected{};
				std::map<Rewards::eRewardType, std::string> reward_translations = {
				    {Rewards::eRewardType::HEIRLOOMS, "Heirlooms"},
				    {Rewards::eRewardType::COINS, "Coins"},
				    {Rewards::eRewardType::ALCBOTTLES, "Alcohol Bottles"},
				    {Rewards::eRewardType::ARROWHEADS, "Arrowheads"},
				    {Rewards::eRewardType::BRACELETS, "Bracelets"},
				    {Rewards::eRewardType::EARRINGS, "Earrings"},
				    {Rewards::eRewardType::NECKLACES, "Necklaces"},
				    {Rewards::eRewardType::RINGS, "Rings"},
				    {Rewards::eRewardType::TAROTCARDS_CUPS, "Tarot Cards - Cups"},
				    {Rewards::eRewardType::TAROTCARDS_PENTACLES, "Tarot Cards - Pentacles"},
				    {Rewards::eRewardType::TAROTCARDS_SWORDS, "Tarot Cards - Swords"},
				    {Rewards::eRewardType::TAROTCARDS_WANDS, "Tarot Cards - Wands"},
				    {Rewards::eRewardType::FOSSILS, "Fossils"},
				    {Rewards::eRewardType::EGGS, "Eggs"},
				    {Rewards::eRewardType::TREASURE, "Treasure Reward"},
				    {Rewards::eRewardType::CAPITALE, "Capitale"},
				    {Rewards::eRewardType::XP, "25K XP"},
				    {Rewards::eRewardType::MOONSHINERXP, "200 Moonshiner XP"},
				    {Rewards::eRewardType::TRADERXP, "200 Trader XP"},
				    {Rewards::eRewardType::COLLECTORXP, "200 Collector XP"},
				    {Rewards::eRewardType::NATURALISTXP, "300 Naturalist XP"},
				    {Rewards::eRewardType::BOUNTYHUNTERXP, "200 Bounty Hunter XP"},
				    {Rewards::eRewardType::TRADERGOODS, "Max Trader Goods"},
				};

				if (ImGui::BeginCombo("Rewards", reward_translations[selected].c_str()))
				{
					for (auto& [type, translation] : reward_translations)
					{
						if (ImGui::Selectable(std::string(translation).c_str(), type == selected, ImGuiSelectableFlags_AllowDoubleClick))
						{
							selected = type;
						}
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							FiberPool::Push([] {
								Rewards::GiveRequestedRewards({selected});
							});
						}
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("Add Selected"))
				{
					FiberPool::Push([] {
						Rewards::GiveRequestedRewards({selected});
					});
				}
			}
			else
			{
				ImGui::Text("Recovery Feature Restricted");
				ImGui::Text("The recovery/collectibles feature is risky and you might face a ban for using it. You are responsible for what you do with this feature. None of the developers or the YimMenu organization are responsible for any damages to your account.");
				if (ImGui::Button("Enable Recovery"))
				{
					recoveryCommand->SetState(true);
				}
			}
		}));
		spawnHerbsGroup->AddItem(std::make_shared<ImGuiItem>([=] {
			if (recoveryCommand->GetState())
			{
				static joaat_t selectedHerb;
				std::map<joaat_t, std::string> herb_translations = {{"HERB_LOOT_ALASKAN_GINSENG"_J, "Alaskan Ginseng"},{"HERB_LOOT_AMERICAN_GINSENG"_J, "American Ginseng"},{"HERB_LOOT_BAY_BOLETE"_J, "Bay Bolete"},{"HERB_LOOT_BLACK_BERRY"_J, "Black Berry"},{"HERB_LOOT_BLACK_CURRANT"_J, "Black Currant"},{"HERB_LOOT_BURDOCK_ROOT"_J, "Burdock Root"},{"HERB_LOOT_CHANTERELLES"_J, "Chanterelles"},{"HERB_LOOT_COMMON_BULRUSH"_J, "Common Bulrush"},{"HERB_LOOT_CREEPING_THYME"_J, "Creeping Thyme"},{"HERB_LOOT_DESERT_SAGE"_J, "Desert Sage"},{"HERB_LOOT_ENGLISH_MACE"_J, "English Mace"},{"HERB_LOOT_EVERGREEN_HUCKLEBERRY"_J, "Evergreen Huckleberry"},{"HERB_LOOT_GOLDEN_CURRANT"_J, "Golden Currant"},{"HERB_LOOT_HUMMINGBIRD_SAGE"_J, "Hummingbird Sage"},{"HERB_LOOT_INDIAN_TOBACCO"_J, "Indian Tobacco"},{"HERB_LOOT_MILKWEED"_J, "Milkweed"},{"HERB_LOOT_OLEANDER_SAGE"_J, "Oleander Sage"},{"HERB_LOOT_OREGANO"_J, "Oregano"},{"HERB_LOOT_PARASOL_MUSHROOM"_J, "Parasol Mushroom"},{"HERB_LOOT_PRAIRIE_POPPY"_J, "Prairie Poppy"},{"HERB_LOOT_RAMS_HEAD"_J, "Rams Head"},{"HERB_LOOT_RED_RASPBERRY"_J, "Red Raspberry"},{"HERB_LOOT_RED_SAGE"_J, "Red Sage"},{"HERB_LOOT_VANILLA_FLOWER"_J, "Vanilla Flower"},{"HERB_LOOT_VIOLET_SNOWDROP"_J, "Violet Snowdrop"},{"HERB_LOOT_WILD_CARROTS"_J, "Wild Carrots"},{"HERB_LOOT_WILD_FEVERFEW"_J, "Wild Feverfew"},{"HERB_LOOT_WILD_MINT"_J, "Wild Mint"},{"HERB_LOOT_WINTERGREEN_BERRY"_J, "Wintergreen Berry"},{"HERB_LOOT_YARROW"_J, "Yarrow"},{"HERB_LOOT_AGARITA"_J, "Agarita"},{"HERB_LOOT_BITTERWEED"_J, "Bitterweed"},{"HERB_LOOT_BLUE_BONNET"_J, "Blue Bonnet"},{"HERB_LOOT_BLOOD_FLOWER"_J, "Blood Flower"},{"HERB_LOOT_CARDINAL_FLOWER"_J, "Cardinal Flower"},{"HERB_LOOT_CHOCOLATE_DAISY"_J, "Chocolate Daisy"},{"HERB_LOOT_CREEK_PLUM"_J, "Creek Plum"},{"HERB_LOOT_RHUBARB"_J, "Rhubarb"},{"HERB_LOOT_WISTERIA"_J, "Wisteria"},{"HERB_LOOT_HARRIETUM"_J, "Harrietum"},};
				if (ImGui::BeginCombo("Herbs", herb_translations[selectedHerb].c_str()))
				{
					for (auto& [herb, translation] : herb_translations)
					{
						if (ImGui::Selectable(std::string(translation).c_str(), herb == selectedHerb))
						{
							selectedHerb = herb;
						}
					}
					ImGui::EndCombo();
				}

				static int amount = 1;
				ImGui::SliderInt("Amount", &amount, 1, 10);

				if (ImGui::Button("Give Selected"))
				{
					FiberPool::Push([] {
						if (!Scripts::RequestScript("interactive_campfire"_J))
							return;

						for (int i = 0; i < amount; i++)
							ScriptFunctions::GiveLootTableAward.StaticCall(selectedHerb, 0);
					});
				}

				static Hash selectedSpHerb = "COMPOSITE_LOOTABLE_ALASKAN_GINSENG_ROOT_DEF"_J;

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

				if (ImGui::BeginCombo("Herbs##spawner", selectedLabel)){
					for (const auto& [asset, translation] : herbTranslations) {
						const bool isSelected = asset == selectedSpHerb;
						if (ImGui::Selectable(translation.c_str(), isSelected)){
							selectedSpHerb = asset;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("Spawn Selected##spawner"))
				{
					const Hash herbToSpawn = selectedSpHerb;

					FiberPool::Push([herbToSpawn] {
						HerbSpawner::SpawnHerbComposite(herbToSpawn);
					});
				}
			}
		}));
		recoveryOptions->AddItem(std::make_shared<BoolCommandItem>("unlimiteditems"_J));
		recovery->AddItem(spawnCollectiblesGroup);
		recovery->AddItem(spawnHerbsGroup);
		recovery->AddItem(recoveryOptions);

		AddCategory(std::move(recovery));
	}
}