#include "Shader.h"
#include "HLSLIncluder.h"
#include "Engine/Source/Utility/Utility.h"
#include <vector>
#include <string>
#include <algorithm>

Hashira::Shader::Shader()
{

}


Hashira::Shader::~Shader()
{

}


Microsoft::WRL::ComPtr<ID3DBlob>& Hashira::Shader::GetShader(Type type)
{
	if (_shaderMap.find(type) != _shaderMap.end()) {

		return _shaderMap[type];
	}
	return Microsoft::WRL::ComPtr<ID3DBlob>();
}

void Hashira::Shader::AddShaderMacro(std::string name, std::string definition)
{

	D3D_SHADER_MACRO macro;
	macro.Name = name.c_str();
	macro.Definition = definition.c_str();
	_shaderMacro.push_back(macro);
}

void Hashira::Shader::EraseShaderMacro(std::string name)
{
	auto itr = std::find_if(_shaderMacro.begin(), _shaderMacro.end(),
		[name](const D3D_SHADER_MACRO& value)->bool {
		if (std::string(value.Name) == name) {
			return true;
		}
		return false;
	}
	);
	this->_shaderMacro.erase(itr);
}

const std::vector<D3D_SHADER_MACRO>& Hashira::Shader::GetShaderMacro() const
{
	return _shaderMacro;
}

HRESULT Hashira::Shader::CompileShader(Type type, std::string shaderPath, std::string functionName, std::string shaderMode)
{
#if defined(_DEBUG)
	//グラフィックデバッグツールによるシェーダーのデバッグの有効化処理
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	UINT compileFlag = 0;
#endif

	Microsoft::WRL::ComPtr<ID3DBlob> shader;
	Microsoft::WRL::ComPtr<ID3DBlob> error;

	HRESULT hret = {};
	auto includer = Hashira::HLSLIncluder(Util::GetRelativePath(shaderPath));
	D3D_SHADER_MACRO* ptr = _shaderMacro.size() > 0 ? &this->_shaderMacro[0] : nullptr;
	ID3DInclude* includePtr = (includer._relativePath == "") ? nullptr : &includer;

	hret = D3DCompileFromFile(Util::StringToWString(shaderPath).c_str(), ptr, includePtr, functionName.c_str(), shaderMode.c_str(), compileFlag, 0, &shader, &error);


	if (FAILED(hret)) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
		error->Release();

	}

	this->_shaderMap[type] = shader;


	return S_OK;
}