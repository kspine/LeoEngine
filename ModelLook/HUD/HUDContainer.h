#ifndef HUD_Container_H
#define HUD_Container_H

#include "Widget.h"

LEO_BEGIN
HUD_BEGIN

//@{
//! \brief Z ˳�����ͣ�����˳��ֵԽ���ʾԽ�ӽ����㡣
using ZOrder = std::uint8_t;
//! \brief Ĭ�� Z ˳��ֵ��
const ZOrder DefaultZOrder(64);
//! \brief Ĭ�ϴ��� Z ˳��ֵ��
const ZOrder DefaultWindowZOrder(128);
//@}

class LB_API MUIContainer {
public:
	/*!
	\brief ��������Ŀ���͡�
	*/
	using ItemType = lref<IWidget>;
	/*!
	\brief ����ӳ������ͣ�ӳ�� Z ˳����������
	*/
	using WidgetMap = std::multimap<ZOrder, ItemType>;
	using PairType = WidgetMap::value_type;
	using iterator = WidgetIterator;

protected:
	/*
	\brief ����ӳ�䣺�洢 Z ˳��ӳ�����������á�
	*/
	WidgetMap mWidgets;

public:
	//@{
	/*!
	\brief �޲������죺Ĭ��ʵ�֡�
	*/
	DefDeCtor(MUIContainer)
	DefDeMoveCtor(MUIContainer)

	/*!
	\brief �򲿼�����Ӳ�����
	\note �����Ѵ���ʱ���ԡ�

	�򲿼��鰴Ĭ�� Z ˳��ֵ��Ӳ�����
	*/
	PDefHOp(void, +=, IWidget& wgt)
	ImplRet(Add(wgt))

	/*!
	\brief �Ӳ������Ƴ�������
	\return ����ָ���������Ƴ��ɹ���

	�Ӳ������Ƴ�������
	*/
	bool
	operator-=(IWidget&);
	//@}

	/*!
	\brief �ж��Ƿ����ָ��������
	*/
	bool
		Contains(IWidget&);

	/*!
	\brief ȡ��������
	*/
	DefGetter(const lnothrow, size_t, Count, mWidgets.size())

		/*!
		\brief �򲿼�����Ӳ�����
		\note �����Ѵ���ʱ���ԡ�

		�򽹵��������ӽ������ͬʱ�򲿼��鰴ָ�� Z ˳��ֵ��Ӳ�����
		*/
		void
		Add(IWidget&, ZOrder = DefaultZOrder);

	/*!
	\brief ���ƿ����Ӳ�����
	*/
	void
		PaintVisibleChildren(PaintEventArgs&);

	/*!
	\brief ��ѯָ�������� Z ˳��
	\throw std::out_of_range ������ָ���Ӳ�����
	*/
	ZOrder
		QueryZ(IWidget&) const;

	iterator
		begin();

	PDefH(void, clear, ) lnothrow
		ImplExpr(mWidgets.clear())

		iterator end();
};

LB_API bool
RemoveFrom(IWidget& wgt, IWidget& con);

HUD_END

LEO_END

#endif
