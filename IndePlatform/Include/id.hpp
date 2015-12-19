////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2015.
// -------------------------------------------------------------------------
//  File name:   IndePlatform/id.hpp
//  Version:     v1.00
//  Created:     12/13/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: ����unique_id�ĸ��ຯ��[hash,uuid...]
// -------------------------------------------------------------------------
//  History:
////////////////////////////////////////////////////////////////////////////

#ifndef IndePlatform_id_h
#define IndePlatform_id_h

#include "ldef.h"
#include "tuple.hpp"
#include <limits>
#include <functional> //for std::hash
#include <numeric> //for std::accmulate

namespace leo {


	/*!	\defgroup hash_extensions Hash Extensions
	\brief ɢ����չ�ӿڡ�
	\note ��ǰʹ�� Boost ����Ľӿںͽ���ʵ�֡�
	\see http://www.boost.org/doc/libs/1_54_0/doc/html/hash/reference.html#boost.hash_combine ��
	*/
	//@{
	/*!
	\brief �ظ�����ɢ�С�
	\note <tt>(1UL << 31) / ((1 + std::sqrt(5)) / 4) == 0x9E3779B9</tt> ��
	\warning ʵ�֣� Boost �ĵ���Ϊ Effects �����ܸı䣬��Ӧ��Ϊ�ӿ�������
	*/
	template<typename _type>
	inline void
		hash_combine(size_t& seed, const _type& val)
		lnoexcept_spec(std::hash<_type>()(val))
	{
		seed ^= std::hash<_type>()(val) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
	}

	template<typename _type>
	lconstfn size_t
		hash_combine_seq(size_t seed, const _type& val)
		lnoexcept_spec(std::hash<_type>()(val))
	{
		return hash_combine(seed, val), seed;
	}
	template<typename _type, typename... _tParams>
	lconstfn size_t
		hash_combine_seq(size_t seed, const _type& val, const _tParams&... args)
		lnoexcept_spec(std::hash<_type>()(val))
	{
		return
			hash_combine_seq(hash_combine_seq(seed, val), args...);
	}


	template<typename _tIn>
	inline size_t
		hash(size_t seed, _tIn first, _tIn last)
	{
		return std::accumulate(first, last, seed,
			[](size_t s, decltype(*first) val) {
			hash_combine(s, val);
			return s;
		});
	}
	template<typename _tIn>
	inline size_t
		hash(_tIn first, _tIn last)
	{
		return hash(0, first, last);
	}


	template< class T, unsigned N >
	inline std::size_t hash(const T(&x)[N])
	{
		return hash_range(x, x + N);
	}

	template< class T, unsigned N >
	inline std::size_t hash(T(&x)[N])
	{
		return hash_range(x, x + N);
	}

	namespace details
	{
		//@{
		template<class, class>
		struct combined_hash_tuple;

		template<typename _type, size_t... _vSeq>
		struct combined_hash_tuple<_type, index_sequence<_vSeq...>>
		{
			static lconstfn size_t
				call(const _type& tp)
				lnoexcept_spec(hash_combine_seq(0, std::get<_vSeq>(tp)...))
			{
				return hash_combine_seq(0, std::get<_vSeq>(tp)...);
			}
		};
		//@}

	}

	template<typename...>
	struct combined_hash;

	template<typename _type>
	struct combined_hash<_type> : std::hash<_type>
	{};

	template<typename... _types>
	struct combined_hash<std::tuple<_types...>>
	{
		using type = std::tuple<_types...>;

		lconstfn size_t
			operator()(const type& tp) const lnoexcept_spec(
				hash_combine_seq(0, std::declval<const _types&>()...))
		{
			return details::combined_hash_tuple<type,
				index_sequence_for<_types... >> ::call(tp);
		}
	};

	template<typename _type1, typename _type2>
	struct combined_hash<std::pair<_type1, _type2>>
		: combined_hash<std::tuple<_type1, _type2>>
	{};
}

#endif