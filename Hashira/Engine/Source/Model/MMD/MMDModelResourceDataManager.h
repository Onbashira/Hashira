#pragma once
#include <map>
#include <vector>
#include <memory>
#include <Windows.h>
#include "Engine/Source/Utility/ManagerComponent.h"

namespace K3D {
	struct MMDModelResourceData;

	//コンバート済みモデルのデータを保持するクラス
	class MMDModelResourceDataManager
	{
	private:
		ManagerComponent< MMDModelResourceData >	_pool;

	public:

		std::weak_ptr< MMDModelResourceData >	GetConvertedModelData(std::string path);
		void									SetConvertedModelData(std::string key, std::shared_ptr< MMDModelResourceData > modelData);
		void									EraseConvertedModelData(std::string key);
		bool									IsLoadedModel(std::string path);
		void									Discard();
		
		MMDModelResourceDataManager();
		~MMDModelResourceDataManager();
	private:
	public:

	};
}