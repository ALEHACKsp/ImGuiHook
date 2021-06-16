#include "../Global.h"
#include <d3d9.h>

namespace DirectX9 {
	bool Init() {
		if (InitWindow() == false) {
			return false;
		}

		HMODULE D3D9Module = GetModuleHandle("d3d9.dll");
		if (D3D9Module == NULL) {
			DeleteWindow();
			return false;
		}

		void* Direct3DCreate9 = GetProcAddress(D3D9Module, "Direct3DCreate9");
		if (Direct3DCreate9 == NULL) {
			DeleteWindow();
			return false;
		}

		LPDIRECT3D9 Direct3D9 = ((LPDIRECT3D9(__stdcall*)(uint32_t))(Direct3DCreate9))(D3D_SDK_VERSION);
		if (Direct3D9 == NULL) {
			DeleteWindow();
			return false;
		}

		D3DDISPLAYMODE DisplayMode;
		if (Direct3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode) < 0) {
			DeleteWindow();
			return false;
		}

		D3DPRESENT_PARAMETERS Params;
		Params.BackBufferWidth = 0;
		Params.BackBufferHeight = 0;
		Params.BackBufferFormat = DisplayMode.Format;
		Params.BackBufferCount = 0;
		Params.MultiSampleType = D3DMULTISAMPLE_NONE;
		Params.MultiSampleQuality = NULL;
		Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		Params.hDeviceWindow = WindowHwnd;
		Params.Windowed = 1;
		Params.EnableAutoDepthStencil = 0;
		Params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
		Params.Flags = NULL;
		Params.FullScreen_RefreshRateInHz = 0;
		Params.PresentationInterval = 0;

		LPDIRECT3DDEVICE9 Device;
		if (Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, WindowHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &Params, &Device) < 0) {
			Direct3D9->Release();
			DeleteWindow();
			return false;
		}

		MethodsTable = (uintx_t*)::calloc(119, sizeof(uintx_t));
		memcpy(MethodsTable, *(uintx_t**)Device, 119 * sizeof(uintx_t));

		MH_Initialize();
		Direct3D9->Release();
		Direct3D9 = NULL;
		Device->Release();
		Device = NULL;
		DeleteWindow();
		return true;
	}
}