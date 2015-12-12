////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2015.
// -------------------------------------------------------------------------
//  File name:   HUD/Widget.hpp
//  Version:     v1.00
//  Created:     11/24/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: HUD��ǩ
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#ifndef HUD_WIDGET_H
#define HUD_WIDGET_H

#include "HUDRenderer.h"

LEO_BEGIN

HUD_BEGIN

using WidgetIterator = any_input_iterator<IWidget>;
using WidgetRange = std::pair<WidgetIterator, WidgetIterator>;

DeclI(LB_API, IWidget)
DeclIEntry(WidgetRange GetChildren())

/*
��ͼ�ӿ�<δ������ͼ>
*/
DeclIEntry(bool IsVisible() const) //�ж��Ƿ�ɼ�
DeclIEntry(bool Contains(platform::unit_type, platform::unit_type) const) //�жϵ��Ƿ��ڲ����Ŀ���������
DeclIEntry(Size GetSizeOf() const) //ȡ������С
DeclIEntry(Box GetBox() const) //ȡ������Χ��
DeclIEntry(Point GetLocationOf() const) //ȡ������С

DeclIEntry(void SetLocationOf(const Point&)) //���ò������Ͻ�λ��
DeclIEntry(void SetSizeOf(const Size&)) //���ò�����С
DeclIEntry(void SetVisible(bool b)) //���ò����ɼ���

/*!
\brief ȡ��������
\note ������Ч����������Լ����
*/
DeclIEntry(AController& GetController() const)
/*!
\brief ȡ��Ⱦ����
\note ������Ч����������Լ����
*/
DeclIEntry(HUDRenderer& GetRenderer() const)
EndDecl

//! \relates IWidget
//@{
/*!
\brief �жϲ����Ƿ�ɼ���
\since build 259
*/
inline PDefH(bool, IsVisible, const IWidget& wgt)
ImplRet(wgt.IsVisible())

LB_API void SetBox(IWidget&, const Box&);
LB_API void SetBoundsOf(IWidget& wgt, const Rect& r);


/*!
\brief ���ò������Ͻ�����λ�ã����������ƫ�����꣩��
\note ������ͼ״̬�󲻻ᴥ�� Move �¼���
*/
LB_API void
SetLocationOf(IWidget&, const Point&);

/*!
\brief ���ò�����С��
\note ����������Ⱦ������ͼ״̬�����ᴥ�� Resize �¼���
*/
LB_API void
SetSizeOf(IWidget&, const Size&);

/*!
\ingroup helper_functions
\brief ȡ����λ�á�
*/
inline PDefH(Point, GetLocationOf, const IWidget& wgt)
ImplRet(wgt.GetLocationOf())

/*!
\ingroup helper_functions
\brief ȡ������С��
*/
inline PDefH(Size, GetSizeOf, const IWidget& wgt)
ImplRet(wgt.GetSizeOf())


/*!
\brief ����ָ���Ӳ����� Paint �¼����Ʋ���ָ�����¼������ߡ�
\sa Renderer::Paint

�� e.Sender() ��Ϊ����Ŀ�꣬�ж���߽��Ƿ������ e.ClipArea �ཻ��
���ཻ����ǿ������ wgt ����Ⱦ���� Paint �������� ��
�����У� e.Location ���޸�Ϊ����Ӳ��������꣬ e.ClipArea ������Ϊ�ཻ����
֮�� e �ɼ����� e.GetSender() ����Ⱦ���� Paint �����޸ġ�
*/
LB_API void
PaintChild(IWidget& wgt, PaintEventArgs&& e);
/*!
\brief ����ָ���Ӳ����� Paint �¼�����ָ���Ӳ�����
\return ʵ�ʻ��Ƶ�����
\note ʹ��ָ���Ӳ�����Ϊ�¼������߲����Ʋ�����

�� wgt ��Ϊ����Ŀ�꣬�ж���߽��Ƿ������ pc.ClipArea �ཻ��
���ཻ����ǿ����½� PaintEventArgs ������ wgt ����Ⱦ���� Paint �������� ��
*/
LB_API Rect
PaintChild(IWidget& wgt, const PaintContext& pc);

/*!
\brief ���� PaintChild ָ���Ӳ������ύ�������ػ�����
*/
LB_API void
PaintChildAndCommit(IWidget&, PaintEventArgs&);

/*!
\brief ���ÿɼ��� PaintChild ָ���Ӳ������ύ�������ػ�����
*/
inline void
PaintVisibleChildAndCommit(IWidget& wgt, PaintEventArgs& e)
ImplExpr(IsVisible(wgt) ? PaintChildAndCommit(wgt, e) : void())
//@}

class LB_API Widget : implements IWidget
{
private:
	std::shared_ptr<HUDRenderer> renderer_ptr;//��Ⱦ��ָ�롣
	std::unique_ptr<AController> controller_ptr;//������ָ�롣

public:
	/*!
	\brief ������
	*/
	mutable HBrush Background;

	explicit
		Widget(const Rect& = {});
	explicit
		Widget(const Rect&, HBrush);
	/*!
	\brief ���죺ʹ����Ⱦ��ָ��Ϳ�����ָ�룬�ޱ�����
	\param pRenderer_ ��Ⱦ��ָ�롣
	\param pController_ ������ָ�롣
	\pre <tt>bool(pRenderer_)</tt> ��
	*/
	template<typename _tRenderer, typename _tController>
	explicit inline
		Widget(const Rect& r = {},_tRenderer&& pRenderer_ = std::make_shared<HUDRenderer>(),
			_tController&& pController_ = {})
		:renderer_ptr(lforward(pRenderer_)),
		controller_ptr(lforward(pController_)), Background()
	{
		LAssertNonnull(renderer_ptr);
		SetBoundsOf(*this, r);
		InitializeEvents();
	}
	/*!
	\brief ���ƹ��죺������ָ��Ϊ������ơ�
	*/
	Widget(const Widget&);
	DefDelMoveCtor(Widget)
	/*!
	\brief ��������ʵ�֡�

	�Զ��ͷŽ�����ͷŲ�����Դ��
	\note ���ڲ��������� WidgetController ���������޷�ʹ�� inline ʵ�֡�
	*/
	virtual
	~Widget();

private:
	/*!
	\brief ��ʼ���¼��顣
	*/
	void InitializeEvents();
public:
	DefGetter(ImplI(IWidget), WidgetRange, Children, WidgetRange())

	/*!
	\brief ȡ͸����ˢ��
	\return HUD����alpha blend��
	\note Ϊ���ٰ���ͷ�ļ���ʹ�÷� inline ʵ�֡�
	\sa SolidBrush
	*/
	static HBrush
	MakeAlphaBrush();
	DefGetter(const ImplI(IWidget), AController&, Controller,
		Deref(controller_ptr))
	DefGetter(const ImplI(IWidget), HUDRenderer&, Renderer, Deref(renderer_ptr))

	DefGetterMem(const lnothrow, platform::unitlength_type, Height, GetSizeOf())
	DefGetterMem(const lnothrow, platform::unitlength_type, Width, GetSizeOf())
	DefGetterMem(const lnothrow, platform::unit_type, X, GetLocationOf())
	DefGetterMem(const lnothrow, platform::unit_type, Y, GetLocationOf())

	DefSetterMem( platform::unitlength_type, Height, GetSizeOf())
	DefSetterMem(platform::unitlength_type, Width, GetSizeOf())
	DefSetterMem(platform::unit_type, X, GetLocationOf())
	DefSetterMem(platform::unit_type, Y, GetLocationOf())

	/*!
	\brief ������Ⱦ��Ϊָ��ָ��ָ��Ķ���ͬʱ������Ⱦ��״̬��
	\note ��ָ��Ϊ�գ���ʹ���Ե�ǰ�����߽��½��� HUDRenderer ����
	*/
	void
	SetRenderer(std::shared_ptr<HUDRenderer>);


	/*!
	\brief ˢ�£���ָ���������ƽ��沢����״̬��
	\note Ĭ�ϰ� GetChildren() �õ��ĵ�������Χ���ƿɼ��Ӳ�����
	\sa PaintContext

	�ɲ���ָ������Ϣ�����¼������ߡ������� ClipArea ��Աָ���߽硣
	�߽��Ϊ��ʾ������ʵ�ֺ��ԣ���Ӧ��֤���ú�߽��ڵ����򱣳�������ʾ״̬��
	���ƽ�������±߽磬��ʾʵ�ʱ����Ƶ�����
	���������ڲ�״̬�ܹ���֤��ʾ״̬���£��򷵻�ʱ�߽�������ܸ�С��
	*/
	virtual void
	Refresh(PaintEventArgs&&);
public:
	//�޳�����ͼ��ɵ�����
	DefIEntryImpl(bool IsVisible() const)
	DefIEntryImpl(bool Contains(platform::unit_type, platform::unit_type) const) 
	DefIEntryImpl(Size GetSizeOf() const) 
	DefIEntryImpl(Box GetBox() const) 
	DefIEntryImpl(Point GetLocationOf() const) 

	DefIEntryImpl(void SetLocationOf(const Point&)) 
	DefIEntryImpl(void SetSizeOf(const Size&)) 
	DefIEntryImpl(void SetVisible(bool b))
private:
	Size mSize;
	Point mLocation;
	bool mVisible;
};


HUD_END

LEO_END


#endif
