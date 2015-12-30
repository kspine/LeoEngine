////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2015.
// -------------------------------------------------------------------------
//  File name:   HUD/Label.hpp
//  Version:     v1.00
//  Created:     11/24/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: HUD��ǩ
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#ifndef HUD_LABEL_H
#define HUD_LABEL_H

#include "Widget.h"
#include "HUDBrush.h"
#include "..\UI\Font.hpp"
#include "..\UI\TextBase.h"

LEO_BEGIN

HUD_BEGIN

using String = Drawing::String;

//�ı�������ʽ
enum class TextAlignment
{
	Left = 1,
	Center = 2,
	Right = 3,
	Up = 4,
	Down = 5
};

class LB_API MLabel {
public:
	/*!
	\brief ��ǰʹ�õļ����ı���������
	\note ������ģ�������ʾ�Ƿ��Զ����С�
	\note �� DrawText ���á�
	*/
	GBrushUpdater<Drawing::TextState&, const String&, bool>
		UpdateClippedText{ DefaultUpdateClippedText };

	Drawing::Color ForeColor{ Drawing::ColorSpace::Black };
	Drawing::Font Font;
	Drawing::Padding Margin{ Drawing::DefaultMargin };

	TextAlignment HorizontalAlignment,
		VerticalAlignment = TextAlignment::Center;

	/*!
	\brief �����Զ����С�
	*/
	bool AutoWrapLine = {};

	String Text{};

	

	explicit
	MLabel(const Drawing::Font& = {},Drawing::Color = Drawing::ColorSpace::Black,
			TextAlignment = TextAlignment::Left);
	DefDeMoveCtor(MLabel)

	/*!
	\brief �軭��ʹ�÷����ߵĴ�С������ DrawText �����ı���
	*/
	void
	operator()(PaintEventArgs&&) const;

	/*!
	\brief ������ָ���ı߽��С�͵�ǰ״̬����ʵ�ƫ��λ�á�
	*/
	Point
	GetAlignedPenOffset(const Size&) const;

	/*!
	\brief �����ı���
	\sa AlignPen
	\sa DrawClipText
	*/
	void
	DrawText(const Size&, const PaintContext&) const;

	/*!
	\brief ���Ƽ����ı���ʹ��ָ���Ļ��������ĺ��ı�״̬��
	*/
	static void
		DefaultUpdateClippedText(const PaintContext&, Drawing::TextState&,
			const String&, bool);
	//@}
};

class LB_API Label : public Widget, protected MLabel
{
public:
	using MLabel::Font;
	using MLabel::Margin;
	using MLabel::HorizontalAlignment;
	using MLabel::VerticalAlignment;
	using MLabel::Text;

	explicit
		Label(const Rect& r = {}, const Drawing::Font& fnt = {},
			HBrush b = MakeAlphaBrush())
		: Widget(r, b), MLabel(fnt)
	{}
	DefDeMoveCtor(Label)

	/*!
	\brief ����ָ���ַ�������ʼ�߽硢����ĵ����ı���Ҫ�ı�ǩ�߽硣
	\note ������ʼ�߽�Ĵ�СΪ Size::Invalid ʱ�����µĴ�С��
	*/
	static Rect
	CalculateBounds(const String&, Rect, const Drawing::Font&,const Drawing::Padding& m);


	/*!
	\brief ˢ�£���ָ���������ƽ��沢����״̬��
	\since build 294
	*/
	void
	Refresh(PaintEventArgs&&) override;
};


LB_API std::unique_ptr<Label>
MakeLabel(const String&, const Rect& = Rect::Invalid, const Drawing::Font& = {}, const Drawing::Padding& = Drawing::DefaultMargin * 2);

HUD_END

LEO_END


#endif
