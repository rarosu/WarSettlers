#include "ViewMap.hpp"

namespace View 
{
	ViewMap::ViewMap(Framework::D3DWrapper* wrapper, Framework::AssetImporter* assetImporter, int width, int height)
	{
		m_D3dwrapper = wrapper; 
		m_assetImporter = assetImporter; 
		m_squareSize = 1.0;
		m_width = width;
		m_height = height;

		GenerateMap(); 
		CreateBuffers(); 
		AddEntity(D3DXVECTOR3(0, 0, 0), ASSET_RTSVEHICLES); 
		AddEntity(D3DXVECTOR3(20, 0, 10), ASSET_RTSVEHICLES); 
		AddEntity(D3DXVECTOR3(50, 0, 50), ASSET_RTSVEHICLES); 
	}

	ViewMap::ViewMap()
	{
		m_squareSize = 1.0;
		m_width = 10;
		m_height = 10;

		GenerateMap(); 
	}


	ViewMap::~ViewMap()
	{
	}

	void ViewMap::AddEntity(D3DXVECTOR3 position, std::string meshName) 
	{
		if(!m_D3dwrapper) 
			return; 

		m_entities.push_back(ViewEntity(m_D3dwrapper, m_assetImporter, meshName, position)); 
	}

	void ViewMap::Render(D3DXMATRIX vp, Framework::COMResource<ID3DX11Effect> *effect, Framework::COMResource<ID3D11InputLayout>* inputLayout, ID3DX11EffectMatrixVariable* variableWVP, D3DX11_TECHNIQUE_DESC techniqueDescription) 
	{
		//TODO Culling here?

		unsigned int offset = 0;
		unsigned int stride = sizeof(Framework::Vertex);
		m_D3dwrapper->GetContext()->IASetInputLayout(inputLayout->Resource());
		m_D3dwrapper->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_D3dwrapper->GetContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer.Resource(), &stride, &offset);
		
		variableWVP->SetMatrix((FLOAT*)vp);

		for (unsigned int p = 0; p < techniqueDescription.Passes; ++p)
		{
			effect->Resource()->GetTechniqueByIndex(0)->GetPassByIndex(p)->Apply(0, m_D3dwrapper->GetContext().Resource());

			m_D3dwrapper->GetContext()->Draw(GetVertexCount(), 0);
		}
		 

		for(std::vector<ViewEntity>::iterator it = m_entities.begin(); it != m_entities.end(); it++) 
		{
			it->Render(vp, effect, inputLayout, variableWVP, techniqueDescription); 
		}
	}

	void ViewMap::GenerateMap()
	{
		int counter = 0; 
		for(int x=0; x<m_width; x++) 
		{					
			for(int y=0; y<m_height; y++) 
			{
				D3DXVECTOR4 pos1 = D3DXVECTOR4(x * m_squareSize, 0, y * m_squareSize, 1); 
				D3DXVECTOR4 pos2 = D3DXVECTOR4(x * m_squareSize + m_squareSize, 0, y * m_squareSize, 1); 
				D3DXVECTOR4 pos3 = D3DXVECTOR4(x * m_squareSize + m_squareSize, 0, y * m_squareSize + m_squareSize, 1); 

				D3DXVECTOR4 pos4 = D3DXVECTOR4(x * m_squareSize, 0, y * m_squareSize, 1); 
				D3DXVECTOR4 pos5 = D3DXVECTOR4(x * m_squareSize, 0, y * m_squareSize + m_squareSize, 1); 
				D3DXVECTOR4 pos6 = D3DXVECTOR4(x * m_squareSize + m_squareSize, 0, y * m_squareSize + m_squareSize, 1); 				

				m_mapMesh.positions.push_back(pos1); 
				m_mapMesh.indices.push_back(counter++); 
				m_mapMesh.positions.push_back(pos2); 
				m_mapMesh.indices.push_back(counter++); 
				m_mapMesh.positions.push_back(pos3); 
				m_mapMesh.indices.push_back(counter++); 
				m_mapMesh.positions.push_back(pos4); 
				m_mapMesh.indices.push_back(counter++); 								
				m_mapMesh.positions.push_back(pos5); 
				m_mapMesh.indices.push_back(counter++); 								
				m_mapMesh.positions.push_back(pos6); 
				m_mapMesh.indices.push_back(counter++); 								
			}
		}	 
	}

	void ViewMap::CreateBuffers() 
	{
		if(m_mapMesh.indices.size() <= 0 
			|| m_mapMesh.positions.size() <= 0
			|| !m_D3dwrapper) 
			return; 

			
		std::vector<Framework::Vertex> vertices(m_mapMesh.positions.size());	
		for(int i=0; i<m_mapMesh.positions.size(); i++) 
		{
			float r = (float)rand();
			r = r / (r + (float)rand()); 
			vertices[i].m_position = m_mapMesh.positions[i]; 
			vertices[i].m_color = D3DXCOLOR(r, r, r, 1.0f);
		}
	

		// Describe the vertex buffer
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(Framework::Vertex) * vertices.size());
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = &vertices[0];
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
	
		HRESULT result = m_D3dwrapper->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer.Resource());
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to create vertex buffer");


		// Describe the index buffer
		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * m_mapMesh.indices.size());
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = &m_mapMesh.indices[0];
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		result = m_D3dwrapper->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer.Resource());
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to create index buffer");
	}
}