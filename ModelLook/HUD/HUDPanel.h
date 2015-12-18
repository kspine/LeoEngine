////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2015.
// -------------------------------------------------------------------------
//  File name:   HUD/HUDPanel.hpp
//  Version:     v1.00
//  Created:     12/12/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: HUD���
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#ifndef HUD_WIDGETCCV_H
#define HUD_WIDGETCCV_H

#include "Widget.h"
#include "HUDControl.h"
#include "HUDContainer.h"
LEO_BEGIN

HUD_BEGIN

class LB_API Panel : public Control, protected MUIContainer
{
public:
	/*!
	\brief ���죺ʹ��ָ���߽硣
	*/
	explicit
	Panel(const Rect& = {});
	DefDeMoveCtor(Panel)

		virtual void
		operator+=(IWidget&);

	virtual bool
		operator-=(IWidget&);

	using MUIContainer::Contains;

	/*!
	\since build 357
	\note ��ʹ�� DefWidgetChildrenGetter �Ա���ԷǱ�Ҫ�ļ���������
	*/
	DefGetter(override, WidgetRange, Children, WidgetRange(begin(), end()))

		/*!
		\brief ��ָ�� Z ˳���򲿼�����Ӳ�����������ָ�롣
		\sa MUIContainer::Add
		\since build 555
		*/
		virtual void
		Add(IWidget&, ZOrder = DefaultZOrder);

	/*!
	\brief ������ݡ�

	�������ָ��Ͳ����鲢��Ч����
	*/
	void
		ClearContents();

	/*!
	\brief ���������� Z ˳����ȵ�ͬ�鲿���Ķ��ˡ�
	\since build 467

	�Ӳ������в���ָ�����������²��������ˡ�
	*/
	bool
		MoveToFront(IWidget&);

	//! \since build 496
	using MUIContainer::QueryZ;

	/*!
	\brief ˢ�£���ָ���������ƽ��沢����״̬��
	\since build 294
	*/
	void
		Refresh(PaintEventArgs&&) override;

	//!
	//@{
	using MUIContainer::begin;

	using MUIContainer::end;
	//@}
};

HUD_END

LEO_END


#endif
