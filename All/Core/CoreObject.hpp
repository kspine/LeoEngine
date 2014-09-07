////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   Core/CoreObject.hpp
//  Version:     v1.00
//  Created:     8/15/2014 by leo hawke.
//  Compilers:   Visual Studio.NET 2013
//  Description: ���Ŀ�Ļ�������
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#ifndef Core_CoreObject_HPP
#define Core_CoreObject_HPP
#include "..\leomath.hpp"
#include "..\IndePlatform\memory.hpp"
namespace leo
{

	class SQTObject :public GeneralAllocatedObject
	{
	protected:
		SQT mSQT;
	public:
		SQTObject(const SQT& sqt)
			:mSQT(sqt)
		{}
		~SQTObject() = default;
	public:
		void inline Scale(float s)
		{
			mSQT.s *= s;
		}

		void inline Rotation(const float4& quaternion)
		{
			auto o = XMMatrixRotationQuaternion(loadfloat4(&mSQT.q));
			auto n = XMMatrixRotationQuaternion(loadfloat4(&quaternion));
			savefloat4(&mSQT.q, XMQuaternionRotationMatrix(o*n));
		}

		void inline Translation(const float3& offset)
		{
			auto t = loadfloat3(&mSQT.t);
			auto off = loadfloat3(&offset);
			savefloat3(&mSQT.t, t + off);
		}

		void inline Transform(const SQT& sqt)
		{
			Scale(sqt.s);
			Rotation(sqt.q);
			Translation(sqt.t);
		}

		void inline Roll(float x)
		{
			Rotation(EulerAngleToQuaternion(float3(x, 0.f, 0.f)));
		}

		void inline Pitch(float y)
		{
			Rotation(EulerAngleToQuaternion(float3(0.f,y, 0.f)));
		}

		void inline Yaw(float z)
		{
			Rotation(EulerAngleToQuaternion(float3(0.f, 0.f,z)));
		}
	};

}

#endif