#include <ldef.h>
#include <LAssert.h>

namespace  leo
{
	/*!
	\brief ʹ�� new ����ָ��ָ��ָ��Ķ���
	*/
	template<typename _type>
	lconstfn auto
		CloneNonpolymorphic(const _type& p) -> decltype(&*p)
	{
		return new typename std::remove_reference_t<decltype(*p)>(*p);
	}

	/*!
	\brief ʹ�� clone ��Ա��������ָ��ָ��ָ��Ķ�̬�����Ͷ���
	\pre ���ԣ� std::is_polymorphic<decltype(*p)>::value ��
	*/
	template<class _type>
	auto
		ClonePolymorphic(const _type& p) -> decltype(&*p)
	{
		static_assert(std::is_polymorphic<std::remove_reference_t<decltype(*p)>>
			::value, "Non-polymorphic class type found.");

		return p->clone();
	}

	namespace HUD {

		/*!
		\brief �ж� i �Ƿ�������ҿ����� [_type(0), b) �С�
		\pre ���ԣ� <tt>_type(0) < b</tt> ��
		*/
		template<typename _type>
		inline bool
			IsInInterval(_type i, _type b) lnothrow
		{
			LAssert(_type(0) < b,
				"Zero element as lower bound is not less than upper bound.");
			return !(i < _type(0)) && i < b;
		}

		/*!
		\brief �ж� i �Ƿ�������ҿ����� [a, b) �С�
		\pre ���ԣ� <tt>a < b</tt> ��
		*/
		template<typename _type>
		inline bool
			IsInInterval(_type i, _type a, _type b) lnothrow
		{
			LAssert(a < b, "Lower bound is not less than upper bound.");
			return !(i < a) && i < b;
		}

		/*!
		\brief �ж� i �Ƿ��ڱ����� [_type(0), b] �С�
		\pre ���ԣ� <tt>_type(0) < b</tt> ��
		*/
		template<typename _type>
		inline bool
			IsInClosedInterval(_type i, _type b) lnothrow
		{
			LAssert(_type(0) < b,
				"Zero element as lower bound is not less than upper bound.");
			return !(i < _type(0) || b < i);
		}
		/*!
		\brief �ж� i �Ƿ��ڱ����� [a, b] �С�
		\pre ���ԣ� <tt>a < b</tt> ��
		*/
		template<typename _type>
		inline bool
			IsInClosedInterval(_type i, _type a, _type b) lnothrow
		{
			LAssert(a < b, "Lower bound is not less than upper bound.");
			return !(i < a || b < i);
		}

		/*!
		\brief �ж� i �Ƿ��ڿ����� (_type(0), b) �ڡ�
		\pre ���ԣ� <tt>_type(0) < b</tt> ��
		*/
		template<typename _type>
		inline bool
			IsInOpenInterval(_type i, _type b) lnothrow
		{
			LAssert(_type(0) < b,
				"Zero element as lower bound is not less than upper bound.");
			return _type(0) < i && i < b;
		}
		/*!
		\brief �ж� i �Ƿ��ڿ����� (a, b) �ڡ�
		\pre ���ԣ� <tt>a < b</tt> ��
		*/
		template<typename _type>
		inline bool
			IsInOpenInterval(_type i, _type a, _type b) lnothrow
		{
			LAssert(a < b, "Lower bound is not less than upper bound.");
			return a < i && i < b;
		}

	}
}
