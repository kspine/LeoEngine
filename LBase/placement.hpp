#ifndef LBase_placement_hpp
#define LBase_placement_hpp 1

#include "LBase/addressof.hpp"
#include "LBase/deref_op.hpp"
#include "LBase/cassert.h"

#include <new>
#include <iterator>

namespace leo {

	/*!
	\brief Ĭ�ϳ�ʼ����ǡ�
	*/
	lconstexpr const struct default_init_t {} default_init{};

	/*!
	\brief ֵ��ʼ����ǡ�
	*/
	lconstexpr const struct value_init_t {} value_init{};


	/*!
	\see WG21 P0032R3 ��
	\see WG21 N4606 20.2.7[utility.inplace] ��
	*/
	//@{
	//! \brief ԭ�ر�����͡�
	struct in_place_tag
	{
		in_place_tag() = delete;
	};

	//! \brief ԭ�ؿձ�����͡�
	using in_place_t = in_place_tag(&)(limpl(empty_base<>));

	//! \brief ԭ�����ͱ��ģ�塣
	template<typename _type>
	using in_place_type_t = in_place_tag(&)(limpl(empty_base<_type>));

	//! \brief ԭ���������ģ�塣
	template<size_t _vIdx>
	using in_place_index_t = in_place_tag(&)(limpl(size_t_<_vIdx>));

#ifdef LB_IMPL_MSCPP
#pragma warning(disable:4646)
#endif

	/*!
	\ingroup helper_functions
	\brief ԭ�ر�Ǻ�����
	\warning ��������δ������Ϊ��
	*/
	limpl(LB_NORETURN) inline in_place_tag
		in_place(limpl(empty_base<>))
	{
		LB_ASSUME(false);
	}
	template<typename _type>
	limpl(LB_NORETURN) in_place_tag
		in_place(limpl(empty_base<_type>))
	{
		LB_ASSUME(false);
	}
	template<size_t _vIdx>
	limpl(LB_NORETURN) in_place_tag
		in_place(limpl(size_t_<_vIdx>))
	{
		LB_ASSUME(false);
	}
	//@}


	/*!
	\tparam _type ����Ķ������͡�
	\param obj ����Ĵ洢����
	*/
	//@{
	//! \brief ��Ĭ�ϳ�ʼ���ڶ����й��졣
	template<typename _type, typename _tObj>
	inline _type*
		construct_default_within(_tObj& obj)
	{
		return ::new(static_cast<void*>(static_cast<_tObj*>(
			constfn_addressof(obj)))) _type;
	}

	/*!
	\brief ��ֵ��ʼ���ڶ����й��졣
	\tparam _tParams ���ڹ������Ĳ��������͡�
	\param args ���ڹ������Ĳ�������
	*/
	template<typename _type, typename _tObj, typename... _tParams>
	inline _type*
		construct_within(_tObj& obj, _tParams&&... args)
	{
		return ::new(static_cast<void*>(static_cast<_tObj*>(
			constfn_addressof(obj)))) _type(lforward(args)...);
	}
	//@}

	/*!
	\brief ��Ĭ�ϳ�ʼ��ԭ�ع��졣
	\tparam _tParams ���ڹ������Ĳ��������͡�
	\param args ���ڹ������Ĳ�������
	*/
	template<typename _type>
	inline void
		construct_default_in(_type& obj)
	{
		construct_default_within<_type>(obj);
	}

	/*!
	\brief ��ֵ��ʼ��ԭ�ع��졣
	\tparam _tParams ���ڹ������Ĳ��������͡�
	\param args ���ڹ������Ĳ�������
	*/
	template<typename _type, typename... _tParams>
	inline void
		construct_in(_type& obj, _tParams&&... args)
	{
		construct_within<_type>(obj, yforward(args)...);
	}

	//@{
	//! \tparam _tIter ���������͡�
	//@{
	/*!
	\tparam _tParams ���ڹ������Ĳ��������͡�
	\param args ���ڹ������Ĳ�������
	\pre ���ԣ�ָ����Χĩβ����ĵ��������� <tt>!is_undereferenceable</tt> ��
	*/
	//@{
	/*!
	\brief ʹ��ָ�������ڵ�����ָ����λ����ָ��������ʼ���������
	\param i ��������
	\note ��ʽת��Ϊ void* ָ����ʵ�ֱ�׼���㷨 uninitialized_* ʵ�����Ƶ����塣
	\see libstdc++ 5 �� Microsoft VC++ 2013 ��׼���������ռ� std �ڶ�ָ�����͵�ʵ�֣�
	_Construct ģ�塣
	*/
	template<typename _tIter, typename... _tParams>
	void
		construct(_tIter i, _tParams&&... args)
	{
		using value_type = typename std::iterator_traits<_tIter>::value_type;

		lconstraint(!is_undereferenceable(i));
		construct_within<value_type>(*i, yforward(args)...);
	}

	/*!
	\brief ʹ��ָ�������ڵ�����ָ����λ����Ĭ�ϳ�ʼ���������
	\param i ��������
	*/
	template<typename _tIter>
	void
		construct_default(_tIter i)
	{
		using value_type = typename std::iterator_traits<_tIter>::value_type;

		lconstraint(!is_undereferenceable(i));
		construct_default_within<value_type>(*i);
	}

	/*!
	\brief ʹ��ָ���Ĳ����ظ������������Χ�ڵĶ������С�
	\note ���������ݵĴ����͹���Ķ�������ͬ��
	*/
	template<typename _tIter, typename... _tParams>
	void
		construct_range(_tIter first, _tIter last, _tParams&&... args)
	{
		for (; first != last; ++first)
			construct(first, yforward(args)...);
	}
	//@}


	/*!
	\brief ԭ�����١�
	\see WG21 N4606 20.10.10.7[specialized.destroy] ��
	*/
	//@{
	/*!
	\see libstdc++ 5 �� Microsoft VC++ 2013 ��׼���������ռ� std �ڶ�ָ�����͵�ʵ�֣�
	_Destroy ģ�塣
	*/
	template<typename _type>
	inline void
		destroy_at(_type* location)
	{
		lconstraint(location);
		location->~_type();
	}

	//! \see libstdc++ 5 ��׼���������ռ� std �ڶԵ�������Χ��ʵ�֣� _Destroy ģ�塣
	template<typename _tFwd>
	inline void
		destroy(_tFwd first, _tFwd last)
	{
		for (; first != last; ++first)
			destroy_at(std::addressof(*first));
	}

	template<typename _tFwd, typename _tSize>
	inline _tFwd
		destroy_n(_tFwd first, _tSize n)
	{
		// XXX: To reduce dependency on resolution of LWG 2598.
		static_assert(is_lvalue_reference<decltype(*first)>(),
			"Invalid iterator reference type found.");

		// XXX: Excessive order refinment by ','?
		for (; n > 0; static_cast<void>(++first), --n)
			destroy_at(std::addressof(*first));
		return first;
	}
	//@}
	//@}


	/*!
	\brief ԭ��������
	\tparam _type ����������������͡�
	\param obj �����Ķ���
	\sa destroy_at
	*/
	template<typename _type>
	inline void
		destruct_in(_type& obj)
	{
		obj.~_type();
	}

	/*!
	\brief ����������ָ��Ķ���
	\param i ��������
	\pre ���ԣ�<tt>!is_undereferenceable(i)</tt> ��
	\sa destroy
	*/
	template<typename _tIter>
	void
		destruct(_tIter i)
	{
		using value_type = typename std::iterator_traits<_tIter>::value_type;

		lconstraint(!is_undereferenceable(i));
		destruct_in<value_type>(*i);
	}

	/*!
	\brief ����d��������Χ�ڵĶ������С�
	\note ��֤˳��������
	\sa destroy
	*/
	template<typename _tIter>
	void
		destruct_range(_tIter first, _tIter last)
	{
		for (; first != last; ++first)
			destruct(first);
	}
	//@}
}

#endif
