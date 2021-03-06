#include "Display.h"

using namespace platform_ex::Windows::D3D12;
using namespace platform_ex;

Display::Display(IDXGIFactory4 * factory_4, ID3D12CommandQueue* cmd_queue, const DisplaySetting& setting,HWND hWnd)
	:hwnd(hWnd)
{
	full_screen = setting.full_screen;
	sync_interval = setting.sync_interval;

	//todo support WM_SIZE Message YSLib ?

	if (full_screen) {
		left = top = 0;
		width = setting.screen_width;
		height = setting.screen_height;
	}
	else {
		//TODO:change this code login to *Window Class
		RECT r;
		GetClientRect(hwnd, &r);
		POINT sp = { r.left,r.top };
		ClientToScreen(hwnd, &sp);
		left = sp.x;
		top = sp.y;
		width = r.right - r.left;
		height = r.bottom - r.top;
	}
	back_format = DXGI_FORMAT_R8G8B8A8_UNORM;//test code

	stereo_feature = factory_4->IsWindowedStereoEnabled();

	//TODO:
	/*
	BOOL allow_tearing = FALSE;
	if(SUCCEEDED(factory_5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,&allow_tearing,sizeof(allow_tearing))))
	tearing_allow = allow_tearing;
	*/

	//todo rotate support
	//std::swap(width,height);

	auto stereo = (Stereo_LCDShutter == setting.stereo_method) && stereo_feature;
	factory_4->RegisterStereoStatusWindow(hwnd, WM_SIZE, &stereo_cookie);

	sc_desc.Width = width; sc_desc.Height = height;
	sc_desc.Format = back_format;
	sc_desc.Stereo = stereo;
	sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sc_desc.BufferCount = NUM_BACK_BUFFERS;
	sc_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	sc_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (stereo) {
		sc_desc.SampleDesc.Count = 1;
		sc_desc.SampleDesc.Quality = 0;
		sc_desc.Scaling = DXGI_SCALING_NONE;
		sc_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	}
	else
	{
		sc_desc.SampleDesc.Count = std::min(static_cast<leo::uint32>(D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT), setting.sample_count);
		sc_desc.SampleDesc.Quality = setting.sample_quality;
		sc_desc.Scaling = DXGI_SCALING_STRETCH;
		sc_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	}

	//TODO
	/*
	if(tearing_fature)
	sc_desc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	*/

	//@{
	//TODO Match Adapter Mode
	sc_fs_desc.RefreshRate.Numerator = 60;
	sc_fs_desc.RefreshRate.Denominator = 1;
	//@}

	sc_fs_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sc_fs_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sc_fs_desc.Windowed = !full_screen;

	CheckHResult(CreateSwapChain(factory_4,cmd_queue));
}

HRESULT Display::CreateSwapChain(IDXGIFactory4 *factory_4,ID3D12CommandQueue* cmd_queue)
{
	COMPtr<IDXGISwapChain1>  swap_chain1 = nullptr;
	CheckHResult(factory_4->CreateSwapChainForHwnd(cmd_queue, hwnd,
		&sc_desc, &sc_fs_desc, nullptr, &swap_chain1.GetRef()));

	return swap_chain1->QueryInterface(IID_IDXGISwapChain3, reinterpret_cast<void**>(&swap_chain.GetRef()));
}
