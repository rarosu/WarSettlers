#ifndef VIEW_MAP_HPP
#define VIEW_MAP_HPP

#include <Model\Map.hpp>
#include "ViewEntity.hpp"
#include "Framework\Global.hpp"
#include "Framework\D3DWrapper.hpp" 
#include "WarsettlersGlobal.hpp"
#include <Libs\Effects11\d3dx11effect.h>
#include <vector>

namespace View
{
	class ViewMap
	{
	public:
		ViewMap();
		ViewMap(Model::Map* modelMap, Framework::D3DWrapper* wrapper,  Framework::AssetImporter* assetImporter,  int width, int height);
		~ViewMap();

		Framework::COMResource<ID3D11Buffer>& GetIndexBuffer() { return m_indexBuffer; }
		Framework::COMResource<ID3D11Buffer>& GetVertexBuffer() { return m_vertexBuffer; }
		int GetVertexCount() { return m_mapMesh.positions.size(); }
		void Render(D3DXMATRIX vp, Framework::COMResource<ID3DX11Effect> *effect, Framework::COMResource<ID3D11InputLayout>* inputLayout, ID3DX11EffectMatrixVariable* variableWVP, D3DX11_TECHNIQUE_DESC techniqueDescription); 
		void AddEntity(D3DXVECTOR3 position, std::string meshName); 
	private: 
		void GenerateMap(); 
		void CreateBuffers(); 

		float m_squareSize; 

		// width and height in number of squares. 
		int m_width; 
		int m_height; 

		Framework::WSMesh m_mapMesh; 
		Framework::D3DWrapper* m_D3dwrapper; 
		Framework::AssetImporter* m_assetImporter;
		Model::Map* m_modelMap;

		// vector of ViewEntities contained in this map. All rendable content is based on the ViewEntity.  
		std::vector<ViewEntity> m_entities; 


		D3D11_BUFFER_DESC m_vertexBufferDesc; 
		D3D11_SUBRESOURCE_DATA m_vertexBufferData; 
		D3D11_BUFFER_DESC m_indexBufferDesc; 
		D3D11_SUBRESOURCE_DATA m_indexBufferData; 

		Framework::COMResource<ID3D11Buffer> m_vertexBuffer;
		Framework::COMResource<ID3D11Buffer> m_indexBuffer;
	};
}
#endif 