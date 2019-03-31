#pragma once
#include <utility>


namespace esc {


/// Stores a non-copyable handle of type T that will be destructed using TDestructor if it is not equal to TDefault.
template <typename T, void(*TDestructor)(T), T TDefault>
class UniqueHandle
{
public:
	using HandleType = T;
	static const T DefaultValue = TDefault;

	UniqueHandle();
	explicit UniqueHandle(T handle);
	~UniqueHandle();
	UniqueHandle(UniqueHandle&& rhs) noexcept;
	UniqueHandle& operator=(UniqueHandle&& rhs) noexcept;
	UniqueHandle(const UniqueHandle&) = delete;
	UniqueHandle& operator=(const UniqueHandle&) = delete;
	
	explicit operator bool() const;
	bool operator==(const UniqueHandle &rhs) const;
	bool operator!=(const UniqueHandle &rhs) const;

	/// Returns true if the handle is not equal to TDefault.
	bool isValid() const;

	T& get();
	const T& get() const;

	void reset(T handle = TDefault);
	T release();

private:
	T _handle;

};


}


template <typename T, void(*TDestructor)(T), T TDefault>
esc::UniqueHandle<T, TDestructor, TDefault>::UniqueHandle() :
	_handle(TDefault)
{

}

template <typename T, void(*TDestructor)(T), T TDefault>
esc::UniqueHandle<T, TDestructor, TDefault>::UniqueHandle(T handle) :
	_handle(handle)
{

}

template <typename T, void(*TDestructor)(T), T TDefault>
esc::UniqueHandle<T, TDestructor, TDefault>::~UniqueHandle()
{
	if (_handle != TDefault)
		TDestructor(_handle);
}

template <typename T, void(*TDestructor)(T), T TDefault>
esc::UniqueHandle<T, TDestructor, TDefault>::UniqueHandle(UniqueHandle&& other) noexcept :
_handle(other.release())
{

}

template <typename T, void(*TDestructor)(T), T TDefault>
esc::UniqueHandle<T, TDestructor, TDefault>& esc::UniqueHandle<T, TDestructor, TDefault>::operator=(UniqueHandle&& rhs) noexcept
{
	reset(rhs.release());
	return *this;
}


template <typename T, void(* TDestructor)(T), T TDefault>
esc::UniqueHandle<T, TDestructor, TDefault>::operator bool() const
{
	return isValid();
}


template <typename T, void(* TDestructor)(T), T TDefault>
bool esc::UniqueHandle<T, TDestructor, TDefault>::operator==(const UniqueHandle& rhs) const
{
	return _handle == rhs._handle;
}


template <typename T, void(* TDestructor)(T), T TDefault>
bool esc::UniqueHandle<T, TDestructor, TDefault>::operator!=(const UniqueHandle& rhs) const
{
	return !(*this == rhs);
}


template <typename T, void(*TDestructor)(T), T TDefault>
bool esc::UniqueHandle<T, TDestructor, TDefault>::isValid() const
{
	return _handle != TDefault;
}


template <typename T, void(*TDestructor)(T), T TDefault>
void esc::UniqueHandle<T, TDestructor, TDefault>::reset(T handle)
{
	using std::swap;

	swap(_handle, handle);

	if (_handle != handle && handle != TDefault)
		TDestructor(handle);
}

template <typename T, void(*TDestructor)(T), T TDefault>
T esc::UniqueHandle<T, TDestructor, TDefault>::release()
{
	auto releasedHandle = _handle;
	_handle = TDefault;
	return releasedHandle;
}

template <typename T, void(*TDestructor)(T), T TDefault>
T& esc::UniqueHandle<T, TDestructor, TDefault>::get()
{
	return _handle;
}

template <typename T, void(*TDestructor)(T), T TDefault>
const T& esc::UniqueHandle<T, TDestructor, TDefault>::get() const
{
	return _handle;
}

