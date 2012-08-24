#ifndef FRAMEWORK_ASSETIMPORTER_HPP
#define FRAMEWORK_ASSETIMPORTER_HPP


#include "Libs/AssImp/Importer.hpp"      
#include "Libs/AssImp/scene.h"      
#include "Libs/AssImp/postprocess.h"
#include "Libs/r2tk/r2-exception.hpp"

namespace Framework
{
	class AssetImporter
	{
	public:
		AssetImporter();
		~AssetImporter();

		void ImportAsset(std::string fileName, std::string assetName); 
	};
}
#endif 