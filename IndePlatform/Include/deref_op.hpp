#ifndef IndePlatform_deref_op_hpp
#define IndePlatform_deref_op_hpp


#include "ldef.h"

namespace std
{
	template<typename _type>
	struct atomic;
}

namespace leo {
	/*!	\defgroup is_undereferenceable Is Undereferenceable Iterator
	\brief �жϵ�����ʵ���Ƿ񲻿ɽ����á�
	\tparam _tIter ���������͡�
	\note ע�ⷵ�� \c false ����ʾ����ʵ�ʿɽ����á�
	*/
	//@{
	template<typename _tIter>
	lconstfn bool
		is_undereferenceable(const _tIter&) lnothrow
	{
		return{};
	}
	template<typename _type>
	lconstfn bool
		is_undereferenceable(_type* p) lnothrow
	{
		return !p;
	}
	template<typename _type>
	lconstfn bool
		is_undereferenceable(const std::atomic<_type>&) = delete;
	template<typename _type>
	lconstfn bool
		is_undereferenceable(const volatile std::atomic<_type>&) = delete;
	//@}
}

#endif