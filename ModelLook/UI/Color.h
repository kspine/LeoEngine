#ifndef UI_COLOR_H
#define UI_COLOR_H

#include "GUI.h"

LEO_DRAW_BEGIN

/*!
\warning Ϊ����YSLIB����,����DXGI_FORMAT��ʽ
*/
enum PixelFormat {
	C48888,//4��ͨ����ÿͨ��8bit
};
template<PixelFormat = C48888>
struct Pixel;

template<>
struct Pixel<C48888> {
	stdex::byte r, g, b, a;
};


lconstfn PDefH(std::uint32_t, FetchPixel,
	stdex::byte r, stdex::byte g, stdex::byte b) lnothrow
	ImplRet(std::uint32_t(r) | std::uint32_t(g) << 8 | std::uint32_t(b) << 16)

#define LL_FetchPixel(r, g, b) FetchPixel(r, g, b)

#define DefColorH_(hex, name) \
	name = (LL_FetchPixel(((hex) >> 16) & 0xFF, \
		((hex) >> 8) & 0xFF, (hex) & 0xFF) << 8 | 0xFF)

//! \brief ϵͳĬ����ɫ�ռ䡣
namespace ColorSpace
{
#define	HexAdd0x(hex) 0x##hex
#define DefColorH(hex_, name) DefColorH_(HexAdd0x(hex_), name)

	/*!
	\brief Ĭ����ɫ����
	\see http://www.w3schools.com/html/html_colornames.asp ��
	*/
	enum ColorSet : uint32
	{
		DefColorH(00FFFF, Aqua),
		DefColorH(000000, Black),
		DefColorH(0000FF, Blue),
		DefColorH(FF00FF, Fuchsia),
		DefColorH(808080, Gray),
		DefColorH(008000, Green),
		DefColorH(00FF00, Lime),
		DefColorH(800000, Maroon),
		DefColorH(000080, Navy),
		DefColorH(808000, Olive),
		DefColorH(800080, Purple),
		DefColorH(FF0000, Red),
		DefColorH(C0C0C0, Silver),
		DefColorH(008080, Teal),
		DefColorH(FFFFFF, White),
		DefColorH(FFFF00, Yellow)
	};

#undef LL_FetchPixel
#undef DefColorH
#undef DefColorH_
#undef HexAdd0x
} // namespace ColorSpace;


  //! \brief ��ɫ��
class LB_API Color
{
public:
	using ColorSet = ColorSpace::ColorSet;

private:
	/*!
	\brief RGB ������
	*/
	stdex::byte r = 0, g = 0, b = 0;
	/*!
	\brief Alpha ������
	*/
	stdex::byte a = 0;

public:
	/*!
	\brief �޲������죺���з���Ϊ 0 ��Ĭ����ɫ��
	*/
	lconstfn
		Color() = default;
	/*!
	\brief ���죺ʹ�ñ�����ɫ����
	*/
	lconstfn
		Color(Pixel<> px) lnothrow
		: r(px.r), g(px.g), b(px.b), a(px.a)
	{}
	/*!
	\brief ���죺ʹ��Ĭ����ɫ��
	*/
	lconstfn
		Color(ColorSet cs) lnothrow
		: r((cs & 0xFF00) >> 8), g((cs & 0xFF0000) >> 16),
		b((cs & 0xFF000000) >> 24), a(0xFF)

	{}
	/*!
	\brief ���죺ʹ�� RGB ֵ�� alpha λ��
	*/
	lconstfn
		Color(stdex::byte r_, stdex::byte g_, stdex::byte b_, stdex::byte a_ = 0xFF) lnothrow
		: r(r_), g(g_), b(b_), a(a_)
	{}
	/*!
	\brief ���죺ʹ����ͬ����ת��Ϊ��ɫ�� RGB ֵ�� alphaλ��
	\note �����б��ʼ��ʱ narrowing ת����
	*/
	template<typename _tScalar>
	lconstfn
		Color(_tScalar r_, _tScalar g_, _tScalar b_, stdex::byte a_ = 0xFF) lnothrow
		: Color(stdex::byte(r_), stdex::byte(g_), stdex::byte(b_), a_)
	{}

	/*!
	\brief ת����������ɫ����
	*/
	lconstfn
		operator Pixel<>() const lnothrow
	{
		return{ b, g, r, a };
	}

	/*!
	\brief ȡ alpha ������
	*/
	lconstfn DefGetter(const lnothrow, stdex::byte, A, a)
		/*!
		\brief ȡ��ɫ������
		*/
		lconstfn DefGetter(const lnothrow, stdex::byte, B, b)
		/*!
		\brief ȡ��ɫ������
		*/
		lconstfn DefGetter(const lnothrow, stdex::byte, G, g)
		/*!
		\brief ȡ��ɫ������
		*/
		lconstfn DefGetter(const lnothrow, stdex::byte, R, r)
};

namespace ColorSpace {
	//͸��
	lconstfn Color Trans = {0,0,0,0};
}

LEO_DRAW_END

#endif
