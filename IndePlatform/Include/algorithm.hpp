#ifndef IndePlatform_algorithm_hpp
#define IndePlatform_algorithm_hpp

#include "utility.hpp"
#include "iterator.hpp"
#include <algorithm>
#include "type_op.hpp"
#include <cstring>

namespace leo
{
	/*Gerneral Algorithms
	�㷨
	��Χ(��������,�ڼ�����) // ISO C++ Clause 23
	*/

	/*����ת��
	  _fop ��������
	  _tOut �������������
	  _fIns �������������
	*/
	template<typename _fOp, typename _tOut, typename... _tIns>
	void
		transform_n(_fOp op, _tOut result, size_t n, _tIns... iters)
	{
		while (n-- != 0)
		{
			lunseq((lconstraint(!is_undereferenceable(result)), 0),
				(lconstraint(!is_undereferenceable(iters)), void(iters), 0)...);
			*result = op((*iters)...);
			lunseq(++result, ++iters...);
		}
	}

	/*POD Type Operations
	  _type ��������
	  ��̬����:����Ƿ�ΪPOD����
	  ����:�����ָ����Ч��
	*/

	template<class _type>
		inline _type*
		pod_fill(_type* first, _type* last, const _type& value)
	{
		static_assert(is_pod<remove_reference_t<_type>>::value,
			"Non-POD type found @ pod_fill;");

		switch ((last - first) & 7)
		{
		case 0:
			while (first != last)
			{
				*first = value; ++first;
		case 7: *first = value; ++first;
		case 6: *first = value; ++first;
		case 5: *first = value; ++first;
		case 4: *first = value; ++first;
		case 3: *first = value; ++first;
		case 2: *first = value; ++first;
		case 1: *first = value; ++first;
			}
		}
		return last;
	}

	template<class _type>
		inline _type*
			pod_copy_n(const _type* first, size_t n, _type* result)
		{
			static_assert(is_pod<remove_reference_t<_type>>::value,
				"Non-POD type found @ pod_copy_n;");

			std::memcpy(result, first, sizeof(*first) * n);
			return result + n;
		}

	template<class _type>
		inline _type*
			pod_copy(const _type* first, const _type* last, _type* result)
		{
			return leo::pod_copy_n(first, last - first, result);
		}

	template<class _type>
		inline _type*
			pod_move_n(const _type* first, size_t n, _type* result)
		{
			static_assert(is_pod<remove_reference_t<_type>>::value,
				"Non-POD type found @ pod_move_n;");

			std::memmove(result, first, sizeof(*first) * n);
			return result + n;
		}

	template<class _type>
		inline _type*
			pod_move(const _type* first, const _type* last, _type* result)
		{
			return leo::pod_move_n(first, last - first, result);
		}

	/*
	ȥ����������Χ�е��ظ�Ԫ��,�Ҳ��ı�Ԫ��֮������˳��.
	range[first,last)
	return newlast
	complex time:O(n^2) n ==> std::advance(first,n) == last
	*/
	template<typename _tFwd>
	_tFwd
		stable_range_unique(_tFwd first, _tFwd last)
	{
		_tFwd result(first);

		for (_tFwd i(first); i != last; ++i)
			if (std::find(first, result, *i) == result)
			{
				using std::swap;

				swap(*i, *result);
				++result;
			}
		return result;
	}
}

#endif