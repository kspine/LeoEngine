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


#include "HUDBase.h"

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




LEO_BEGIN

HUD_BEGIN

/*!
\brief ��Ļ��ά��(ֱ������)
*/
using Point = GBinaryGroup<platform::unit_type>;
using Vec = GBinaryGroup<platform::unit_type>;

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
	\warning ���ʼ����
	*/
	lconstfn Size() lnothrow
		:Width(0),Height(0)
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

		/*!
		\brief �ж��Ƿ�Ϊ�߶�
		*/
		lconstfn DefPred(const lnothrow,LineSegment,!((Width == 0) ^ (Height == 0)))

		lconstfn DefPred(const lnothrow, UnStrictlyEmpty,Width == 0 || Height == 0)
};

lconstfn PDefHOp(bool,==,const Size& x,const Size& y) lnothrow
	ImplRet(x.Width == y.Width && x.Height == y.Height)

lconstfn PDefHOp(bool,!= , const Size& x, const Size& y) lnothrow
	ImplRet(!(x == y))

lconstfn PDefHOp(Size, &, const Size& x, const Size& y) lnothrow
	ImplRet({ std::min(x.Width, y.Width),std::min(x.Height, y.Height) })

lconstfn PDefHOp(Size, | , const Size& x, const Size& y) lnothrow
	ImplRet({ std::max(x.Width, y.Width), std::max(x.Height, y.Height) })

lconstfn PDefH(size_t, GetAreaOf, const Size& s) lnothrow
	ImplRet(size_t(s.Width * s.Height))


template<size_t _vIdx>
lconstfn platform::unitlength_type&
get(Size& s)
{
	static_assert(_vIdx < 2, "Invalid index found.");

	return _vIdx == 0 ? s.Width : s.Height;
}
template<size_t _vIdx>
lconstfn const platform::unitlength_type&
get(const Size& s)
{
	static_assert(_vIdx < 2, "Invalid index found.");

	return _vIdx == 0 ? s.Width : s.Height;
}


template<typename _type>
lconstfn GBinaryGroup<_type>
operator+(GBinaryGroup<_type> val, const Size& s) lnothrow
{
	// XXX: Conversion to '_type' might be implementation-defined.
	return{ val.X + _type(s.Width), val.Y + _type(s.Height) };
}

template<typename _type>
lconstfn GBinaryGroup<_type>
operator-(GBinaryGroup<_type> val, const Size& s) lnothrow
{
	// XXX: Conversion to '_type' might be implementation-defined.
	return{ val.X - _type(s.Width), val.Y - _type(s.Height) };
}

lconstfn PDefH(Size, Transpose, const Size& s) lnothrow
	ImplRet({ s.Height, s.Width })

struct LB_API Rect :private Point, private Size
{
	static const Rect Invalid;
	using Point::X;
	using Point::Y;
	using Size::Width;
	using Size::Height;

	DefDeCtor(Rect)

	explicit lconstfn
	Rect(const Point& pt) lnothrow
		:Point(pt),Size()
	{}

	/*!
	\brief ���죺ʹ�� Size ����
	*/
	lconstfn
		Rect(const Size& s) lnothrow
		: Point(), Size(s)
	{}
	/*!
	\brief ���죺ʹ����Ļ��ά��� Size ����
	*/
	lconstfn
		Rect(const Point& pt, const Size& s) lnothrow
		: Point(pt), Size(s)
	{}
	/*!
	\brief ���죺ʹ����Ļ��ά��ͱ�ʾ��������� platform::unitlength_type ֵ��
	*/
	lconstfn
		Rect(const Point& pt, platform::unitlength_type w, platform::unitlength_type h) lnothrow
		: Point(pt.X, pt.Y), Size(w, h)
	{}
	/*!
	\brief ���죺ʹ�ñ�ʾλ�õ����� platform::unit_type ֵ�� Size ����
	*/
	lconstfn
		Rect(platform::unit_type x, platform::unit_type y, const Size& s) lnothrow
		: Point(x, y), Size(s.Width, s.Height)
	{}
	/*!
	\brief ���죺ʹ�ñ�ʾλ�õ����� platform::unit_type ֵ�ͱ�ʾ��С������ platform::unitlength_type ֵ��
	*/
	lconstfn
		Rect(platform::unit_type x, platform::unit_type y, platform::unitlength_type w, platform::unitlength_type h) lnothrow
		: Point(x, y), Size(w, h)
	{}
	/*!
	\brief ���ƹ��죺Ĭ��ʵ�֡�
	*/
	lconstfn DefDeCopyCtor(Rect)

		DefDeCopyAssignment(Rect)
		//! \since build 319
		//@{
		Rect&
		operator=(const Point& pt) lnothrow
	{
		lunseq(X = pt.X, Y = pt.Y);
		return *this;
	}
	Rect&
		operator=(const Size& s) lnothrow
	{
		lunseq(Width = s.Width, Height = s.Height);
		return *this;
	}
	//@}

	/*!
	\brief ������һ����Ļ��׼���εĽ���
	\note ��������Ϊ Rect() ������Ϊ���������������е������Ρ�
	*/
	Rect&
		operator&=(const Rect&) lnothrow;

	/*!
	\brief ������һ����Ļ��׼���εĲ���
	\note ���Ϊ�������������е���С���Ρ�
	*/
	Rect&
		operator|=(const Rect&) lnothrow;

	/*!
	\brief �ж��Ƿ�Ϊ�ա�
	\sa Size::operator!
	*/
	using Size::operator!;

	/*!
	\brief �ж��Ƿ�ǿա�
	\sa Size::bool
	*/
	using Size::operator bool;

	/*!
	\brief �жϵ� (px, py) �Ƿ��ھ����ڻ���ϡ�
	*/
	bool
		Contains(platform::unit_type px, platform::unit_type py) const lnothrow;
	/*!
	\brief �жϵ� pt �Ƿ��ھ����ڻ���ϡ�
	*/
	PDefH(bool, Contains, const Point& pt) const lnothrow
		ImplRet(Contains(pt.X, pt.Y))
		/*!
		\brief �жϾ����Ƿ��ھ����ڻ���ϡ�
		\note �վ������ǲ���������
		*/
		bool
		Contains(const Rect&) const lnothrow;
	/*!
	\brief �жϵ� (px, py) �Ƿ��ھ����ڡ�
	*/
	bool
		ContainsStrict(platform::unit_type px, platform::unit_type py) const lnothrow;
	/*!
	\brief �жϵ� pt �Ƿ��ھ����ڡ�
	*/
	PDefH(bool, ContainsStrict, const Point& pt) const lnothrow
		ImplRet(ContainsStrict(pt.X, pt.Y))
		/*!
		\brief �жϾ����Ƿ��ھ����ڻ���ϡ�
		\note �վ������ǲ���������
		*/
		bool
		ContainsStrict(const Rect&) const lnothrow;
	/*!
	\brief �жϾ����Ƿ�Ϊ�߶Σ����Ϳ�������һ����ֵ���� 0 ��
	\sa Size::IsLineSegment
	*/
	using Size::IsLineSegment;

	using Size::IsUnStrictlyEmpty;

	// XXX: Conversion to 'platform::unit_type' might be implementation-defined.
	lconstfn DefGetter(const lnothrow, platform::unit_type, Bottom, Y + platform::unit_type(Height))
		/*!
		\brief ȡ���Ͻ�λ�á�
		*/
		lconstfn DefGetter(const lnothrow, const Point&, Point,
			static_cast<const Point&>(*this))
		/*!
		\brief ȡ���Ͻ�λ�����á�
		*/
		DefGetter(lnothrow, Point&, PointRef, static_cast<Point&>(*this))
		// XXX: Conversion to 'platform::unit_type' might be implementation-defined.
		lconstfn DefGetter(const lnothrow, platform::unit_type, Right, X + platform::unit_type(Width))
		/*!
		\brief ȡ��С��
		*/
		lconstfn DefGetter(const lnothrow, const Size&, Size,
			static_cast<const Size&>(*this))
		/*!
		\brief ȡ��С���á�
		*/
		DefGetter(lnothrow, Size&, SizeRef, static_cast<Size&>(*this))

		using Point::GetX;
	using Point::GetY;

	using Point::SetX;
	using Point::SetY;
	//@}
};

struct LB_API Box :public Rect
{

};


struct IWidget;
class AController;

HUD_END

LEO_END

#endif
