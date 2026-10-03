#include "Controller.hpp"


#include "core/hooking/BaseHook.hpp"
#include "core/hooking/DetourHook.hpp"
#include "game/hooks/Hooks.hpp"
#include "game/frontend/GUI.hpp"

namespace YimMenu
{
	XINPUT_GAMEPAD Controller::s_Prev{};
	bool Controller::s_Connected = false;

	float Controller::Deadzone(float v, float dz)
	{ 
		if (std::abs(v) < dz)
			return 0.0f;
		return (v - dz * std::copysign(1.0f, v)) / (1.0f - dz);
	}

	void Controller::UpdateKey(ImGuiKey key, bool down)
	{ 
		ImGui::GetIO().AddKeyEvent(key, down);
	}

	void Controller::UpdateAnalog(ImGuiKey key, float value)
	{ 
		ImGui::GetIO().AddKeyAnalogEvent(key, value > 0.0f, value);
	}

	bool Controller::IsConnected()
	{ 
		return s_Connected;
	}

	void Controller::Poll()
	{ 
		DWORD pIndex = 0;
		XINPUT_STATE state{};

		DWORD result = BaseHook::Get<Hooks::Window::XInputGetState, DetourHook<decltype(&XInputGetState)>>()->Original()(pIndex, &state);
		if (result != ERROR_SUCCESS) {

			if (s_Connected) {
				s_Connected = false;
				ImGui::GetIO().BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
				// Release all digital keys
				UpdateKey(ImGuiKey_GamepadFaceDown, false);
				UpdateKey(ImGuiKey_GamepadFaceRight, false);
				UpdateKey(ImGuiKey_GamepadFaceLeft, false);
				UpdateKey(ImGuiKey_GamepadFaceUp, false);
				UpdateKey(ImGuiKey_GamepadL1, false);
				UpdateKey(ImGuiKey_GamepadR1, false);
				UpdateKey(ImGuiKey_GamepadL3, false);
				UpdateKey(ImGuiKey_GamepadR3, false);
				UpdateKey(ImGuiKey_GamepadDpadUp, false);
				UpdateKey(ImGuiKey_GamepadDpadDown, false);
				UpdateKey(ImGuiKey_GamepadDpadLeft, false);
				UpdateKey(ImGuiKey_GamepadDpadRight, false);
				UpdateKey(ImGuiKey_GamepadStart, false);
				UpdateKey(ImGuiKey_GamepadBack, false);
				// Release all analog
				UpdateAnalog(ImGuiKey_GamepadL2, 0.0f);
				UpdateAnalog(ImGuiKey_GamepadR2, 0.0f);
				UpdateAnalog(ImGuiKey_GamepadLStickLeft, 0.0f);
				UpdateAnalog(ImGuiKey_GamepadLStickRight, 0.0f);
				UpdateAnalog(ImGuiKey_GamepadLStickUp, 0.0f);
				UpdateAnalog(ImGuiKey_GamepadLStickDown, 0.0f);
				UpdateAnalog(ImGuiKey_GamepadRStickLeft, 0.0f);
				UpdateAnalog(ImGuiKey_GamepadRStickRight, 0.0f);
				UpdateAnalog(ImGuiKey_GamepadRStickUp, 0.0f);
				UpdateAnalog(ImGuiKey_GamepadRStickDown, 0.0f);
				s_Prev = {};
			}
			return;
		}

		s_Connected = true;
		auto& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

		// digi buttons (edge detect)
		auto btn = [&](WORD mask, ImGuiKey key) {
			bool down = (state.Gamepad.wButtons & mask) != 0;
			bool wasDown = (s_Prev.wButtons & mask) != 0;
			if (down != wasDown)
				UpdateKey(key, down);
		};

		btn(XINPUT_GAMEPAD_A, ImGuiKey_GamepadFaceDown);
		btn(XINPUT_GAMEPAD_B, ImGuiKey_GamepadFaceRight);
		btn(XINPUT_GAMEPAD_X, ImGuiKey_GamepadFaceLeft);
		btn(XINPUT_GAMEPAD_Y, ImGuiKey_GamepadFaceUp);
		btn(XINPUT_GAMEPAD_LEFT_SHOULDER, ImGuiKey_GamepadL1);
		btn(XINPUT_GAMEPAD_RIGHT_SHOULDER, ImGuiKey_GamepadR1);
		btn(XINPUT_GAMEPAD_BACK, ImGuiKey_GamepadBack);
		btn(XINPUT_GAMEPAD_START, ImGuiKey_GamepadStart);
		btn(XINPUT_GAMEPAD_LEFT_THUMB, ImGuiKey_GamepadL3);
		btn(XINPUT_GAMEPAD_RIGHT_THUMB, ImGuiKey_GamepadR3);
		btn(XINPUT_GAMEPAD_DPAD_UP, ImGuiKey_GamepadDpadUp);
		btn(XINPUT_GAMEPAD_DPAD_DOWN, ImGuiKey_GamepadDpadDown);
		btn(XINPUT_GAMEPAD_DPAD_LEFT, ImGuiKey_GamepadDpadLeft);
		btn(XINPUT_GAMEPAD_DPAD_RIGHT, ImGuiKey_GamepadDpadRight);

		// --- Analog triggers (always send, no edge detection needed) ---
		UpdateAnalog(ImGuiKey_GamepadL2, state.Gamepad.bLeftTrigger / 255.0f);
		UpdateAnalog(ImGuiKey_GamepadR2, state.Gamepad.bRightTrigger / 255.0f);

		// The below commented lines seem to break the scrolling on sticks so keep them commented I guess and remove them if not needed at all.

		// --- Left stick (analog, with deadzone) ---
		// float lx = Deadzone(state.Gamepad.sThumbLX / 32767.0f);
		// float ly = Deadzone(state.Gamepad.sThumbLY / 32767.0f);
		//
		//
		// UpdateAnalog(ImGuiKey_GamepadLStickLeft, std::max(-lx, 0.0f));
		// UpdateAnalog(ImGuiKey_GamepadLStickRight, std::max(lx, 0.0f));
		// UpdateAnalog(ImGuiKey_GamepadLStickUp, std::max(-ly, 0.0f));
		// UpdateAnalog(ImGuiKey_GamepadLStickDown, std::max(ly, 0.0f));
		// 
		// // --- Right stick (analog, with deadzone) ---
		// float rx = Deadzone(state.Gamepad.sThumbRX / 32767.0f);
		// float ry = Deadzone(state.Gamepad.sThumbRY / 32767.0f);
		// UpdateAnalog(ImGuiKey_GamepadRStickLeft, std::max(-rx, 0.0f));
		// UpdateAnalog(ImGuiKey_GamepadRStickRight, std::max(rx, 0.0f));
		// UpdateAnalog(ImGuiKey_GamepadRStickUp, std::max(-ry, 0.0f));
		// UpdateAnalog(ImGuiKey_GamepadRStickDown, std::max(ry, 0.0f));

		constexpr WORD Combo = XINPUT_GAMEPAD_RIGHT_SHOULDER | XINPUT_GAMEPAD_DPAD_RIGHT;
		bool openButtonsDown = (state.Gamepad.wButtons & Combo) == Combo;
		bool wasButtonsDown = (s_Prev.wButtons & Combo) == Combo;

		if (openButtonsDown && !wasButtonsDown) {
			GUI::ToggleMenu();
		}

		s_Prev = state.Gamepad;
	}
}