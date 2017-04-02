/*! \file deref_op.hpp
\ingroup LBase
\brief �����ò�����
\par �޸�ʱ��:
2016-12-10 01:51 +0800
*/

#ifndef LBase_deref_op_hpp
#define LBase_deref_op_hpp 1

#include "LBase/type_traits.hpp"

using stdex::nullptr_t;

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
	//@}


	//! \since build 1.4
	//@{
	//! \brief ȡ�ǿ����û�Ĭ��ֵ��
	//@{
	template<typename _type>
	lconstfn auto
		nonnull_or(_type&& p) -> decay_t<decltype(lforward(p))>
	{
		return p ? lforward(p) : decay_t<decltype(lforward(p))>();
	}
	template<typename _tOther, typename _type>
	lconstfn auto
		nonnull_or(_type&& p, _tOther&& other)
		->limpl(decltype(p ? lforward(p) : lforward(other)))
	{
		return p ? lforward(p) : lforward(other);
	}
	template<typename _tOther, typename _type, typename _tSentinal = nullptr_t>
	lconstfn auto
		nonnull_or(_type&& p, _tOther&& other, _tSentinal&& last)->limpl(
			decltype(!bool(p == lforward(last)) ? lforward(p) : lforward(other)))
	{
		return !bool(p == lforward(last)) ? lforward(p) : lforward(other);
	}
	//@}

	/*!
	\brief ���÷����û�Ĭ��ֵ��
	*/
	//@{
	template<typename _type, typename _func>
	lconstfn auto
		call_nonnull_or(_func f, _type&& p) -> decay_t<decltype(f(lforward(p)))>
	{
		return p ? f(lforward(p)) : decay_t<decltype(f(lforward(p)))>();
	}
	template<typename _tOther, typename _type, typename _func>
	lconstfn auto
		call_nonnull_or(_func f, _type&& p, _tOther&& other)
		->limpl(decltype(p ? f(lforward(p)) : lforward(other)))
	{
		return p ? f(lforward(p)) : lforward(other);
	}
	template<typename _tOther, typename _type, typename _func,
		typename _tSentinal = nullptr_t>
		lconstfn auto
		call_nonnull_or(_func f, _type&& p, _tOther&& other, _tSentinal&& last)
		->limpl(
			decltype(!bool(p == lforward(last)) ? f(lforward(p)) : lforward(other)))
	{
		return !bool(p == lforward(last)) ? f(lforward(p)) : lforward(other);
	}
	//@}

	//! \brief ȡ�ǿ�ֵ��Ĭ��ֵ��
	//@{
	template<typename _type>
	lconstfn auto
		value_or(_type&& p) -> decay_t<decltype(*lforward(p))>
	{
		return p ? *lforward(p) : decay_t<decltype(*lforward(p))>();
	}
	template<typename _tOther, typename _type>
	lconstfn auto
		value_or(_type&& p, _tOther&& other)
		->limpl(decltype(p ? *lforward(p) : lforward(other)))
	{
		return p ? *lforward(p) : lforward(other);
	}
	template<typename _tOther, typename _type, typename _tSentinal = nullptr_t>
	lconstfn auto
		value_or(_type&& p, _tOther&& other, _tSentinal&& last)->limpl(
			decltype(!bool(p == lforward(last)) ? *lforward(p) : lforward(other)))
	{
		return !bool(p == lforward(last)) ? *lforward(p) : lforward(other);
	}
	//@}


	//! \brief ���÷ǿ�ֵ��ȡĬ��ֵ��
	//@{
	template<typename _type, typename _func>
	lconstfn auto
		call_value_or(_func f, _type&& p) -> decay_t<decltype(f(*lforward(p)))>
	{
		return p ? f(*lforward(p)) : decay_t<decltype(f(*lforward(p)))>();
	}
	template<typename _tOther, typename _type, typename _func>
	lconstfn auto
		call_value_or(_func f, _type&& p, _tOther&& other)
		->limpl(decltype(p ? f(*lforward(p)) : lforward(other)))
	{
		return p ? f(*lforward(p)) : lforward(other);
	}
	template<typename _tOther, typename _type, typename _func,
		typename _tSentinal = nullptr_t>
		lconstfn auto
		call_value_or(_func f, _type&& p, _tOther&& other, _tSentinal&& last)->limpl(
			decltype(!bool(p == lforward(last)) ? f(*lforward(p)) : lforward(other)))
	{
		return !bool(p == lforward(last)) ? f(*lforward(p)) : lforward(other);
	}
	//@}
	//@}
}

#endif