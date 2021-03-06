#include "AssetImporter.hpp"

namespace Framework 
{
	

	WSMesh::WSMesh()
	{

	}

	AssetImporter::AssetImporter()
	{

	}

	AssetImporter::AssetImporter(const Framework::AssetImporter &)
	{

	}


	AssetImporter::~AssetImporter()
	{

	}

	bool AssetImporter::ProcessMeshes(const aiScene* scene, std::string assetName) 
	{				
		if(!scene->HasMeshes())
			return false; 

		std::vector<WSMesh> vecMeshes; 

		int nMeshes = scene->mNumMeshes; 
		aiMesh** meshes = scene->mMeshes; 
		for(int i=0; i<nMeshes; i++) 
		{			
			WSMesh wsMesh; 
			aiMesh* mesh = meshes[i];			

			if(mesh->HasPositions())
			{
				for(unsigned int i=0; i<mesh->mNumVertices; i++) 
				{
					aiVector3D vertice = mesh->mVertices[i]; 
					D3DXVECTOR4 v = D3DXVECTOR4(vertice.x, vertice.y, vertice.z, 1.0); 
					wsMesh.positions.push_back(v); 					
				}
			}

			if(mesh->HasNormals())
			{
				for(unsigned int i=0; i<mesh->mNumVertices; i++)
				{
					aiVector3D normal = mesh->mNormals[i]; 
					D3DXVECTOR3 v = D3DXVECTOR3(normal.x, normal.y, normal.z); 
					wsMesh.normals.push_back(v); 
				}
			}

			if(mesh->HasTextureCoords(0))
			{
				for(unsigned int i=0; i<mesh->mNumVertices; i++)
				{
					aiVector3D t = mesh->mTextureCoords[0][i]; 
					D3DXVECTOR2 v = D3DXVECTOR2(t.x, t.y); 
					wsMesh.texcoords.push_back(v); 
				}
			}

			if(mesh->HasFaces()) 
			{
				for(unsigned int i=0; i<mesh->mNumFaces; i++) 
				{
					aiFace face = mesh->mFaces[i]; 
					for(unsigned int f=0; f<face.mNumIndices; f++) 
					{
						int index = face.mIndices[f]; 
						wsMesh.indices.push_back(index); 
					}
				}
			}

			vecMeshes.push_back(wsMesh); 
		}

		m_mMeshes.insert(std::pair<std::string, std::vector<WSMesh>>(assetName, vecMeshes)); 

		return true; 
	}

	bool AssetImporter::ImportMesh(std::string fileName, std::string meshName)
	{
		 Assimp::Importer importer;
		 const aiScene* scene = importer.ReadFile(fileName, aiProcess_MakeLeftHanded); 
		 
		 if(!scene) 
		 {
			 std::string exceptionMessage = "Unable to import asset (ImportAsset) "; 
			 exceptionMessage += importer.GetErrorString(); 
			 r2ExceptionRuntimeM(exceptionMessage);  
			 return false; 
		 }
		 else 
		 {
			 return ProcessMeshes(scene, meshName); 										 
		 }
	}

	std::vector<WSMesh>* AssetImporter::GetMeshes(const std::string assetName) 
	{
		std::map<std::string, std::vector<WSMesh>>::iterator it = m_mMeshes.find(assetName); 

		return &it->second; 
	}

	
}