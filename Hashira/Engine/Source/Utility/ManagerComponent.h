#pragma once
#include <memory>
#include <map>
#include <unordered_map>
#include <string>


namespace Hashira {
	//�V���v���ȃ��\�[�X�L�����A�[�̃N���X
	template <class T>
	class ManagerComponent
	{
	private:
		//!���\�[�X�L�����A�[
		std::map<std::string, std::shared_ptr<T>> _resourceMap;

	protected:
	
	public:
	
	private:
	
	public:

		ManagerComponent() {};

		virtual ~ManagerComponent() { DiscardMap(); };
		//�v�f�̒ǉ��B��O��������True�ŋ����㏑��
		void Set(std::string name, std::shared_ptr<T> resource, bool forceOverride = false);
		//�v�f�̒ǉ��B��O��������True�ŋ����㏑��
		void Set(std::string name, const T& resource, bool forceOverride = false);
		//�v�f�̎擾
		std::weak_ptr<T>	 Get(std::string name);
		//�v�f�̍폜
		void Erase(std::string name);
		//�}�b�v�̖����I�N���A
		void DiscardMap();
		//���\�[�X�����[�h�ς݂��ۂ�
		bool IsLooded(std::string name);
		//�}�b�v���̂̎Q�Ƃ��擾����
		std::map<std::string, std::shared_ptr<T>>& GetMap();


	};


	template <class T>
	class NonRaiiManagerComponent
	{
	private:

	protected:

		std::map<std::string, T> _resourceMap;

	public:

	private:

	public:

		NonRaiiManagerComponent() {};

		virtual ~NonRaiiManagerComponent() { DiscardMap(); };
		//�v�f�̒ǉ��B��O��������True�ŋ����㏑��
		void Set(std::string name,const T& resource, bool forceOverride = false);
		//�v�f�̎擾
		T*	 Get(std::string name);
		//�v�f�̍폜
		void Erase(std::string name);
		//�}�b�v�̖����I�N���A
		void DiscardMap();
		//���\�[�X�����[�h�ς݂��ۂ�
		bool IsLooded(std::string name);
		//�}�b�v���̂̎Q�Ƃ��擾����
		std::map<std::string, T>& GetMap();


	};

	template <class T>
	class UnorderedManagerComponent
	{
	private:

	protected:

		std::unordered_map<std::string, std::shared_ptr<T>> _resourceMap;

	public:

	private:

	public:	
		
		UnorderedManagerComponent() {};

		virtual ~UnorderedManagerComponent() { DiscardMap(); };
		//�v�f�̒ǉ��B��O��������True�ŋ����㏑��
		void Set(std::string name, std::shared_ptr<T> resource, bool forceOverride = false);
		//�v�f�̒ǉ��B��O��������True�ŋ����㏑��
		void Set(std::string name, const T & resource, bool forceOverride = false);
		//�v�f�̎擾
		std::weak_ptr<T>  Get(std::string name);
		//�v�f�̍폜
		void Erase(std::string name);
		//�}�b�v�̖����I�N���A
		void DiscardMap();
		//���\�[�X�����[�h�ς݂��ۂ�
		bool IsLooded(std::string name);
		//�}�b�v���̂̎Q�Ƃ��擾����
		std::unordered_map<std::string, std::shared_ptr<T>>& GetMap();
	};
}

template<class T>
inline void Hashira::ManagerComponent<T>::Set(std::string name, std::shared_ptr<T> resource, bool forceOverride)
{

	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = resource;
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = resource;

	}
}

template<class T>
inline void Hashira::ManagerComponent<T>::Set(std::string name, const T & resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = std::make_shared<T>(resource);
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = std::make_shared<T>(resource);

	}
}

template<class T>
inline std::weak_ptr<T> Hashira::ManagerComponent<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return _resourceMap[name];
	}
	return std::weak_ptr<T>();
}

template<class T>
inline void Hashira::ManagerComponent<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void Hashira::ManagerComponent<T>::DiscardMap()
{
	_resourceMap.clear();
}

template<class T>
inline bool Hashira::ManagerComponent<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}

template<class T>
inline std::map<std::string, std::shared_ptr<T>>& Hashira::ManagerComponent<T>::GetMap()
{

	return _resourceMap;
}


//NoRaii


template<class T>
inline void Hashira::NonRaiiManagerComponent<T>::Set(std::string name, const T & resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = resource;
	}
	else if (forceOverride) {
		//_resourceMap[name];
		_resourceMap[name] = resource;

	}
}

template<class T>
inline T* Hashira::NonRaiiManagerComponent<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return &_resourceMap[name];
	}
	return nullptr;
}

template<class T>
inline void Hashira::NonRaiiManagerComponent<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void Hashira::NonRaiiManagerComponent<T>::DiscardMap()
{
	_resourceMap.clear();

}

template<class T>
inline bool Hashira::NonRaiiManagerComponent<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}

template<class T>
inline std::map<std::string, T>& Hashira::NonRaiiManagerComponent<T>::GetMap()
{
	return _resourceMap;
}

//UnorderedMap

template<class T>
inline void Hashira::UnorderedManagerComponent<T>::Set(std::string name, std::shared_ptr<T> resource, bool forceOverride)
{

	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = resource;
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = resource;

	}
}

template<class T>
inline void Hashira::UnorderedManagerComponent<T>::Set(std::string name, const T & resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = std::make_shared<T>(resource);
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = std::make_shared<T>(resource);

	}
}

template<class T>
inline std::weak_ptr<T> Hashira::UnorderedManagerComponent<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return _resourceMap[name];
	}
	return std::weak_ptr<T>();
}

template<class T>
inline void Hashira::UnorderedManagerComponent<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void Hashira::UnorderedManagerComponent<T>::DiscardMap()
{
	_resourceMap.clear();
}  

template<class T>
inline bool Hashira::UnorderedManagerComponent<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}

template<class T>
inline std::unordered_map<std::string, std::shared_ptr<T>>& Hashira::UnorderedManagerComponent<T>::GetMap()
{
	return _resourceMap;
}

