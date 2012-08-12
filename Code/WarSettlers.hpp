#ifndef WAR_SETTLERS_HPP
#define WAR_SETTLERS_HPP

#include <Framework\Game.hpp>

/**
	This function will read configuration, enumerate display modes
	and then create a WarSettlers object and starting the game. 
	Use this instead of manually creating a WarSettlers object.

	The return value is the return value of the application.
*/
int CreateAndStartGame(HINSTANCE instance, int showState);


/**
	The main, entry class. Manages the game.
*/
class WarSettlers : public Framework::Game, public Framework::InputListener
{
public:
	WarSettlers(HINSTANCE instance, const Framework::Game::Description& description, const Framework::DisplayCapabilities& displayCapabilities);
	~WarSettlers() throw();

	/**
		Interface: InputListener
		Handle events on key press.
	*/
	void KeyPressed(int keyCode);
protected:
	/**
		Update the game logic one tick.
	*/
	void Update(double dt);

	/**
		Render everything but the entities in the scene manager (i.e.
		HUD, debug output, buffers, etc.)
	*/
	void Render(double dt, float interpolation);
private:
	Framework::DisplayCapabilities m_displayCapabilities;
	unsigned int m_currentDisplayMode;
};

#endif