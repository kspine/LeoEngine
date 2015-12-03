#ifndef IndePlatform_ref_hpp
#define IndePlatform_ref_hpp

#include "ldef.h"
#include <functional>
#include <memory>

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
			: ptr(std::addressof(t))
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


	/*!
	\ingroup unary_type_traits
	\brief ȡ���ð�װʵ��������
	*/
	//@{
	template<typename _type>
	struct wrapped_traits : std::false_type
	{
		using type = _type;
	};

	template<class _tWrapped>
	struct wrapped_traits<std::reference_wrapper<_tWrapped>> : std::true_type
	{
		using type = _tWrapped;
	};

	template<class _tWrapped>
	struct wrapped_traits<lref<_tWrapped>> : std::true_type
	{
		using type = _tWrapped;
	};
	//@}

	/*!
	\ingroup metafunctions
	*/
	template<typename _type>
	using wrapped_traits_t = typename wrapped_traits<_type>::type;


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
}


#endif
