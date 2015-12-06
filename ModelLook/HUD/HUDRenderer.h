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
class LB_API HUDRenderer : public cloneable {
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

	DefClone(const ImplI(cloneable), HUDRenderer)

	/*!
	\brief �ύ��Ч����
	*/
	virtual PDefH(Rect, CommitInvalidation, const Rect& r)
	ImplRet(r)

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
public:
	DefDeCtor(HUDPseudoRenderer)
	DefDeCopyMoveCtorAssignment(HUDPseudoRenderer)

	~HUDPseudoRenderer() override;

	DefClone(const ImplI(cloneable), HUDPseudoRenderer)

	PDefH(Rect,Paint,IWidget&,PaintEventArgs&&) override
		ImplRet({})
};

/*!
\brief ��Ⱦ��D3D RT�����ϵ���Ⱦ��
\note D3D RT�����������
\note PaintContext ���� ��������?
*/
class LB_API BufferedRenderer :public HUDRenderer
{
protected:
	//!<��Ч����:���������»������������(GDIʵ����Ч)��
	mutable Rect rInvalidated;

	/*!
	\brief RT����
	*/
	std::shared_ptr<IImage> pImageBuffer;
public:
	/*!
	\brief ָ����֤ʱ�����ϲ㻺����������
	*/
	bool IgnoreBackground = true;

	/*!
	\brief ���죺ָ���Ƿ�����ϲ㻺����������
	\note ��ָ��Ϊ��ʱ�½���������
	*/
	BufferedRenderer(bool = {},std::shared_ptr<IImage> = {});
	//note ��copyһ��IImage
	BufferedRenderer(const BufferedRenderer&);
	DefDeMoveCtor(BufferedRenderer)

	/*!
	\brief �ж��Ƿ���Ҫˢ�¡�
	\note ����Ч���򳤿���Ϊ�㣬����Ҫˢ�¡�
	*/
	bool
	RequiresRefresh() const;

	DefGetter(const lnothrow, IImage&, ImageBuffer, *pImageBuffer)
	/*!
	\brief ȡ��Ч����
	*/
	DefGetter(const lnothrow, const Rect&, InvalidatedArea, rInvalidated)
	/*!
	\brief ȡͼ�νӿ������ġ�
	\warning ��GDIʵ�ֿ���Ϊ�ա�
	\return ������ͼ�νӿ������ġ�
	*/
	DefGetterMem(const lnothrow, Graphics, Context, GetImageBuffer())

	/*!
	\brief ���û�������С��
	\warning ���ܵ���ԭ������ָ��ʧЧ��
	*/
	void
	SetSize(const Size&) override;
	void
	SetImageBuffer(std::shared_ptr<IImage>,bool clone = false);

	DefClone(const override, BufferedRenderer)

	/*!
	\brief �ύ��Ч����ʹ֮�ϲ���������Ч�����С�
	\return �µ���Ч����
	\note ������Ч�������״���ƣ����ܻ���ڲ�����Ч���򱻺ϲ���
	*/
	Rect
	CommitInvalidation(const Rect&) override;

	/*!
	\brief ���������ơ�
	\pre ��Ӷ��ԣ� <tt>&e.GetSender().GetRenderer() == this</tt> ��
	\note �� Validate �� Update ��
	\note ����鲿���ɼ��ԡ�
	*/
	Rect Paint(IWidget& wgt, PaintEventArgs&&) override;

	/*!
	\brief ������ָ��ͼ���豸�����ĵ�ָ���㡣
	\note ������ʾ���������ݡ�
	*/
	void
	UpdateTo(const PaintContext&) const;

	/*!
	\brief ��֤��������ơ�
	\pre ���ԣ� <tt>&sender.GetRenderer() == this</tt> ��
	\return ��֤�б�ˢ�µ�����߽硣

	��֤ sender ��ָ��ͼ�νӿ������ĵĹ����Ļ�������
	��������Ч�������½� PaintEventArgs �� ���� wgt �� Paint �¼����ơ�
	*/
	Rect
	Validate(IWidget& wgt, IWidget& sender, const PaintContext&);
};

HUD_END
LEO_END

#endif
