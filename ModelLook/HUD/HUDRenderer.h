////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2015.
// -------------------------------------------------------------------------
//  File name:   HUD/HUDRenderer.h
//  Version:     v1.00
//  Created:     11/29/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: HUD������Ⱦ��
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef HUD_RENDERER_H
#define HUD_RENDERER_H

#include <utility.hpp>
#include "WidgetEvent.h"

LEO_BEGIN
HUD_BEGIN

/*!
\brief HUD������Ⱦ��
\note ��״̬
*/
class LB_API HUDRenderer {
public:
	DefDeCtor(HUDRenderer)
	DefDeCopyMoveCtorAssignment(HUDRenderer)
	DefVrDtor(HUDRenderer)
	/*!
	\brief ���û�������С
	\note  ��ʵ�֡�
	*/
	virtual void SetSize(const Size&)
	{}

	/*
	\brief ���������Ʋ���
	*/
	virtual Rect Paint(IWidget& wgt,PaintEventArgs&&);
};

/*!
\brief α��Ⱦ��
*/
class LB_API HUDPseudoRenderer : public HUDRenderer
{
	~HUDPseudoRenderer() override;
	PDefH(Rect,Paint,IWidget&,PaintEventArgs&&) override
		ImplRet({})
};

/*!
\brief ��Ⱦ��D3D RT�����ϵ���Ⱦ��
\note D3D RT�����������
*/
class LB_API HUDD3DRTRenderer:public HUDRenderer
{
public:
	/*!
	\brief ���û�������С��
	*/
	void SetSize(const Size&) override;

	/*
	\brief ���������Ʋ���
	*/
	Rect Paint(IWidget& wgt, PaintEventArgs&&) override;
};

HUD_END
LEO_END

#endif
