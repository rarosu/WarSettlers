#ifndef FRAMEWORK_GAME_HPP
#define FRAMEWORK_GAME_HPP

#include "Window.hpp"
#include "D3DWrapper.hpp"
#include <Utility\Logger.hpp>

namespace Framework
{
	/**
		Abstract base class for a game. Manages the game loop,
		the creation of a window, D3D11 wrapper and the camera
		and the scene manager.
	*/
	class Game
	{
	public:
		/**
			Describes the attributes needed to create a game
		*/
		struct Description
		{
			Window::Description m_windowDescription;
			D3DWrapper::Description m_d3dDescription;
		};


		/**
			Constructor
			Initialize the components, save the data, but do not
			start the game.
		*/
		Game(HINSTANCE m_instance, const Description& description);


		/**
			Starts running the game loop, essentially starting the game.
			Returns the application's return value.
		*/
		int Start();
	protected:
		/**
			Update the game logic one tick. Implemented by the actual game.
		*/
		virtual void Update(float dt) = 0;

		/**
			Render a scene. The entities of the scene have already been
			rendered when this is called. Should be used to render everything
			else.
		*/
		virtual void Render(float dt, float interpolation) = 0;

		/**
			Get a reference to the window.
		*/
		Window& GetWindow();

		/**
			Get a reference to the D3D wrapper.
		*/
		D3DWrapper& GetD3D();
	private:
		bool m_running;

		Window m_window;
		D3DWrapper m_d3d;


		/**
			Render a scene. Will render the entities in the scene manager,
			and then give control to the derived class by calling Render().
		*/
		void RenderWrapper(float dt, float interpolation);
	};
}

#endif