////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2015.
// -------------------------------------------------------------------------
//  File name:   HUD/Label.hpp
//  Version:     v1.00
//  Created:     11/24/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: HUD��ǩ
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#ifndef HUD_WIDGET_H
#define HUD_WIDGET_H

#include "WidgetEvent.h"

LEO_BEGIN

HUD_BEGIN

using WidgetIterator = any_input_iterator<IWidget>;
using WidgetRange = std::pair<WidgetIterator, WidgetIterator>;

DeclI(LB_API, IWidget)
DeclIEntry(WidgetRange GetChildren())

/*
��ͼ�ӿ�<δ������ͼ>
*/
DeclIEntry(bool IsVisible() const) //�ж��Ƿ�ɼ�
DeclIEntry(bool Contains(platform::unit_type, platform::unit_type) const) //�жϵ��Ƿ��ڲ����Ŀ���������
DeclIEntry(Size GetSizeOf() const) //ȡ������С
DeclIEntry(Box GetBox() const) //ȡ������Χ��
DeclIEntry(Point GetLocationOf() const) //ȡ������С

DeclIEntry(void SetLocationOf(const Point&)) //���ò������Ͻ�λ��
DeclIEntry(void SetSizeOf(const Size&)) //���ò�����С
DeclIEntry(void SetVisible(bool b)) //���ò����ɼ���

/*!
\brief ȡ��������
\note ������Ч����������Լ����
*/
DeclIEntry(AController& GetController() const)
EndDecl

LB_API void SetBox(IWidget&, const Box&);

class LB_API Widget : implements IWidget
{
public:
	Widget() = default;
	~Widget() = default;

	DefIEntryImpl(bool IsVisible() const)
	DefIEntryImpl(bool Contains(platform::unit_type, platform::unit_type) const) 
	DefIEntryImpl(Size GetSizeOf() const) 
	DefIEntryImpl(Box GetBox() const) 
	DefIEntryImpl(Point GetLocationOf() const) 

	DefIEntryImpl(void SetLocationOf(const Point&)) 
	DefIEntryImpl(void SetSizeOf(const Size&)) 
	DefIEntryImpl(void SetVisible(bool b))
private:
	Size mSize;
	Point mLocation;
	bool mVisible;
};


HUD_END

LEO_END


#endif
