#include <atomic>
#include <thread>
#include <mutex>
#include <platform.h>
#include <Core\COM.hpp>
#include <Core\FileSearch.h>
#include <Core\Camera.hpp>
#include <Core\BilateralFilter.hpp>
#include <Core\EffectQuad.hpp>
#include <exception.hpp>
#include "d3dx11.hpp"
#include "PostProcess.hpp"
class HDRImpl {
public:
	HDRImpl(ID3D11Device* create, ID3D11Texture2D* src, ID3D11RenderTargetView* dst)
	:mScalerProcess(std::make_unique<leo::ScalaerProcess<4>>(create)){
		std::thread create_thread(&HDRImpl::create_method, this, create, src, dst);
		create_thread.detach();
	}

	~HDRImpl() {
	}

	void ReSize(ID3D11Device* create, ID3D11Texture2D* src, ID3D11RenderTargetView* dst) {
		std::thread resize_thread(&HDRImpl::resize_method, this, create, src, dst, true);
		resize_thread.detach();
	}

	void Apply(ID3D11DeviceContext* context) {
		//do nothing
	}

	void Draw(ID3D11DeviceContext* context) {
		while (!ready)
			;
		//TODO:SUPPORT MSAA
		context->CopyResource(mSrcCopyTex, mSrcPtr);
		context->ExecuteCommandList(mCommandList, false);
	}
protected:
	void create_method(ID3D11Device* create, ID3D11Texture2D* src, ID3D11RenderTargetView* dst) {
		resize_method(create, src, dst, false);
	}

	void resize_method(ID3D11Device* create, ID3D11Texture2D* src, ID3D11RenderTargetView* dst, bool release = true) {
		if (!mutex.try_lock())
			return;//无视两个连续的resize调用中的后一个
		ready = false;
		if (release) {
			mSrcCopy->Release();
			mSrcCopyTex->Release();
		}

		{
		//first create scale tex res
			D3D11_TEXTURE2D_DESC texDesc;
			src->GetDesc(&texDesc);

			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			leo::dxcall(create->CreateTexture2D(&texDesc, nullptr, &mSrcCopyTex));
			leo::dxcall(create->CreateShaderResourceView(mSrcCopyTex, nullptr, &mSrcCopy));
			mSrcPtr = src;

			D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
			dst->GetDesc(&rtDesc);
			
			texDesc.Width /= 4;
			texDesc.Height /= 4;
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			auto mScaleTex = leo::win::make_scope_com<ID3D11Texture2D>(nullptr);
			leo::dxcall(create->CreateTexture2D(&texDesc, nullptr, &mScaleTex));
			leo::dxcall(create->CreateShaderResourceView(mScaleTex, nullptr, &mScaleCopy));
			leo::dxcall(create->CreateRenderTargetView(mScaleTex, nullptr, &mScaleRT));
		}

		//some resourecr don't need resize
		static std::once_flag once_flag;
		std::call_once(once_flag, &HDRImpl::once_method, this, create);

		ready = true;
		mutex.unlock();
	}

	void once_method(ID3D11Device* create) {
		ID3D11DeviceContext* deferredcontext;
		leo::dxcall(create->CreateDeferredContext(0, &deferredcontext));

		mScalerProcess->Apply(deferredcontext);
		mScalerProcess->Draw(deferredcontext, mSrcCopy, mScaleRT);

		mCommandList.reset(nullptr);
		leo::dxcall(deferredcontext->FinishCommandList(false, &mCommandList));
		deferredcontext->Release();
	}
private:
	using atomic_bool = std::atomic_bool;
	atomic_bool ready = false;
	std::mutex mutex = {};
	leo::win::unique_com<ID3D11CommandList> mCommandList = nullptr;

	//common
	ID3D11Texture2D* mSrcPtr = nullptr;
	leo::win::unique_com<ID3D11Texture2D> mSrcCopyTex = nullptr;
	leo::win::unique_com<ID3D11ShaderResourceView> mSrcCopy = nullptr;

	leo::win::unique_com<ID3D11ShaderResourceView> mScaleCopy = nullptr;
	leo::win::unique_com<ID3D11RenderTargetView> mScaleRT = nullptr;

	std::unique_ptr<leo::PostProcess> mScalerProcess = nullptr;
};
