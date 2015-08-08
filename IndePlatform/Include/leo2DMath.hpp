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
	//todo:����һ���ļ�

	inline float saturate(float x)
	{
		return max(0.f, min(1.f, x));
	}


	inline float smoothstep(float a, float b, float x)
	{
		float t = saturate((x - a) / (b - a));
		return t*t*(3.0f - (2.0f*t));
	}


	namespace ops{
		enum class axis_system {
			cartesian_system,
			windows_system,
			dx_texture_system,
			gl_texture_system
		};


		struct Rect {
			//top-left(x,y)
			//bottom-right(z,w)
			float4 tlbr;

			Rect(const float4& TopLeftBottomRight)
				:tlbr(TopLeftBottomRight) {
			}

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
