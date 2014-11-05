////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   Core/Skeleton.h
//  Version:     v1.00
//  Created:     10/27/2014 by leo hawke.
//  Compilers:   Visual Studio.NET 2013
//  Description: �ṩ��������ؽӿ�
// -------------------------------------------------------------------------
//  History:
//				11/05/2014 �ӿ�,�߼�������ײ�
//
////////////////////////////////////////////////////////////////////////////
#ifndef Core_Skeleton_Hpp
#define Core_Skeleton_Hpp

#include<memory>

#include "..\IndePlatform\LeoMath.h"
namespace leo{
	//��������(������Ϣ,������Ϣ,������Ϣ)
	struct SkeletonData;

	//����ʵ��(�������ݿɱ����ʵ������)
	class SkeletonInstance;

	//δʵ��
	class SkeletonGroup;

	class SkeletonInstance{
		//����Ĺ�������
		std::shared_ptr<SkeletonData> mSkeData;
		//��������(����hash���ֵ)
		std::size_t  mAniIndex;
		//��׼ʱ���(range[0.f,1.f])
		float mNorT;
		//��Ƥ����
		std::unique_ptr<float4x4[]> mSkinMatrix;
	};
}

#endif