#pragma once


namespace Hashira {
	template<typename T>
	class Singleton
	{
	public:

		static std::unique_ptr<T>& GetInstance()
		{
			static auto instance = std::make_unique<T>();
			return instance;
		}

		virtual ~Singleton() {}

	protected:
		Singleton() {}
	private:
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator=(Singleton&&) = delete;
	};
}