//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   Core/MeshData.hpp
//  Version:     v1.00
//  Created:     11/6/2014 by leo hawke.
//  Compilers:   Visual Studio.NET 2013
//  Description: �ṩ�������ݽṹ
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#ifndef Core_MeshData_Hpp
#define Core_MeshData_Hpp



#include "..\IndePlatform\\leoint.hpp"

#include "Vertex.hpp"
#include "..\LightBuffer.h"
#include<vector>

struct ID3D11ShaderResourceView;
struct ID3D11Buffer;


namespace leo{
	//?��Ҫ���������е����������? no
	//?����ṹӦ���ͷ��Լ�����Դ��? yes
	//��Χģ����ʱ����QAQ

	struct MeshData{
		struct SubSetData{
			Material mMat;
			ID3D11ShaderResourceView* mDiff;
			ID3D11ShaderResourceView* mNormalMap;
			uint32 mIndexOffset;
			uint32 mIndicesCount;
			//std::wstring mDiffName;
			//std::wstring mNormalMapName;
		};
		//������Ϣ,GPU
		ID3D11Buffer* mNormalMapVB;
		//������Ϣ,GPU
		ID3D11Buffer* mIB;

		//������Ϣ,CPU
		//std::vector<uint32> mIndices;
		//������Ϣ,CPU
		//std::vector<Vertex::NormalMap> mVertexs;

		std::vector<SubSetData> mSubSets;
	};
}


#endif