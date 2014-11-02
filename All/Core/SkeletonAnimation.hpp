////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   Core/SkeletonAnimation.hpp
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
#include "..\IndePlatform\clock.hpp"
namespace leo{
	struct AnimationSample{
		//һ�����������,��Сȡ���ڹؽ���Ŀ->AnimationClip.mSkeleton.mJointCount;
		std::unique_ptr<JointPose[]> mJointsPose;
		//ÿ�����������Լ��ĳ���ʱ��
		float mTimePoint;
	};

	struct AnimationClip{
		//����,��Źؽ���Ŀ
		std::shared_ptr<Skeleton> mSkeleton;
		//֡�������
		std::uint8_t mFCount;
		std::unique_ptr<AnimationSample[]> mSamples;
		bool mLoop;
		//if ture => arrsize(mSamples) = mFCount;
		//else => arrsiez(mSamples) => mFCount +1;

		//��λ,��
		float GetTotalTime() const{
			return mSamples[mFCount].mTimePoint;
		}

		//��λ,֡
		float CalcFrame(float t) const{
			if (t == 1.f)
				if (mLoop)
					return mFCount + 0.5f;
				else
					return float(mFCount);
			auto time = t*GetTotalTime();
			auto f = 0u;
			for(; f != mFCount - 1; ++f)
				if ((time > mSamples[f].mTimePoint) && (time < mSamples[f + 1].mTimePoint))
					break;
			auto delta = mSamples[f + 1].mTimePoint - mSamples[f].mTimePoint;
			auto frac = (time - mSamples[f].mTimePoint) / delta;
			return f + frac;
		}

		leo::AnimationClip& operator=(leo::AnimationClip&& rvalue){
			mSkeleton = std::move(rvalue.mSkeleton);
			mFCount = rvalue.mFCount;
			mLoop = rvalue.mLoop;
			mSamples = std::move(rvalue.mSamples);
			return *this;
		}

		AnimationClip(leo::AnimationClip&& rvalue)
			:mSkeleton(std::move(rvalue.mSkeleton)), mFCount(rvalue.mFCount),
			mLoop(rvalue.mLoop),mSamples(std::move(rvalue.mSamples)){

		}

		AnimationClip() = default;
	};

	class Animation{
		AnimationClip mClip;
		//range (0,1),Current Time State 
		float mT;
		//ctor/loop's Game Elapsed
		float mElapsed;
		//���ڴ�ż�����
		SkeletonPose mSkePose;
		//��������
		float mSpeed = 1.f;
	public:
		Animation(Animation&& rvalue)
			:mClip(std::move(rvalue.mClip)), mT(rvalue.mT), mElapsed(rvalue.mElapsed), mSkePose(std::move(mSkePose)),
			mSpeed(rvalue.mSpeed){

		}

		Animation() = default;

		std::pair<uint32, uint32> CalcFrameIndex(float frame){
			auto first = (uint32)(std::floor(frame));
			auto second = first + 1u;

			if (mClip.mLoop)
				first %= (mClip.mFCount + 1), second %= (mClip.mFCount + 1);
			else
				clamp(0u, unsigned int(mClip.mFCount), second);
			return{ first, second };
		}
		float CalcFrameInterpolate(float frame){
			return frame - std::floor(frame);
		}
	public:
		SkeletonPose& Update(){
			//�Ѿ�����������,ѭ�������߼�,��ѭ��ֱ�ӷ���
			if (mT == 1.f)
				if (mClip.mLoop)
					mT = 0.f;
				else
					return mSkePose;
			auto cElapsed = clock::GameClock::Now<>();
			auto ElapsedT = (cElapsed - mElapsed) / mClip.GetTotalTime();
			mElapsed = cElapsed;

			mT += ElapsedT;
			clamp(0.f, 1.f, mT);
			
			auto frame = mClip.CalcFrame(mT);
			auto Indices = CalcFrameIndex(frame);

			for (auto jointIndex = 0u; jointIndex != mClip.mSkeleton->mJointCount; ++jointIndex){
				auto & p1 = mClip.mSamples[Indices.first].mJointsPose[jointIndex];
				auto & p2 = mClip.mSamples[Indices.second].mJointsPose[jointIndex];

				mSkePose.mLocalPoses[jointIndex] = Lerp(p1, p2, CalcFrameInterpolate(frame));
			}

			//�ӽڵ��ں���,ֻ���ҵ���,�������
			for (auto jointIndex = 0u; jointIndex != mClip.mSkeleton->mJointCount; ++jointIndex){
				auto & joint = mSkePose.mSkeleton->mJoints[jointIndex];
				if (joint.mParent != 0xFFu){
					save(mSkePose.mGlobalPoses[jointIndex] ,Multiply(load(mSkePose.mGlobalPoses[joint.mParent]),mSkePose.mLocalPoses[jointIndex].operator std::array<__m128, 4U>()));
				}
				save(mSkePose.mSkinMatrixs[jointIndex],Multiply(load(joint.mInvBindPose),load(mSkePose.mGlobalPoses[jointIndex])));
			}

			return mSkePose;
		}

		void SetData(AnimationClip&& clip){
			mClip = std::move(clip);
			mSkePose.mSkeleton =std::move( mClip.mSkeleton);
		}
	};

	//�����������Ƥ��ɫ��,����Ϊһ�Ѿ��������,��SkeltonPose������
	//��Ҫʵ�ֲ�ֵ����,����ʱ��,��������<static ����>
}
#endif