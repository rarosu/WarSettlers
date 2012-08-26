#ifndef FRAMEWORK_ASSETIMPORTER_HPP
#define FRAMEWORK_ASSETIMPORTER_HPP


#include "Libs/AssImp/Importer.hpp"      
#include "Libs/AssImp/scene.h"      
#include "Libs/AssImp/postprocess.h"
#include "Libs/r2tk/r2-exception.hpp"
#include "D3DInclude.hpp"
#include <vector>

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

	class AssetImporter 
	{
	public:
		AssetImporter();
		~AssetImporter();

		bool ImportAsset(std::string fileName, std::string assetName); 
		std::vector<WSMesh>& GetMeshes() { return m_meshes; }
	private: 
		std::vector<WSMesh> m_meshes; 

		bool ProcessMeshes(const aiScene* scene); 
	};
}
#endif 