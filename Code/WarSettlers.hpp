#ifndef WAR_SETTLERS_HPP
#define WAR_SETTLERS_HPP

#include <Framework\Game.hpp>
#include "Framework\FPSCameraController.hpp"
#include "Framework\AssetImporter.hpp"
#include <Libs\Effects11\d3dx11effect.h>

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
	void Render(double dt, double interpolation);
private:
	Framework::DisplayCapabilities m_displayCapabilities;
	unsigned int m_currentDisplayMode;

	/**
		Testing
		Need to wrap up this process in a class in someway.
	*/
	struct Vertex
	{
		D3DXVECTOR4 m_position;
		D3DXCOLOR m_color;
	};

	void SetupBuffers();
	void SetupEffect();

	unsigned int m_vertexCount;
	unsigned int m_indexCount;
	Framework::COMResource<ID3D11Buffer> m_vertexBuffer;
	Framework::COMResource<ID3D11Buffer> m_indexBuffer;

	Framework::COMResource<ID3DX11Effect> m_effect;
	Framework::COMResource<ID3D11InputLayout> m_inputLayout;
	ID3DX11EffectMatrixVariable* m_variableWVP;
	D3DX11_TECHNIQUE_DESC m_techniqueDescription;

	Framework::FPSCameraController m_fpsCameraController; 
	Framework::Camera m_camera; 
	Framework::AssetImporter m_assetImporter; 
	

	/**
		/Testing
	*/
};

#endif