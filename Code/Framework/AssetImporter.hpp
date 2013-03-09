#ifndef FRAMEWORK_ASSETIMPORTER_HPP
#define FRAMEWORK_ASSETIMPORTER_HPP

#include "Libs/AssImp/Importer.hpp"      
#include "Libs/AssImp/scene.h"      
#include "Libs/AssImp/postprocess.h"
#include "Libs/r2tk/r2-exception.hpp"
#include "Libs/r2tk/r2-singleton.hpp"
#include "D3DInclude.hpp"
#include <vector>
#include <map>

namespace Framework
{
	struct WSMesh
	{		
		std::vector<D3DXVECTOR4> positions; 		
		std::vector<D3DXVECTOR3> normals; 
		std::vector<D3DXVECTOR2> texcoords; 
		std::vector<int> indices;  // this construction assumes that a face is equal to a triangle, i.e every face consist of exactly three indices.		

		WSMesh(); 
	}; 

	class AssetImporter : public r2::Singleton<AssetImporter>
	{
	public:
		AssetImporter();
		AssetImporter(const Framework::AssetImporter &); 
		~AssetImporter();

		bool ImportMesh(std::string fileName, std::string meshName); 		
		std::vector<WSMesh>* GetMeshes(std::string assetName); 
	protected: 		
		std::map<std::string, std::vector<WSMesh> > m_mMeshes; 
		bool ProcessMeshes(const aiScene* scene, std::string assetName); 
	};
}
#endif 