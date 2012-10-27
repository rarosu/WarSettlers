#include "ViewEntity.hpp"

namespace View 
{
	ViewEntity::ViewEntity()				
	{
		
	}

	ViewEntity::ViewEntity(Framework::D3DWrapper* wrapper, Framework::AssetImporter* assetImporter, std::string meshName, D3DXVECTOR3 pos)				
	{
		m_D3dwrapper = wrapper;
		m_assetImporter = assetImporter; 
		std::vector<Framework::WSMesh> *vMesh = m_assetImporter->GetMeshes(meshName); 
		if(vMesh->size() > 0) 
			m_mesh = &vMesh->at(0); 
		
		D3DXMatrixTranslation(&this->m_worldMatrix, pos.x, pos.y, pos.z); 

		CreateBuffers(); 
	}	


	ViewEntity::~ViewEntity()
	{
	}


	void ViewEntity::Render(D3DXMATRIX vp, Framework::COMResource<ID3DX11Effect> *effect, Framework::COMResource<ID3D11InputLayout>* inputLayout, ID3DX11EffectMatrixVariable* variableWVP, D3DX11_TECHNIQUE_DESC techniqueDescription) 
	{
		unsigned int offset = 0;
		unsigned int stride = sizeof(Framework::Vertex);
		m_D3dwrapper->GetContext()->IASetInputLayout(inputLayout->Resource());
		m_D3dwrapper->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_D3dwrapper->GetContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer.Resource(), &stride, &offset);		
		
		variableWVP->SetMatrix((FLOAT*)(m_worldMatrix * vp));

		for (unsigned int p = 0; p < techniqueDescription.Passes; ++p)
		{
			effect->Resource()->GetTechniqueByIndex(0)->GetPassByIndex(p)->Apply(0, m_D3dwrapper->GetContext().Resource());

			m_D3dwrapper->GetContext()->Draw(m_mesh->positions.size(), 0);
		}
	}

	
	void ViewEntity::CreateBuffers() 
	{
		// TODO: Add exception handling! 
		if(!m_mesh) 
			return; 

		// TODO: Add exception handling! 
		if(m_mesh->indices.size() <= 0 
			|| m_mesh->positions.size() <= 0
			|| !m_D3dwrapper) 
			return; 

			
		std::vector<Framework::Vertex> vertices(m_mesh->positions.size());	
		for(int i=0; i<m_mesh->positions.size(); i++) 
		{
			float r = (float)rand();
			r = r / (r + (float)rand()); 
			vertices[i].m_position = m_mesh->positions[i]; 
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
		indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * m_mesh->indices.size());
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = &m_mesh->indices[0];
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		result = m_D3dwrapper->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer.Resource());
		if (FAILED(result))
			throw DirectXErrorM(result, "Failed to create index buffer");
	}
}