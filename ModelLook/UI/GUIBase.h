#ifndef UI_GUIBase_h
#define UI_GUIBase_h

#include "UI.h"

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

LEO_DRAW_BEGIN

/*!
\brief ��Ļ��Ԫ�顣
\warning ����������
*/
template<typename _type>
class GBinaryGroup
{
public:
	static const GBinaryGroup Invalid; //!< ��Ч��������Ļ����ϵ�У�����

	_type X = 0, Y = 0; //!< ������

						/*!
						\brief �޲������졣
						\note ���ʼ����
						\since build 319
						*/
	lconstfn DefDeCtor(GBinaryGroup)
		/*!
		\brief ���ƹ��죺Ĭ��ʵ�֡�
		*/
		lconstfn DefDeCopyCtor(GBinaryGroup)
		/*!
		\brief ���죺ʹ�� Size ����
		\since build 319
		*/
		explicit lconstfn
		GBinaryGroup(const Size&) lnothrow;
	/*!
	\brief ���죺ʹ�� Rect ����
	\since build 319
	*/
	explicit lconstfn
		GBinaryGroup(const Rect&) lnothrow;
	/*!
	\brief ���죺ʹ������������
	\tparam _tScalar1 ��һ�����������͡�
	\tparam _tScalar2 �ڶ������������͡�
	\warning ģ������� _type ���Ų�ͬʱ��ʽת�����ܸı���ţ�����֤Ψһ�����
	*/
	template<typename _tScalar1, typename _tScalar2>
	lconstfn
		GBinaryGroup(_tScalar1 x, _tScalar2 y) lnothrow
		: X(_type(x)), Y(_type(y))
	{}
	/*!
	\brief ���죺ʹ�ô����ԡ�
	\note ʹ�� std::get ȡ��������ȡǰ����������
	*/
	template<typename _tPair>
	lconstfn
		GBinaryGroup(const _tPair& pr) lnothrow
		: X(std::get<0>(pr)), Y(std::get<1>(pr))
	{}

	//! \since build 554
	DefDeCopyAssignment(GBinaryGroup)

		/*!
		\brief �����㣺ȡ�ӷ���Ԫ��
		*/
		lconstfn PDefHOp(GBinaryGroup, -, ) const lnothrow
		ImplRet(GBinaryGroup(-X, -Y))

		/*!
		\brief �ӷ���ֵ��
		*/
		PDefHOp(GBinaryGroup&, +=, const GBinaryGroup& val) lnothrow
		ImplRet(lunseq(X += val.X, Y += val.Y), *this)
		/*!
		\brief ������ֵ��
		*/
		PDefHOp(GBinaryGroup&, -=, const GBinaryGroup& val) lnothrow
		ImplRet(lunseq(X -= val.X, Y -= val.Y), *this)

		lconstfn DefGetter(const lnothrow, _type, X, X)
		lconstfn DefGetter(const lnothrow, _type, Y, Y)

		DefSetter(_type, X, X)
		DefSetter(_type, Y, Y)

		/*!
		\brief �ж��Ƿ�����Ԫ�ء�
		*/
		lconstfn DefPred(const lnothrow, Zero, X == 0 && Y == 0)

		/*!
		\brief ѡ��������á�
		\note �ڶ�����Ϊ true ʱѡ���һ����������ѡ��ڶ�������
		*/
		//@{
		PDefH(_type&, GetRef, bool b = true) lnothrow
		ImplRet(b ? X : Y)
		PDefH(const _type&, GetRef, bool b = true) const lnothrow
		ImplRet(b ? X : Y)
		//@}
};

//! \relates GBinaryGroup
//@{
template<typename _type>
const GBinaryGroup<_type> GBinaryGroup<_type>::Invalid{
	std::numeric_limits<_type>::lowest(), std::numeric_limits<_type>::lowest() };

template<typename _type>
lconstfn bool
operator==(const GBinaryGroup<_type>& x, const GBinaryGroup<_type>& y) lnothrow
{
	return x.X == y.X && x.Y == y.Y;
}

template<typename _type>
lconstfn bool
operator!=(const GBinaryGroup<_type>& x, const GBinaryGroup<_type>& y) lnothrow
{
	return !(x == y);
}

template<typename _type>
lconstfn GBinaryGroup<_type>
operator+(const GBinaryGroup<_type>& x, const GBinaryGroup<_type>& y) lnothrow
{
	return GBinaryGroup<_type>(x.X + y.X, x.Y + y.Y);
}

template<typename _type>
lconstfn GBinaryGroup<_type>
operator-(const GBinaryGroup<_type>& x, const GBinaryGroup<_type>& y) lnothrow
{
	return GBinaryGroup<_type>(x.X - y.X, x.Y - y.Y);
}

template<typename _type, typename _tScalar>
lconstfn GBinaryGroup<_type>
operator*(const GBinaryGroup<_type>& val, _tScalar l) lnothrow
{
	return GBinaryGroup<_type>(val.X * l, val.Y * l);
}

template<class _tBinary>
lconstfn _tBinary
Transpose(const _tBinary& obj) lnothrow
{
	return _tBinary(obj.Y, obj.X);
}

template<size_t _vIdx, typename _type>
lconstfn _type&
get(GBinaryGroup<_type>& val)
{
	static_assert(_vIdx < 2, "Invalid index found.");

	return _vIdx == 0 ? val.X : val.Y;
}
template<size_t _vIdx, typename _type>
lconstfn const _type&
get(const GBinaryGroup<_type>& val)
{
	static_assert(_vIdx < 2, "Invalid index found.");

	return _vIdx == 0 ? val.X : val.Y;
}

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

	DefGetter(const lnothrow, platform::unitlength_type, Height, Height)
		DefGetter(const lnothrow, platform::unitlength_type, Width, Width)
		DefSetter(platform::unitlength_type, Height, Height)
		DefSetter(platform::unitlength_type, Width, Width)
		/*!
		\brief �޲�������
		\warning ���ʼ����
		*/
		lconstfn Size() lnothrow
		:Width(0), Height(0)
	{}

	lconstfn Size(const Size& s) lnothrow
		: Width(s.Width), Height(s.Height)
	{}

	template<typename Scalar1, typename Scalar2>
	lconstfn Size(Scalar1 w, Scalar2 h) lnothrow
		: Width(static_cast<PDst>(w)), Height(static_cast<PDst>(h))
	{}

	template<typename Scalar1, typename Scalar2>
	lconstfn Size(std::pair<Scalar1, Scalar1> s) lnothrow
		: Width(static_cast<PDst>(s.first)), Height(static_cast<PDst>(s.second))
	{}

	DefDeCopyAssignment(Size)

		lconstfn DefBoolNeg(lconstfn explicit, Width != 0 || Height != 0)

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
		lconstfn DefPred(const lnothrow, LineSegment, !((Width == 0) ^ (Height == 0)))

		lconstfn DefPred(const lnothrow, UnStrictlyEmpty, Width == 0 || Height == 0)
};

lconstfn PDefHOp(bool, == , const Size& x, const Size& y) lnothrow
ImplRet(x.Width == y.Width && x.Height == y.Height)

lconstfn PDefHOp(bool, != , const Size& x, const Size& y) lnothrow
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
		:Point(pt), Size()
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
	using Rect::Rect;
};

//! \relates Rect
//@{
/*!
\brief �Ƚϣ���Ļ��׼������ȹ�ϵ��
*/
lconstfn PDefHOp(bool, == , const Rect& x, const Rect& y) lnothrow
ImplRet(x.GetPoint() == y.GetPoint() && x.GetSize() == y.GetSize())

/*!
\brief �Ƚϣ���Ļ��׼���β��ȹ�ϵ��
*/
lconstfn PDefHOp(bool, != , const Rect& x, const Rect& y) lnothrow
ImplRet(!(x == y))

/*!
\brief �ӷ���ʹ�ñ�׼���� r ��ƫ������ v ������Ļ��׼���Ρ�
*/
lconstfn PDefHOp(Rect, +, const Rect& r, const Vec& v) lnothrow
ImplRet({ r.GetPoint() + v, r.GetSize() })

/*!
\brief ������ʹ�ñ�׼���� r ��ƫ�������ļӷ���Ԫ v ������Ļ��׼���Ρ�
*/
lconstfn PDefHOp(Rect, -, const Rect& r, const Vec& v) lnothrow
ImplRet({ r.GetPoint() - v, r.GetSize() })

/*!
\brief ��������Ļ��׼���εĽ���
\sa Rect::operator&=
*/
inline PDefHOp(Rect, &, const Rect& x, const Rect& y) lnothrow
ImplRet(Rect(x) &= y)

/*!
\brief ��������Ļ��׼���εĲ���
\sa Rect::operator|=
*/
inline PDefHOp(Rect, | , const Rect& x, const Rect& y) lnothrow
ImplRet(Rect(x) |= y)

/*!
\brief ���в�����ȡ��׼���ν������ж��Ƿ��ϸ�ǿա�
*/
inline PDefH(bool, Clip, Rect& x, const Rect& y)
ImplRet(x &= y, !x.IsUnStrictlyEmpty())

LEO_DRAW_END

LEO_BEGIN
/*!
\brief �ж� i �Ƿ�������ҿ����� [_type(0), b) �С�
\pre ���ԣ� <tt>_type(0) < b</tt> ��
*/
template<typename _type>
inline bool
IsInInterval(_type i, _type b) lnothrow
{
	LAssert(_type(0) < b,
		"Zero element as lower bound is not less than upper bound.");
	return !(i < _type(0)) && i < b;
}

/*!
\brief �ж� i �Ƿ�������ҿ����� [a, b) �С�
\pre ���ԣ� <tt>a < b</tt> ��
*/
template<typename _type>
inline bool
IsInInterval(_type i, _type a, _type b) lnothrow
{
	LAssert(a < b, "Lower bound is not less than upper bound.");
	return !(i < a) && i < b;
}

/*!
\brief �ж� i �Ƿ��ڱ����� [_type(0), b] �С�
\pre ���ԣ� <tt>_type(0) < b</tt> ��
*/
template<typename _type>
inline bool
IsInClosedInterval(_type i, _type b) lnothrow
{
	LAssert(_type(0) < b,
		"Zero element as lower bound is not less than upper bound.");
	return !(i < _type(0) || b < i);
}
/*!
\brief �ж� i �Ƿ��ڱ����� [a, b] �С�
\pre ���ԣ� <tt>a < b</tt> ��
*/
template<typename _type>
inline bool
IsInClosedInterval(_type i, _type a, _type b) lnothrow
{
	LAssert(a < b, "Lower bound is not less than upper bound.");
	return !(i < a || b < i);
}

/*!
\brief �ж� i �Ƿ��ڿ����� (_type(0), b) �ڡ�
\pre ���ԣ� <tt>_type(0) < b</tt> ��
*/
template<typename _type>
inline bool
IsInOpenInterval(_type i, _type b) lnothrow
{
	LAssert(_type(0) < b,
		"Zero element as lower bound is not less than upper bound.");
	return _type(0) < i && i < b;
}
/*!
\brief �ж� i �Ƿ��ڿ����� (a, b) �ڡ�
\pre ���ԣ� <tt>a < b</tt> ��
*/
template<typename _type>
inline bool
IsInOpenInterval(_type i, _type a, _type b) lnothrow
{
	LAssert(a < b, "Lower bound is not less than upper bound.");
	return a < i && i < b;
}

LEO_END


#endif
