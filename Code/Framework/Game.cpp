#include "Game.hpp"

namespace Framework
{
	Game::Game(HINSTANCE m_instance, const Description& description)
		: m_running(true)
		, m_logger("WarSettlers.log")
		, m_window(m_instance, description.m_windowDescription)
		, m_d3d(&m_window, description.m_d3dDescription)
	{}

	int Game::Start()
	{
		while (m_running)
		{
			m_running = m_window.ProcessMessages();

			// TODO: Add timing
			Update(0.0f);
			Render(0.0f, 0.0f);
		}

		return m_window.GetExitValue();
	}


	Window& Game::GetWindow()
	{
		return m_window;
	}

	D3DWrapper& Game::GetD3D()
	{
		return m_d3d;
	}


	void Game::RenderWrapper(float dt, float interpolation)
	{
		// Clear the back buffer.
		m_d3d.Clear();

		// TODO: Traverse the scene manager and render entities.

		// Let the subclass render the rest.
		Render(dt, interpolation);

		// Swap front- and back buffer.
		m_d3d.SwapBuffers();
	}
}