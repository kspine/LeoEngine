/*! \file sutility.h
\ingroup LBase
\brief ����ʵ����ʩ��

*/
#ifndef LBase_sutility_h
#define LBase_sutility_h 1

#include "LBase/ldef.h"

namespace leo
{
	/*!
	\brief ���ɸ��ƶ��󣺽�ֹ���������Ĭ��ԭ�͵ĸ��ƹ��캯���͸��Ƹ�ֵ��������
	\warning ����������
	\since change 1.4
	*/
	class noncopyable
	{
	protected:
		/*!
		\brief \c protected ���죺Ĭ��ʵ�֡�
		\note �����Ƕ�̬�ࡣ
		*/
		lconstfn noncopyable() = default;
		//! \brief \c protected ������Ĭ��ʵ�֡�
		~noncopyable() = default;

	public:
		//! \brief ��ֹ���ƹ��졣
		lconstfn noncopyable(const noncopyable&) = delete;

		/*!
		\brief ����ת�ƹ��졣
		\since build 1.4
		*/
		lconstfn
			noncopyable(noncopyable&&) = default;

		//! \brief ��ֹ���Ƹ�ֵ��
		noncopyable&
			operator=(const noncopyable&) = delete;

		/*!
		\brief ����ת�Ƹ�ֵ��
		\since build 1.4
		*/
		noncopyable&
			operator=(noncopyable&&) = default;
	};

	/*
	\brief ����ת�ƶ��󣺽�ֹ�̳д���Ķ������Ĭ��ԭ�͵�ת�ƹ��캯����ת�Ƹ�ֵ��������
	\warning ����������
	\since change 1.4
	*/
	class nonmovable
	{
	protected:
		/*!
		\brief \c protected ���죺Ĭ��ʵ�֡�
		\note �����Ƕ�̬�ࡣ
		*/
		lconstfn nonmovable() = default;
		//! \brief \c protected ������Ĭ��ʵ�֡�
		~nonmovable() = default;

	public:
		//! \since build 1.4
		//@{
		//! \brief �����ƹ��졣
		lconstfn nonmovable(const nonmovable&) = default;
		//! \brief ��ֹת�ƹ��졣
		lconstfn
			nonmovable(nonmovable&&) = delete;

		//! \brief �����Ƹ�ֵ��
		nonmovable&
			operator=(const nonmovable&) = default;
		//! \brief ��ֹת�Ƹ�ֵ��
		nonmovable&
			operator=(const nonmovable&&) = delete;
		//@}
	};


	/*!
	\brief �ɶ�̬���Ƶĳ�����ࡣ
	*/
	class LB_API cloneable
	{
	public:
		cloneable() = default;
		cloneable(const cloneable&) = default;
		//! \brief ���������ඨ����Ĭ��ʵ�֡�
		virtual
		~cloneable();

		virtual cloneable*
		clone() const = 0;
	};

	/*!
	\brief ��Ӳ����������������������á�
	\note ������ ::indirect_input_iterator ��ת���������ʡ�
	\todo �ṩ�ӿ������û�ָ��ʹ�� ::polymorphic_cast �ȼ��ת����
	*/
	template<typename _type>
	struct deref_self
	{
		_type&
			operator*() lnothrow
		{
			return *static_cast<_type*>(this);
		}
		const _type&
			operator*() const lnothrow
		{
			return *static_cast<const _type*>(this);
		}
		volatile _type&
			operator*() volatile lnothrow
		{
			return *static_cast<volatile _type*>(this);
		}
		const volatile _type&
			operator*() const volatile lnothrow
		{
			return *static_cast<const volatile _type*>(this);
		}
	};
}


#endif