#include "Players.hpp"

#include "core/frontend/widgets/imgui_colors.h"
#include "game/backend/PlayerData.hpp"
#include "game/backend/Players.hpp"
#include "game/backend/PlayerDatabase.hpp"
#include "game/features/Features.hpp"
#include "game/frontend/items/Items.hpp"

#include "Player/Helpful.hpp"
#include "Player/Info.hpp"
#include "Player/Kick.hpp"
#include "Player/Toxic.hpp"
#include "Player/Trolling.hpp"

namespace YimMenu::Submenus
{
	struct Tag
	{
		std::string Name;
		ImVec4 Color;
	};

	static std::vector<Tag> GetPlayerTags(YimMenu::Player player)
	{
		std::vector<Tag> tags;

		if (player.IsHost())
			tags.push_back({"HOST", ImGui::Colors::DeepSkyBlue});

		if (player.IsModder())
			tags.push_back({"MOD", ImGui::Colors::DeepPink});

		if (player.GetPed() && player.GetPed().IsInvincible())
			tags.push_back({"GOD", ImGui::Colors::Crimson});

		if (player.GetPed() && !player.GetPed().IsVisible())
			tags.push_back({"INVIS", ImGui::Colors::MediumPurple});

		return tags;
	}

	static void DrawPlayerList()
	{
		struct ComparePlayerNames
		{
			bool operator()(YimMenu::Player a, YimMenu::Player b) const
			{
				return a.GetName() < b.GetName();
			}
		};

		std::multimap<uint8_t, Player, ComparePlayerNames> sortedPlayers(YimMenu::Players::GetPlayers().begin(),
		    YimMenu::Players::GetPlayers().end());

		ImGui::Checkbox("Spectate", &YimMenu::g_Spectating);
		for (auto& [id, player] : sortedPlayers)
		{
			std::string display_name = player.GetName();

			ImGui::PushID(id);
			if (ImGui::Selectable(display_name.c_str(), (YimMenu::Players::GetSelected() == player)))
			{
				YimMenu::Players::SetSelected(id);
			}
			ImGui::PopID();

			if (player.IsModder() && ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				for (auto detection : player.GetData().m_Detections)
					ImGui::BulletText("%s", g_PlayerDatabase->ConvertDetectionToDescription(detection).c_str());
				ImGui::EndTooltip();
			}

			auto tags = GetPlayerTags(player);

			auto old_item_spacing = ImGui::GetStyle().ItemSpacing.x;

			for (auto& tag : tags)
			{
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, tag.Color);
				ImGui::Text(("[" + tag.Name + "]").c_str());
				ImGui::PopStyleColor();
				ImGui::GetStyle().ItemSpacing.x = 1;
			}

			ImGui::GetStyle().ItemSpacing.x = old_item_spacing;
		}
	}

	void Players::DrawSidePanel()
	{
		DrawPlayerList();
	}

	Players::Players() :
	    Submenu::Submenu("Players")
	{
		m_SidePanelWidth = 215.0f;

		AddCategory(std::move(BuildInfoMenu()));
		AddCategory(std::move(BuildHelpfulMenu()));
		AddCategory(std::move(BuildTrollingMenu()));
		AddCategory(std::move(BuildToxicMenu()));
		AddCategory(std::move(BuildKickMenu()));
	}
}
