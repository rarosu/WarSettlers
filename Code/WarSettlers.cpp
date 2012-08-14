#include "WarSettlers.hpp"

#include <Framework\DisplayCapabilities.hpp>

int CreateAndStartGame(HINSTANCE instance, int showState)
{
	Framework::DisplayCapabilities displayCapabilities(true);
	Framework::Game::Description gameDescription;

	gameDescription.m_windowDescription.m_caption = "War Settlers";
	gameDescription.m_windowDescription.m_clientWidth = displayCapabilities.m_displayModes[0].m_width;
	gameDescription.m_windowDescription.m_clientHeight = displayCapabilities.m_displayModes[0].m_height;
	gameDescription.m_windowDescription.m_hasFrame = true;
	gameDescription.m_windowDescription.m_resizable = true;
	gameDescription.m_windowDescription.m_showState = showState;
	gameDescription.m_windowDescription.m_x = -1;
	gameDescription.m_windowDescription.m_y = -1;

	gameDescription.m_d3dDescription.m_displayMode = displayCapabilities.m_displayModes[0];
	gameDescription.m_d3dDescription.m_fullscreen = false;
	gameDescription.m_d3dDescription.m_viewports.push_back(Framework::D3DWrapper::Viewport());

	WarSettlers game(instance, gameDescription, displayCapabilities);
	return game.Start();
}


WarSettlers::WarSettlers(HINSTANCE instance, const Framework::Game::Description& description, const Framework::DisplayCapabilities& displayCapabilities)
	: Framework::Game(instance, description)
	, m_displayCapabilities(displayCapabilities)
	, m_currentDisplayMode(0)
	, m_vertexCount(3)
	, m_indexCount(3)
{
	Framework::InputManager::Instance().AddInputListener(this);

	SetupBuffers();
	SetupEffect();
}

WarSettlers::~WarSettlers() throw()
{
	Framework::InputManager::Instance().RemoveInputListener(this);
}


void WarSettlers::KeyPressed(int keyCode)
{
	switch (keyCode)
	{
		case 'R':
			++m_currentDisplayMode;
			m_currentDisplayMode %= m_displayCapabilities.m_displayModes.size();

			GetD3D().ChangeResolution(m_displayCapabilities.m_displayModes[m_currentDisplayMode]);
		break;

		case 'F':
			GetD3D().ToggleFullscreen();
		break;
	}
}


void WarSettlers::Update(float dt)
{
	// TODO: Add game logic
}

void WarSettlers::Render(float dt, float interpolation)
{
	// TODO: Render HUD, debug output, etc.

	// TODO: Remove ugly test code
	for (unsigned int p = 0; p < m_techniqueDescription.Passes; ++p)
	{
		m_effect->GetTechniqueByIndex(0)->GetPassByIndex(p)->Apply(0, GetD3D().GetContext().Resource());

		GetD3D().GetContext()->Draw(m_vertexCount, 0);
	}
}



/**
	Testing
*/
void WarSettlers::SetupBuffers()
{
	std::vector<WarSettlers::Vertex> vertices(m_vertexCount);
	std::vector<unsigned int> indices(m_indexCount);

	// Setup a list of all vertices in a triangle
	vertices[0].m_position = D3DXVECTOR4(-1.0f, 0.0f, 0.0f, 1.0f);
	vertices[0].m_color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	
	vertices[1].m_position = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].m_color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	
	vertices[2].m_position = D3DXVECTOR4(0.0f, 2.0f, 0.0f, 1.0f);
	vertices[2].m_color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	// Setup a list of the order the triangles should come in (not necessary in this case, but
	// useful for not having to repeat vertices in a model).
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;


	// Describe the vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(WarSettlers::Vertex) * vertices.size();
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	HRESULT result = GetD3D().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer.Resource());
	if (FAILED(result))
		throw DirectXErrorM(result, "Failed to create vertex buffer");


	// Describe the index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = GetD3D().GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer.Resource());
	if (FAILED(result))
		throw DirectXErrorM(result, "Failed to create index buffer");
}

void WarSettlers::SetupEffect()
{
	// Compile the effect
	ID3D10Blob* shader;
	ID3D10Blob* errors;

	HRESULT result = D3DX11CompileFromFile("Resources/Effects/BasicColor.fx",
		NULL,
		NULL,
		NULL,
		"fx_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG,
		0,
		NULL,
		&shader,
		&errors,
		NULL);

	if (FAILED(result))
	{
		std::string errorMessage;
		if (errors != NULL)
		{
			errorMessage = (const char*)errors;
		}
		else
		{
			errorMessage = "Could not find effect file";
		}

		throw DirectXErrorM(result, errorMessage);
	}

	result = D3DX11CreateEffectFromMemory(shader->GetBufferPointer(), shader->GetBufferSize(), 0, GetD3D().GetDevice().Resource(), &m_effect.Resource());
	if (FAILED(result))
	{
		throw DirectXErrorM(result, "Failed to create effect from compiled blob.");
	}


	// Setup the input layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout(2);
	inputLayout[0].SemanticName = "POSITION";
	inputLayout[0].SemanticIndex = 0;
	inputLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout[0].InputSlot = 0;
	inputLayout[0].AlignedByteOffset = 0;
	inputLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[0].InstanceDataStepRate = 0;

	inputLayout[1].SemanticName = "COLOR";
	inputLayout[1].SemanticIndex = 0;
	inputLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout[1].InputSlot = 0;
	inputLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[1].InstanceDataStepRate = 0;

	D3DX11_PASS_DESC passDesc;
	m_effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetDesc(&passDesc);
	
	result = GetD3D().GetDevice()->CreateInputLayout(&inputLayout[0], inputLayout.size(), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_inputLayout.Resource());
	if (FAILED(result))
		throw DirectXErrorM(result, "Failed to create input layout");

	// Store the technique description
	m_effect->GetTechniqueByIndex(0)->GetDesc(&m_techniqueDescription);
}
