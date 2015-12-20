#ifndef UI_TEXTBASE_H
#define UI_TEXTBASE_H

#include "GUI.h"
#include "Color.h"
#include "Font.hpp"

LEO_DRAW_BEGIN

/*!
\brief 默认边距。
*/
lconstexpr const Padding DefaultMargin(2, 2, 2, 2);


/*!
\brief 笔样式：字体和笔颜色。
\warning 非虚析构。
\since build 145
*/
class LB_API PenStyle
{
public:
	Drawing::Font Font; //!< 字体。
	Drawing::Color Color; //!< 笔颜色。

						  /*!
						  \brief 构造：使用指定字体家族、字体大小和颜色。
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
\brief 文本状态。

包含笔样式、显示区域边界、字体缓存和行距。
文本区域指文本状态描述的平面区域。
文本区域位置坐标是文本区域左上角为原点的屏幕坐标系。
笔位置以文本区域位置坐标表示。
显示区域为文本区域内部实际显示文本光栅化结果的区域。
边距描述显示区域和文本区域的位置关系。
文本状态不包含文本区域和显示区域的大小，应由外部图形接口上下文或缓冲区状态确定。
*/
class LB_API TextState : protected PenStyle
{
public:
	//! \since build 371
	using PenStyle::Font;
	//! \since build 371
	using PenStyle::Color;
	Padding Margin; //!< 边距：文本区域到显示区域的距离。
					/*!
					\brief 笔坐标。
					\since build 371
					*/
	Point Pen{};
	std::uint8_t LineGap = 0; //!< 行距。

							  /*!
							  \brief 构造：使用指定字体和边距。
							  \since build 515
							  */
	explicit
		TextState(const Drawing::Font& = {}, const Padding& = DefaultMargin);
	/*!
	\brief 构造：使用指定字体缓存和边距。
	\since build 515
	*/
	explicit
		TextState(FontCache&, const Padding& = DefaultMargin);

	/*!
	\brief 赋值：笔样式。
	*/
	PDefHOp(TextState&, =, const PenStyle& ps)
		ImplRet(PenStyle::operator=(ps), *this)
		/*!
		\brief 赋值：边距。
		*/
		PDefHOp(TextState&, =, const Padding& ms)
		ImplRet(Margin = ms, *this)

		/*!
		\brief 取当前状态下继续在行内打印指定字符占据的边界。
		\note 不考虑换行。
		\since build 486
		*/
		PDefH(Rect, GetCharBounds, char32_t c) const lnothrow
		ImplRet(Rect(Pen.X, Pen.Y - Font.GetAscender(),
			SDst(Font.GetAdvance(c)), Font.GetHeight()))

		/*!
		\brief 打印换行。
		*/
		void
		PutNewline();

	/*!
	\brief 复位笔：按字体大小设置笔位置为默认位置。
	\note 默认笔位置在由边距约束的显示区域左上角。
	*/
	void
		ResetPen();
	/*!
	\brief 按指定显示区域左上角位置和附加边距重新设置笔位置。
	\since build 372

	通过已有的区域左上角坐标和附加边距的左和上分量约束新和笔位置。
	*/
	void
		ResetPen(const Point&, const Padding& = {});
};

//! \relates TextState
//@{
/*!
\brief 取当前指定文本状态的字体设置对应的行高。
\since build 231
*/
inline PDefH(SDst, GetTextLineHeightOf, const TextState& ts)
ImplRet(ts.Font.GetHeight())

/*!
\brief 取当前指定文本状态的字体设置对应的行高与行距之和。
\since build 231
*/
inline PDefH(SDst, GetTextLineHeightExOf, const TextState& ts)
ImplRet(SDst(ts.Font.GetHeight() + ts.LineGap))

/*!
\brief 取笔所在的当前行数。
\since build 587
*/
inline PDefH(ptrdiff_t, GetCurrentTextLineNOf, const TextState& ts)
ImplRet((ts.Pen.Y - ts.Margin.Top) / SPos(GetTextLineHeightExOf(ts)))

/*!
\brief 取第一行的基线位置（纵坐标）。
\since build 372
*/
inline PDefH(SPos, GetTextLineBaseOf, const TextState& ts)
ImplRet(ts.Margin.Top + SPos(ts.Font.GetAscender()))

/*!
\brief 设置笔位置。
\since build 231
*/
inline PDefH(void, SetPenOf, TextState& ts, SPos x, SPos y)
ImplExpr(ts.Pen = Point(x, y))

/*!
\brief 设置笔的行位置。
\since build 231
*/
LB_API void
SetCurrentTextLineNOf(TextState&, std::uint16_t);

/*!
\brief 按指定显示区域和文本区域的宽重新设置右边距。
\note 右边距参与行的结尾位置计算。
\note 单独使用时，需要设置笔的起始位置（横坐标），否则只适用于单行显示。
\sa Drawing::PutChar
\since build 372
*/
inline PDefH(void, AdjustEndOfLine, TextState& ts, const Rect& r, SDst w)
// XXX: Conversion to 'SPos' might be implementation-defined.
ImplExpr(ts.Margin.Right = SPos(w) - r.X - SPos(r.Width))
//	ts.Margin = FetchMargin(r, s);

/*!
\brief 回车。

指定文本状态的笔的水平位置移至左端，竖直位置不变。
\since build 270
*/
inline PDefH(void, CarriageReturn, TextState& ts)
ImplExpr(ts.Pen.X = ts.Margin.Left)

/*!
\brief 按字符跨距移动笔。
*/
LB_API void
MovePen(TextState&, char32_t);
//@}


LEO_DRAW_END

#endif
