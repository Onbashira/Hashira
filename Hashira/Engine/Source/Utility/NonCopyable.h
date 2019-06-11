#pragma once

namespace Hashira{
	class NonCopyable {
	private:

	protected:
		NonCopyable() {};
		~NonCopyable() {};
	public:
	private:
	protected:
		void operator =(const NonCopyable& ) {};
		NonCopyable(const NonCopyable& ) {};
		void operator =(NonCopyable&& ) {};
		NonCopyable(NonCopyable&& )  {};
	public:
	};
}