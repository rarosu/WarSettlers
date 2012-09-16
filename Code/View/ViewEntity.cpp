#include "ViewEntity.hpp"

namespace View 
{
	ViewEntity::ViewEntity()		
		: m_assetImporter()
	{
		
	}

	ViewEntity::ViewEntity(std::string meshName)		
		: m_assetImporter()
	{
		std::vector<Framework::WSMesh> vMesh = m_assetImporter.Instance().GetMeshes(meshName); 
		if(vMesh.size() > 0) 
			m_mesh = &vMesh[0]; 
	}

	ViewEntity::~ViewEntity()
	{
	}
}