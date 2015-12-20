#ifndef UI_GUI_h
#define UI_GUI_h

#include "GUIBase.h"
#include "Color.h"

LEO_DRAW_BEGIN

using SPos = platform::unit_type;
using SDst = platform::unitlength_type;

struct LB_API Padding
{
	/*
	\brief �հ׾��룺���ҡ��ϡ��¡�
	*/
	SPos Left = 0, Right = 0, Top = 0, Bottom = 0;

	/*!
	\brief Ĭ�Ϲ��죺ʹ����߾ࡣ
	*/
	DefDeCtor(Padding)
		/*!
		\brief ���죺ʹ�� 4 ����ͬ�ı߾ࡣ
		*/
		explicit lconstfn
		Padding(SPos x)
		: Padding(x, x, x, x)
	{}
	/*!
	\brief ���죺ʹ�� 4 ���߾ࡣ
	*/
	lconstfn
		Padding(SPos l, SPos r, SPos t, SPos b)
		: Left(l), Right(r), Top(t), Bottom(b)
	{}

	/*!
	\brief �ӷ���ֵ����Ӧ�������� operator+= ��
	*/
	PDefHOp(Padding&, +=, const Padding& m)
		ImplRet(lunseq(Left += m.Left, Right += m.Right, Top += m.Top,
			Bottom += m.Bottom), *this)

		/*!
		\brief ������ֵ����Ӧ�������� operator-= ��
		*/
		PDefHOp(Padding&, -=, const Padding& m)
		ImplRet(lunseq(Left -= m.Left, Right -= m.Right, Top -= m.Top,
			Bottom -= m.Bottom), *this)

		/*!
		\brief �˷���ֵ����Ӧ�������� operator-= ��
		*/
		PDefHOp(Padding&, *=, size_t n)
		// XXX: Conversion to 'SPos' might be implementation-defined.
		ImplRet(lunseq(Left *= SPos(n), Right *= SPos(n), Top *= SPos(n),
			Bottom *= SPos(n)), *this)
};

//! \relates Padding
//@{
/*!
\brief �ӷ���Ԫ����Ӧ��������һԪ operator- ��
*/
lconstfn PDefHOp(Padding, -, const Padding& x)
ImplRet(Padding(-x.Left, -x.Right, -x.Top, -x.Bottom))

/*!
\brief �ӷ�����Ӧ�������� operator+ ��
*/
lconstfn PDefHOp(Padding, +, const Padding& x, const Padding& y)
ImplRet(Padding(x.Left + y.Left, x.Right + y.Right, x.Top + y.Top,
	x.Bottom + y.Bottom))

	/*!
	\brief ��������Ӧ�������� operator- ��
	\since build 572
	*/
	lconstfn PDefHOp(Padding, -, const Padding& x, const Padding& y)
	ImplRet(Padding(x.Left - y.Left, x.Right - y.Right, x.Top - y.Top,
		x.Bottom - y.Bottom))

	/*!
	\brief �˷�����Ӧ�������� operator* ��
	\since build 587
	*/
	lconstfn PDefHOp(Padding, *, const Padding& x, size_t n)
	ImplRet(Padding(SPos(x.Left * ptrdiff_t(n)), SPos(x.Right * ptrdiff_t(n)),
		SPos(x.Top * ptrdiff_t(n)), SPos(x.Bottom * ptrdiff_t(n))))

	/*!
	\brief �ӷ�����С��Ļ��׼���Σ����λ����ָ���߾������
	\note ���߾��������εĿ��߿���Ϊ 0 ��
	*/
	LB_API Rect
	operator+(const Rect&, const Padding&);

/*!
\brief �������Ŵ���Ļ��׼���Σ����λ����ָ���߾������
\note ���߾��������εĿ��߿���Ϊ 0 ��
*/
inline PDefHOp(Rect, -, const Rect& r, const Padding& m)
ImplRet(r + -m)
//@}


/*!
\brief ȡˮƽ�߾�͡�
*/
inline PDefH(SDst, GetHorizontalOf, const Padding& m)
ImplRet(SDst(std::max<SPos>(0, m.Left) + std::max<SPos>(0, m.Right)))

/*!
\brief ȡ��ֱ�߾�͡�
*/
inline PDefH(SDst, GetVerticalOf, const Padding& m)
ImplRet(SDst(std::max<SPos>(0, m.Top) + std::max<SPos>(0, m.Bottom)))


/*!
\brief ȡ�ڱ߽������߽�ı߾ࡣ
*/
LB_API Padding
FetchMargin(const Rect&, const Size&);


/*!
\brief ����ָ��Դ�ı߽��Ż����������ĵļ�������
\return ���߾���������Ա���Ⱦ��Ϊ Point() ������ΪԴ����ʼƫ��λ�á�
\note ������Ҫ����ʱ�����޸�ƫ�����ꡣ

���߾���������Ҫ�������Ƶ����򣬽�����������������ĵĳ���ȾĿ�����������Ա��
*/
LB_API Point
ClipBounds(Rect&, const Rect&);

LEO_DRAW_END


#endif
