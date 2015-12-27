#ifndef UI_TEXTBASE_H
#define UI_TEXTBASE_H

#include "GUI.h"
#include "Color.h"
#include "Font.hpp"
#include <CHRLib\CharacterProcessing.h>

namespace Text
{
	using std::string;
	using std::wstring;
	using leo::sfmt;
	using leo::vsfmt;

	using leo::to_string;
	using leo::to_wstring;
	using std::to_string;
	using std::to_wstring;

	using namespace CHRLib;
	
	/*!
	\brief YSLib �����ַ�����ʹ�� UCS-2 ����
	*/
	using ucs2string = std::basic_string<ucs2_t>;

	/*!
	\brief YSLib ��׼�ַ�����ʹ�� UCS-2 ��Ϊ�ڲ����룩��
	\warning ����������
	*/
	class LB_API String : public ucs2string
	{
	public:
		/*!
		\brief �޲������죺Ĭ��ʵ�֡�
		*/
		DefDeCtor(String)
			using ucs2string::ucs2string;
		/*!
		\brief ���죺ʹ���ַ�ָ���ʾ�� NTCTS ��ָ�����롣
		*/
		template<typename _tChar>
		String(const _tChar* s, Encoding enc = CS_Default)
			: ucs2string(MakeUCS2LE<ucs2string>(s, enc))
		{}
		/*!
		\brief ���죺ʹ���ַ��ĳ�ֵ���б�
		*/
		template<typename _tChar>
		String(std::initializer_list<_tChar> il)
			: ucs2string(il.begin(), il.end())
		{}
		/*!
		\brief ���죺ʹ�� YSLib �����ַ�����
		*/
		String(const ucs2string& s)
			: ucs2string(s)
		{}
		/*!
		\brief ���죺ʹ�� YSLib �����ַ�����ֵ���á�
		\since build 285
		*/
		String(ucs2string&& s)
			: ucs2string(std::move(s))
		{}
		/*!
		\brief ���죺ʹ��ָ���ַ����͵� std::basic_string ��ָ�����롣
		\since build 281
		*/
		template<typename _tChar>
		String(const std::basic_string<_tChar>& s, Encoding enc = CS_Default)
			: String(s.c_str(), enc)
		{}
		/*!
		\brief ���ƹ��죺Ĭ��ʵ�֡�
		*/
		DefDeCopyCtor(String)
			/*!
			\brief ת�ƹ��죺Ĭ��ʵ�֡�
			*/
			DefDeMoveCtor(String)
			DefDeDtor(String)

			/*!
			\brief ���Ƹ�ֵ��Ĭ��ʵ�֡�
			*/
			DefDeCopyAssignment(String)
			/*!
			\brief ת�Ƹ�ֵ��Ĭ��ʵ�֡�
			*/
			DefDeMoveAssignment(String)

			/*!
			\brief �ظ����ӡ�
			*/
			String&
			operator*=(size_t);

		/*!
		\brief ȡָ������Ķ��ֽ��ַ�����
		*/
		PDefH(string, GetMBCS, Encoding enc = CS_Default) const
			ImplRet(MakeMBCS<string>(*this, enc))
	};

	/*!
	\relates String
	*/
	inline PDefHOp(String, *, const String& str, size_t n)
		ImplRet(String(str) *= n)
}


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
	using PenStyle::Font;
	using PenStyle::Color;
	Padding Margin; //!< �߾ࣺ�ı�������ʾ����ľ��롣
					/*!
					\brief �����ꡣ
					*/
	Point Pen{};
	std::uint8_t LineGap = 0; //!< �оࡣ

							  /*!
							  \brief ���죺ʹ��ָ������ͱ߾ࡣ
							  */
	explicit
		TextState(const Drawing::Font& = {}, const Padding& = DefaultMargin);
	/*!
	\brief ���죺ʹ��ָ�����建��ͱ߾ࡣ
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
