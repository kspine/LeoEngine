////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2015.
// -------------------------------------------------------------------------
//  File name:   HUD/HUD.h
//  Version:     v1.00
//  Created:     11/24/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: HUD����ͷ�ļ�
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef HUD_COMMON_H
#define HUD_COMMON_H

#include<ldef.h>
#include<BaseMacro.h>
#include<algorithm.hpp>
#include<any_iterator.hpp>

//ƽ̨��ؽӿ������ݽṹ
namespace platform
{
#ifdef PLATFORM_WIN32
	//ƽ̨��ص���Ļ���굥λ
	using unit_type = long;
	//ƽ̨��ص���Ļ�������
	using unitlength_type = unsigned long;
#else
#error "unsupprot platform"
#endif
}


#ifndef HUD_BEGIN
#define HUD_BEGIN namespace HUD {
#define HUD_END }
#endif

LEO_BEGIN

HUD_BEGIN

/*!
\brief ��Ļ��ά��(ֱ������)
*/
using Point = std::pair<platform::unit_type, platform::unit_type>;

/*!
\brief ��Ļ�����С
*/
struct LB_API Size
{
	/*!
	\brief ��Ч����
	*/
	static const Size Invalid;

	using PDst = platform::unitlength_type;
	platform::unitlength_type Width, Height;

	/*!
	\brief �޲�������
	\warning �����ʼ����
	*/
	Size() lnothrow
	{}

	lconstfn Size(const Size& s) lnothrow
		:Width(s.Width),Height(s.Height)
	{}

	template<typename Scalar1,typename Scalar2>
	lconstfn Size(Scalar1 w,Scalar2 h) lnothrow
		:Width(static_cast<PDst>(w)),Height(static_cast<PDst>(h))
	{}

	DefDeCopyAssignment(Size)

	lconstfn DefBoolNeg(lconstfn explicit,Width != 0 || Height != 0)

	/*!
	\brief ��������һ������Ľ�
	*/
	PDefHOp(Size&, &=, const Size& s) lnothrow
		ImplRet(lunseq(Width = std::min(Width, s.Width),
			Height = std::min(Height, s.Height)), *this)

		/*!
		\brief ��������һ������Ĳ�
		*/
		PDefHOp(Size&, |=, const Size& s) lnothrow
		ImplRet(lunseq(Width = std::max(Width, s.Width),
			Height = std::max(Height, s.Height)), *this)
};

struct LB_API Rect :private Point, private Size
{

};

struct LB_API Box :private Rect
{

};


struct IWidget;

HUD_END

LEO_END

#endif
