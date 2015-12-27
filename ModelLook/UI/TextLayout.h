#ifndef UI_TextLayout_h
#define UI_TextLayout_h

#include "Font.hpp"
#include "TextBase.h"
#include "GUI.h"

LEO_DRAW_BEGIN

/*!
\brief ȡָ���ı�״̬���ı�����ߵ����ĵױ߾ࡣ
\pre ���ԣ� <tt>GetTextLineHeightExOf(ts) != 0</tt> ��
\post <tt>ts.Margin.Bottom</tt> ��С��ԭֵ��
\return ���ص�����ĵױ߾�ֵ���������С���о�͸߾�������
*/
LB_API SDst
FetchResizedBottomMargin(const TextState&, SDst);

/*!
\brief ȡָ���ı�״̬���ı������������ʾ������ı�������
\pre ���ԣ� <tt>GetTextLineHeightExOf(ts) != 0</tt> ��
\return ��������ɵ��ı�������
*/
LB_API std::uint16_t
FetchResizedLineN(const TextState& ts, SDst);

/*!
\brief ȡָ���ı�״̬��ָ���ߵ������б�ʾ������еĻ���λ�ã������꣩��
\note ������һ�����������Ϊ���С�
\warning �����߾���ȷ�ԡ������߾���ȷ���򷵻�ֵӦС������ĸߡ�
*/
LB_API SPos
FetchLastLineBasePosition(const TextState&, SDst);


/*!
\brief ȡָ�����ַ�ʹ��ָ���������ʾ��ȡ�
\note �ޱ߽����ơ�
*/
LB_API SDst
FetchCharWidth(const Font&, ucs4_t);

/*!
\brief ȡ������ָ���ĵ����ַ���������ָ�����ޱ߽�����ʱ����ʾ��ȡ�
\note ������ s ָ���ַ������ַ�������ֱ���ַ�����������
*/
template<typename _tIter,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	SDst
	FetchStringWidth(const Font& fnt, _tIter s)
{
	SDst w(0);

	for (; *s != char(); ++s)
	{

		LAssert(!is_undereferenceable(s), "Invalid iterator found.");
		w += FetchCharWidth(fnt, *s);
	}
	return w;
}
/*!
\brief ȡ������ָ���ĵ����ַ���������ָ�����ޱ߽�����ʱ����ʾ��ȡ�
\note ������ s ָ���ַ������ַ�������ֱ�� n �λ�ָ���ַ� c ��
*/
template<typename _tIter,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	SDst
	FetchStringWidth(const Font& fnt, _tIter s, size_t n, ucs4_t c = {})
{
	SDst w(0);

	for (; n-- != 0 && *s != c; ++s)
	{

		LAssert(!is_undereferenceable(s), "Invalid iterator found.");
		w += FetchCharWidth(fnt, *s);
	}
	return w;
}
/*!
\brief ȡ������ָ���ĵ����ַ���������ָ�����ޱ߽�����ʱ����ʾ��ȡ�
\note ������ s ָ���ַ������ַ�������ֱ���߽������ g ��ָ���ַ� c ��
*/
template<typename _tIter,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	SDst
	FetchStringWidth(const Font& fnt, _tIter s, _tIter g, ucs4_t c = {})
{
	SDst w(0);

	for (; s != g && *s != c; ++s)
	{

		LAssert(!is_undereferenceable(s), "Invalid iterator found.");
		w += FetchCharWidth(fnt, *s);
	}
	return w;
}
/*!
\brief ȡ�����ַ���������ָ�����ޱ߽�����ʱ����ʾ��ȡ�
*/
template<class _tString,
	limpl(typename = enable_for_string_class_t<_tString>)>
	inline SDst
	FetchStringWidth(const Font& fnt, const _tString& str)
{
	return Drawing::FetchStringWidth(fnt, str.c_str());
}
/*!
\brief ȡ�����ַ���ǰ������ n ���ַ�������ָ�����ޱ߽�����ʱ����ʾ��ȡ�
*/
template<class _tString,
	limpl(typename = enable_for_string_class_t<_tString>)>
	inline SDst
	FetchStringWidth(const Font& fnt, const _tString& str, size_t n)
{
	return Drawing::FetchStringWidth(fnt, str.data(), n);
}
/*!
\brief ȡ������ָ���ĵ����ַ�����ָ���ı�״̬�͸߶�����ʱ����ʾ��ȡ�
\note ������ s ָ���ַ������ַ�������ֱ���ַ�����������
\note �������ı�״ָ̬����
*/
template<typename _tIter,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	SDst
	FetchStringWidth(TextState& ts, SDst h, _tIter s)
{
	const SPos x(ts.Pen.X);
	EmptyTextRenderer r(ts, h);

	PrintString(r, s);
	return SDst(ts.Pen.X - x);
}
/*!
\brief ȡ������ָ���ĵ����ַ�����ָ���ı�״̬�͸߶�����ʱ����ʾ��ȡ�
\note ������ s ָ���ַ������ַ�������ֱ���߽������ g ��ָ���ַ� c ��
\note �������ı�״ָ̬����
*/
template<typename _tIter,
	limpl(typename = enable_for_iterator_t<_tIter>)>
	SDst
	FetchStringWidth(TextState& ts, SDst h, _tIter s, _tIter g, ucs4_t c = {})
{
	const SPos x(ts.Pen.X);
	EmptyTextRenderer r(ts, h);

	PrintString(r, s, g, c);
	return SDst(ts.Pen.X - x);
}
/*!
\brief ȡ�����ַ�����ָ���ı�״̬�͸߶�����ʱ����ʾ��ȡ�
\note �������ı�״ָ̬����
\since build 483
*/
template<class _tString,
	limpl(typename = enable_for_string_class_t<_tString>)>
	inline SDst
	FetchStringWidth(TextState& ts, SDst h, const _tString& str)
{
	return FetchStringWidth(ts, h, str.c_str());
}


LEO_DRAW_END

#endif