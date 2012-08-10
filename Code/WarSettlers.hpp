#ifndef WAR_SETTLERS_HPP
#define WAR_SETTLERS_HPP

#include <Framework\Game.hpp>

/**
	The main, entry class. Manages the game.
*/
class WarSettlers : public Framework::Game
{
public:
	WarSettlers(HINSTANCE instance, int showState);
protected:
	/**
		Update the game logic one tick.
	*/
	void Update(float dt);

	/**
		Render everything but the entities in the scene manager (i.e.
		HUD, debug output, buffers, etc.)
	*/
	void Render(float dt, float interpolation);
private:
	/**
		Will read configuration and create a description
		to create the game from.
	*/
	class DescriptionConstructor
	{
	public:
		DescriptionConstructor(int showState);

		Framework::Game::Description m_description;
	};
};

#endif