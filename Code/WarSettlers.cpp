#include "WarSettlers.hpp"

#include "Framework\FPSCameraController.hpp"
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
	, m_camera(D3DXVECTOR3(0, 5, -10), D3DXVECTOR3(0, 0, 10), 0.1, 500.0, 45, (double)displayCapabilities.m_displayModes[0].m_width / (double)displayCapabilities.m_displayModes[0].m_height)
	, m_fpsCameraController(&m_camera, displayCapabilities.m_displayModes[0].m_width, displayCapabilities.m_displayModes[0].m_height)
	, m_assetImporter()	
{
    // Initialize Model
    m_modelMap.AddEntity(Model::Entity(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
    m_modelMap.AddEntity(Model::Entity(D3DXVECTOR3(30.0f, 0.0f, 4.0f)));
    m_modelMap.AddEntity(Model::Entity(D3DXVECTOR3(-10.0f, 0.0f, 10.0f)));

    // Initialize View
	m_assetImporter.ImportMesh("Resources/Models/tank.dae", ASSET_RTSVEHICLES); 	
	m_viewMap = View::ViewMap(&m_modelMap, &GetD3D(), &m_assetImporter,  50, 50);


	// Do other stuff...
	SetCursorPos(displayCapabilities.m_displayModes[0].m_width/2, displayCapabilities.m_displayModes[0].m_height / 2); 

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


void WarSettlers::Update(double dt)
{
	// TODO: Add game logic
	m_fpsCameraController.Update(dt); 
	m_camera.Commit(); 
}

void WarSettlers::Render(double dt, double interpolation)
{
	// TODO: Render HUD, debug output, etc.
	
	D3DXMATRIX vp;	
	vp = this->m_fpsCameraController.GetViewProjection(); 
	// TODO can we move some of these params to the object? 
	m_viewMap.Render(vp, &m_effect, &m_inputLayout, m_variableWVP, m_techniqueDescription); 
}



/**
	Testing
*/
void WarSettlers::SetupBuffers()
{		
	std::vector<Framework::WSMesh>::iterator it = m_assetImporter.GetMeshes(ASSET_RTSVEHICLES)->begin()+1; 
	m_vertexCount = it->positions.size(); 
	int nIndices = it->indices.size(); 
	

	std::vector<Framework::Vertex> vertices(m_vertexCount);	
	for(int i=0; i<m_vertexCount; i++) 
	{
		float r = (float)rand();
		r = r / (r + (float)rand()); 
		vertices[i].m_position = it->positions[i]; 
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
	
	HRESULT result = GetD3D().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer.Resource());
	if (FAILED(result))
		throw DirectXErrorM(result, "Failed to create vertex buffer");

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
			errorMessage = static_cast<const char*>(errors->GetBufferPointer());
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
	
	result = GetD3D().GetDevice()->CreateInputLayout(&inputLayout[0], (UINT)inputLayout.size(), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_inputLayout.Resource());
	if (FAILED(result))
		throw DirectXErrorM(result, "Failed to create input layout");

	// Store the technique description
	m_effect->GetTechniqueByIndex(0)->GetDesc(&m_techniqueDescription);

	// Store variable references
	m_variableWVP = m_effect->GetVariableByName("gWVP")->AsMatrix();
}
