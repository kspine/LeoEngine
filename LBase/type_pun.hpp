/*! \file type_pun.hpp
\ingroup LBase
\brief ����洢��ֱ��ת����

*/
#ifndef LBase_type_pun_hpp
#define LBase_type_pun_hpp 1


#include "LBase/type_traits.hpp" // for "type_traits.hpp", bool_constant, lalignof,
//	and_, is_trivial, enable_if_t, is_object_pointer, remove_pointer_t,
//	aligned_storage_t, is_reference, remove_reference_t, exclude_self_t,
//	decay_t;
#include <new> // for placement ::operator new from standard library;

#ifdef LB_IMPL_MSCPP
#pragma warning(push)
#pragma warning(disable:4554)
#endif

namespace leo
{
	using stdex::byte;
	/*!	\defgroup aligned_type_traits Aligned Type Traits
	\ingroup binary_type_traits
	\brief ��������������
	\note ��һ�������ͱȵڶ��������Ͷ���Ҫ����ϸ����ͬ��
	*/
	//@{
	/*!
	\brief �ж��Ƿ������ݡ�
	\since build 1.4
	*/
	template<typename _type, typename _type2>
	struct is_aligned_compatible : bool_constant<!(lalignof(_type)
		< lalignof(_type2)) & lalignof(_type) % lalignof(_type2) == 0>
	{};


	/*!
	\brief �ж��Ƿ��ԭ�ض���洢��
	\since build 1.4
	*/
	template<typename _type, typename _tDst>
	struct is_aligned_placeable : bool_constant<sizeof(_type)
		== sizeof(_tDst) && is_aligned_compatible<_type, _tDst>::value>
	{};


	/*!
	\brief �ж��Ƿ�ɶ���洢��
	\since build 1.4
	*/
	template<typename _type, typename _tDst>
	struct is_aligned_storable : bool_constant<!(sizeof(_type) < sizeof(_tDst))
		&& is_aligned_compatible<_type, _tDst>::value>
	{};


	/*!
	\brief �ж��Ƿ���滻����洢��
	\since build 1.4
	*/
	template<typename _type, typename _tDst>
	struct is_aligned_replaceable : and_<is_aligned_storable<_type, _tDst>,
		is_aligned_storable<_tDst, _type>>
	{};


	/*!
	\brief �ж��Ƿ��ƽ���滻�洢��
	\todo ʹ�� \c is_trivially_copyable ���� is_trivial ��
	*/
	template<typename _type, typename _tDst>
	struct is_trivially_replaceable : and_<is_trivial<_type>, is_trivial<_tDst>,
		is_aligned_replaceable<_type, _tDst>>
	{};
	//@}


	//! \since build 1.4
	//@{
	/*!
	\ingroup metafunctions
	\brief ѡ����ض����Ϳ��滻�ַ������͵��ض������Ա����ͻ��
	*/
	template<typename _tSrc, typename _tDst, typename _type = void>
	using enable_if_replaceable_t
		= enable_if_t<is_trivially_replaceable<_tSrc, _tDst>::value, _type>;


	/*!
	\ingroup transformation_traits
	\brief ��ʽת�����������洢����洢���͡�
	*/
	template<typename _type, size_t _vAlign = lalignof(_type)>
	using pun_storage_t = aligned_storage_t<sizeof(_type), _vAlign>;


	/*!
	\ingroup cast
	\pre Ŀ�������Ƕ���ָ������á�
	\note ʹ�� \c reinterpret_cast �ұ�֤Ŀ�����͵Ķ���Ҫ�󲻱�Դ���͸��ϸ��Ա�֤���档
	*/
	//@{
	/*!
	\brief ��֤������ݵ���ʽת����
	\note �������������� is_aligned_compatible Ҫ������ \c reinterpret_cast ��
	*/
	//@{
	template<typename _pDst, typename _tSrc>
	inline limpl(enable_if_t)<and_<is_object_pointer<_pDst>,
		is_aligned_compatible<_tSrc, remove_pointer_t<_pDst>>>::value, _pDst>
		aligned_cast(_tSrc* v) lnothrow
	{
		return reinterpret_cast<_pDst>(v);
	}
	template<typename _rDst, typename _tSrc, limpl(typename = enable_if_t<
		and_<is_reference<_rDst>, is_aligned_compatible<remove_reference_t<_tSrc>,
		remove_reference_t<_rDst>>>::value>)>
		inline auto
		aligned_cast(_tSrc&& v) lnothrow
		-> decltype(lforward(reinterpret_cast<_rDst>(v)))
	{
		return lforward(reinterpret_cast<_rDst>(v));
	}
	//@}


	/*!
	\brief ��֤����洢����ʽת����
	\note �������������� is_aligned_storable Ҫ������ \c reinterpret_cast ��
	*/
	//@{
	template<typename _pDst, typename _tSrc>
	inline limpl(enable_if_t)<and_<is_object_pointer<_pDst>,
		is_aligned_storable<_tSrc, remove_pointer_t<_pDst>>>::value, _pDst>
		aligned_store_cast(_tSrc* v) lnothrow
	{
		return reinterpret_cast<_pDst>(v);
	}
	template<typename _rDst, typename _tSrc, limpl(typename = enable_if_t<
		and_<is_reference<_rDst>, is_aligned_storable<remove_reference_t<_tSrc>,
		remove_reference_t<_rDst>>>::value>)>
		inline auto
		aligned_store_cast(_tSrc&& v) lnothrow
		-> decltype(lforward(reinterpret_cast<_rDst>(v)))
	{
		return lforward(reinterpret_cast<_rDst>(v));
	}
	//@}


	/*!
	\brief ��֤�����滻�洢����ʽת����
	\note �������������� is_aligned_replaceable Ҫ������ \c reinterpret_cast ��
	*/
	//@{
	template<typename _pDst, typename _tSrc>
	inline limpl(enable_if_t)<and_<is_object_pointer<_pDst>,
		is_aligned_replaceable<_tSrc, remove_pointer_t<_pDst>>>::value, _pDst>
		aligned_replace_cast(_tSrc* v) lnothrow
	{
		return reinterpret_cast<_pDst>(v);
	}
	template<typename _rDst, typename _tSrc, limpl(typename = enable_if_t<
		and_<is_reference<_rDst>, is_aligned_replaceable<remove_reference_t<_tSrc>,
		remove_reference_t<_rDst>>>::value>)>
		inline auto
		aligned_replace_cast(_tSrc&& v) lnothrow
		-> decltype(lforward(reinterpret_cast<_rDst>(v)))
	{
		return lforward(reinterpret_cast<_rDst>(v));
	}
	//@}


	/*!
	\brief ��֤ƽ���滻�洢����ʽת����
	\note �������������� is_trivially_replaceable Ҫ������ \c reinterpret_cast ��
	\since build 1.4
	*/
	//@{
	template<typename _pDst, typename _tSrc>
	inline limpl(enable_if_t)<and_<is_object_pointer<_pDst>,
		is_trivially_replaceable<_tSrc, remove_pointer_t<_pDst>>>::value, _pDst>
		replace_cast(_tSrc* v) lnothrow
	{
		return reinterpret_cast<_pDst>(v);
	}
	template<typename _rDst, typename _tSrc, limpl(typename = enable_if_t<
		and_<is_reference<_rDst>, is_trivially_replaceable<remove_reference_t<_tSrc>,
		remove_reference_t<_rDst>>>::value>)>
		inline auto
		replace_cast(_tSrc&& v) lnothrow
		-> decltype(lforward(reinterpret_cast<_rDst>(v)))
	{
		return lforward(reinterpret_cast<_rDst>(v));
	}
	//@}
	//@}
	//@}


	/*!
	\brief �����׼�������ʹ洢��
	\since build 1.4
	*/
	template<typename _tUnderlying = aligned_storage_t<sizeof(void*)>>
	struct standard_layout_storage
	{
		static_assert(is_standard_layout<_tUnderlying>::value,
			"Invalid underlying type found.");

		using underlying = _tUnderlying;

		underlying object;

		standard_layout_storage() = default;
		standard_layout_storage(const standard_layout_storage&) = default;
		//! \since build 1.4
		template<typename _type,
			limpl(typename = exclude_self_t<standard_layout_storage, _type>)>
			inline
			standard_layout_storage(_type&& x)
		{
			new(access()) decay_t<_type>(lforward(x));
		}

		standard_layout_storage&
			operator=(const standard_layout_storage&) = default;
		/*!
		\note Ϊ�������ʹ�����Ҫȷ������ʱӦʹ����ʽʹ�� access ָ�����͸�ֵ��
		\since build 1.4
		*/
		template<typename _type,
			limpl(typename = exclude_self_t<standard_layout_storage, _type>)>
			inline standard_layout_storage&
			operator=(_type&& x)
		{
			assign(lforward(x));
			return *this;
		}

		lconstfn_relaxed LB_PURE void*
			access() lnothrow
		{
			return &object;
		}
		lconstfn LB_PURE const void*
			access() const lnothrow
		{
			return &object;
		}
		template<typename _type>
		lconstfn_relaxed LB_PURE _type&
			access() lnothrow
		{
			static_assert(is_aligned_storable<standard_layout_storage, _type>(),
				"Invalid type found.");

			return *static_cast<_type*>(access());
		}
		template<typename _type>
		lconstfn LB_PURE const _type&
			access() const lnothrow
		{
			static_assert(is_aligned_storable<standard_layout_storage, _type>(),
				"Invalid type found.");

			return *static_cast<const _type*>(access());
		}

		lconstfn_relaxed LB_PURE byte*
			data() lnothrow
		{
			return static_cast<byte*>(access());
		}
		lconstfn LB_PURE const byte*
			data() const lnothrow
		{
			return static_cast<const byte*>(access());
		}

		//! \since build 1.4
		template<typename _type>
		inline void
			assign(_type&& x)
		{
			access<decay_t<_type>>() = lforward(x);
		}
	};


	/*!
	\ingroup transformation_traits
	\brief ��ʽת���������׼��������洢���͡�
	\since build 1.4
	*/
	template<typename _type, size_t _vAlign = lalignof(_type)>
	using replace_storage_t
		= standard_layout_storage<pun_storage_t<_type, _vAlign>>;

} // namespace leo;

#ifdef LB_IMPL_MSCPP
#pragma warning(pop)
#endif

#endif
