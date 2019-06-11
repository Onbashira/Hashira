#ifndef __D3D12_COMMON_H__
#define __D3D12_COMMON_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "d3dx12.h"

#include <d3d12.h>
#include <dxgicommon.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <dwrite.h>
#include <d2d1_3.h>
#include <d3d11on12.h>
#include <wrl.h>
#include <Windows.h>
#include <DirectXMath.h>


#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"d3d11.lib")

#define CHECK_RESULT(hr) if(FAILED(hr)) { return hr; }

#endif // !__D3D12_COMMON_H__

