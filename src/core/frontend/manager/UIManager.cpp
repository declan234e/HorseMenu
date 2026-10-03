#include "UIManager.hpp"

namespace YimMenu
{
	void UIManager::AddSubmenuImpl(const std::shared_ptr<Submenu>&& submenu)
	{
		if (!m_ActiveSubmenu)
			m_ActiveSubmenu = submenu;

		m_Submenus.push_back(std::move(submenu));
	}

	void UIManager::SetActiveSubmenuImpl(const std::shared_ptr<Submenu> Submenu)
	{
		m_ActiveSubmenu = Submenu;
	}

	void UIManager::DrawImpl()
	{
		auto pos = ImGui::GetCursorPos();
		float sideW = (m_ActiveSubmenu) ? m_ActiveSubmenu->m_SidePanelWidth : 0.0f;
		float gap = (sideW > 0) ? 10.0f : 0.0f;
		float mainW = ImGui::GetContentRegionAvail().x - 130 - sideW - gap;

		if (ImGui::BeginChild("##submenus", ImVec2(120, ImGui::GetContentRegionAvail().y - 20), ImGuiChildFlags_Borders))
		{
			for (auto& submenu : m_Submenus)
			{
				if (ImGui::Selectable(submenu->m_Name.data(), (submenu == m_ActiveSubmenu)))
				{
					SetActiveSubmenu(submenu);
				}
			}
		}
		ImGui::EndChild();
		
		ImGui::Text("Terminus");

		pos.y -= 28;
		ImGui::SetCursorPos(ImVec2(pos.x + 130, pos.y));

		if (ImGui::BeginChild("##minisubmenus", ImVec2(0, 50), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar))
		{
			if (m_ActiveSubmenu)
				m_ActiveSubmenu->DrawCategorySelectors();
		}
		ImGui::EndChild();

		ImGui::SetCursorPos(ImVec2(pos.x + 130, pos.y + 60));

		if (ImGui::BeginChild("##options", ImVec2(mainW, 0), ImGuiChildFlags_Borders))
		{
			if (m_OptionsFont)
				ImGui::PushFont(m_OptionsFont);

			if (m_ActiveSubmenu)
				m_ActiveSubmenu->Draw();

			if (m_OptionsFont)
				ImGui::PopFont();
		}
		ImGui::EndChild();

		if (sideW > 0 && m_ActiveSubmenu)
		{
			ImGui::SameLine(0, 10);
			if (ImGui::BeginChild("##sidepanel", ImVec2(sideW, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened))
			{
				m_ActiveSubmenu->DrawSidePanel();
			}
			ImGui::EndChild();
		}
	}

	std::shared_ptr<Submenu> UIManager::GetActiveSubmenuImpl()
	{
		if (m_ActiveSubmenu)
		{
			return m_ActiveSubmenu;
		}

		return nullptr;
	}

	std::shared_ptr<Category> UIManager::GetActiveCategoryImpl()
	{
		if (m_ActiveSubmenu)
		{
			return m_ActiveSubmenu->GetActiveCategory();
		}

		return nullptr;
	}
}
