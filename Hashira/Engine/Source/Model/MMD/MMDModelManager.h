#pragma once

#include "Engine/Source/Utility/ManagerComponent.h"
#include <vector>
#include <memory>
#include <map>
#include <string>

namespace K3D {
	class MMDModel;

	class MMDModelManager
	{
	private:
	public:

	private:
		ManagerComponent<MMDModel>					 _pool;
	public:

		void SetMMDModel(std::string modelPath, std::shared_ptr<MMDModel> model);
		std::weak_ptr<MMDModel> GetMMDModel(std::string modelPath);
		void EraseMMDModel(std::string modelPath);
		void Discard();

		MMDModelManager();
		~MMDModelManager();
	};

}