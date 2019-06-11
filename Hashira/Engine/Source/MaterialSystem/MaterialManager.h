#pragma once
namespace Hashira {
	class Material;

	class MaterialManager
	{

	private:

		std::mutex _mutex;

		std::vector<std::shared_ptr<Material>> _materials;

		std::map<unsigned int, std::shared_ptr<Material>> _materialMap;


	public:
		
		template <class... U>
		void AddMaterial(U&& ...args);

		MaterialManager();

		~MaterialManager();
	};


	template<class ...U>
	inline void MaterialManager::AddMaterial(U && ...args)
	{
		_materials.push_back(std::make_shared<Material>(args));
	}
}