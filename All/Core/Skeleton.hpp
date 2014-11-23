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


#include "..\IndePlatform\LeoMath.h"
#include "..\IndePlatform\ldef.h"
#include "..\IndePlatform\memory.hpp"
#include "..\IndePlatform\clock.hpp"
#include "Lod.h"
#include "Material.h"
#include "Vertex.hpp"
#include "CoreObject.hpp"
#include <vector>
#include<map>

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
namespace leo{
	//��������(������Ϣ,������Ϣ,������Ϣ)
	struct SkeletonData;

	//����ʵ��(�������ݿɱ����ʵ������)
	class SkeletonInstance;

	//δʵ��
	class SkeletonGroup;

	class Camera;


	struct AnimationSample;
	struct AnimationClip;

	struct SkeletonData{
		~SkeletonData();

		//��ʹ�õ����LOD����,��͵�ϸ��
		const static std::uint8_t MinLodLevel = 3;

		//���ļ�����<����:�ļ���>
		static std::shared_ptr<SkeletonData> Load(const std::wstring& fileName);

		static std::shared_ptr<SkeletonData> Load(const wchar_t* fileName){
			return SkeletonData::Load(std::wstring(fileName));
		}
		//���ڴ�����,δʵ��
		static std::shared_ptr<SkeletonData> Load(const MemoryChunk& memory);

		//Mesh begin
		ID3D11Buffer* mVertexBuffer = nullptr;
		//IndicesBuffer,Not IndiceBuffers
		ID3D11Buffer* mIndicesBuffer = nullptr;
		struct SubSet{
			LodIndex mLodIndices[MinLodLevel + 1];
			Material mMat;
			ID3D11ShaderResourceView* mTexSRV = nullptr;
			ID3D11ShaderResourceView* mNormalSRV = nullptr;
		};
		std::vector<SubSet> mSubSets;
		using vertex = Vertex::NormalMap;
		using vertex_adj = Vertex::SkeAdjInfo;
		//Mesh end

		//Skeleton begin
		struct Joint : public GeneralAllocatedObject{
			//�����Ƶ���任
			float4x4 mInvBindPose;
			//�ַ���ɢ�б�ʶ��
			std::size_t mNameSid = 0;
			//���ڵ���±�,����ʽ���->Skeleton�������Joint
			std::uint8_t mParent;

			const wchar_t* GetName() const{
				return unhash(mNameSid);
			}

			void SetName(const wchar_t* str){
				mNameSid = hash(str);
			}
		};

		struct Skeleton{
			std::unique_ptr<Joint[]> mJoints;
			//�ؽ���Ŀ
			std::uint8_t mJointCount;
		} mSkeleton;

		using JointPose = SQTObject;

		ID3D11Buffer* mAnimationDataBUffer = nullptr;
		//Skeleton end

		std::map<std::size_t, AnimationClip> mAnimations;
		std::vector<std::size_t> mAnimaNames;
	};

	struct AnimationSample{
		std::unique_ptr<SkeletonData::JointPose[]> mJointsPose;
		float mTimePoint;
	};

	struct AnimationClip{
		std::uint8_t mFCount = 0;
		std::unique_ptr<AnimationSample[]> mSamples;
		bool mLoop = true;

		AnimationClip() = default;

		AnimationClip(AnimationClip&& rvalue)
			:mFCount(rvalue.mFCount), mSamples(std::move(rvalue.mSamples)), mLoop(rvalue.mLoop){

		}

		//��λ,��
		float GetTotalTime() const;

		//��λ,֡
		float CalcFrame(float t) const;
	};

	class SkeletonInstance : public SQTObject{
		//����Ĺ�������
		std::shared_ptr<SkeletonData> mSkeData;
		//��������(����hash���ֵ)
		std::size_t  mAniIndex;
		//��׼ʱ���(range[0.f,1.f])
		float mNorT;
		float mElapsed = clock::GameClock::Now<>();
		//ÿ�������Ĳ�������
		std::map<std::size_t, float> mSpeedPerAni;
		//��Ƥ����
		std::unique_ptr<float4x4Object[]> mSkinMatrixs;
		std::unique_ptr<SkeletonData::JointPose[]> mLocalPoses;
		std::unique_ptr<float4x4Object[]> mGlobalPoses;
	public:
		SkeletonInstance(const std::shared_ptr<SkeletonData>& skeData);
		~SkeletonInstance();

		SkeletonInstance() = default;
		SkeletonInstance& operator=(const std::shared_ptr<SkeletonData>& skeData);

		bool SwitchAnimation(const wchar_t* aniName);

		void SetCurrentAniSpeed(float speed){
			mSpeedPerAni[mAniIndex] = speed;
 		}

		std::vector<const wchar_t*> GetAniNames() const;

		void Update();
		void Render(const Camera& camera);
	};

	
}

#endif