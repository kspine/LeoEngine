/*!	\file ref.hpp
\ingroup LBase
\brief ���ð�װ��
��չ��׼��ͷ <functional> ���ṩ��� std::reference_wrapper �Ľӿڡ�
*/

#ifndef LBase_ref_hpp
#define LBase_ref_hpp 1

#include "LBase/addressof.hpp"// for "addressof.hpp", leo::constfn_addressof,
//	exclude_self_t, cond_t, not_, is_object;
#include <functional> // for std::reference_wrapper;

/*!
\brief \<functional\> ���Բ��Ժꡣ
\see WG21 P0096R1 3.5 ��
\see https://blogs.msdn.microsoft.com/vcblog/2015/06/19/c111417-features-in-vs-2015-rtm/ ��
\since build 1.4
*/
//@{
#if LB_IMPL_MSCPP >= 1800
#	ifndef __cpp_lib_transparent_operators
#		define __cpp_lib_transparent_operators 201210
#	endif
#endif
#if LB_IMPL_MSCPP >= 1900
#	ifndef __cpp_lib_invoke
#		define __cpp_lib_invoke 201411
#	endif
#endif
//@}


namespace leo
{
	//!
	//@{
	/*!
	\brief ��ֵ���ð�װ��
	\tparam _type ����װ�����͡�

	���� \c std::reference_wrapper �� \c boost::reference_wrapper �����ӿڼ��ݵ�
	���ð�װ��ʵ�֡�
	�� \c std::reference_wrapper ��ͬ���� \c boost::reference_wrapper ���ƣ�
	��Ҫ��ģ�����Ϊ�������͡�
	*/
	//@{
	template<typename _type>
	class lref
	{
	public:
		using type = _type;

	private:
		_type* ptr;

	public:
		lconstfn
			lref(_type& t) lnothrow
			: ptr(leo::addressof(t))
		{}
		lconstfn
			lref(std::reference_wrapper<_type> t) lnothrow
			: lref(t.get())
		{}

		//! \since build 556
		lref(const lref&) = default;

		//! \since build 556
		lref&
			operator=(const lref&) = default;

		operator _type&() const lnothrow
		{
			return *ptr;
		}

		_type&
			get() const lnothrow
		{
			return *ptr;
		}
	};

	/*!
	\brief �������ð�װ��
	\relates lref
	*/
	//@{
	template<typename _type>
	lconstfn lref<_type>
		ref(_type& t)
	{
		return lref<_type>(t);
	}
	template <class _type>
	void
		ref(const _type&&) = delete;

	template<typename _type>
	lconstfn lref<const _type>
		cref(const _type& t)
	{
		return lref<const _type>(t);
	}
	template<class _type>
	void
		cref(const _type&&) = delete;
	//@}
	//@}

	//! \since build 1.4
	//@{
	/*!
	\ingroup unary_type_traits
	\brief �ж�ģ�����ָ���������Ƿ�
	\note �ӿں������� boost::is_reference_wrapper ��
	*/
	//@{
	template<typename _type>
	struct is_reference_wrapper : false_type
	{};

	template<typename _type>
	struct is_reference_wrapper<std::reference_wrapper<_type>> : true_type
	{};

	template<typename _type>
	struct is_reference_wrapper<lref<_type>> : true_type
	{};
	//@}

	/*!
	\ingroup metafunctions
	\brief ȡ���ð�װ�����ͻ�δ����װ��ģ��������͡�
	\note �ӿں������� boost::unwrap_reference ��
	\since build 1.4
	*/
	//@{
	template<typename _type>
	struct unwrap_reference
	{
		using type = _type;
	};

	template<typename _type>
	using unwrap_reference_t = _t<unwrap_reference<_type>>;

	template<typename _type>
	struct unwrap_reference<std::reference_wrapper<_type>>
	{
		using type = _type;
	};

	template<typename _type>
	struct unwrap_reference<lref<_type>>
	{
		using type = _type;
	};
	//@}
	//@}

	/*!
	\brief ������ð�װ��
	\note Ĭ�Ͻ��ṩ�� \c std::reference_wrapper �� lref ��ʵ�����͵����ء�
	\note ʹ�� ADL ��
	*/
	//@{
	template<typename _type>
	lconstfn _type&
		unref(_type&& x) lnothrow
	{
		return x;
	}
	//! \since build 554
	template<typename _type>
	lconstfn _type&
		unref(const lref<_type>& x) lnothrow
	{
		return x.get();
	}
	//@}


	/*!
	\brief ��������������͡�
	\warning ����� cv-qualifier ��
	\todo ��ֵ���ð汾��
	*/
	class void_ref
	{
	private:
		const volatile void* ptr;

	public:
		template<typename _type>
		lconstfn
			void_ref(_type& obj)
			: ptr(&obj)
		{}

		template<typename _type>
		lconstfn LB_PURE
			operator _type&() const
		{
			return *static_cast<_type*>(&*this);
		}

		LB_PURE void*
			operator&() const volatile
		{
			return const_cast<void*>(ptr);
		}
	};

	/*!
	\brief α�������
	\note �������и�ֵ������
	\since build 1.4
	*/
	struct pseudo_output
	{
		//! \since build 690
		//@{
		template<typename... _tParams>
		lconstfn
			pseudo_output(_tParams&&...) lnothrow
		{}

		template<typename _tParam,
			limpl(exclude_self_t<pseudo_output, _tParam>)>
			lconstfn const pseudo_output&
			operator=(_tParam&&) const lnothrow
		{
			return *this;
		}
		template<typename... _tParams>
		lconstfn const pseudo_output&
			operator()(_tParams&&...) const lnothrow
		{
			return *this;
		}
		//@}
	};

	/*!
	\ingroup metafunctions
	\since build 1.4
	\see ������صĺ����������ԣ� WG21 P0146R0 ��
	*/
	//@{
	//! \brief �����Ͳ��ǿ�������ȡ�󱸽�����ͣ�Ĭ��Ϊ pseudo_output ����
	template<typename _type, typename _tRes = pseudo_output>
	using nonvoid_result_t = cond_t<not_<is_void<_type>>, _type, pseudo_output>;

	//! \brief �����Ͳ��Ƕ���������ȡ�󱸽�����ͣ�Ĭ��Ϊ pseudo_output ����
	template<typename _type, typename _tRes = pseudo_output>
	using object_result_t = cond_t<is_object<_type>, _type, pseudo_output>;
	//@}

}

#endif