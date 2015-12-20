#ifndef UI_GUI_h
#define UI_GUI_h

#include "GUIBase.h"

LEO_DRAW_BEGIN

using SPos = platform::unit_type;

struct LB_API Padding
{
	/*
	\brief �հ׾��룺���ҡ��ϡ��¡�
	\since build 365
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
	\since build 365
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
		\since build 587
		*/
		PDefHOp(Padding&, *=, size_t n)
		// XXX: Conversion to 'SPos' might be implementation-defined.
		ImplRet(lunseq(Left *= SPos(n), Right *= SPos(n), Top *= SPos(n),
			Bottom *= SPos(n)), *this)
};

LEO_DRAW_END


#endif
