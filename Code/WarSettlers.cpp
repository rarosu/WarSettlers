#include "WarSettlers.hpp"

#include <Framework\DisplayCapabilities.hpp>

int CreateAndStartGame(HINSTANCE instance, int showState)
{
	Framework::DisplayCapabilities displayCapabilities(true);
	Framework::Game::Description gameDescription;

	gameDescription.m_windowDescription.m_caption = "War Settlers";
	gameDescription.m_windowDescription.m_clientWidth = displayCapabilities.m_displayModes[0].m_width;
	gameDescription.m_windowDescription.m_clientHeight = displayCapabilities.m_displayModes[0].m_height;
	gameDescription.m_windowDescription.m_hasFrame = true;
	gameDescription.m_windowDescription.m_resizable = true;
	gameDescription.m_windowDescription.m_showState = showState;
	gameDescription.m_windowDescription.m_x = -1;
	gameDescription.m_windowDescription.m_y = -1;

	gameDescription.m_d3dDescription.m_displayMode = displayCapabilities.m_displayModes[0];
	gameDescription.m_d3dDescription.m_fullscreen = false;
	gameDescription.m_d3dDescription.m_viewports.push_back(Framework::D3DWrapper::Viewport());

	WarSettlers game(instance, gameDescription, displayCapabilities);
	return game.Start();
}


WarSettlers::WarSettlers(HINSTANCE instance, const Framework::Game::Description& description, const Framework::DisplayCapabilities& displayCapabilities)
	: Framework::Game(instance, description)
	, m_displayCapabilities(displayCapabilities)
	, m_currentDisplayMode(0)
{
	Framework::InputManager::Instance().AddInputListener(this);
}

WarSettlers::~WarSettlers() throw()
{
	Framework::InputManager::Instance().RemoveInputListener(this);
}


void WarSettlers::KeyPressed(int keyCode)
{
	switch (keyCode)
	{
		case 'R':
			++m_currentDisplayMode;
			m_currentDisplayMode %= m_displayCapabilities.m_displayModes.size();

			GetD3D().ChangeResolution(m_displayCapabilities.m_displayModes[m_currentDisplayMode]);
		break;

		case 'F':
			GetD3D().ToggleFullscreen();
		break;
	}
}


void WarSettlers::Update(double dt)
{
	// TODO: Add game logic
}

void WarSettlers::Render(double dt, float interpolation)
{
	// TODO: Render HUD, debug output, etc.
}
