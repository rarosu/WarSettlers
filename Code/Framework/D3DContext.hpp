#ifndef FRAMEWORK_D3D_CONTEXT_HPP
#define FRAMEWORK_D3D_CONTEXT_HPP

#include <vector>

#include "WinInclude.hpp"
#include "D3DInclude.hpp"
#include "COMResource.hpp"
#include "Window.hpp"
#include "DisplayCapabilities.hpp"
#include <Libs\r2tk\r2-singleton.hpp>

namespace Framework
{
	/**
		Forward-declare
	*/
	class Window;

	/**
		Initializes and manages the D3D11 context. Handles the device,
		the swap chain and the back buffer, the depth/stencil buffer and
		the viewports.
	*/
	class D3DContext : public r2::Singleton<D3DContext>, public WindowEventListener
	{
	public:
		/**
			Describes a viewport rendering area on the back buffer. All values
			are normalized [0.0, 1.0].
		*/
		struct Viewport
		{
			float m_left;
			float m_top;
			float m_width;
			float m_height;

			Viewport();
			Viewport(float left, float top, float width, float height);
		};
		
		/**
			Describes the properties of the context, for creation.
		*/
		struct Description
		{
			// The display mode to start the application in (resolution, refresh rate, etc.)
			DisplayMode m_displayMode;

			// Determines if the application should start in fullscreen
			bool m_fullscreen;

			// Determines if we're using vsync (using monitor's refresh rate).
			bool m_vsync;

			// The viewports used to render to.
			std::vector<Viewport> m_viewports;


			Description();
		};


		D3DContext(Window* targetWindow, const Description& description);
		~D3DContext() throw();


		// Viewport management.
		const std::vector<Viewport> GetViewports() const;
		void SetViewports(const std::vector<Viewport>& viewports);
		void SetActiveViewport(unsigned int index);
		unsigned int GetActiveViewport() const;


		// Get metadata about the back buffer.
		const D3D11_TEXTURE2D_DESC& GetBackBufferDescription() const;

		// Get metadata about the depth-stencil buffer.
		const D3D11_TEXTURE2D_DESC& GetDepthStencilBufferDescription() const;

		// Return whether we're in fullscreen mode or not.
		bool IsFullscreen();

		// Return whether VSync is enabled or not.
		bool IsVSyncEnabled() const;


		// Change the display mode, and whether we should be in fullscreen or using vsync.
		bool ChangeResolution(const DisplayMode& displayMode, bool fullscreen, bool vsync);

		// Toggle fullscreen mode.
		bool ToggleFullscreen();

		// Toggle vsync mode.
		bool ToggleVSync();


		// Clear the back buffer with the specified color, clear depth buffer to 1.0
		// and clear the stencil buffer to 0.
		void Clear(const D3DXCOLOR& color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		// Swap the back- and front buffer.
		void SwapBuffers();


		// Interface: WindowEventListener
		void WindowResized(unsigned int clientWidth, unsigned int clientHeight, unsigned int windowWidth, unsigned int windowHeight);
	private:
		Window* m_targetWindow;

		COMResource<ID3D11Device> m_device;
		COMResource<ID3D11DeviceContext> m_deviceContext;
		COMResource<IDXGISwapChain> m_swapChain;
		COMResource<IDXGIAdapter> m_adapter;

		COMResource<ID3D11RenderTargetView> m_backBufferView;
		COMResource<ID3D11Texture2D> m_depthStencilBuffer;
		COMResource<ID3D11DepthStencilView> m_depthStencilView;

		DisplayMode m_displayMode;
		bool m_vsync;
		D3D11_TEXTURE2D_DESC m_backBufferDescription;
		D3D11_TEXTURE2D_DESC m_depthStencilBufferDescription;

		std::vector<Viewport> m_viewports;
		unsigned int m_activeViewport;


		// Methods for setting up D3D11
		HRESULT CreateDeviceAndSwapChain(const DisplayMode& displayMode, bool fullscreen, bool vsync);
		HRESULT CreateBackBufferView();
		HRESULT CreateDepthStencilBuffer(const DisplayMode& displayMode);
	};
}

#endif