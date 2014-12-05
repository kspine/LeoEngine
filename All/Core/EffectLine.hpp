////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   Core/EffectLine.h
//  Version:     v1.00
//  Created:     8/29/2014 by leo hawke.
//  Compilers:   Visual Studio.NET 2013
//  Description: �ṩ�򵥵�ֱ����Ⱦ
//				 ���Ըı�ֱ����ɫ	
// -------------------------------------------------------------------------
//  History:
//				8/19/2014 �޸�Ϊ����ģʽ,����IMPLʵ��,NormalLine����Render
//
////////////////////////////////////////////////////////////////////////////

#ifndef Core_EffectLine_hpp
#define Core_EffectLine_hpp

#include "effect.h"

namespace leo
{
	class EffectLine :public Effect, ABSTRACT
	{
	public:
		void Apply(ID3D11DeviceContext* context);

		void ViewProj(const float4x4& matrix, ID3D11DeviceContext* context = nullptr);

		void Color(const float4& color, ID3D11DeviceContext* context = nullptr);

		bool SetLevel(EffectConfig::EffectLevel l) lnothrow
		{
			return true;
		}
	public:
		static const std::unique_ptr<EffectLine>& GetInstance(ID3D11Device* device = nullptr);
	};
}

#endif