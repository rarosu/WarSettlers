#ifndef VIEW_ENTITY_HPP
#define VIEW_ENTITY_HPP

#include "Framework\AssetImporter.hpp"

namespace View
{

	/* 
	This class represents a renderable entity of the game. 	
	*/ 
	class ViewEntity
	{
	public:
		ViewEntity();
		ViewEntity(std::string meshName); 
		~ViewEntity();
	private: 
		Framework::AssetImporter m_assetImporter; 
		Framework::WSMesh* m_mesh; 
		D3DXMATRIX m_worldMatrix; 

		// TODO add material 
		// TODO add vertex buffer? 

	};
}
#endif