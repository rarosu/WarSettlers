#include "D3DWrapper.hpp"

#include <sstream>
#include "Global.hpp"

namespace Framework
{
	D3DWrapper::Viewport::Viewport()
		: m_left(0.0f)
		, m_top(0.0f)
		, m_width(1.0f)
		, m_height(1.0f)
	{}

	D3DWrapper::Viewport::Viewport(float left, float top, float width, float height)
		: m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
	{}

	D3DWrapper::Description::Description()
		: m_fullscreen(false)
	{}



	D3DWrapper::D3DWrapper(Window* targetWindow, const Description& description)
		: m_targetWindow(targetWindow)
	{
		HRESULT result = S_OK;

		m_targetWindow->AddEventListener(this);

		result = CreateDeviceAndSwapChain(description.m_displayMode, description.m_fullscreen);
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to create device and swap chain");

		result = CreateBackBufferView();
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to create back buffer view");

		result = CreateDepthStencilBuffer(description.m_displayMode.m_width, description.m_displayMode.m_height);
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to create depth stencil buffer/view");

		m_deviceContext->OMSetRenderTargets(1, &m_backBufferView.Resource(), m_depthStencilView.Resource());

		SetViewports(description.m_viewports);
	}

	D3DWrapper::~D3DWrapper() throw()
	{
		if (m_deviceContext != NULL)
			m_deviceContext->ClearState();

		if (m_swapChain != NULL)
			m_swapChain->SetFullscreenState(FALSE, NULL);

		m_targetWindow->RemoveEventListener(this);
	}



	const std::vector<D3DWrapper::Viewport> D3DWrapper::GetViewports() const
	{
		return m_viewports;
	}

	void D3DWrapper::SetViewports(const std::vector<Viewport>& viewports)
	{
		m_viewports = viewports;
		UpdateViewports(m_viewports);
	}


	COMResource<ID3D11Device> D3DWrapper::GetDevice()
	{
		return m_device;
	}

	COMResource<ID3D11DeviceContext> D3DWrapper::GetContext()
	{
		return m_deviceContext;
	}



	const D3D11_TEXTURE2D_DESC& D3DWrapper::GetBackBufferDescription() const
	{
		return m_backBufferDescription;
	}

	const D3D11_TEXTURE2D_DESC& D3DWrapper::GetDepthStencilBufferDescription() const
	{
		return m_depthStencilBufferDescription;
	}

	bool D3DWrapper::IsFullscreen()
	{
		BOOL fullscreen;

		HRESULT result = m_swapChain->GetFullscreenState(&fullscreen, NULL);
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to query fullscreen state");

		return fullscreen == TRUE;
	}


	HRESULT D3DWrapper::ChangeResolution(const DisplayMode& displayMode)
	{
		HRESULT result = S_OK;

		DXGI_MODE_DESC modeDescription;
		ZeroMemory(&modeDescription, sizeof(modeDescription));
		modeDescription.Format = DXGI_FORMAT_UNKNOWN;
		modeDescription.Width = displayMode.m_width;
		modeDescription.Height = displayMode.m_height;
		modeDescription.RefreshRate = displayMode.m_refreshRate;

		result = m_swapChain->ResizeTarget(&modeDescription);

		return result;
	}

	HRESULT D3DWrapper::ToggleFullscreen()
	{
		HRESULT result = S_OK;

		result = m_swapChain->SetFullscreenState(!IsFullscreen(), NULL);

		return result;
	}


	void D3DWrapper::Clear(const D3DXCOLOR& color)
	{
		m_deviceContext->ClearRenderTargetView(m_backBufferView.Resource(), (const FLOAT*)color);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView.Resource(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3DWrapper::SwapBuffers()
	{
		HRESULT result = m_swapChain->Present(0, 0);
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to swap buffers");
	}


	void D3DWrapper::WindowResized(unsigned int clientWidth, unsigned int clientHeight, unsigned int windowWidth, unsigned int windowHeight)
	{
		// Output debug information
		std::stringstream modeString;
		modeString << "Resize: " << clientWidth << " x " << clientHeight << "\n";
		OutputDebugString(modeString.str().c_str());

		// Release references to resources
		m_backBufferView = NULL;
		m_depthStencilView = NULL;
		m_depthStencilBuffer = NULL;

		// Resize the internal buffers
		HRESULT result = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to resize buffers");
		
		// Recreate view to internal back buffer.
		result = CreateBackBufferView();
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to recreate back buffer view");

		// Recreate depth/stencil buffer
		result = CreateDepthStencilBuffer(m_backBufferDescription.Width, m_backBufferDescription.Height);
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to recreate depth/stencil buffer");

		// Recreate the viewports
		UpdateViewports(m_viewports);
	}


	HRESULT D3DWrapper::CreateDeviceAndSwapChain(const DisplayMode& displayMode, bool fullscreen)
	{
		unsigned int deviceFlags = 0;

#ifdef DEBUG_MODE
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


		// Create the swap chain description
		DXGI_SWAP_CHAIN_DESC scDescription;
		ZeroMemory(&scDescription, sizeof(scDescription));
		
		scDescription.BufferCount = 1;
		scDescription.BufferDesc.Width = displayMode.m_width;
		scDescription.BufferDesc.Height = displayMode.m_height;
		scDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (fullscreen)
		{
			scDescription.BufferDesc.RefreshRate = displayMode.m_refreshRate;
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

	HRESULT D3DWrapper::CreateBackBufferView()
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

	HRESULT D3DWrapper::CreateDepthStencilBuffer(unsigned int width, unsigned int height)
	{
		HRESULT result = S_OK;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC depthTextureDescription;
		ZeroMemory(&depthTextureDescription, sizeof(depthTextureDescription));

		depthTextureDescription.Width = width;
		depthTextureDescription.Height = height;
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


	void D3DWrapper::UpdateViewports(const std::vector<Viewport>& viewportDescriptions)
	{
		std::vector<D3D11_VIEWPORT> viewports(viewportDescriptions.size());
		unsigned int backBufferWidth = m_backBufferDescription.Width;
		unsigned int backBufferHeight = m_backBufferDescription.Height;

		for (size_t i = 0; i < viewportDescriptions.size(); ++i)
		{
			viewports[i].TopLeftX = viewportDescriptions[i].m_left * backBufferWidth;
			viewports[i].TopLeftY = viewportDescriptions[i].m_top * backBufferHeight;
			viewports[i].Width = viewportDescriptions[i].m_width * backBufferWidth;
			viewports[i].Height = viewportDescriptions[i].m_height * backBufferHeight;
			viewports[i].MinDepth = 0.0f;
			viewports[i].MaxDepth = 1.0f;
		}

		m_deviceContext->RSSetViewports(viewports.size(), &viewports[0]);
	}

}
