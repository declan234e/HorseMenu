#include "core/hooking/DetourHook.hpp"
#include "core/renderer/Renderer.hpp"
#include "game/hooks/Hooks.hpp"
#include "game/frontend/GUI.hpp"

#include <Xinput.h>

namespace YimMenu::Hooks
{
	LRESULT Window::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		if (g_Running)
			Renderer::WndProc(hwnd, umsg, wparam, lparam);

		return BaseHook::Get<Window::WndProc, DetourHook<decltype(&WndProc)>>()->Original()(hwnd, umsg, wparam, lparam);
	}
	
	BOOL Window::SetCursorPos(int x, int y)
	{
		if (GUI::IsOpen() && !Renderer::IsResizing())
		{
			return true;
		}
		
		return BaseHook::Get<Window::SetCursorPos, DetourHook<decltype(&SetCursorPos)>>()->Original()(x, y);
	}

	BOOL Window::ShowWindow(HWND hWnd, int nCmdShow)
	{
		LOG(INFO) << hWnd << " " << nCmdShow;
		// prevent game from hiding console window
		if (hWnd == GetConsoleWindow() && nCmdShow == 0)
		{
			return false;
		}

		return BaseHook::Get<Window::ShowWindow, DetourHook<decltype(&ShowWindow)>>()->Original()(hWnd, nCmdShow);
	}
	DWORD Window::XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
	{
		auto result = BaseHook::Get<Window::XInputGetState, DetourHook<decltype(&XInputGetState)>>()->Original()(dwUserIndex, pState);

		if (result == ERROR_SUCCESS && pState && g_BlockPadInput)
		{
			constexpr WORD BlockedButtons = XINPUT_GAMEPAD_A | XINPUT_GAMEPAD_B | XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_DOWN | XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT | XINPUT_GAMEPAD_RIGHT_SHOULDER | XINPUT_GAMEPAD_LEFT_SHOULDER;

			pState->Gamepad.wButtons &= ~BlockedButtons;
			pState->Gamepad.sThumbLX = 0;
			pState->Gamepad.sThumbLY = 0;
			pState->Gamepad.sThumbRX = 0;
			pState->Gamepad.sThumbRY = 0;
		}

		return result;
	}
}