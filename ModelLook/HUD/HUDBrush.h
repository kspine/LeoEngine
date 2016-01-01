#ifndef HUD_Brush_H
#define HUD_Brush_H

#include "HUDBase.h"
#include "HUDEvent.hpp"
#include <leomathtype.hpp>

LEO_BEGIN

HUD_BEGIN

class LB_API NullBrush
{
public:
	NullBrush(Drawing::Color)
	{}

	void
		operator()(const PaintContext&) const;
};


class LB_API SolidBrush
{
public:
	Drawing::Color Color;

	SolidBrush(const Drawing::Color c)
		:Color(c)
	{}

	void
		operator()(const PaintContext&) const;
};

class LB_API SolidBlendBrush
{
public:
	Drawing::Color Color;

	SolidBlendBrush(Drawing::Color c)
		: Color(c)
	{}

	SolidBlendBrush(Drawing::ColorSpace::ColorSet c,Drawing::AlphaType a)
		: Color(c,a)
	{}

	void
		operator()(const PaintContext&) const;
};

/*!
\brief ��ˢ���������͡�

��Ŀ����������ġ�Դ�͸���ƫ�Ƹ���Ŀ��ͼ��Ļص��ӿڡ�
*/
template<typename... _types>
using GBrushUpdater = GHEvent<void(const PaintContext&, _types...)>;

HUD_END

LEO_END

#endif
