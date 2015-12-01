//  File name:   HUD/Label.hpp
//  Version:     v1.00
//  Created:     12/1/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: ��׼�����¼�����

#ifndef HUD_WIDGETEVENT_HPP
#define HUD_WIDGETEVENT_HPP

#include "HUD.h"
#include "HUDEvent.hpp"

LEO_BEGIN
namespace HUD
{
	/*!
	\brief �û�����������ȼ���
	\since build 294
	*/
	//@{
	lconstexpr const EventPriority BackgroundPriority(0xC0);
	lconstexpr const EventPriority BoundaryPriority(0x60);
	lconstexpr const EventPriority ForegroundPriority(0x40);
	//@}

	/*!
	\brief �û������¼��������ࡣ
	\since build 255
	*/
	struct LB_API UIEventArgs
	{
	private:
		IWidget* pSender;

	public:
		explicit
			UIEventArgs(IWidget& wgt)
			: pSender(&wgt)
		{}
		/*!
		\brief ���ƹ��죺Ĭ��ʵ�֡�
		\since build 295
		*/
		DefDeCopyCtor(UIEventArgs)
			/*!
			\brief ���������ඨ����Ĭ��ʵ�֡�
			\since build 423
			*/
			virtual ~UIEventArgs();

		/*!
		\brief ���Ƹ�ֵ��Ĭ��ʵ�֡�
		\since build 295
		*/
		DefDeCopyAssignment(UIEventArgs)

			DefGetter(const lnothrow, IWidget&, Sender, *pSender)
			PDefH(void, SetSender, IWidget& wgt)
			ImplExpr(pSender = &wgt)
	};
}
LEO_END

#endif
