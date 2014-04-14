/*!
  \copyright (c) RDO-Team, 2011
  \file      intrusive_ptr.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.06.2010
  \brief
  \indent    4T
*/

#ifndef _UTILS_SMART_PTR_INTRUSIVE_PTR_H_
#define _UTILS_SMART_PTR_INTRUSIVE_PTR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdomacros.h"
#include "utils/src/smart_ptr/ref_counter/counter_reference.h"
#include "utils/src/smart_ptr/interface_ptr/interface_ptr.h"
#include "utils/src/smart_ptr/ref_counter/ref_counter_i.h"
// --------------------------------------------------------------------------------

namespace rdo {

template<class T>
class intrusive_ptr
{
template <typename P> friend class intrusive_ptr;
template <class W>    friend class smart_ptr_wrapper;
public:
	typedef T                object_type;
	typedef intrusive_ptr<T> this_type;

	 intrusive_ptr();
	 intrusive_ptr(PTR (T)         object);
	 intrusive_ptr(CREF(this_type) sptr  );
	 template <class P>
	 intrusive_ptr(CREF(interface_ptr<P>) pInterface);
	~intrusive_ptr();

	REF(this_type) operator= (CREF(this_type) sptr);

	//! Сравнивает по указателям
	template <class P>
	bool operator==(CREF(intrusive_ptr<P>) sptr) const;
	template <class P>
	bool operator!=(CREF(intrusive_ptr<P>) sptr) const;

	//! Сравнивает по значениям
	template<class P>
	bool compare(CREF(intrusive_ptr<P>) sptr) const;

	operator bool() const;
	 PTR(T) operator->() const;
	 PTR(T) operator->();
	CREF(T) operator*() const;
	 REF(T) operator*();

	template <class P>
	operator intrusive_ptr<P>() const;

	template <class P>
	intrusive_ptr<P> object_parent_cast() const;

	template <class P>
	intrusive_ptr<P> object_static_cast() const;

	template <class P>
	intrusive_ptr<P> object_dynamic_cast() const;

	template <class P>
	interface_ptr<P> interface_cast() const;

	template <class P>
	interface_ptr<P> interface_dynamic_cast() const;

	/// @todo переместить в protected
	PTR(T) get();
	const T* get() const;

	bool owner() const;

protected:
	void addref();
	void release();

private:
	PTR(T) m_object;

	REF(std::size_t) counter();
};

#define DECLARE_POINTER(TYPE)    typedef rdo::intrusive_ptr<TYPE> LP##TYPE;
#define PREDECLARE_POINTER(TYPE) class TYPE; DECLARE_POINTER(TYPE);

} // namespace rdo

#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.inl"

#endif // _UTILS_SMART_PTR_INTRUSIVE_PTR_H_
