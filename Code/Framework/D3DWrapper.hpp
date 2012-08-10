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
		Initializes and manages D3D11. Handles the device,
		the swap chain and the back buffer, the depth/stencil buffer and
		the viewports.
	*/
	class D3DWrapper : public r2::Singleton<D3DWrapper>, public WindowEventListener
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
			Describes properties for initialization.
		*/
		struct Description
		{
			// The display mode to start the application in (resolution, refresh rate, etc.)
			DisplayMode m_displayMode;

			// Determines if the application should start in fullscreen
			bool m_fullscreen;

			// The viewports used to render to.
			std::vector<Viewport> m_viewports;


			Description();
		};


		/**
			Constructor & Destructor
		*/
		D3DWrapper(Window* targetWindow, const Description& description);
		~D3DWrapper() throw();


		/**
			Viewport management
		*/
		const std::vector<Viewport> GetViewports() const;
		void SetViewports(const std::vector<Viewport>& viewports);


		/**
			Get D3D11 interfaces
		*/
		COMResource<ID3D11Device> GetDevice();
		COMResource<ID3D11DeviceContext> GetContext();


		/** 
			Get metadata about the back buffer.
		*/
		const D3D11_TEXTURE2D_DESC& GetBackBufferDescription() const;

		/** 
			Get metadata about the depth-stencil buffer.
		*/
		const D3D11_TEXTURE2D_DESC& GetDepthStencilBufferDescription() const;

		/** 
			Return whether we're in fullscreen mode or not.
		*/
		bool IsFullscreen();


		/** 
			Change the display mode (this resizes the window in windowed mode, and sets the resolution in fullscreen mode)
		*/
		HRESULT ChangeResolution(const DisplayMode& displayMode);

		/**
			Toggle fullscreen mode.
		*/
		HRESULT ToggleFullscreen();


		/** 
			Clear the back buffer with the specified color, the depth buffer to 1.0
			and the stencil buffer to 0.
		*/
		void Clear(const D3DXCOLOR& color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		/**
			Swap the back- and front buffer.
		*/
		void SwapBuffers();


		/** 
			Interface: WindowEventListener
			Listen to WM_SIZE messages and resize the internal buffers properly.
		*/
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

		D3D11_TEXTURE2D_DESC m_backBufferDescription;
		D3D11_TEXTURE2D_DESC m_depthStencilBufferDescription;

		std::vector<Viewport> m_viewports;


		/** 
			Methods for setting up D3D11
		*/
		HRESULT CreateDeviceAndSwapChain(const DisplayMode& displayMode, bool fullscreen);
		HRESULT CreateBackBufferView();
		HRESULT CreateDepthStencilBuffer(unsigned int width, unsigned int height);

		/**
			Sets the internal viewports in D3D11, described by the given data.
		*/
		void UpdateViewports(const std::vector<Viewport>& viewportDescriptions);
	};
}

#endif