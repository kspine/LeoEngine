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

namespace {

	class LB_API FontFamily final : private noncopyable{
	} nullFontFamily;

	class LB_API FontCache final : private noncopyable{
	} nullFontCache;
}
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
	Font()
		:Font(DefaultSize, FontStyle::Regular)
	{}

	Font(FontSize,
			FontStyle = FontStyle::Regular);

	DefGetter(const lnothrow, FontSize, Size, font_size)
	DefGetter(const lnothrow, FontStyle, Style, style)

	DefGetter(const lnothrow, FontCache&, Cache, nullFontCache)
	DefGetter(const lnothrow, const FontFamily&, FontFamily, nullFontFamily)

		/*!
		\brief ȡ��ࡣ
		*/
		std::int8_t
		GetAdvance(char32_t) const;
	/*!
	\brief ȡ������
	*/
	std::int8_t
		GetAscender() const;
	/*!
	\brief ȡ������
	*/
	std::int8_t
		GetDescender() const;


	/*!
	\brief ȡ�����Ӧ���ַ��߶ȡ�
	*/
	FontSize
		GetHeight() const lnothrow;
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
