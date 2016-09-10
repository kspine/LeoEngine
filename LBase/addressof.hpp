/*! \file addressof.hpp
\ingroup LBase

�ṩ��һԪ������ & ��صĽӿڣ�
����Ƿ����� operator& ��������
�Լ�������ʱ���� constexpr �� leo::addressof ��� constfn_addressof ��
\note ��ֹ����<memory>
*/

#ifndef LBase_addressof_hpp
#define LBase_addressof_hpp 1

#include "LBase/type_traits.hpp"

namespace leo {
		namespace details {
			// TEMPLATE FUNCTION addressof
			template<class _Ty> inline
				_Ty *_Addressof(_Ty& _Val, true_type) lnoexcept
			{	// return address of function _Val
				return (_Val);
			}

			template<class _Ty> inline
				_Ty *_Addressof(_Ty& _Val, false_type) lnoexcept
			{	// return address of object _Val
				return (reinterpret_cast<_Ty *>(
					&const_cast<char&>(
						reinterpret_cast<const volatile char&>(_Val))));
			}
		}

		template<class _Ty> inline
			_Ty *addressof(_Ty& _Val) lnoexcept
		{	// return address of _Val
			return (details::_Addressof(_Val, is_function<_Ty>()));
		}

		namespace details {
			//@{
			template<typename _type>
			using addressof_mem_t = decltype(std::declval<const _type&>().operator&());

			template<typename _type>
			using addressof_free_t = decltype(operator&(std::declval<const _type&>()));
			//@}
		}

		/*!
		\ingroup unary_type_traits
		\brief �ж��Ƿ���ں�ʽ���� & ����������ָ�����͵ı��ʽ��
		\pre ������Ϊ���������͡�
		\since build 1.4
		*/
		template<typename _type>
		struct has_overloaded_addressof
			: or_<is_detected<details::addressof_mem_t, _type>,
			is_detected<details::addressof_free_t, _type >>
		{};


		/*!
		\brief ���ԶԷ����� operator& �ṩ constexpr �� addressof �����
		\note ��ǰ ISO C++ �� std::addressof �� constexpr ��
		\since build 1.4
		\see http://wg21.cmeerw.net/lwg/issue2296 ��
		*/
		//@{
		template<typename _type>
		lconstfn limpl(enable_if_t)<!has_overloaded_addressof<_type>::value, _type*>
			constfn_addressof(_type& r)
		{
			return &r;
		}
		//! \note ��Ϊ����ֲ����Ҫ������ֱ���ṩ constexpr ��
		template<typename _type,
			limpl(typename = enable_if_t<has_overloaded_addressof<_type>::value>)>
			inline _type*
			constfn_addressof(_type& r)
		{
			return addressof(r);
		}
		//@}


		/*!
		\brief ȡ \c void* ���͵�ָ�롣
		\note �ʺ� std::fprintf �ȵ� \c %p ת�����
		\since build 1.4
		*/
		//@{
		template<typename _type>
		lconstfn limpl(enable_if_t)<or_<is_object<_type>, is_void<_type>>::value, void*>
			pvoid(_type* p) lnothrow
		{
			return const_cast<void*>(static_cast<const volatile void*>(p));
		}
		//! \note ISO C++11 ָ������ָ�� \c reinterpret_cast Ϊ����ָ��������֧�֡�
		template<typename _type>
		inline limpl(enable_if_t)<is_function<_type>::value, void*>
			pvoid(_type* p) lnothrow
		{
			return const_cast<void*>(reinterpret_cast<const volatile void*>(p));
		}

		/*!
		\return �ǿ�ָ�롣
		\since build 1.4
		*/
		template<typename _type>
		lconstfn void*
			pvoid_ref(_type&& ref)
		{
			return pvoid(addressof(ref));
		}
		//@}
}
#endif
