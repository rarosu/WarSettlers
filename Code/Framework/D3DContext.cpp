#include "D3DContext.hpp"

#include "Global.hpp"
#include "Window.hpp"
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

	D3DContext::Description::Buffer::Buffer()
		: m_width(0)
		, m_height(0)
	{}

	D3DContext::Description::Buffer::Buffer(unsigned int width, unsigned int height)
		: m_width(width)
		, m_height(height)
	{}

	D3DContext::Description::Description()
		: m_fullscreen(false)
	{}



	D3DContext::D3DContext(Window* targetWindow, const Description& description)
		: m_targetWindow(targetWindow)
		, m_activeViewport(0)
	{
		HRESULT result = S_OK;

		result = CreateDeviceAndSwapChain(description);
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to create device and swap chain");

		result = CreateBackBufferView();
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to create back buffer view");

		result = CreateDepthStencilBuffer(description.m_depthBuffer);
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


	void D3DContext::Clear(const D3DXCOLOR& color)
	{
		m_deviceContext->ClearRenderTargetView(m_backBufferView.Resource(), (const FLOAT*)color);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView.Resource(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3DContext::SwapBuffers()
	{
		m_swapChain->Present(0, 0);
	}


	HRESULT D3DContext::CreateDeviceAndSwapChain(Description description)
	{
		unsigned int deviceFlags = 0;

#ifdef DEBUG_MODE
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// Set the back buffer size to the window's client size, if the size of it is 0.
		description.m_backBuffer.m_width = (description.m_backBuffer.m_width == 0) ? m_targetWindow->GetClientWidth() : description.m_backBuffer.m_width;
		description.m_backBuffer.m_height = (description.m_backBuffer.m_height == 0) ? m_targetWindow->GetClientHeight() : description.m_backBuffer.m_height;

		// Create the swap chain description
		DXGI_SWAP_CHAIN_DESC scDescription;
		ZeroMemory(&scDescription, sizeof(scDescription));

		scDescription.BufferCount = 1;
		scDescription.BufferDesc.Width = description.m_backBuffer.m_width;
		scDescription.BufferDesc.Height = description.m_backBuffer.m_height;
		scDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scDescription.BufferDesc.RefreshRate.Numerator = 60;
		scDescription.BufferDesc.RefreshRate.Denominator = 1;
		scDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDescription.OutputWindow = m_targetWindow->GetHandle();
		scDescription.SampleDesc.Count = 1;
		scDescription.SampleDesc.Quality = 0;
		scDescription.Windowed = !description.m_fullscreen;

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

		m_backBufferDescription = description.m_backBuffer;

		return S_OK;
	}

	HRESULT D3DContext::CreateBackBufferView()
	{
		HRESULT result = S_OK;
		COMResource<ID3D11Texture2D> backBuffer;

		result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)(&backBuffer.Resource()));
		if (FAILED(result))
			return result;

		result = m_device->CreateRenderTargetView(backBuffer.Resource(), NULL, &m_backBufferView.Resource());
		if (FAILED(result))
			return result;

		return result;
	}

	HRESULT D3DContext::CreateDepthStencilBuffer(Description::Buffer description)
	{
		HRESULT result = S_OK;

		// If the size of the depth/stencil description is 0, set it to the same as the back buffer.
		description.m_width = (description.m_width == 0) ? m_backBufferDescription.m_width : description.m_width;
		description.m_height = (description.m_height == 0) ? m_backBufferDescription.m_height : description.m_height;

		r2AssertM(description.m_width >= m_backBufferDescription.m_width, "Invalid depth/stencil size");
		r2AssertM(description.m_height >= m_backBufferDescription.m_height, "Invalid depth/stencil size");

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC depthTextureDescription;
		ZeroMemory(&depthTextureDescription, sizeof(depthTextureDescription));

		depthTextureDescription.Width = description.m_width;
		depthTextureDescription.Height = description.m_height;
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

		m_depthStencilBufferDescription.m_width = description.m_width;
		m_depthStencilBufferDescription.m_height = description.m_height;

		return result;
	}

}
