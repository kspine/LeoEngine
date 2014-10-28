////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   Core/Joint.hpp
//  Version:     v1.00
//  Created:     10/28/2014 by leo hawke.
//  Compilers:   Visual Studio.NET 2013
//  Description: �ṩ�����㶯������߼�
// -------------------------------------------------------------------------
//  History:
//				
//
////////////////////////////////////////////////////////////////////////////

#ifndef Core_SkeletonAnimation_Hpp
#define Core_SkeletonAnimation_Hpp

#include "Skeleton.hpp"

namespace leo{
	struct AnimationSample{
		//һ�����������,��Сȡ���ڹؽ���Ŀ->AnimationClip.mSkeleton.mJointCount;
		std::unique_ptr<JointPose[]> mJointsPose;
	};

	struct AnimationClip{
		//����,��Źؽ���Ŀ
		std::unique_ptr<Skeleton> mSkeleton;
		//ÿ�����֡
		float mFPS;
		//֡��Ŀ
		std::uint8_t mFCount;
		std::unique_ptr<AnimationSample[]> mSamples;
		bool mLoop;
		//if ture => arrsize(mSamples) = mFCount;
		//else => arrsiez(mSamples) => mFCount +1;
	};

	class Animation{
		AnimationClip mClip;
		float t;
	};

	//�����������Ƥ��ɫ��,����Ϊһ�Ѿ��������,��SkeltonPose������
	//��Ҫʵ�ֲ�ֵ����,����ʱ��,��������<static ����>
}
#endif