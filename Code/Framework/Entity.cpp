#include "Entity.hpp"

namespace Framework 
{
	Entity::Entity()		
		: m_assetImporter()
	{
		std::vector<WSMesh> vMesh = m_assetImporter.Instance().GetMeshes(ASSET_RTSVEHICLES); 
		if(vMesh.size() > 0) 
			m_mesh = vMesh[0]; 
	}

	Entity::Entity(std::string meshName)		
		: m_assetImporter()
	{
		std::vector<WSMesh> vMesh = m_assetImporter.Instance().GetMeshes(meshName); 
		if(vMesh.size() > 0) 
			m_mesh = vMesh[0]; 
	}

	Entity::~Entity()
	{
	}
}