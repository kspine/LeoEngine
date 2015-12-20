#ifndef UI_TEXTBASE_H
#define UI_TEXTBASE_H

#include "GUI.h"
#include "Color.h"
#include "Font.hpp"

LEO_DRAW_BEGIN

/*!
\brief Ĭ�ϱ߾ࡣ
*/
lconstexpr const Padding DefaultMargin(2, 2, 2, 2);


/*!
\brief ����ʽ������ͱ���ɫ��
\warning ����������
\since build 145
*/
class LB_API PenStyle
{
public:
	Drawing::Font Font; //!< ���塣
	Drawing::Color Color; //!< ����ɫ��

						  /*!
						  \brief ���죺ʹ��ָ��������塢�����С����ɫ��
						  */
	explicit
		PenStyle(const Drawing::Font& fnt = {},
			Drawing::Color c = Drawing::ColorSpace::White)
		: Font(fnt), Color(c)
	{}

	DefGetterMem(const lnothrow, const FontFamily&, FontFamily, Font)
	DefGetterMem(const lnothrow, FontCache&, Cache, Font)
};


/*!
\brief �ı�״̬��

��������ʽ����ʾ����߽硢���建����оࡣ
�ı�����ָ�ı�״̬������ƽ������
�ı�����λ���������ı��������Ͻ�Ϊԭ�����Ļ����ϵ��
��λ�����ı�����λ�������ʾ��
��ʾ����Ϊ�ı������ڲ�ʵ����ʾ�ı���դ�����������
�߾�������ʾ������ı������λ�ù�ϵ��
�ı�״̬�������ı��������ʾ����Ĵ�С��Ӧ���ⲿͼ�νӿ������Ļ򻺳���״̬ȷ����
*/
class LB_API TextState : protected PenStyle
{
public:
	//! \since build 371
	using PenStyle::Font;
	//! \since build 371
	using PenStyle::Color;
	Padding Margin; //!< �߾ࣺ�ı�������ʾ����ľ��롣
					/*!
					\brief �����ꡣ
					\since build 371
					*/
	Point Pen{};
	std::uint8_t LineGap = 0; //!< �оࡣ

							  /*!
							  \brief ���죺ʹ��ָ������ͱ߾ࡣ
							  \since build 515
							  */
	explicit
		TextState(const Drawing::Font& = {}, const Padding& = DefaultMargin);
	/*!
	\brief ���죺ʹ��ָ�����建��ͱ߾ࡣ
	\since build 515
	*/
	explicit
		TextState(FontCache&, const Padding& = DefaultMargin);

	/*!
	\brief ��ֵ������ʽ��
	*/
	PDefHOp(TextState&, =, const PenStyle& ps)
		ImplRet(PenStyle::operator=(ps), *this)
		/*!
		\brief ��ֵ���߾ࡣ
		*/
		PDefHOp(TextState&, =, const Padding& ms)
		ImplRet(Margin = ms, *this)

		/*!
		\brief ȡ��ǰ״̬�¼��������ڴ�ӡָ���ַ�ռ�ݵı߽硣
		\note �����ǻ��С�
		\since build 486
		*/
		PDefH(Rect, GetCharBounds, char32_t c) const lnothrow
		ImplRet(Rect(Pen.X, Pen.Y - Font.GetAscender(),
			SDst(Font.GetAdvance(c)), Font.GetHeight()))

		/*!
		\brief ��ӡ���С�
		*/
		void
		PutNewline();

	/*!
	\brief ��λ�ʣ��������С���ñ�λ��ΪĬ��λ�á�
	\note Ĭ�ϱ�λ�����ɱ߾�Լ������ʾ�������Ͻǡ�
	*/
	void
		ResetPen();
	/*!
	\brief ��ָ����ʾ�������Ͻ�λ�ú͸��ӱ߾��������ñ�λ�á�
	\since build 372

	ͨ�����е��������Ͻ�����͸��ӱ߾������Ϸ���Լ���ºͱ�λ�á�
	*/
	void
		ResetPen(const Point&, const Padding& = {});
};

//! \relates TextState
//@{
/*!
\brief ȡ��ǰָ���ı�״̬���������ö�Ӧ���иߡ�
\since build 231
*/
inline PDefH(SDst, GetTextLineHeightOf, const TextState& ts)
ImplRet(ts.Font.GetHeight())

/*!
\brief ȡ��ǰָ���ı�״̬���������ö�Ӧ���и����о�֮�͡�
\since build 231
*/
inline PDefH(SDst, GetTextLineHeightExOf, const TextState& ts)
ImplRet(SDst(ts.Font.GetHeight() + ts.LineGap))

/*!
\brief ȡ�����ڵĵ�ǰ������
\since build 587
*/
inline PDefH(ptrdiff_t, GetCurrentTextLineNOf, const TextState& ts)
ImplRet((ts.Pen.Y - ts.Margin.Top) / SPos(GetTextLineHeightExOf(ts)))

/*!
\brief ȡ��һ�еĻ���λ�ã������꣩��
\since build 372
*/
inline PDefH(SPos, GetTextLineBaseOf, const TextState& ts)
ImplRet(ts.Margin.Top + SPos(ts.Font.GetAscender()))

/*!
\brief ���ñ�λ�á�
\since build 231
*/
inline PDefH(void, SetPenOf, TextState& ts, SPos x, SPos y)
ImplExpr(ts.Pen = Point(x, y))

/*!
\brief ���ñʵ���λ�á�
\since build 231
*/
LB_API void
SetCurrentTextLineNOf(TextState&, std::uint16_t);

/*!
\brief ��ָ����ʾ������ı�����Ŀ����������ұ߾ࡣ
\note �ұ߾�����еĽ�βλ�ü��㡣
\note ����ʹ��ʱ����Ҫ���ñʵ���ʼλ�ã������꣩������ֻ�����ڵ�����ʾ��
\sa Drawing::PutChar
\since build 372
*/
inline PDefH(void, AdjustEndOfLine, TextState& ts, const Rect& r, SDst w)
// XXX: Conversion to 'SPos' might be implementation-defined.
ImplExpr(ts.Margin.Right = SPos(w) - r.X - SPos(r.Width))
//	ts.Margin = FetchMargin(r, s);

/*!
\brief �س���

ָ���ı�״̬�ıʵ�ˮƽλ��������ˣ���ֱλ�ò��䡣
\since build 270
*/
inline PDefH(void, CarriageReturn, TextState& ts)
ImplExpr(ts.Pen.X = ts.Margin.Left)

/*!
\brief ���ַ�����ƶ��ʡ�
*/
LB_API void
MovePen(TextState&, char32_t);
//@}


LEO_DRAW_END

#endif
