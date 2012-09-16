#ifndef FRAMEWORK_ENTITY_HPP
#define FRAMEWORK_ENTITY_HPP

#include "AssetImporter.hpp"
#include "../Global.hpp"

namespace Framework
{

	/* 
	This class represents a renderable entity of the game. 
	It contains reference to a mesh and one or more materials
	*/ 
	class Entity
	{
	public:
		Entity();
		Entity(std::string meshName); 
		~Entity();
	private: 
		AssetImporter m_assetImporter; 
		WSMesh m_mesh; 
	};
}
#endif