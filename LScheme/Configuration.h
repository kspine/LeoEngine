#ifndef LScheme_Configuration_H
#define LScheme_Configuration_H 1

#include "LScheme.h"

namespace scheme {
	/*!
	\brief ���ã�ʹ�� S ���ʽ�洢�ⲿ״̬��
	\warning ����������
	*/
	class LS_API Configuration
	{
	private:
		ValueNode root;

	public:
		DefDeCtor(Configuration)
			//@{
			Configuration(const ValueNode& node)
			: root(node)
		{}
		Configuration(ValueNode&& node)
			: root(std::move(node))
		{}
		//@}
		//@{
		template<typename _tParam,
			limpl(typename = leo::exclude_self_t<Configuration, _tParam>)>
			explicit
			Configuration(_tParam&& arg)
			: root(leo::NoContainer, lforward(arg))
		{}
		template<typename _tParam1, typename _tParam2, typename... _tParams>
		explicit
			Configuration(_tParam1&& arg1, _tParam2&& arg2, _tParams&&... args)
			: root(leo::NoContainer, lforward(arg1), lforward(arg2),
				lforward(args)...)
		{}
		//@}
		DefDeCopyMoveCtorAssignment(Configuration)

			DefGetter(const lnothrow, const ValueNode&, Root, root)

			/*!
			\brief ���ļ��������á�
			\throw GeneralEvent �ļ���Ч��
			\throw LoggedEvent �ļ����ݶ�ȡʧ�ܡ�
			*/
			LS_API friend std::istream&
			operator >> (std::istream&, Configuration&);

		DefGetter(const lnothrow, const ValueNode&, Node, root)
			/*!
			\brief ȡ���ýڵ����ֵ���á�
			*/
			DefGetter(lnothrow, ValueNode&&, NodeRRef, std::move(root))
	};

	/*!
	\brief ����������������
	\relates Configuration
	*/
	LS_API std::ostream&
		operator<<(std::ostream&, const Configuration&);
}

#endif