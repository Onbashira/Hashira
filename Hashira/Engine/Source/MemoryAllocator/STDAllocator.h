#pragma once


#include <limits>
#include <utility>
#include "Engine/Source/MemoryAllocator/MemoryAllocator.h"
#include "Engine/Source/Utility/BasicTypes.h"
namespace Hashira
{

	template <typename T>
	typename std::enable_if<std::is_destructible<T>::value, void>::type Destruct(T *ptr)
	{
		ptr->~T();
	}

	template <typename T>
	typename std::enable_if<!std::is_destructible<T>::value, void>::type Destruct(T *ptr)
	{
	}

	template <typename T, typename AllocatorType>
	struct STDAllocator
	{
		using value_type = T;
		using pointer = value_type * ;
		using const_pointer = const value_type*;
		using reference = value_type & ;
		using const_reference = const value_type&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		STDAllocator(AllocatorType& Allocator, const char* Description, const char* FileName, const Int32 LineNumber)noexcept :
			_allocator(Allocator)
		{
		}

		template <class U>
		STDAllocator(const STDAllocator<U, AllocatorType>& other)noexcept :
			_allocator(other._allocator)

		{
		}

		template <class U>
		STDAllocator(STDAllocator<U, AllocatorType>&& other)noexcept :
			_allocator(other._allocator)

		{
		}

		template <class U>
		STDAllocator& operator = (STDAllocator<U, AllocatorType>&& other)noexcept
		{

			return *this;
		}

		template< class U > struct rebind
		{
			typedef STDAllocator<U, AllocatorType> other;
		};

		T* allocate(std::size_t count)
		{
			static constexpr const char* _dvpDescription = "<Unavailable in release build>";
			static constexpr const char* _dvpFileName = "<Unavailable in release build>";
			static constexpr Int32       _dvpLineNumber = -1;

			return reinterpret_cast<T*>(_allocator.Allocate(count * sizeof(T), _dvpDescription, _dvpFileName, _dvpLineNumber));
		}

		pointer       address(reference r) { return &r; }
		const_pointer address(const_reference r) { return &r; }

		void deallocate(T* p, std::size_t count)
		{
			_allocator.Free(p);
		}

		inline size_type max_size() const
		{
			//max“WŠJ‰ñ”ð
			return (std::numeric_limits<size_type>::max)() / sizeof(T);

		}

		//    construction/destruction
		template< class U, class... Args >
		void construct(U* p, Args&&... args)
		{
			::new(p) U(std::forward<Args>(args)...);
		}

		inline void destroy(pointer p)
		{
			p->~T();
		}

		AllocatorType &_allocator;

	};

#define STD_ALLOCATOR(Type, AllocatorType, Allocator, Description) STDAllocator<Type, AllocatorType>(Allocator, Description, __FILE__, __LINE__)

	template <class T, class U, class A>
	bool operator==(const STDAllocator<T, A>&left, const STDAllocator<U, A>&right)
	{
		return &left._allocator == &right._allocator;
	}

	template <class T, class U, class A>
	bool operator!=(const STDAllocator<T, A> &left, const STDAllocator<U, A> &right)
	{
		return !(left == right);
	}

	template<class T> using STDAllocatorRawMem = STDAllocator<T, IMemoryAllocator>;
#define STD_ALLOCATOR_RAW_MEM(Type, Allocator, Description) STDAllocatorRawMem<Type>(Allocator, Description, __FILE__, __LINE__)

	template< class T, typename AllocatorType >
	struct STDDeleter
	{
		STDDeleter() noexcept {}

		STDDeleter(AllocatorType& Allocator) noexcept :
			_allocator(&Allocator)
		{}

		STDDeleter(const STDDeleter&) = default;
		STDDeleter& operator = (const STDDeleter&) = default;

		STDDeleter(STDDeleter&& rhs) noexcept :
			_allocator(rhs._allocator)
		{
			rhs._allocator = nullptr;
		}

		STDDeleter& operator = (STDDeleter&& rhs) noexcept
		{
			_allocator = rhs._allocator;
			rhs._allocator = nullptr;
			return *this;
		}

		void operator()(T *ptr) noexcept
		{
			//VERIFY(_allocator != nullptr, "The deleter has been moved away or never initialized, and can't be used");
			Destruct(ptr);
			_allocator->Free(ptr);
		}

	private:
		AllocatorType* m_Allocator = nullptr;
	};

	template<class T> using STDDeleterRawMem = STDDeleter<T, IMemoryAllocator>;

}
