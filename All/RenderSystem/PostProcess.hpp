////////////////////////////////////////////////////////////////////////////
//
//  Leo Engine Source File.
//  Copyright (C), FNS Studios, 2014-2015.
// -------------------------------------------------------------------------
//  File name:   RenderSystem/PostProcess.hpp
//  Version:     v1.00
//  Created:     08/08/2015 by leo hawke.
//  Compilers:   Visual Studio.NET 2015
//  Description: ͼ����
// -------------------------------------------------------------------------
//  History:
////////////////////////////////////////////////////////////////////////////

#ifndef ShaderSystem_PostProcess_Hpp
#define ShaderSystem_PostProcess_Hpp

#include <leo2DMath.hpp> //ops::Rect
#include <Core\COM.hpp>  //win::unique_com
#include <Core\Vertex.hpp> //Cord::NDCtoUV
#include <utility.hpp>
/*!	\defgroupRenderSystem Library
\brief ��Ⱦϵͳ�⡣
\since build 1.00
*/
//@{

struct ID3D11PixelShader;
struct ID3D11VertexShader;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11InputLayout;
LEO_BEGIN
/*!
\ingroup RenderSystem
\def PostProcess
\brief ���ڸ���ͼ�β����Ļ����ࡣ
\note  ������DeferredShading
\note  ������ԴmPixelShader��mVertexBuffer
\since build 1.00
*/
class PostProcess : noncopyable {
public:
	PostProcess(ID3D11Device*);
	virtual ~PostProcess();

	PostProcess(PostProcess && rvalue);
	void operator=(PostProcess&& rvalue);

	/*!
	\def BindProcess
	\brief ��PixelShader��
	\param psfilename ָ��PixelShader�������ļ���
	\since build 1.00
	*/
	bool BindProcess(ID3D11Device*,const std::wstring& psfilename);
	bool BindProcess(ID3D11Device*,const wchar_t* psfilename);

	/*!
	\def BindRect
	\brief �󶨻�������
	\param src ָ��Դ��ͼʹ�õ�����
	\param dst ָ��Ŀ����ͼ���Ƶ�����
	\note  axis_system::dx_texture_system
	\since build 1.00
	*/
	bool BindRect(ID3D11Device*,const ops::Rect& src,const ops::Rect& dst);

	virtual bool Apply(ID3D11DeviceContext*);


	void Draw(ID3D11DeviceContext* context, ID3D11ShaderResourceView* src, ID3D11RenderTargetView* dst);
public:
	struct Vertex {
		float4 PosH;
		float2 Tex;

		//�ѿ�������ϵ ӳ���� UV����
		Vertex(const float4& pos)
			:PosH(pos), Tex(leo::Coord::NDCtoUV(pos.x, pos.y))
		{}
	};
private:
	win::unique_com<ID3D11PixelShader> mPixelShader;
	win::unique_com<ID3D11Buffer> mVertexBuffer;

	//TRIANGLESTRIP
	/*
	2-----0
	|     |
	|     |
	3-----1
	*/
	Vertex mVertexs[4] = {
		{ float4(+1.f, +1.f, 1.f, 1.f) },
		{ float4(+1.f, -1.f, 1.f, 1.f) },
		{ float4(-1.f, +1.f, 1.f, 1.f) },
		{ float4(-1.f, -1.f, 1.f, 1.f) },
	};

	static struct {
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout*  mLayout = nullptr;
		std::size_t			mRefCount = 0;
	} mCommonThunk;
};

template<uint16 scalaer> 
class ScalaerProcess;

namespace details {
	class ScalaerProcessDelegate;
}

template<>
class ScalaerProcess<2> :public PostProcess {
public:
	ScalaerProcess(ID3D11Device*);
	~ScalaerProcess();

	bool Apply(ID3D11DeviceContext*);

private:
	std::unique_ptr<details::ScalaerProcessDelegate> mImpl;
};

template<>
class ScalaerProcess<4> :public PostProcess {
public:
	ScalaerProcess(ID3D11Device*);
	~ScalaerProcess();

	bool Apply(ID3D11DeviceContext*);

private:
	std::unique_ptr<details::ScalaerProcessDelegate> mImpl;
};

template<>
class ScalaerProcess<8> :public PostProcess {
public:
	ScalaerProcess(ID3D11Device*);
	~ScalaerProcess();

	bool Apply(ID3D11DeviceContext*);

private:
	std::unique_ptr<details::ScalaerProcessDelegate> mImpl;
};

LEO_END
//@}



#endif
