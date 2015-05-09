////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   IndePlatform/leo2dmath.hpp
//  Version:     v1.00
//  Created:     5/9/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2013
//  Description: ��ѡ��2D�߼�(������3D���,z =1.f)
// -------------------------------------------------------------------------
//  History:
////////////////////////////////////////////////////////////////////////////

#ifndef IndePlatform_Leo2DMath_hpp
#define IndePlatform_Leo2DMath_hpp

#include "leomathtype.hpp"

namespace leo {
	namespace ops{
		struct Rect {
			//top-left(x,y)
			//bottom-right(z,w)
			float4 tlbr;

			float2 GetLeftTopCornet() const noexcept{
				return float2(tlbr.x, tlbr.y);
			}

			float2 GetRightBottomCornet() const noexcept{
				return float2(tlbr.z, tlbr.w);
			}

			float2& GetLeftTopCornet() noexcept {
				return * (float2*)(tlbr.begin());
			}

			float2& GetRightBottomCornet() noexcept {
				return *(float2*)(tlbr.begin()+2);
			}
		};
	}
}

#endif
