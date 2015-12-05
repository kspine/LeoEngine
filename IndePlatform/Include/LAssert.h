#ifndef IndePlatform_LAssert_h
#define IndePlatform_LAssert_h

#include "ldef.h"
#include "iterator.hpp"
#include <assert.h>

#pragma warning(push)
#pragma warning(disable:4800)

#undef LAssert
#define LAssert(_expr,_msg) assert(_expr)

#ifndef LAssertNonnull
#define LAssertNonnull(_expr) LAssert(bool(_expr), "Null reference found.")
#endif

namespace leo {

	/*!
	\brief ���Բ����طǿ�ָ�롣
	\pre ���ԣ�ָ��ǿա�
	*/
	template<typename _type>
	inline _type&&
		Nonnull(_type&& p) lnothrow
	{
		LAssertNonnull(p);
		return lforward(p);
	}

	/*!
	\brief ����������
	\pre ���ԣ���������ȷ�����ɽ����á�
	*/
	template<typename _type>
	inline _type&&
		CheckIter(_type&& i) lnothrow
	{
		LAssert(!is_undereferenceable(i), "Invalid iterator found.");
		return lforward(i);
	}


	/*!
	\brief ���Բ������÷ǿ�ָ�롣
	\pre ʹ�� ADL ָ���� CheckIter ���õı��ʽ����ȼ��� platform::CheckIter ��
	\pre ��Ӷ��ԣ�ָ��ǿա�
	*/
	template<typename _type>
	lconstfn auto
		Deref(_type&& p) -> decltype(*p)
	{
		return *CheckIter(lforward(p));
	}
}

#pragma warning(pop)
#endif
