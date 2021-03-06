#ifndef SMALLOBJECTALLOCATOR_HPP_INCLUDED
#define SMALLOBJECTALLOCATOR_HPP_INCLUDED

#include "SmallObjectHeap.hpp"
#include <stddef.h>

namespace details{
  //  inline void destruct(char*){}
  //  inline void destruct(wchar_t*) {}
    template <typename T>
    inline void destruct(T* t) {t->~T();}
}
template <typename T>
class SmallObjectAllocator
{
public:
	typedef std::size_t	size_type;
	typedef ptrdiff_t	difference_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	typedef T&			reference;
	typedef const T&	const_reference;
	typedef T			value_type;

	template <class U>
	struct rebind { typedef SmallObjectAllocator<U> other; };

	SmallObjectAllocator() {}
	SmallObjectAllocator( const SmallObjectAllocator& ) {}

	template <class U>
	SmallObjectAllocator( const SmallObjectAllocator<U>& ) {}

	~SmallObjectAllocator() {}

	pointer address( reference x )				const { return &x; }
	const_pointer address( const_reference x )	const { return &x; }

	pointer allocate( size_type n, const_pointer = 0 )
	{
		return static_cast<pointer>(SmallObjectHeap<n>::getInstance()->allocate());
	}

	void deallocate( pointer p, size_type size )	{
        SmallObjectHeap<size>::getInstance()->deallocate(static_cast<void*>(p));
    }
	size_type max_size() const						{ return size_type( -1 ) / sizeof( T ); }

	void construct( pointer p, const value_type& x )
    {
		new(static_cast<void*>(p)) value_type(x);
	}



	void destroy( pointer p )
    {
        details::destruct(p);
    }

};

template<>
class SmallObjectAllocator<void>
{
	typedef void        value_type;
	typedef void*       pointer;
	typedef const void* const_pointer;

	template <class U> 
	struct rebind { typedef SmallObjectAllocator<U> other; };
};


template <typename T>
bool operator == ( const SmallObjectAllocator<T>&, const SmallObjectAllocator<T>& )
{
	return true;
}

template <typename T>
bool operator != ( const SmallObjectAllocator<T>&, const SmallObjectAllocator<T>& )
{
	return false;
}

#endif	// SMALLOBJECTALLOCATOR_HPP_INCLUDED
