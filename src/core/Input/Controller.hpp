#pragma once
#include <Xinput.h>
#include <imgui.h>

namespace YimMenu {
	class Controller
	{
	public:
		static void Poll();
		static bool IsConnected();
	
	private:
		static void UpdateKey(ImGuiKey key, bool down);
		static void UpdateAnalog(ImGuiKey key, float value);
		static float Deadzone(float v, float dz = 0.25f);

		static XINPUT_GAMEPAD s_Prev;
		static bool s_Connected;
	};
}