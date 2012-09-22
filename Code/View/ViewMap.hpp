#ifndef VIEW_MAP_HPP
#define VIEW_MAP_HPP

#include "ViewEntity.hpp"
#include "Framework\Global.hpp"
#include "Framework\D3DWrapper.hpp" 
#include <vector>

namespace View
{
	class ViewMap
	{
	public:
		ViewMap();
		ViewMap(Framework::D3DWrapper* wrapper,  int width, int height);
		~ViewMap();

		Framework::COMResource<ID3D11Buffer>& GetIndexBuffer() { return m_indexBuffer; }
		Framework::COMResource<ID3D11Buffer>& GetVertexBuffer() { return m_vertexBuffer; }
		int GetVertexCount() { return m_mapMesh.positions.size(); }
	private: 
		void GenerateMap(); 
		void CreateBuffers(); 

		float m_squareSize; 

		// width and height in number of squares. 
		int m_width; 
		int m_height; 

		Framework::WSMesh m_mapMesh; 
		Framework::D3DWrapper* m_D3dwrapper; 

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