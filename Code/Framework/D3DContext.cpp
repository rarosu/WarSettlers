#include "D3DContext.hpp"

#include "Global.hpp"
#include <Libs\r2tk\r2-exception.hpp>
#include <Libs\r2tk\r2-assert.hpp>

namespace Framework
{
	D3DContext::Viewport::Viewport()
		: m_left(0.0f)
		, m_top(0.0f)
		, m_width(1.0f)
		, m_height(1.0f)
	{}

	D3DContext::Viewport::Viewport(float left, float top, float width, float height)
		: m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
	{}

	D3DContext::Description::Description()
		: m_fullscreen(false)
		, m_vsync(false)
	{}



	D3DContext::D3DContext(Window* targetWindow, const Description& description)
		: m_targetWindow(targetWindow)
		, m_activeViewport(0)
		, m_displayMode(description.m_displayMode)
		, m_vsync(description.m_vsync)
	{
		HRESULT result = S_OK;

		result = CreateDeviceAndSwapChain(description.m_displayMode, description.m_fullscreen, description.m_vsync);
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to create device and swap chain");

		result = CreateBackBufferView();
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to create back buffer view");

		result = CreateDepthStencilBuffer(description.m_displayMode);
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to create depth stencil buffer/view");

		m_deviceContext->OMSetRenderTargets(1, &m_backBufferView.Resource(), m_depthStencilView.Resource());

		SetViewports(description.m_viewports);
		SetActiveViewport(0);
	}

	D3DContext::~D3DContext() throw()
	{
		if (m_deviceContext != NULL)
			m_deviceContext->ClearState();

		if (m_swapChain != NULL)
			m_swapChain->SetFullscreenState(FALSE, NULL);
	}



	const std::vector<D3DContext::Viewport> D3DContext::GetViewports() const
	{
		return m_viewports;
	}

	void D3DContext::SetViewports(const std::vector<Viewport>& viewports)
	{
		m_viewports = viewports;
	}

	void D3DContext::SetActiveViewport(unsigned int index)
	{
		r2AssertM(index < m_viewports.size(), "Invalid viewport");
		m_activeViewport = index;

		D3D11_VIEWPORT vp;
		vp.TopLeftX = m_viewports[m_activeViewport].m_left * m_targetWindow->GetClientWidth();
		vp.TopLeftY = m_viewports[m_activeViewport].m_top * m_targetWindow->GetClientHeight();
		vp.Width = m_viewports[m_activeViewport].m_width * m_targetWindow->GetClientWidth();
		vp.Height = m_viewports[m_activeViewport].m_height * m_targetWindow->GetClientHeight();
		vp.MinDepth = 0;
		vp.MaxDepth = 1;

		m_deviceContext->RSSetViewports(1, &vp);
	}

	unsigned int D3DContext::GetActiveViewport() const
	{
		return m_activeViewport;
	}


	const D3D11_TEXTURE2D_DESC& D3DContext::GetBackBufferDescription() const
	{
		return m_backBufferDescription;
	}

	const D3D11_TEXTURE2D_DESC& D3DContext::GetDepthStencilBufferDescription() const
	{
		return m_depthStencilBufferDescription;
	}

	bool D3DContext::IsFullscreen()
	{
		BOOL fullscreen;
		if (FAILED(m_swapChain->GetFullscreenState(&fullscreen, NULL)))
			throw r2ExceptionRuntimeM("Failed to query fullscreen state");

		return fullscreen == TRUE;
	}

	bool D3DContext::IsVSyncEnabled() const
	{
		return m_vsync;
	}


	bool D3DContext::ChangeResolution(const DisplayMode& displayMode, bool fullscreen, bool vsync)
	{
		// Release buffers relying on the back buffer and the swap chain
		m_backBufferView = NULL;
		m_depthStencilView = NULL;
		m_depthStencilBuffer = NULL;

		// Change resolution (or the size of the target window, if windowed)
		DXGI_MODE_DESC modeDescription;
		modeDescription.Width = displayMode.m_description.Width;
		modeDescription.Height = displayMode.m_description.Height;
		modeDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (vsync)
		{
			modeDescription.RefreshRate.Numerator = displayMode.m_description.RefreshRate.Numerator;
			modeDescription.RefreshRate.Denominator = displayMode.m_description.RefreshRate.Denominator;
		}
		else
		{
			modeDescription.RefreshRate.Numerator = 0;
			modeDescription.RefreshRate.Denominator = 1;
		}
		
		modeDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		modeDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		// Change the resolution and update fields
		if (FAILED(m_swapChain->ResizeTarget(&modeDescription)))
			return false;
		if (FAILED(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
			return false;
		m_displayMode = displayMode;
		m_vsync = vsync;

		// Handle fullscreen switch
		if (IsFullscreen() != fullscreen)
		{
			if (FAILED(m_swapChain->SetFullscreenState(fullscreen, NULL)))
				return false;

			// To prevent flickering, call resize target again with refresh rate members zeroed out.
			modeDescription.RefreshRate.Numerator = 0;
			modeDescription.RefreshRate.Denominator = 0;

			if (FAILED(m_swapChain->ResizeTarget(&modeDescription)))
				return false;
		}

		// Recreate the back buffer view
		if (FAILED(CreateBackBufferView()))
			return false;

		// Recreate the depth stencil view
		if (FAILED(CreateDepthStencilBuffer(displayMode)))
			return false;

		// TODO: Update the viewport dimensions

		return true;
	}

	bool D3DContext::ToggleFullscreen()
	{
		return ChangeResolution(m_displayMode, !IsFullscreen(), m_vsync);
	}

	bool D3DContext::ToggleVSync()
	{
		return ChangeResolution(m_displayMode, IsFullscreen(), !m_vsync);
	}



	void D3DContext::Clear(const D3DXCOLOR& color)
	{
		m_deviceContext->ClearRenderTargetView(m_backBufferView.Resource(), (const FLOAT*)color);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView.Resource(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3DContext::SwapBuffers()
	{
		if (m_vsync)
			m_swapChain->Present(1, 0);
		else
			m_swapChain->Present(0, 0);
	}


	void D3DContext::WindowResized(unsigned int clientWidth, unsigned int clientHeight, unsigned int windowWidth, unsigned int windowHeight)
	{

	}


	HRESULT D3DContext::CreateDeviceAndSwapChain(const DisplayMode& displayMode, bool fullscreen, bool vsync)
	{
		unsigned int deviceFlags = 0;

#ifdef DEBUG_MODE
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


		// Create the swap chain description
		DXGI_SWAP_CHAIN_DESC scDescription;
		ZeroMemory(&scDescription, sizeof(scDescription));
		
		scDescription.BufferCount = 1;
		scDescription.BufferDesc.Width = displayMode.m_description.Width;
		scDescription.BufferDesc.Height = displayMode.m_description.Height;
		scDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (vsync)
		{
			scDescription.BufferDesc.RefreshRate.Numerator = displayMode.m_description.RefreshRate.Numerator;
			scDescription.BufferDesc.RefreshRate.Denominator = displayMode.m_description.RefreshRate.Denominator;
		}
		else
		{
			scDescription.BufferDesc.RefreshRate.Numerator = 0;
			scDescription.BufferDesc.RefreshRate.Denominator = 1;
		}
		
		scDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDescription.OutputWindow = m_targetWindow->GetHandle();
		scDescription.SampleDesc.Count = 1;
		scDescription.SampleDesc.Quality = 0;
		scDescription.Windowed = !fullscreen;
		scDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		

		// List the different Direct3D driver types. We'll try to render using hardware first, and only using the
		// reference driver if that fails.
		std::vector<D3D_DRIVER_TYPE> driverTypes;
		driverTypes.push_back(D3D_DRIVER_TYPE_HARDWARE);
		driverTypes.push_back(D3D_DRIVER_TYPE_REFERENCE);

		// Attempt to create the swap chain, the device and the device context, with the first working driver.
		HRESULT result = S_OK;
		for (size_t i = 0; i < driverTypes.size(); ++i)
		{
			result = D3D11CreateDeviceAndSwapChain(NULL,
				driverTypes[i],
				NULL,
				deviceFlags,
				NULL,
				0,
				D3D11_SDK_VERSION,
				&scDescription,
				&m_swapChain.Resource(),
				&m_device.Resource(),
				NULL,
				&m_deviceContext.Resource());

			if (SUCCEEDED(result))
				break;
		}

		// No driver worked - report error
		if (FAILED(result))
			return result;

		return S_OK;
	}

	HRESULT D3DContext::CreateBackBufferView()
	{
		HRESULT result = S_OK;
		COMResource<ID3D11Texture2D> backBuffer;

		// Get a reference to the back buffer from the swap chain
		result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)(&backBuffer.Resource()));
		if (FAILED(result))
			return result;

		// Create a view to the back buffer texture
		result = m_device->CreateRenderTargetView(backBuffer.Resource(), NULL, &m_backBufferView.Resource());
		if (FAILED(result))
			return result;

		// Store the description of the back buffer
		backBuffer->GetDesc(&m_backBufferDescription);

		return result;
	}

	HRESULT D3DContext::CreateDepthStencilBuffer(const DisplayMode& displayMode)
	{
		HRESULT result = S_OK;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC depthTextureDescription;
		ZeroMemory(&depthTextureDescription, sizeof(depthTextureDescription));

		depthTextureDescription.Width = displayMode.m_description.Width;
		depthTextureDescription.Height = displayMode.m_description.Height;
		depthTextureDescription.MipLevels = 1;
		depthTextureDescription.ArraySize = 1;
		depthTextureDescription.Format = DXGI_FORMAT_D32_FLOAT;
		depthTextureDescription.SampleDesc.Count = 1;
		depthTextureDescription.SampleDesc.Quality = 0;
		depthTextureDescription.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextureDescription.CPUAccessFlags = 0;
		depthTextureDescription.MiscFlags = 0;

		result = m_device->CreateTexture2D(&depthTextureDescription, NULL, &m_depthStencilBuffer.Resource());
		if (FAILED(result))
			return result;

		// Create depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescription;
		ZeroMemory(&depthStencilViewDescription, sizeof(depthStencilViewDescription));

		depthStencilViewDescription.Format = depthTextureDescription.Format;
		depthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDescription.Texture2D.MipSlice = 0;
	
		result = m_device->CreateDepthStencilView(m_depthStencilBuffer.Resource(), &depthStencilViewDescription, &m_depthStencilView.Resource());
		if (FAILED(result))
			return result;

		// Store the description of the buffer
		m_depthStencilBuffer->GetDesc(&m_depthStencilBufferDescription);

		return result;
	}

}
