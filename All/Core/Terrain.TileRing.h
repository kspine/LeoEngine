////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2014.
// -------------------------------------------------------------------------
//  File name:   Core/Terrain.TileRing.h
//  Version:     v1.00
//  Created:     9/7/2014 by leo hawke.
//  Compilers:   Visual Studio.NET 2013
//  Description: �ṩ������Ⱦ��Tile������,Tile����ͬ���,������,ÿ��Ringʹ�ò�ͬ
//				 ��Tile/Patch��С���첻ͬ��ϸ�ڲ��,ʵ�������Ƕ���������,�����ܹ�
//				 �ڳ����ϱ�ʾΪԲ,�ڲ�����ϸ�ھ���������,��֮Ϊ�˻���
// -------------------------------------------------------------------------
//  History:
//				
//
////////////////////////////////////////////////////////////////////////////

#ifndef Core_TileRing_h
#define Core_TileRing_h

#include "..\leomath.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
namespace leo
{
	namespace Vertex
	{
		struct InstanceData;
		struct Adjacency;
	}
	//Ctor
	//Int dimensions specified to the ctor in numbers of tiles
	//    <-   outerWidth  ->
	//    ###################
	//    ###################
	//    ###             ###
	//    ###<-holeWidth->###
	//    ###             ###
	//    ###    (0,0)    ###
	//    ###             ###
	//    ###             ###
	//    ###             ###
	//    ###################
	//    ###################
	class TileRing
	{
	public:
		// holeWidth & outerWidth are nos. of tiles
		// tileSize is a world-space length
		TileRing(ID3D11Device*, int holeWidth, int outerWidth, float tileSize);
		~TileRing();

		void SetRenderingState(ID3D11DeviceContext*) const;

		int   outerWidth() const { return mOuterWidth; }
		int   nTiles()     const { return mnTiles; }
		float tileSize()   const { return mtileSize; }

		TileRing(const TileRing&) = delete;
		TileRing& operator=(const TileRing&) = delete;
	private:
		void CreateInstanceDataVB(ID3D11Device*);
		bool InRing(int x, int y) const;
		void AssignNeighbourSizes(int x, int y,Vertex::Adjacency*) const;

		ID3D11Buffer* mPositionsVB;

		const int mHoleWidth, mOuterWidth, mRingWidth;
		const int mnTiles;
		const float mtileSize;
		Vertex::InstanceData* mVBData;
	};

}
#endif