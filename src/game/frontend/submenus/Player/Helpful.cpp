#include "Helpful.hpp"
#include "game/backend/FiberPool.hpp"
#include "game/backend/Players.hpp"
#include "game/features/players/nice/SpawnTreasureChest.hpp"

namespace YimMenu::Submenus
{
	void RenderChestSpawnerMenu()
	{
		ImGui::PushID("ChestSpawner");
		static int amount = 3;
		ImGui::PushItemWidth(120);
		ImGui::SliderInt("Amount of gold bars", &amount, 1, 10);
		ImGui::PopItemWidth();
		if (ImGui::Button("Spawn Treasure Chest"))
		{
			int spawnAmount = amount;
			FiberPool::Push([spawnAmount] {
				if (Players::GetSelected().IsValid())
					Features::SpawnGoldChest(Players::GetSelected(), spawnAmount);
			});
		}
		ImGui::PopID();
	}

	std::shared_ptr<Category> BuildHelpfulMenu()
	{
		auto menu = std::make_shared<Category>("Helpful");
		auto group = std::make_shared<Group>("Treasure Chest Spawner");
		group->AddItem(std::make_shared<ImGuiItem>([] {
			RenderChestSpawnerMenu();
		}));
		menu->AddItem(group);
		return menu;
	}
}