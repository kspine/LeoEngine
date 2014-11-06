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
#include "..\IndePlatform\ldef.h"
struct ID3D11DeviceContext;

namespace leo{
	//��������(������Ϣ,������Ϣ,������Ϣ)
	struct SkeletonData;

	//����ʵ��(�������ݿɱ����ʵ������)
	class SkeletonInstance;

	//δʵ��
	class SkeletonGroup;

	class Camera;

	class SkeletonInstance{
		//����Ĺ�������
		std::shared_ptr<SkeletonData> mSkeData;
		//��������(����hash���ֵ)
		std::size_t  mAniIndex;
		//��׼ʱ���(range[0.f,1.f])
		float mNorT;
		//��Ƥ����
		std::unique_ptr<float4x4[]> mSkinMatrix;
	public:
		SkeletonInstance(const std::shared_ptr<SkeletonData>& skeData);
		~SkeletonInstance();

		bool SwitchAnimation(const std::wstring& aniName);
		bool SwitchAnimation(const wchar_t* aniName);

		void Update();
		void Render(ID3D11DeviceContext* context, const Camera& camera);
	};

	struct SkeletonData{
		//���ļ�����<����:�ļ���>
		static std::shared_ptr<SkeletonData> Load(const std::wstring& fileName);
		static std::shared_ptr<SkeletonData> Load(const wchar_t* fileName);
		//���ڴ�����,δʵ��
		static std::shared_ptr<SkeletonData> Load(const std::unique_ptr<stdex::byte[]>& data, std::size_t dataSize);
	};
}

#endif