#include "AssetImporter.h"

namespace Framework 
{
	AssetImporter::AssetImporter()
	{

	}


	AssetImporter::~AssetImporter()
	{

	}

	void ImportAsset(std::string fileName, std::string assetName)
	{
		 Assimp::Importer importer;
		 const aiScene* scene = importer.ReadFile(fileName, 0); 
		 
		 if(!scene) 
		 {
			 std::string exceptionMessage = "Unable to import asset (ImportAsset) "; 
			 exceptionMessage += importer.GetErrorString(); 
			 r2ExceptionRuntimeM(exceptionMessage);  
		 }
	}
}