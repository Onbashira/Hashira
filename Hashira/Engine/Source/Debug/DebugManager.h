#pragma once
#include "Engine/Source/Debug/DebugWindow.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/ThirdParty/imgui/imgui.h"
#include <Windows.h>

namespace Hashira {
	class DebugManager
	{

	public:

	private:
		
		INT64 _time;

		INT64 _ticksPerSecond;

		bool _wantUpdateHasGamepad;

		bool _hasGamepad;

		HWND _forcusWnd;

		std::shared_ptr<DebugWindow> _debugWindow;

	public:

		DebugManager();

		~DebugManager();

		IMGUI_IMPL_API LRESULT ImGuiWndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		bool Init(HWND hwnd, std::shared_ptr<D3D12Device>& dev, int frame, DXGI_FORMAT format, D3D12_CPU_DESCRIPTOR_HANDLE fontSrvCPUDescHandle, D3D12_GPU_DESCRIPTOR_HANDLE fontSrvGPUDescHandle);

		void DebugWindowNewFrame();

		void DebugWindowShutdown();
		
		std::shared_ptr<DebugWindow>& GetDebugWindow();


	private:

		bool ImGuiInit();

		bool ImGuiImpleInit(HWND hwnd);

		void ImGuiNewFrame();

		bool ImGuiUpdateMouseCursor();

		void ImGuiUpdateMousePos();

		void ImGuiUpdateGamepads();
	};


}