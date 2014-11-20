////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   IndePlatform/ldef.hpp
//  Version:     v1.01
//  Created:     02/06/2014 by leo hawke.
//  Compilers:   Visual Studio.NET 2013
//  Description: 
// -------------------------------------------------------------------------
//  History:
//		2014-9-27 11:02: ǿ��Ҫ�������֧��alignas�Լ�noexcept(��VS14 CTP1֧������Ϊ��)
//
////////////////////////////////////////////////////////////////////////////
#ifndef IndePlatform_ldef_h
#define IndePlatform_ldef_h


//def LB_IMPL_CPP
//brief C++ʵ��֧�ְ汾
//����Ϊ __cplusplus
#ifdef __cplusplus
#define LB_IMPL_CPP __cplusplus
#else
# error "This header is only for C++."
#endif

//def LB_IMPL_MSCPP
//brief Microsof C++ ʵ��֧�ְ汾
//����Ϊ _MSC_VER �����İ汾��
#ifdef _MSC_VER
#	undef LB_IMPL_MSCPP
#	define LB_IMPL_MSCPP _MSC_VER
#elif __clang__
#	undef LB_IMPL_CLANGPP
#	define LB_IMPL_CLANGPP (__clang__ * 10000 + __clang_minor__ * 100 \
			+ __clang_patchlevel__)
#	elif defined(__GNUC__)
#		undef LB_IMPL_GNUCPP
#		define LB_IMPL_GNUCPP (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 \
			+ __GNUC_PATCHLEVEL__)
#endif

//��ֹCL�������İ�ȫ����
#if LB_IMPL_MSCPP >= 1400
//��ָ������������������error C4996
//See doucumentation on how to use Visual C++ 'Checked Iterators'
#undef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

//ʹ�ò���ȫ�Ļ���������������error C4996
#undef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstddef> //std::nullptr_t,std::size_t,std::ptrdiff_t,offsetof;
#include <climits> //CHAR_BIT;
#include <cassert> //assert;
#include <cstdint>
#include <cwchar>  //std::wint_t;
#include <utility> //std::foward;
#include <type_traits> //std:is_class,std::is_standard_layout;

#ifndef __has_feature
#define __has_feature(...) 0
#endif

#ifndef __has_extension
#define __has_extension(...) 0
#endif

//Ԥ�����������ֺ�
#define LPP_Empty
#define LPP_Comma ,
//��׼δ����##��ֵ˳��,GCC��,##����Ϊ�겻�ᱻչ��,�ʶ�һ���װ
#define LPP_Join(x,y) LPP_Concat(x,y) 
#define LPP_Concat(x,y) x ## y

//����ʵ�ֵı�Ҫ֧��:�ɱ������.
#define limpl(...) __VA_ARGS__

//�������Լ��

//LB_HAS_ALIGNAS
//�ڽ� alignas ֧��
#undef  LB_HAS_ALIGNAS
#define LB_HAS_ALIGNAS \
	(__has_feature(cxx_alignas) || __has_extension(cxx_alignas) || \
		LB_IMPL_CPP >= 201103L || LB_IMPL_MSCPP >= 1900)
#if !LB_HAS_ALIGNAS && LB_IMPL_MSCPP != 1800
#error "compiler must support alignas"
#endif


#undef LB_HAS_ALIGNOF
#define LB_HAS_ALIGNOF (LB_IMPL_CPP >= 201103L)


#undef LB_HAS_BUILTIN_NULLPTR
#define LB_HAS_BUILTIN_NULLPTR \
	(__has_feature(cxx_nullptr) || __has_extension(cxx_nullptr) || \
	 LB_IMPL_CPP >= 201103L || LB_IMPL_MSCPP >= 1600)
#if !LB_HAS_BUILTIN_NULLPTR
#error "compiler must support builtin_nullptr"
#endif
#undef LB_HAS_CONSTEXPR
#define LB_HAS_CONSTEXPR \
	(__has_feature(cxx_constexpr) || __has_extension(cxx_constexpr) || \
	LB_IMPL_CPP >= 201103L || LB_IMPL_MSCPP >= 1900)
#if !LB_HAS_CONSTEXPR && LB_IMPL_MSCPP != 1800
#error "compiler must support constexpr"
#endif
#undef LB_HAS_NOEXCEPT
#define LB_HAS_NOEXCEPT \
	(__has_feature(cxx_noexcept) || __has_extension(cxx_noexcept) || \
		LB_IMPL_CPP >= 201103L || LB_IMPL_MSCPP >= 2000)
#if !defined(LB_HAS_NOEXCEPT) && !defined(LB_IMPL_MSCPP)
#error "compiler must support noexcept"
#endif
#undef LB_HAS_THREAD_LOCAL
#define LB_HAS_THREAD_LOCAL \
	(__has_feature(cxx_thread_local) || (LB_IMPL_CPP >= 201103L \
		&& !LB_IMPL_GNUCPP))


//������ʾ
//��֤����ʱ����������ʱ����������ʾ,��Ҫ���ڱ���ʵ�ֿ��ܵ��Ż�

//����
#if LB_IMPL_GNUCPP >= 20500
#define LB_ATTR(...) __attribute__((__VA_ARGS__))
#else
#define LB_ATTR(...)
#endif

//���ε��Ƿ�����,�򷵻ط��������õĺ�������ģ��
//��Ϊ���� std::malloc �� std::calloc
//���������طǿ�ָ��,���ص�ָ�벻��������Чָ��ı���,��ָ��ָ�����ݲ��������洢����
#if LB_IMPL_GNUCPP >= 20296
#	define LB_ALLOCATOR LB_ATTR(__malloc__)
#else
#	define LB_ALLOCATOR
#endif

//��֧Ԥ����ʾ
#if LB_IMPL_GNUCPP >= 29600
#	define LB_EXPECT(expr, constant) (__builtin_expect(expr, constant))
#	define LB_LIKELY(expr) (__builtin_expect(bool(expr), 1))
#	define LB_UNLIKELY(expr) (__builtin_expect(bool(expr), 0))
#else
#	define LB_EXPECT(expr, constant) (expr)
#	define LB_LIKELY(expr) (expr)
#	define LB_UNLIKELY(expr) (expr)
#endif

//ָ���޷���ֵ����
#if LB_IMPL_GNUCPP >= 40800
#	define LB_NORETURN [[noreturn]]
#elif LB_IMPL_GNUCPP >= 20296
#	define LB_NORETURN LB_ATTR(__noreturn__)
#else
#	define LB_NORETURN
#endif

//ָ����������ģ��ʵ��Ϊ������
//�ٶ������ɷ���,�ٶ��������ⲿ�ɼ��ĸ�����
#if LB_IMPL_GNUCPP >= 20296
#	define LB_PURE LB_ATTR(__pure__)
#else
#	define LB_PURE
#endif

//ָ������Ϊ��״̬����
#if LB_IMPL_GNUCPP >= 20500
#	define LB_STATELESS LB_ATTR(__const__)
#else
#	define LB_STATELESS
#endif

//��ѡ��
//LB_DLL ʹ�ö�̬���ӿ�
//LB_BUILD_DLL ������̬���ӿ�
#if defined(LB_DLL) && defined(LB_BUILD_DLL)
#	error "DLL could not be built and used at the same time."
#endif

#ifdef LB_DLL
#	define LB_API __declspec(dllimport)
#elif defined(LB_BUILD_DLL)
#	define LB_API __declspec(dllexport)
#else
#	define LB_API
#endif

//ָ�����͵Ķ���
#if LB_HAS_ALIGNOF
#	define lalignof alignof
#else
#	define lalignof(_type) std::alignment_of<_type>::value
#endif

#if LB_HAS_ALIGNAS
#	define lalignas alignas
#else
#	define lalignas(_n) _declspec(align(_n))
#endif

//����������
#if LB_HAS_CONSTEXPR && !LB_IMPL_MSCPP
#	define lconstexpr constexpr
#	define lconstfn constexpr
#else
#	define lconstexpr const
#	define lconstfn inline
#endif

//�㲻�����쳣�淶,����
#if LB_USE_EXCEPTION_SPECIFICATION
#	define lthrow throw
#else
#	define lthrow(...)
#endif

#ifdef LB_USE_EXCEPTION_VALIDATION
#	define lnothrowv lnothrow
#else
#	define lnothrowv
#endif

#if LB_HAS_NOEXCEPT
#	define lnothrow lnoexcept
#elif LB_IMPL_GNUCPP >= 30300
#	define lnothrow __attribute__ ((nothrow))
#else
#	define lnothrow lthrow()
#endif

#if LB_HAS_NOEXCEPT
#	define lnoexcept noexcept
#else
#	define lnoexcept(...)
#endif

#if LB_HAS_THREAD_LOCAL && defined(_MT)
#	define lthread thread_local
#else
#	define lthread static
#endif

namespace leo
{
#if LB_HAS_BUILTIN_NULLPTR
	using std::nullptr_t;
#else
	const class nullptr_t
	{
	public:
		template<typename _type>
		inline
			operator _type*() const
		{
			return 0;
		}

		template<class _tClass, typename _type>
		inline
			operator _type _tClass::*() const
		{
			return 0;
		}
		template<typename _type>
		bool
			equals(const _type& rhs) const
		{
			return rhs == 0;
		}

		void operator&() const = delete;
	} nullptr = {};

	template<typename _type>
	inline bool
		operator==(nullptr_t lhs, const _type& rhs)
	{
		return lhs.equals(rhs);
	}
	template<typename _type>
	inline bool
		operator==(const _type& lhs, nullptr_t rhs)
	{
		return rhs.equals(lhs);
	}

	template<typename _type>
	inline bool
		operator!=(nullptr_t lhs, const _type& rhs)
	{
		return !lhs.equals(rhs);
	}
	template<typename _type>
	inline bool
		operator!=(const _type& lhs, nullptr_t rhs)
	{
		return !rhs.equals(lhs);
	}
#endif
	namespace stdex
	{
		//char��unsigned��signedָ��
		using byte = unsigned char;
#if  CHAR_BIT == 8
		//һ�ֽڲ���һ������8λ!
		using octet = byte;
#else
		using octet = void;
#endif
		using errno_t = int;
		using std::ptrdiff_t;
		using std::size_t;
		using std::wint_t;


		template<typename...>
		struct empty_base
		{};

		//tuple,pair����Ĺ�������
		using raw_tag = empty_base<>;

		//��Ա���㾲̬���ͼ��. 
		template<bool bMemObjPtr, bool bNoExcept, typename T>
		class offsetof_check
		{
			static_assert(std::is_class<T>::value, "Non class type found.");
			static_assert(std::is_standard_layout<T>::value,
				"Non standard layout type found.");
			static_assert(bMemObjPtr, "Non-static member object violation found.");
			static_assert(bNoExcept, "Exception guarantee violation found.");
		};

#define lunused(...) static_cast<void>(__VA_ARGS__)

#if LB_HAS_NOEXCEPT
#define loffsetof(type,member) \
	(decltype(sizeof(leo::stdex::offsetof_check<std::is_member_object_pointer< \
	decltype(&type::member)>::value,lnoexcept(offsetof(type,member)), \
	type>))(offsetof(type,member)))
#else
#define loffsetof(type,member) \
	(decltype(sizeof(leo::stdex::offsetof_check<std::is_member_object_pointer< \
	decltype(&type::member)>::value,true, \
	type>))(offsetof(type,member)))
#endif
#define lforward(expr) std::forward<decltype(expr)>(expr)

		template<typename type, typename ...tParams>
		lconstfn auto
			unsequenced(type && arg, tParams&&...)->decltype(lforward(arg))
		{
			return lforward(arg);
		}

		//������ֵ
#define lunseq leo::stdex::unsequenced
	}
}

#endif