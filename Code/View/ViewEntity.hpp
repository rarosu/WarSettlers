#ifndef VIEW_ENTITY_HPP
#define VIEW_ENTITY_HPP

#include "Framework\AssetImporter.hpp"
#include "Framework\Global.hpp"
#include "Framework\D3DWrapper.hpp" 
#include <Libs\Effects11\d3dx11effect.h>

namespace View
{

	/* 
	This class represents a renderable entity of the game. 	
	*/ 
	class ViewEntity
	{
	public:
		ViewEntity();
		ViewEntity(Framework::D3DWrapper* wrapper, Framework::AssetImporter* assetImporter, std::string meshName, D3DXVECTOR3 pos);			
		~ViewEntity();

		void Render(D3DXMATRIX vp, Framework::COMResource<ID3DX11Effect> *effect, Framework::COMResource<ID3D11InputLayout>* inputLayout, ID3DX11EffectMatrixVariable* variableWVP, D3DX11_TECHNIQUE_DESC techniqueDescription); 
	private: 
		void CreateBuffers(); 

		Framework::D3DWrapper* m_D3dwrapper;
		Framework::AssetImporter* m_assetImporter;
		Framework::WSMesh* m_mesh;
		D3DXMATRIX m_worldMatrix;

		// TODO add material 

		D3D11_BUFFER_DESC m_vertexBufferDesc;
		D3D11_SUBRESOURCE_DATA m_vertexBufferData;
		D3D11_BUFFER_DESC m_indexBufferDesc;
		D3D11_SUBRESOURCE_DATA m_indexBufferData;

		Framework::COMResource<ID3D11Buffer> m_vertexBuffer;
		Framework::COMResource<ID3D11Buffer> m_indexBuffer;

	};
}
#endif