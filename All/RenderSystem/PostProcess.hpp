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
	~PostProcess();

	PostProcess(PostProcess && rvalue);
	void operator=(PostProcess&& rvalue);

	bool BindProcess(ID3D11Device*,const std::string& psfilename);
	bool BindProcess(ID3D11Device*,const char* psfilename);

	/*!
	\def BindRect
	\brief �󶨻�������
	\param src ָ��Դ��ͼʹ�õ�����
	\param dst ָ��Ŀ����ͼ���Ƶ�����
	\note  axis_system::dx_texture_system
	\since build 1.00
	*/
	bool BindRect(ops::Rect& src, ops::Rect& dst);

	void Draw(ID3D11DeviceContext* context, ID3D11ShaderResourceView* src, ID3D11RenderTargetView* dst);
private:
	win::unique_com<ID3D11PixelShader> mPixelShader;
	win::unique_com<ID3D11Buffer> mVertexBuffer;

	static struct {
		ID3D11VertexShader* mVertexShader = nullptr;
		std::size_t			mRefCount = 0;
	} mCommonThunk;
};
LEO_END
//@}



#endif
