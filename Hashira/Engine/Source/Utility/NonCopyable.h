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
		void operator =(const NonCopyable& )noexcept {};
		NonCopyable(const NonCopyable& )noexcept {};
		void operator =(NonCopyable&& )noexcept {};
		NonCopyable(NonCopyable&& ) noexcept {};
	public:
	};
}