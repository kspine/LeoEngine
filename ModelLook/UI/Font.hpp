#ifndef UI_Font_h
#define UI_Font_h

#include "UI.h"
#include <leoint.hpp>

LEO_DRAW_BEGIN

using FontSize = std::uint8_t;

enum class FontStyle : std::uint8_t
{
	Regular = 0, //!< �������塣
};

/*!
\brief ���壺��ģ��������ʽ�ʹ�С��
\todo ����
*/
class LB_API Font final{
public:
	static lconstexpr const FontSize DefaultSize = 12,
		MinimalSize = 4, MaximalSize = 96;

private:
private:
	FontSize font_size;
	/*!
	\brief ������ʽ��
	*/
	FontStyle style;
public:
	explicit
	Font(FontSize = DefaultSize,
			FontStyle = FontStyle::Regular);

	DefGetter(const lnothrow, FontSize, Size, font_size)
	DefGetter(const lnothrow, FontStyle, Style, style)
public:
	/*!
	\brief ���������С��
	*/
	void
		SetSize(FontSize = DefaultSize);
	/*!
	\brief ������ʽ��
	\note ������������ʱ������ʽ��
	*/
	bool
		SetStyle(FontStyle);
};

LEO_DRAW_END

#endif
