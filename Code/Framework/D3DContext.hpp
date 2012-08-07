#ifndef FRAMEWORK_D3D_CONTEXT_HPP
#define FRAMEWORK_D3D_CONTEXT_HPP

#include <vector>

#include "WinInclude.hpp"
#include "D3DInclude.hpp"
#include "COMResource.hpp"
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
	class D3DContext : public r2::Singleton<D3DContext>
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
			struct Buffer
			{
				unsigned int m_width;
				unsigned int m_height;

				Buffer();
				Buffer(unsigned int width, unsigned int height);
			};

			// Determines if the application should start in fullscreen
			bool m_fullscreen;
			
			// The description of the back buffer. Set width or height to 0 to make it the same size as the target window.
			Buffer m_backBuffer;

			// The description of the depth buffer. Set width or height to 0 to make it the same size as the back buffer.
			Buffer m_depthBuffer;

			// The viewports used to render to.
			std::vector<Viewport> m_viewports;


			Description();
		};


		D3DContext(Window* targetWindow, const Description& description);
		~D3DContext() throw();


		// Viewport management
		const std::vector<Viewport> GetViewports() const;
		void SetViewports(const std::vector<Viewport>& viewports);
		void SetActiveViewport(unsigned int index);
		unsigned int GetActiveViewport() const;


		// Clear the back buffer with the specified color, clear depth buffer to 1.0
		// and clear the stencil buffer to 0.
		void Clear(const D3DXCOLOR& color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		// Swap the back- and front buffer.
		void SwapBuffers();
	private:
		Window* m_targetWindow;

		COMResource<ID3D11Device> m_device;
		COMResource<ID3D11DeviceContext> m_deviceContext;
		COMResource<IDXGISwapChain> m_swapChain;

		COMResource<ID3D11RenderTargetView> m_backBufferView;
		COMResource<ID3D11Texture2D> m_depthStencilBuffer;
		COMResource<ID3D11DepthStencilView> m_depthStencilView;

		Description::Buffer m_backBufferDescription;
		Description::Buffer m_depthStencilBufferDescription;

		std::vector<Viewport> m_viewports;
		unsigned int m_activeViewport;


		// Methods for setting up D3D11
		HRESULT CreateDeviceAndSwapChain(Description description);
		HRESULT CreateBackBufferView();
		HRESULT CreateDepthStencilBuffer(Description::Buffer description);
	};
}

#endif