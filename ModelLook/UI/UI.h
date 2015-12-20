#ifndef UI_UI_h
#define UI_UI_h

#include <ldef.h>
#include <BaseMacro.h>
#include <LAssert.h>
#include <algorithm>

#ifndef UI_BEGIN
//UI����2D UI��3D UI����Ҫ������Ϸ�ض�UI
#define UI_BEGIN namespace UI{
#define UI_END }
#define LEO_UI_BEGIN LEO_BEGIN UI_BEGIN
#define LEO_UI_END LEO_END UI_END
//Drawing ΪHUD��UI���ò���
#define DRAW_BEGIN namespace Drawing{
#define DRAW_END }
#define LEO_DRAW_BEGIN LEO_BEGIN DRAW_BEGIN
#define LEO_DRAW_END LEO_END DRAW_END
#endif

LEO_DRAW_BEGIN

class Font;
struct Padding;
struct Size;
struct Rect;

LEO_DRAW_END

#endif
