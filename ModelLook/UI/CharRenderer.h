#ifndef UI_CharRenderer_h
#define UI_CharRenderer_h

#include "UI.h"
#include "TextBase.h"

namespace leo
{
	/*!
	\brief ���Ĭ��������ָ���ַ��Ƿ�Ϊ�ɴ�ӡ�ַ���
	\note MSVCRT �� isprint/iswprint ʵ��ȱ�ݵı�ͨ��
	\sa https://connect.microsoft.com/VisualStudio/feedback/details/799287/isprint-incorrectly-classifies-t-as-printable-in-c-locale
	*/
	//@{
	inline PDefH(bool, IsPrint, char c)
		ImplRet(stdex::isprint(c))
		inline PDefH(bool, IsPrint, wchar_t c)
		ImplRet(stdex::iswprint(c))
		template<typename _tChar>
	bool
		IsPrint(_tChar c)
	{
		return leo::IsPrint(wchar_t(c));
	}
}

LEO_DRAW_BEGIN

/*!
\brief �ַ������ؿ鴫�䡣
\tparam _tOut ������������͡�
\tparam _tIn ������������͡�
\tparam _fPixelShader ������ɫ�����͡�
\param blit ���ز�����
\param src Դ��������
\param ss Դ���������ڻ�������С��
\param pc ָ���ַ������������ȾĿ��Ļ��������ģ�
���� Location Ϊ���Դ�����ꡣ
\param neg_pitch ָ����������Ⱦ˳��
\sa Drawing::Blit
\sa Drawing::BlitPixels
\since build 440
*/
template<typename _tOut, typename _tIn, typename _fPixelShader>
void
BlitGlyphPixels(_fPixelShader blit, _tOut dst, _tIn src, const Size& ss,
	const PaintContext& pc, bool neg_pitch)
{
	const auto& ds(pc.Target.GetSize());
	const Rect& bounds(pc.ClipArea);

	if (neg_pitch)
		BlitPixels<false, true>(blit, dst, src, ds, ss, bounds.GetPoint(),
			pc.Location, bounds.GetSize());
	else
		BlitPixels<false, false>(blit, dst, src, ds, ss, bounds.GetPoint(),
			pc.Location, bounds.GetSize());
}

/*!
\brief ��Ⱦ�����ַ���
\param pc ָ���ַ������������ȾĿ��Ļ��������ģ�
���� Location Ϊ���Դ�����ꡣ
\pre ���ԣ��������ǿա�
\note ���� Alpha ���塣
*/
LB_API void
RenderChar(PaintContext&& pc, Color, bool, CharBitmap::BufferType,
	CharBitmap::FormatType, const Size&);

/*!
\brief ȡ�ı���Ⱦ������ĩλ�ã������꣩��
*/
template<class _tRenderer>
inline SPos
GetEndOfLinePositionOf(const _tRenderer& r)
{
	return r.GetTextState().Margin.Right;
}

/*!
\brief ȡ�ı���Ⱦ������ĩλ��ʣ��ƫ�ơ�
\note ʹ�� ADL <tt>GetEndOfLinePositionOf</tt> ȡ��ĩλ�á�
*/
template<class _tRenderer>
inline SPos
GetEndOfLineOffsetOf(const _tRenderer& r)
{
	return SPos(r.GetContext().GetWidth()) - GetEndOfLinePositionOf(r);
}


/*!
\brief ��ӡ�����ɴ�ӡ�ַ���
\todo �еĽ�βλ�ü���ͱ߾�����ϡ�
*/
template<class _tRenderer>
void
PrintChar(_tRenderer& r, ucs4_t c)
{
	if (LB_LIKELY(IsPrint(c)))
		r(c);
}

//@{
//! \brief ����ַ������
enum class PutCharResult
{
	//! \brief �����޷����ɶ����С�
	NeedNewline,
	//! \brief ������з���
	PutNewline,
	//! \brief �������ɴ�ӡ�ַ���
	NotPrintable,
	//! \brief �ɼ�����ͬһ������ɴ�ӡ�ַ���
	Normal
};

//! \brief ʹ��ָ�����ı�״̬����ĩλ�ð����ӡ���в��ж��Ƿ���Ҫ��Ⱦ�����ַ���
LB_API PutCharResult
PutCharBase(TextState&, SDst, ucs4_t);

/*!
\brief ��ӡ�����ַ���
\return ���������޷����ɶ�����ʱ���ط���ֵ�����򷵻� 0 ��
\note �����з���
\note �������޷����������ַ�ʱ���С�
*/
//@{
template<class _tRenderer>
PutCharResult
PutChar(_tRenderer& r, ucs4_t c, SDst eol)
{
	const auto res(PutCharBase(r.GetTextState(), eol, c));

	if (res == PutCharResult::Normal)
		r(c);
	return res;
}
template<class _tRenderer>
PutCharResult
PutChar(_tRenderer& r, ucs4_t c)
{
	const SPos seol(GetEndOfLineOffsetOf(r));

	if (seol >= 0)
		return PutChar(r, c, SDst(seol));
	return PutCharResult::NeedNewline;
}

template<class _tRenderer, typename _tChar, typename... _tParams>
inline PutCharResult
PutChar(_tRenderer& r, _tChar c, _tParams&&... args)
{
	return PutChar(r, ucs4_t(c), lforward(args)...);
}
//@}


LEO_DRAW_END

#endif
