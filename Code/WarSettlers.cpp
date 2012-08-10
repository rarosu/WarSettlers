#include "WarSettlers.hpp"

#include <Framework\DisplayCapabilities.hpp>

WarSettlers::DescriptionConstructor::DescriptionConstructor(int showState)
{
	Framework::DisplayCapabilities displayCapabilities(true);

	m_description.m_windowDescription.m_caption = "War Settlers";
	m_description.m_windowDescription.m_clientWidth = displayCapabilities.m_displayModes[0].m_width;
	m_description.m_windowDescription.m_clientHeight = displayCapabilities.m_displayModes[0].m_height;
	m_description.m_windowDescription.m_hasFrame = true;
	m_description.m_windowDescription.m_resizable = true;
	m_description.m_windowDescription.m_showState = showState;
	m_description.m_windowDescription.m_x = -1;
	m_description.m_windowDescription.m_y = -1;

	m_description.m_d3dDescription.m_displayMode = displayCapabilities.m_displayModes[0];
	m_description.m_d3dDescription.m_fullscreen = false;
	m_description.m_d3dDescription.m_viewports.push_back(Framework::D3DWrapper::Viewport());
}

WarSettlers::WarSettlers(HINSTANCE instance, int showState)
	: Framework::Game(instance, DescriptionConstructor(showState).m_description)
{
	
}


void WarSettlers::Update(float dt)
{
	// TODO: Add game logic
}

void WarSettlers::Render(float dt, float interpolation)
{
	// TODO: Render HUD, debug output, etc.
}
