#ifndef UI_TextRenderer_h
#define UI_TextRenderer_h

#include "TextBase.h"
#include "TextLayout.h"
#include "CharRenderer.h"

LEO_DRAW_BEGIN

using String = std::string;

/*!
\note ʹ�� ADL <tt>GetEndOfLineOffsetOf</tt> �ж���βλ�á�
\note ʹ�� ADL <tt>PrintChar</tt> �� <tt>PutChar</tt> ��Ⱦ�ַ���
*/
//@{
/*!
\brief ��ӡ������ָ������ʼ�ַ����ַ�����ֱ����β���ַ�������ֹ��
\param rd ʹ�õ��ַ���Ⱦ����
\param s ָ���ַ�����ʼ�ַ��������������
\return ָ�����λ�õĵ�������
\note ����ֱ���ַ�����������
*/
template<typename _tIter, class _tRenderer,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	_tIter
	PrintLine(_tRenderer& rd, _tIter s)
{
	while (*s != 0 && *s != '\n')
	{
		PrintChar(rd, *s);
		++s;
	}
	return s;
}
/*!
\brief ��ӡ������ָ������ʼ�ַ����ַ�����ֱ����β���ַ�������ֹ��
\param rd ʹ�õ��ַ���Ⱦ����
\param s ָ���ַ�����ʼ�ַ��������������
\param g ָʾ������ֹλ�õ������������
\param c ָ�������ֹ���ַ���
\return ָ�����λ�õĵ�������
\note ����ֱ�� g ָ����λ�û�ָ��λ�õ��ַ�Ϊ c ��
*/
template<typename _tIter, class _tRenderer,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	_tIter
	PrintLine(_tRenderer& rd, _tIter s, _tIter g, ucs4_t c = {})
{
	while (s != g && ucs4_t(*s) != c && *s != '\n')
	{
		PrintChar(rd, *s);
		++s;
	}
	return s;
}
/*!
\brief ��ӡ�ַ�����ֱ����β���ַ���������
\param rd ʹ�õ��ַ���Ⱦ����
\param str ��������ַ�����
\return ��ӡ�ַ�����
*/
template<class _tRenderer, class _tString,
	limpl(typename = enable_for_string_class_t<_tString>)>
	inline String::size_type
	PrintLine(_tRenderer& rd, const _tString& str)
{
	return String::size_type(Drawing::PrintLine(rd, &str[0]) - &str[0]);
}

/*!
\brief ��ӡ������ָ������ʼ�ַ����ַ�����ֱ����β���ַ�������ֹ��
\param rd ʹ�õ��ַ���Ⱦ����
\param s ָ���ַ�����ʼ�ַ��������������
\return ָ�����λ�õĵ�������
\note ����ֱ���ַ�����������
\note �������޷����������ַ�ʱ���С�
*/
template<typename _tIter, class _tRenderer,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	_tIter
	PutLine(_tRenderer& rd, _tIter s)
{
	const SPos seol(GetEndOfLineOffsetOf(rd));

	if (seol >= 0)
	{
		TextState& ts(rd.GetTextState());
		const SPos fpy(ts.Pen.Y);

		while (*s != 0 && fpy == ts.Pen.Y)
			if (PutChar(rd, *s, SDst(seol)) != PutCharResult::NeedNewline)
				++s;
	}
	return s;
}
/*!
\brief ��ӡ������ָ������ʼ�ַ����ַ�����ֱ����β���ַ�������ֹ��
\param rd ʹ�õ��ַ���Ⱦ����
\param s ָ���ַ�����ʼ�ַ��������������
\param g ָʾ������ֹλ�õ������������
\param c ָ�������ֹ���ַ���
\return ָ�����λ�õĵ�������
\note ����ֱ�� g ָ����λ�û�ָ��λ�õ��ַ�Ϊ c ��
\note �������޷����������ַ�ʱ���С�
*/
template<typename _tIter, class _tRenderer,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	_tIter
	PutLine(_tRenderer& rd, _tIter s, _tIter g, ucs4_t c = {})
{
	const SPos seol(GetEndOfLineOffsetOf(rd));

	if (seol >= 0)
	{
		TextState& ts(rd.GetTextState());
		const SPos fpy(ts.Pen.Y);

		while (s != g && ucs4_t(*s) != c && fpy == ts.Pen.Y)
			if (PutChar(rd, *s, SDst(seol)) != PutCharResult::NeedNewline)
				++s;
	}
	return s;
}
/*!
\brief ��ӡ�ַ�����ֱ����β���ַ���������
\param rd ʹ�õ��ַ���Ⱦ����
\param str ��������ַ�����
\return ��ӡ�ַ�����
\note �������޷����������ַ�ʱ���С�
*/
template<class _tRenderer, class _tString,
	limpl(typename = enable_for_string_class_t<_tString>)>
	inline String::size_type
	PutLine(_tRenderer& rd, const _tString& str)
{
	return String::size_type(Drawing::PutLine(rd, &str[0]) - &str[0]);
}

/*!
\brief ��ӡ������ָ������ʼ�ַ����ַ�����ֱ������ĩβ���ַ�������ֹ��
\param rd ʹ�õ��ַ���Ⱦ����
\param s ָ���ַ�����ʼ�ַ��������������
\return ָ�����λ�õĵ�������
\note ����ֱ���ַ�����������
*/
template<typename _tIter, class _tRenderer,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	_tIter
	PrintString(_tRenderer& rd, _tIter s)
{
	while (*s != 0 && *s != '\n')
		PrintChar(rd, *s++);
	return s;
}
/*!
\brief ��ӡ������ָ������ʼ�ַ����ַ�����ֱ������ĩβ���ַ�������ֹ��
\param rd ʹ�õ��ַ���Ⱦ����
\param s ָ���ַ�����ʼ�ַ��������������
\param g ָʾ������ֹλ�õ������������
\param c ָ�������ֹ���ַ���
\return ָ�����λ�õĵ�������
\note ����ֱ�� g ָ����λ�û�ָ��λ�õ��ַ�Ϊ c ��
*/
template<typename _tIter, class _tRenderer,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	_tIter
	PrintString(_tRenderer& rd, _tIter s, _tIter g, ucs4_t c = {})
{
	while (s != g && ucs4_t(*s) != c && *s != '\n')
		PrintChar(rd, *s++);
	return s;
}
/*!
\brief ��ӡ�ַ�����ֱ������ĩβ���ַ���������
\param rd ʹ�õ��ַ���Ⱦ����
\param str ��������ַ�����
\return ��ӡ�ַ�����
*/
template<class _tRenderer, class _tString,
	limpl(typename = enable_for_string_class_t<_tString>)>
	inline String::size_type
	PrintString(_tRenderer& rd, const _tString& str)
{
	return String::size_type(Drawing::PrintString(rd, &str[0]) - &str[0]);
}

/*!
\brief ��ӡ������ָ������ʼ�ַ����ַ�����ֱ������ĩβ���ַ�������ֹ��
\param rd ʹ�õ��ַ���Ⱦ����
\param s ָ���ַ�����ʼ�ַ��������������
\return ָ�����λ�õĵ�������
\note ����ֱ���ַ�����������
\note �������޷����������ַ�ʱ���С�
*/
template<typename _tIter, class _tRenderer,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	_tIter
	PutString(_tRenderer& rd, _tIter s)
{
	const SPos seol(GetEndOfLineOffsetOf(rd));

	if (seol >= 0)
	{
		TextState& ts(rd.GetTextState());
		const SPos mpy(FetchLastLineBasePosition(ts, rd.GetHeight()));

		while (*s != 0 && ts.Pen.Y <= mpy)
			if (PutChar(rd, *s, SDst(seol)) != PutCharResult::NeedNewline)
				++s;
	}
	return s;
}
/*!
\brief ��ӡ������ָ������ʼ�ַ����ַ�����ֱ������ĩβ���ַ�������ֹ��
\param rd ʹ�õ��ַ���Ⱦ����
\param s ָ���ַ�����ʼ�ַ��������������
\param g ָʾ������ֹλ�õ������������
\param c ָ�������ֹ���ַ���
\return ָ�����λ�õĵ�������
\note ����ֱ�� g ָ����λ�û�ָ��λ�õ��ַ�Ϊ c ��
\note �������޷����������ַ�ʱ���С�
*/
template<typename _tIter, class _tRenderer,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	_tIter
	PutString(_tRenderer& rd, _tIter s, _tIter g, ucs4_t c = {})
{
	const SPos seol(GetEndOfLineOffsetOf(rd));

	if (seol >= 0)
	{
		TextState& ts(rd.GetTextState());
		const SPos mpy(FetchLastLineBasePosition(ts, rd.GetHeight()));

		while (s != g && ucs4_t(*s) != c && ts.Pen.Y <= mpy)
			if (PutChar(rd, *s, SDst(seol)) != PutCharResult::NeedNewline)
				++s;
	}
	return s;
}
/*!
\brief ��ӡ�ַ�����ֱ������ĩβ���ַ���������
\param rd ʹ�õ��ַ���Ⱦ����
\param str ��������ַ�����
\return ��ӡ�ַ�����
\note �������޷����������ַ�ʱ���С�
*/
template<class _tRenderer, class _tString,
	limpl(typename = enable_for_string_class_t<_tString>)>
	inline String::size_type
	PutString(_tRenderer& rd, const _tString& str)
{
	return String::size_type(Drawing::PutString(rd, &str[0]) - &str[0]);
}
//@}

/*!
\brief ��ӡ�ı���
\param multi ָ���Ƿ��������С�
\param args ���ݸ� PutString �� PutLine �Ĳ�����
*/
template<typename... _tParams>
void
PutText(bool multi, _tParams&&... args)
{
	if (multi)
		Drawing::PutString(lforward(args)...);
	else
		Drawing::PutLine(lforward(args)...);
}


/*!	\defgroup TextRenderers Text Renderers
\brief �ı���Ⱦ����
*/

/*!
\ingroup TextRenderers
\brief ���ı���Ⱦ����
*/
class LB_API EmptyTextRenderer
{
public:
	TextState& State;
	SDst Height;

	EmptyTextRenderer(TextState& ts, SDst h)
		: State(ts), Height(h)
	{}

	/*!
	\brief ��Ⱦ�����ַ������ƶ��ʣ������ơ�
	*/
	PDefHOp(void, (), ucs4_t c)
		ImplExpr(MovePen(State, c))

		DefGetter(const lnothrow, const TextState&, TextState, State)
		DefGetter(lnothrow, TextState&, TextState, State)
		DefGetter(const lnothrow, SDst, Height, Height)
};


/*!
\brief �ı���Ⱦ����̬��̬����ģ�塣
\warning ����������
*/
template<class _type>
class GTextRendererBase
{
public:
	DeclSEntry(const TextState& GetTextState() const) //!< ȡ�ı�״̬��
		DeclSEntry(TextState& GetTextState()) //!< ȡ�ı�״̬��
											  /*
											  \brief ȡͼ�νӿ������ġ�
											  */
		DeclSEntry(Graphics GetContext() const)

#define This static_cast<_type*>(this)
#define CThis static_cast<const _type*>(this)

		/*!
		\brief ȡ����ǰ�иߺ��о�������ʾ�����������
		*/
		DefGetter(const, std::uint16_t, TextLineN, FetchResizedLineN(
			CThis->GetTextState(), CThis->GetContext().GetHeight()))
		/*!
		\brief ȡ����ǰ�иߺ��оࣨ�м����С������ 1 ��������ʾ�����������
		*/
		DefGetter(const, std::uint16_t, TextLineNEx,
			FetchResizedLineN(CThis->GetTextState(), CThis->GetContext().GetHeight()
				+ CThis->GetTextState().LineGap))

#undef CThis
#undef This

};


/*!
\ingroup TextRenderers
\brief �ı���Ⱦ������ʵ�֡�
\warning ����������
*/
class LB_API TextRenderer : public GTextRendererBase<TextRenderer>
{
public:
	TextState& State;
	const Graphics& Buffer;
	Rect ClipArea;

	TextRenderer(TextState& ts, const Graphics& g)
		: GTextRendererBase<TextRenderer>(),
		State(ts), Buffer(g), ClipArea(g.GetSize())
	{}
	/*
	\brief ���죺ʹ���ı�״̬��ͼ�νӿ������ĺ�ָ������߽硣
	*/
	TextRenderer(TextState& ts, const Graphics& g, const Rect& bounds)
		: GTextRendererBase<TextRenderer>(),
		State(ts), Buffer(g), ClipArea(bounds)
	{}

	/*!
	\brief ��Ⱦ�����ַ���
	*/
	void
		operator()(ucs4_t);

	ImplS(GTextRendererBase) DefGetter(const lnothrow, const TextState&,
		TextState, State)
		ImplS(GTextRendererBase) DefGetter(lnothrow, TextState&, TextState, State)
		ImplS(GTextRendererBase) DefGetter(const lnothrow, Graphics, Context,
			Buffer)
		//@{
		DefGetterMem(const lnothrow, SDst, Height, Buffer)
		DefGetterMem(const lnothrow, SDst, Width, Buffer)
		DefGetterMem(const lnothrow, const Size&, Size, Buffer)
		//@}

		/*!
		\brief ����������� l ����ʼ�� n �����ء�
		\note ͼ�νӿ������Ĳ����û� l Խ��ʱ���ԡ�
		\note n ������Ϊ��Խ�硣
		\note n Ϊ 0 ʱ���֮��������С�
		*/
		void
		ClearLine(size_t l, SDst n);
};

void
DrawClippedText(const Graphics& g, const Rect& bounds, TextState& ts,
	const String& str, bool line_wrap);

LEO_DRAW_END

#endif