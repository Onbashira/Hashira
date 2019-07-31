#pragma once

#define WIN32_LEAN_AND_MEAN
#define SHADER_TARGET_DIR "Engine/Shader/"
#define THIRD_PARTY_TARGET_DIR "Engine/ThirdParty/"


#include <Windows.h>
#include <memory>
#include <vector>
#include <map>
#include <array>
#include <algorithm>
#include <assert.h>
#include <comdef.h>
#include <wrl.h>
#include <mutex>
#include <iomanip>
#include <tuple>
#include "Engine/Source/Utility/BasicTypes.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Math/Math.h"
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Debug/Logger.h"

