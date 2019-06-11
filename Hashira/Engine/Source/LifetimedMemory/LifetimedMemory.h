#pragma once
#include <atomic>
#include <xmemory>
#include <exception>
#include <typeinfo>
#include <type_traits>
#include <xatomic0.h>
#include <memory>
#include <yvals.h>
#include <atomic>


/**
 * @file LifetimedMemory
 * @brief shared_ptrに生存時間を付与した改造クラス
 * @author 小林開人
 * @date 
 */


#pragma pack(push,_CRT_PACKING)
#pragma warning(push,_STL_WARNING_LEVEL)
#pragma warning(disable: _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new


// DECLARATIONS
template<class _Ty>
class LifetimedShared_Ptr;

template<class _Ty>
class LifetimedWeak_Ptr;

template<class _Yty,
	class = void>
	struct _Can_enable_shared
	: std::false_type
{	// detect unambiguous and accessible inheritance from enable_shared_from_this
};

template<class _Yty>
struct _Can_enable_shared<_Yty, std::void_t<typename _Yty::_Esft_type>>
	: std::is_convertible<std::remove_cv_t<_Yty> *, typename _Yty::_Esft_type *>::type
{	// is_convertible is necessary to verify unambiguous inheritance
};
template<class _Other,
	class _Yty>
	void _Enable_lifetimed_shared_from_this1(const LifetimedShared_Ptr<_Other>& _This, _Yty * _Ptr, std::true_type)
{	// enable shared_from_this
	if (_Ptr && _Ptr->_Wptr.expired())
	{
		_Ptr->_Wptr = LifetimedShared_Ptr<std::remove_cv_t<_Yty>>(_This, const_cast<std::remove_cv_t<_Yty> *>(_Ptr));
		}
	}

template<class _Other,
	class _Yty>
	void _Enable_lifetimed_shared_from_this1(const LifetimedShared_Ptr<_Other>&, _Yty *, std::false_type)
{	
	// don't enable shared_from_this
}

template<class _Other,
	class _Yty>
	void _Enable_lifetimed_shared_from_this(const LifetimedShared_Ptr<_Other>& _This, _Yty * _Ptr)
{	// possibly enable shared_from_this
	_Enable_shared_from_this1(_This, _Ptr, std::bool_constant<std::conjunction_v<
		std::negation<std::is_array<_Other>>,
		std::negation<std::is_volatile<_Yty>>,
		_Can_enable_shared<_Yty>>>{});
}

class __declspec(novtable) LifetimedRefCountBase
{	// common code for reference counting
private:
#ifdef _M_CEE_PURE
	virtual void Destroy() noexcept
	{	// permanent workaround to avoid mentioning _purecall in msvcurt.lib, ptrustu.lib, or other support libs
		_STD terminate();
	}

	virtual void DeleteThis() noexcept
	{	// permanent workaround to avoid mentioning _purecall in msvcurt.lib, ptrustu.lib, or other support libs
		_STD terminate();
	}
#else /* ^^^ _M_CEE_PURE ^^^ // vvv !_M_CEE_PURE vvv */
	virtual void Destroy() noexcept = 0;
	virtual void DeleteThis() noexcept = 0;
#endif /* _M_CEE_PURE */

	//!ユーザーカウント
	std::_Atomic_counter_t _uses;

	//!弱参照カウント
	std::_Atomic_counter_t _weaks;

	//!生存時間
	std::atomic<unsigned long> _lifetime;

protected:
	LifetimedRefCountBase()
		: _uses(1), _weaks(1), _lifetime(1)	// non-atomic initializations
	{	// construct
	
	}

public:
	virtual ~LifetimedRefCountBase() noexcept
	{	// TRANSITION, should be non-virtual
	}

	bool Incref_nz()
	{	// increment use count if not zero, return true if successful
		for (;;)
		{	// loop until state is known
#if _USE_INTERLOCKED_REFCOUNTING
			const std::_Atomic_integral_t _Count =
				static_cast<volatile std::_Atomic_counter_t&>(_uses);

			if (_Count == 0)
				return (false);

			if (static_cast<std::_Atomic_integral_t>(_InterlockedCompareExchange(
				reinterpret_cast<volatile long *>(&_uses),
				static_cast<long>(_Count + 1), static_cast<long>(_Count))) == _Count)
				return (true);

#else /* _USE_INTERLOCKED_REFCOUNTING */
			const _Atomic_integral_t _Count =
				_Load_atomic_counter(_uses);

			if (_Count == 0)
				return (false);

			if (_Compare_increment_atomic_counter(_uses, _Count))
				return (true);
#endif /* _USE_INTERLOCKED_REFCOUNTING */
		}
	}

	void IncRefCount()
	{	// increment use count
		_InterlockedIncrement(reinterpret_cast<volatile long *>(&_uses));
	}

	void IncWeakRefCount()
	{	// increment weak reference count
		_InterlockedIncrement(reinterpret_cast<volatile long *>(&_weaks));
	}

	void IncLifetime()
	{
		++_lifetime;
	}

	void SetLifetime(unsigned long lifetime)
	{
		_lifetime.store(lifetime);
	}

	void DecRefCount()
	{	// decrement use count
		
		if (_InterlockedDecrement(reinterpret_cast<volatile long *>(&_uses)) == 0)
		{	// destroy managed resource, decrement weak reference count
			Destroy();
			DecWeakRefCount();
		}
	}

	void DecWeakRefCount()
	{	// decrement weak reference count
		if (_InterlockedDecrement(reinterpret_cast<volatile long *>(&_weaks)) == 0)
		{
			DeleteThis();
		}
	}

	void DecLifetime()
	{	// decrement lifetime reference count
		if (_lifetime > 0)
		{
			--_lifetime;
		}
		if (_lifetime <= 0 && _uses <= 0)
		{
			Destroy();
			DecWeakRefCount();
		}
	}

	long UseCount() const noexcept
	{	// return use count
		return (static_cast<long>(std::_Get_atomic_count(_uses)));
	}

	long Lifetime() const noexcept
	{
		return (static_cast<long>(_lifetime.load()));
	}

	virtual void * GetDeleter(const type_info&) const noexcept
	{	// return address of deleter object
		return (nullptr);
	}
};

template<class _Ty>
class LifetimedRefCount
	: public LifetimedRefCountBase
{	// handle reference counting for pointer without deleter
public:
	explicit LifetimedRefCount(_Ty * _Px)
		: LifetimedRefCountBase(), _ptr(_Px)
	{	// construct
	}
private:

	virtual void Destroy() noexcept override
	{	// destroy managed resource
		delete _ptr;
	}

	virtual void DeleteThis() noexcept override
	{	// destroy self
		delete this;
	}

	_Ty * _ptr;
};

// CLASS TEMPLATE LifetimedRefCountObj
template<class _Ty>
class LifetimedRefCountObj
	: public LifetimedRefCountBase
{	// handle reference counting for object in control block, no allocator
public:
	template<class... _Types>
	explicit LifetimedRefCountObj(_Types&&... _Args)
		: LifetimedRefCountBase()
	{	// construct from argument list
		::new (static_cast<void *>(&_Storage)) _Ty(std::forward<_Types>(_Args)...);
	}

	_Ty * _Getptr()
	{	// get pointer
		return (reinterpret_cast<_Ty *>(&_Storage));
	}

private:
	virtual void Destroy() noexcept override
	{	// destroy managed resource
		_Getptr()->~_Ty();
	}

	virtual void DeleteThis() noexcept override
	{	// destroy self
		delete this;
	}

	std::aligned_union_t<1, _Ty> _Storage;
};

// CLASS TEMPLATE _Ref_count_obj_alloc
template<class _Ty,class _Alloc>
class LifetimedRefCountObjAlloc: public LifetimedRefCountBase
{	// handle reference counting for object in control block, allocator
public:
	template<class... _Types>
	explicit LifetimedRefCountObjAlloc(const _Alloc& _Al_arg, _Types&&... _Args)
		: LifetimedRefCountBase(), _Mypair(std::_One_then_variadic_args_t(), _Al_arg)
	{	// construct from argument list, allocator
		::new (static_cast<void *>(&_Mypair._Get_second())) _Ty(std::forward<_Types>(_Args)...);
	}

	_Ty * _Getptr()
	{	// get pointer
		return (reinterpret_cast<_Ty *>(&_Mypair._Get_second()));
	}

private:
	using _Myalty = std::_Rebind_alloc_t<_Alloc, LifetimedRefCountObjAlloc>;
	using _Mystoragety = std::aligned_union_t<1, _Ty>;

	virtual void _Destroy() noexcept override
	{	// destroy managed resource
		_Getptr()->~_Ty();
	}

	virtual void _Delete_this() noexcept override
	{	// destroy self
		_Myalty _Al = _Mypair._Get_first();
		std::allocator_traits<_Myalty>::destroy(_Al, this);
		std::_Deallocate_plain(_Al, this);
	}

	std::_Compressed_pair<_Myalty, _Mystoragety> _Mypair;
};


template<class _Ty>
class LifetimedPtrBase
{	// base class for shared_ptr and LifetimedWeak_Ptr
public:
	using element_type = std::remove_extent_t<_Ty>;

	_NODISCARD long use_count() const noexcept
	{	// return use count
		return (_Rep ? _Rep->UseCount() : 0);
	}

	_NODISCARD long lifetime_count() const noexcept
	{	// return use count
		return (_Rep ? _Rep->Lifetime() : 0);
	}

	void lifetime_decrent()
	{
		_Declref();
	}

	void set_lifetime_count(unsigned long lifetime) {
		if (_Rep) {
			_Rep->SetLifetime(lifetime);
		}
	}

	template<class _Ty2>
	_NODISCARD bool owner_before(const LifetimedPtrBase<_Ty2>& _Right) const noexcept
	{	// compare addresses of manager objects
		return (_Rep < _Right._Rep);
	}

	LifetimedPtrBase(const LifetimedPtrBase&) = delete;

	LifetimedPtrBase& operator=(const LifetimedPtrBase&) = delete;

protected:
	_NODISCARD element_type * get() const noexcept
	{	// return pointer to resource
		return (_Ptr);
	}

	constexpr LifetimedPtrBase() noexcept = default;

	~LifetimedPtrBase() = default;

	template<class _Ty2>
	void _Move_construct_from(LifetimedPtrBase<_Ty2>&& _Right)
	{	// implement shared_ptr's (converting) move ctor and LifetimedWeak_Ptr's move ctor
		_Ptr = _Right._Ptr;
		_Rep = _Right._Rep;

		_Right._Ptr = nullptr;
		_Right._Rep = nullptr;
	}

	template<class _Ty2>
	void _Copy_construct_from(const LifetimedShared_Ptr<_Ty2>& _Other)
	{	// implement shared_ptr's (converting) copy ctor
		if (_Other._Rep)
		{
			_Other._Rep->IncRefCount();
		}

		_Ptr = _Other._Ptr;
		_Rep = _Other._Rep;
	}

	template<class _Ty2>
	void _Alias_construct_from(const LifetimedShared_Ptr<_Ty2>& _Other, element_type * _Px)
	{	// implement shared_ptr's aliasing ctor
		if (_Other._Rep)
		{
			_Other._Rep->IncRefCount();
		}

		_Ptr = _Px;
		_Rep = _Other._Rep;
	}

	template<class _Ty0>
	friend class LifetimedWeak_Ptr;	// specifically, LifetimedWeak_Ptr::lock()

	template<class _Ty2>
	bool _Construct_from_weak(const LifetimedWeak_Ptr<_Ty2>& _Other)
	{	// implement shared_ptr's ctor from LifetimedWeak_Ptr, and LifetimedWeak_Ptr::lock()
		if (_Other._Rep && _Other._Rep->Incref_nz())
		{
			_Ptr = _Other._Ptr;
			_Rep = _Other._Rep;
			return (true);
		}

		return (false);
	}

	void _Decref()
	{	// decrement reference count
		if (_Rep)
		{
			_Rep->DecRefCount();
		}
	}

	void _Swap(LifetimedPtrBase& _Right) noexcept
	{	// swap pointers
		std::swap(_Ptr, _Right._Ptr);
		std::swap(_Rep, _Right._Rep);
	}

	void _Set_ptr_rep(element_type * _Other_ptr, LifetimedRefCountBase * _Other_rep)
	{	// take new resource
		_Ptr = _Other_ptr;
		_Rep = _Other_rep;
	}

	template<class _Ty2>
	void _Weakly_construct_from(const LifetimedPtrBase<_Ty2>& _Other)
	{	// implement LifetimedWeak_Ptr's ctors
		if (_Other._Rep)
		{
			_Other._Rep->IncWeakRefCount();
		}

		_Ptr = _Other._Ptr;
		_Rep = _Other._Rep;
	}

	void _Decwref()
	{	// decrement weak reference count
		if (_Rep)
		{
			_Rep->DecWeakRefCount();
		}
	}

	void _Declref() {
		// decrement lifetime reference count
		if (_Rep) 
		{
			_Rep->DecLifetime();
		}
	}

private:

	element_type * _Ptr{ nullptr };
	LifetimedRefCountBase * _Rep{ nullptr };

	template<class _Ty0>
	friend class LifetimedPtrBase;

#if _HAS_STATIC_RTTI
	template<class _Dx,
		class _Ty0>
		friend _Dx * get_deleter(const LifetimedShared_Ptr<_Ty0>& _Sx) noexcept;
#endif /* _HAS_STATIC_RTTI */
};


template<class _Ty>
class LifetimedWeak_Ptr: public LifetimedPtrBase<_Ty>
{	// class for pointer to reference counted resource
public:
	constexpr LifetimedWeak_Ptr() noexcept
	{	// construct empty LifetimedWeak_Ptr object
	}

	LifetimedWeak_Ptr(const LifetimedWeak_Ptr& _Other) noexcept
	{	// construct LifetimedWeak_Ptr object for resource pointed to by _Other
		this->_Weakly_construct_from(_Other);
	}

	template<class _Ty2,
		std::enable_if_t<std::_SP_pointer_compatible<_Ty2, _Ty>::value, int> = 0>
		LifetimedWeak_Ptr(const LifetimedShared_Ptr<_Ty2>& _Other) noexcept
	{	// construct LifetimedWeak_Ptr object for resource owned by _Other
		this->_Weakly_construct_from(_Other);
	}

	template<class _Ty2,
		std::enable_if_t<std::_SP_pointer_compatible<_Ty2, _Ty>::value, int> = 0>
		LifetimedWeak_Ptr(const LifetimedWeak_Ptr<_Ty2>& _Other) noexcept
	{	// construct LifetimedWeak_Ptr object for resource pointed to by _Other
		this->_Weakly_construct_from(_Other.lock());
	}

	LifetimedWeak_Ptr(LifetimedWeak_Ptr&& _Other) noexcept
	{	// move construct from _Other
		this->_Move_construct_from(std::move(_Other));
	}

	template<class _Ty2,
		std::enable_if_t<std::_SP_pointer_compatible<_Ty2, _Ty>::value, int> = 0>
		LifetimedWeak_Ptr(LifetimedWeak_Ptr<_Ty2>&& _Other) noexcept
	{	// move construct from _Other
		this->_Weakly_construct_from(_Other.lock());
		_Other.reset();
	}

	~LifetimedWeak_Ptr() noexcept
	{	// release resource
		this->_Decwref();
	}

	LifetimedWeak_Ptr& operator=(const LifetimedWeak_Ptr& _Right) noexcept
	{	// assign from _Right
		LifetimedWeak_Ptr(_Right).swap(*this);
		return (*this);
	}

	template<class _Ty2>
	LifetimedWeak_Ptr& operator=(const LifetimedWeak_Ptr<_Ty2>& _Right) noexcept
	{	// assign from _Right
		LifetimedWeak_Ptr(_Right).swap(*this);
		return (*this);
	}

	LifetimedWeak_Ptr& operator=(LifetimedWeak_Ptr&& _Right) noexcept
	{	// move assign from _Right
		LifetimedWeak_Ptr(std::move(_Right)).swap(*this);
		return (*this);
	}

	template<class _Ty2>
	LifetimedWeak_Ptr& operator=(LifetimedWeak_Ptr<_Ty2>&& _Right) noexcept
	{	// move assign from _Right
		LifetimedWeak_Ptr(std::move(_Right)).swap(*this);
		return (*this);
	}

	template<class _Ty2>
	LifetimedWeak_Ptr& operator=(const LifetimedShared_Ptr<_Ty2>& _Right) noexcept
	{	// assign from _Right
		LifetimedWeak_Ptr(_Right).swap(*this);
		return (*this);
	}

	void reset() noexcept
	{	// release resource, convert to null LifetimedWeak_Ptr object
		LifetimedWeak_Ptr().swap(*this);
	}

	void swap(LifetimedWeak_Ptr& _Other) noexcept
	{	// swap pointers
		this->_Swap(_Other);
	}

	_NODISCARD bool expired() const noexcept
	{	// return true if resource no longer exists
		return (this->use_count() == 0);
	}

	_NODISCARD LifetimedShared_Ptr<_Ty> lock() const noexcept
	{	// convert to shared_ptr
		LifetimedShared_Ptr<_Ty> _Ret;
		(void)_Ret._Construct_from_weak(*this);
		return (_Ret);
	}
};

template<class _Ty>
class LifetimedShared_Ptr : public LifetimedPtrBase<_Ty>
{

public:

protected:

private:
	using _MyBase = LifetimedPtrBase<_Ty>;
	using typename _MyBase::element_type;
public:

#if _HAS_CXX17
	using weak_type = LifetimedWeak_Ptr<_Ty>;
#endif /* _HAS_CXX17 */

	constexpr LifetimedShared_Ptr() noexcept 
	{

	}

	constexpr LifetimedShared_Ptr(nullptr_t) noexcept
	{

	}

	template<class _Ux,
		std::enable_if_t<std::conjunction_v<std::conditional_t<std::is_array_v<_Ty>, std::_Can_array_delete<_Ux>, std::_Can_scalar_delete<_Ux>>,
		std::_SP_convertible<_Ux, _Ty>>, int> = 0>
		explicit LifetimedShared_Ptr(_Ux * _Px)
	{
		_Setp(_Px, std::is_array<_Ty>{});
	}

	template<class _Ux,
		class _Dx,
		std::enable_if_t<std::conjunction_v<std::is_move_constructible<_Dx>,
		std::_Can_call_function_object<_Dx&, _Ux *&>,
		std::_SP_convertible<_Ux, _Ty>>, int> = 0>
		LifetimedShared_Ptr(_Ux * _Px, _Dx _Dt)
	{	// construct with _Px, deleter
		_Setpd(_Px, std::move(_Dt));
	}

	template<class _Ux,
		class _Dx,
		class _Alloc,
		std::enable_if_t<std::conjunction_v<std::is_move_constructible<_Dx>,
		std::_Can_call_function_object<_Dx&, _Ux *&>,
		std::_SP_convertible<_Ux, _Ty>>, int> = 0>
		LifetimedShared_Ptr(_Ux * _Px, _Dx _Dt, _Alloc _Ax)
	{	// construct with _Px, deleter, allocator
		_Setpda(_Px, std::move(_Dt), _Ax);
	}

	template<class _Dx,
		std::enable_if_t<std::conjunction_v<std::is_move_constructible<_Dx>,
		std::_Can_call_function_object<_Dx&, nullptr_t&>>, int> = 0>
		LifetimedShared_Ptr(nullptr_t, _Dx _Dt)
	{	// construct with nullptr, deleter
		_Setpd(nullptr, std::move(_Dt));
	}

	template<class _Dx,
		class _Alloc,
		std::enable_if_t<std::conjunction_v<std::is_move_constructible<_Dx>,
		std::_Can_call_function_object<_Dx&, nullptr_t&>>, int> = 0>
		LifetimedShared_Ptr(nullptr_t, _Dx _Dt, _Alloc _Ax)
	{	// construct with nullptr, deleter, allocator
		_Setpda(nullptr, std::move(_Dt), _Ax);
	}

	template<class _Ty2>
	LifetimedShared_Ptr(const LifetimedShared_Ptr<_Ty2>& _Right, element_type * _Px) noexcept
	{	// construct shared_ptr object that aliases _Right
		this->_Alias_construct_from(_Right, _Px);
	}

	LifetimedShared_Ptr(const LifetimedShared_Ptr& _Other) noexcept
	{	// construct shared_ptr object that owns same resource as _Other
		this->_Copy_construct_from(_Other);
	}

	template<class _Ty2,
		std::enable_if_t<std::_SP_pointer_compatible<_Ty2, _Ty>::value, int> = 0>
		LifetimedShared_Ptr(const LifetimedShared_Ptr<_Ty2>& _Other) noexcept
	{	// construct shared_ptr object that owns same resource as _Other
		this->_Copy_construct_from(_Other);
	}

	LifetimedShared_Ptr(LifetimedShared_Ptr&& _Right) noexcept
	{	// construct shared_ptr object that takes resource from _Right
		this->_Move_construct_from(std::move(_Right));
	}

	template<class _Ty2,
		std::enable_if_t<std::_SP_pointer_compatible<_Ty2, _Ty>::value, int> = 0>
		LifetimedShared_Ptr(LifetimedShared_Ptr<_Ty2>&& _Right) noexcept
	{	// construct shared_ptr object that takes resource from _Right
		this->_Move_construct_from(std::move(_Right));
	}

#if _HAS_AUTO_PTR_ETC
	template<class _Ty2,
		std::enable_if_t<std::is_convertible_v<_Ty2 *, _Ty *>, int> = 0>
		LifetimedShared_Ptr(std::auto_ptr<_Ty2>&& _Other)
	{	// construct shared_ptr object that owns *_Other.get()
		_Ty2 * _Px = _Other.get();
		this->_Set_ptr_rep_and_enable_shared(_Px, new LifetimedRefCount<_Ty2>(_Px));
		_Other.release();
	}
#endif /* _HAS_AUTO_PTR_ETC */

	template<class _Ux,class _Dx, std::enable_if_t<std::conjunction_v<std::_SP_pointer_compatible<_Ux, _Ty>,
		std::is_convertible<typename std::unique_ptr<_Ux, _Dx>::pointer, element_type *>>, int> = 0>
		LifetimedShared_Ptr(std::unique_ptr<_Ux, _Dx>&& _Other)
	{	// construct from unique_ptr
		using _Fancy_t = typename std::unique_ptr<_Ux, _Dx>::pointer;
		using _Raw_t = typename std::unique_ptr<_Ux, _Dx>::element_type *;
		using _Deleter_t = std::conditional_t<std::is_reference_v<_Dx>, decltype(std::ref(_Other.get_deleter())), _Dx>;

		const _Fancy_t _Fancy = _Other.get();

		if (_Fancy)
		{
			const _Raw_t _Raw = _Fancy;
			const auto _Rx = new std::_Ref_count_resource<_Fancy_t, _Deleter_t>(_Fancy, _Other.get_deleter());
			_Set_ptr_rep_and_enable_shared(_Raw, _Rx);
			_Other.release();
		}
	}

	~LifetimedShared_Ptr() noexcept
	{	// release resource
		this->_Decref();
	}

	LifetimedShared_Ptr& operator=(const LifetimedShared_Ptr& _Right) noexcept
	{	// assign shared ownership of resource owned by _Right
		LifetimedShared_Ptr(_Right).swap(*this);
		return (*this);
	}

	template<class _Ty2>
	LifetimedShared_Ptr& operator=(const LifetimedShared_Ptr<_Ty2>& _Right) noexcept
	{	// assign shared ownership of resource owned by _Right
		LifetimedShared_Ptr(_Right).swap(*this);
		return (*this);
	}

	LifetimedShared_Ptr& operator=(LifetimedShared_Ptr&& _Right) noexcept
	{	// take resource from _Right
		LifetimedShared_Ptr(std::move(_Right)).swap(*this);
		return (*this);
	}

	template<class _Ty2>
	LifetimedShared_Ptr& operator=(LifetimedShared_Ptr<_Ty2>&& _Right) noexcept
	{	// take resource from _Right
		LifetimedShared_Ptr(std::move(_Right)).swap(*this);
		return (*this);
	}

#if _HAS_AUTO_PTR_ETC
	template<class _Ty2>
	LifetimedShared_Ptr& operator=(std::auto_ptr<_Ty2>&& _Right)
	{	// assign ownership of resource pointed to by _Right
		LifetimedShared_Ptr(std::move(_Right)).swap(*this);
		return (*this);
	}
#endif /* _HAS_AUTO_PTR_ETC */

	template<class _Ux,
		class _Dx>
		LifetimedShared_Ptr& operator=(std::unique_ptr<_Ux, _Dx>&& _Right)
	{	// move from unique_ptr
		LifetimedShared_Ptr(std::move(_Right)).swap(*this);
		return (*this);
	}

	void swap(LifetimedShared_Ptr& _Other) noexcept
	{	// swap pointers
		this->_Swap(_Other);
	}

	void reset() noexcept
	{	// release resource and convert to empty shared_ptr object
		LifetimedShared_Ptr().swap(*this);
	}

	template<class _Ux>
	void reset(_Ux * _Px)
	{	// release, take ownership of _Px
		LifetimedShared_Ptr(_Px).swap(*this);
	}

	template<class _Ux,
		class _Dx>
		void reset(_Ux * _Px, _Dx _Dt)
	{	// release, take ownership of _Px, with deleter _Dt
		LifetimedShared_Ptr(_Px, _Dt).swap(*this);
	}

	template<class _Ux,
		class _Dx,
		class _Alloc>
		void reset(_Ux * _Px, _Dx _Dt, _Alloc _Ax)
	{	// release, take ownership of _Px, with deleter _Dt, allocator _Ax
		LifetimedShared_Ptr(_Px, _Dt, _Ax).swap(*this);
	}

	using _MyBase::get;

	template<class _Ty2 = _Ty,
		std::enable_if_t<!std::disjunction_v<std::is_array<_Ty2>, std::is_void<_Ty2>>, int> = 0>
		_NODISCARD _Ty2& operator*() const noexcept
	{	// return reference to resource
		return (*get());
	}

	template<class _Ty2 = _Ty,
		std::enable_if_t<!std::is_array_v<_Ty2>, int> = 0>
		_NODISCARD _Ty2 * operator->() const noexcept
	{	// return pointer to resource
		return (get());
	}

	template<class _Ty2 = _Ty,
		class _Elem = element_type,
		std::enable_if_t<std::is_array_v<_Ty2>, int> = 0>
		_NODISCARD _Elem& operator[](ptrdiff_t _Idx) const
	{	// subscript
		return (get()[_Idx]);
	}

	_NODISCARD _CXX17_DEPRECATE_SHARED_PTR_UNIQUE bool unique() const noexcept
	{	// return true if no other shared_ptr object owns this resource
		return (this->use_count() == 1);
	}

	explicit operator bool() const noexcept
	{	// test if shared_ptr object owns a resource
		return (get() != nullptr);
	}

private:

	template<class _Ux>
	void _Setp(_Ux * _Px, std::true_type)
	{	// take ownership of _Px
		_Setpd(_Px, std::default_delete<_Ux[]>{});
	}

	template<class _Ux>
	void _Setp(_Ux * _Px, std::false_type)
	{	// take ownership of _Px
		_TRY_BEGIN	// allocate control block and set
			_Set_ptr_rep_and_enable_shared(_Px, new LifetimedRefCount<_Ux>(_Px));
		_CATCH_ALL	// allocation failed, delete resource
			delete _Px;
		_RERAISE;
		_CATCH_END
	}

	template<class _UxptrOrNullptr,
		class _Dx>
		void _Setpd(_UxptrOrNullptr _Px, _Dx _Dt)
	{	// take ownership of _Px, deleter _Dt
		_TRY_BEGIN	// allocate control block and set
			_Set_ptr_rep_and_enable_shared(_Px, new std::_Ref_count_resource<_UxptrOrNullptr, _Dx>(_Px, _STD move(_Dt)));
		_CATCH_ALL	// allocation failed, delete resource
			_Dt(_Px);
		_RERAISE;
		_CATCH_END
	}

	template<class _UxptrOrNullptr,
		class _Dx,
		class _Alloc>
		void _Setpda(_UxptrOrNullptr _Px, _Dx _Dt, _Alloc _Ax)
	{	// take ownership of _Px, deleter _Dt, allocator _Ax
		using _Refd = std::_Ref_count_resource_alloc<_UxptrOrNullptr, _Dx, _Alloc>;
		using _Alref_alloc = std::_Rebind_alloc_t<_Alloc, _Refd>;
		using _Alref_traits = std::allocator_traits<_Alref_alloc>;
		_Alref_alloc _Alref(_Ax);

		_TRY_BEGIN	// allocate control block and set
		const auto _Pfancy = _Alref_traits::allocate(_Alref, 1);
		_Refd * const _Pref = _Unfancy(_Pfancy);
			_TRY_BEGIN
			_Alref_traits::construct(_Alref, _Pref, _Px, _STD move(_Dt), _Ax);
			_Set_ptr_rep_and_enable_shared(_Px, _Pref);
			_CATCH_ALL
			_Alref_traits::deallocate(_Alref, _Pfancy, 1);
			_RERAISE;
			_CATCH_END
		_CATCH_ALL	// allocation failed, delete resource
		_Dt(_Px);
		_RERAISE;
		_CATCH_END
	}

	template<class _Ty0,
		class... _Types>
		friend LifetimedShared_Ptr<_Ty0> lifetimed_make_shared(_Types&&... _Args);

	template<class _Ty0,
		class _Alloc,
		class... _Types>
		friend LifetimedShared_Ptr<_Ty0> lifetimed_allocate_shared(const _Alloc& _Al_arg, _Types&&... _Args);

	template<class _Ux>
	void _Set_ptr_rep_and_enable_shared(_Ux * _Px, LifetimedRefCountBase * _Rx)
	{	// take ownership of _Px
		this->_Set_ptr_rep(_Px, _Rx);
		_Enable_lifetimed_shared_from_this(*this, _Px);
	}

	void _Set_ptr_rep_and_enable_shared(nullptr_t, LifetimedRefCountBase * _Rx)
	{	// take ownership of nullptr
		this->_Set_ptr_rep(nullptr, _Rx);
	}

};

// FUNCTION TEMPLATE make_shared
template<class _Ty,
	class... _Types>
	_NODISCARD inline LifetimedShared_Ptr<_Ty> lifetimed_make_shared(_Types&&... _Args)
{	// make a shared_ptr
	const auto _Rx = new LifetimedRefCountObj<_Ty>(std::forward<_Types>(_Args)...);

	LifetimedShared_Ptr<_Ty> _Ret;
	_Ret._Set_ptr_rep_and_enable_shared(_Rx->_Getptr(), _Rx);
	return (_Ret);
}

// FUNCTION TEMPLATE allocate_shared
template<class _Ty,
	class _Alloc,
	class... _Types>
	_NODISCARD inline LifetimedShared_Ptr<_Ty> lifetimed_allocate_shared(const _Alloc& _Al_arg, _Types&&... _Args)
{	// make a shared_ptr
	using _Refoa = LifetimedRefCountObjAlloc<_Ty, _Alloc>;
	using _Alref_alloc = std::_Rebind_alloc_t<_Alloc, _Refoa>;
	using _Alref_traits = std::allocator_traits<_Alref_alloc>;
	_Alref_alloc _Alref(_Al_arg);

	const auto _Rx = _Alref_traits::allocate(_Alref, 1);

	_TRY_BEGIN
		_Alref_traits::construct(_Alref, std::_Unfancy(_Rx), _Al_arg, std::forward<_Types>(_Args)...);
	_CATCH_ALL
		_Alref_traits::deallocate(_Alref, _Rx, 1);
	_RERAISE;
	_CATCH_END

	LifetimedShared_Ptr<_Ty> _Ret;
	_Ret._Set_ptr_rep_and_enable_shared(_Rx->_Getptr(), std::_Unfancy(_Rx));
	return (_Ret);
}

#if _HAS_STATIC_RTTI
template<class _Dx,
	class _Ty>
	_NODISCARD _Dx * get_deleter(const LifetimedShared_Ptr<_Ty>& _Sx) noexcept
{	// return pointer to shared_ptr's deleter object if its type is _Dx
	if (_Sx._Rep)
	{
		return (static_cast<_Dx *>(_Sx._Rep->_Get_deleter(typeid(_Dx))));
	}

	return (nullptr);
}
#else /* _HAS_STATIC_RTTI */
template<class _Dx,
	class _Ty>
	_Dx * get_deleter(const LifetimedShared_Ptr<_Ty>&) noexcept = delete;	// requires static RTTI
#endif /* _HAS_STATIC_RTTI */

#if _HAS_CXX17
template<class _Ty>
LifetimedWeak_Ptr(LifetimedShared_Ptr<_Ty>)->LifetimedWeak_Ptr<_Ty>;
#endif /* _HAS_CXX17 */

template<class _Ty>
void swap(LifetimedWeak_Ptr<_Ty>& _Left, LifetimedWeak_Ptr<_Ty>& _Right) noexcept
{	// swap contents of _Left and _Right
	_Left.swap(_Right);
}

// CLASS TEMPLATE enable_shared_from_this
template<class _Ty>
class enable_lifetimed_shared_from_this
{	// provide member functions that create shared_ptr to this
public:
	using _Esft_type = enable_lifetimed_shared_from_this;

	_NODISCARD LifetimedShared_Ptr<_Ty> shared_from_this()
	{	// return shared_ptr
		return (LifetimedShared_Ptr<_Ty>(_Wptr));
	}

	_NODISCARD LifetimedShared_Ptr<const _Ty> shared_from_this() const
	{	// return shared_ptr
		return (LifetimedShared_Ptr<const _Ty>(_Wptr));
	}

	_NODISCARD LifetimedWeak_Ptr<_Ty> weak_from_this() noexcept
	{	// return weak_ptr
		return (_Wptr);
	}

	_NODISCARD LifetimedWeak_Ptr<const _Ty> weak_from_this() const noexcept
	{	// return weak_ptr
		return (_Wptr);
	}

protected:
	constexpr enable_lifetimed_shared_from_this() noexcept
		: _Wptr()
	{	// construct
	}

	enable_lifetimed_shared_from_this(const enable_lifetimed_shared_from_this&) noexcept
		: _Wptr()
	{	// construct (must value-initialize _Wptr)
	}

	enable_lifetimed_shared_from_this& operator=(const enable_lifetimed_shared_from_this&) noexcept
	{	// assign (must not change _Wptr)
		return (*this);
	}

	~enable_lifetimed_shared_from_this() = default;

private:
	template<class _Other,
		class _Yty>
		friend void _Enable_lifetimed_shared_from_this1(const LifetimedShared_Ptr<_Other>& _This, _Yty * _Ptr, std::true_type);

	mutable LifetimedWeak_Ptr<_Ty> _Wptr;
};

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)


