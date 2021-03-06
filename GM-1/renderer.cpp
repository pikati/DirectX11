#include "main.h"
#include "renderer.h"
#include <io.h>
#include "ImguiManager.h"
#include <d3dcompiler.h>


D3D_FEATURE_LEVEL       CRenderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device* CRenderer::m_D3DDevice = NULL;
ID3D11DeviceContext* CRenderer::m_ImmediateContext = NULL;
IDXGISwapChain* CRenderer::m_SwapChain = NULL;
ID3D11RenderTargetView* CRenderer::m_RenderTargetView = NULL;
ID3D11DepthStencilView* CRenderer::m_DepthStencilView = NULL;



std::vector<ID3D11VertexShader*>     CRenderer::m_VertexShader;
std::vector<ID3D11PixelShader*>     CRenderer::m_PixelShader;
std::vector<ID3D11InputLayout*>     CRenderer::m_VertexLayout;
ID3D11Buffer* CRenderer::m_WorldBuffer = NULL;
ID3D11Buffer* CRenderer::m_ViewBuffer = NULL;
ID3D11Buffer* CRenderer::m_ProjectionBuffer = NULL;
ID3D11Buffer* CRenderer::m_MaterialBuffer = NULL;
ID3D11Buffer* CRenderer::m_LightBuffer = NULL;
ID3D11Buffer* CRenderer::m_CameraBuffer = NULL;
ID3D11Buffer* CRenderer::m_paramBuffer = NULL;


ID3D11DepthStencilState* CRenderer::m_DepthStateEnable = NULL;
ID3D11DepthStencilState* CRenderer::m_DepthStateDisable = NULL;





void CRenderer::Init()
{
	HRESULT hr = S_OK;

	// デバイス、スワップチェーン、コンテキスト生成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = GetWindow();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_SwapChain,
		&m_D3DDevice,
		&m_FeatureLevel,
		&m_ImmediateContext);


	// レンダーターゲットビュー生成、設定
	ID3D11Texture2D* pBackBuffer = NULL;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	pBackBuffer->Release();



	//深度ステンシル用テクスチャー作成
	ID3D11Texture2D* depthTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = sd.BufferDesc.Width;
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	m_D3DDevice->CreateTexture2D(&td, NULL, &depthTexture);

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	m_D3DDevice->CreateDepthStencilView(depthTexture, &dsvd, &m_DepthStencilView);


	m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);


	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs;
	m_D3DDevice->CreateRasterizerState(&rd, &rs);

	m_ImmediateContext->RSSetState(rs);




	// ブレンドステート設定 加算合成や減算合成をするときはここをいじる
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);



	// 深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

	m_ImmediateContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);




	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	m_ImmediateContext->PSSetSamplers(0, 1, &samplerState);


	CreateDefaultShader();
	CreateColorShader();
	CreateTextureShader();
	CreateMorphingShader();

	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_WorldBuffer);
	m_ImmediateContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_ViewBuffer);
	m_ImmediateContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_ProjectionBuffer);
	m_ImmediateContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);


	hBufferDesc.ByteWidth = sizeof(MATERIAL);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_MaterialBuffer);
	m_ImmediateContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);
	m_ImmediateContext->PSSetConstantBuffers(3, 1, &m_MaterialBuffer);


	hBufferDesc.ByteWidth = sizeof(LIGHT);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_LightBuffer);
	m_ImmediateContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);

	hBufferDesc.ByteWidth = sizeof(D3DXVECTOR4);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_CameraBuffer);
	m_ImmediateContext->PSSetConstantBuffers(5, 1, &m_CameraBuffer);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_paramBuffer);
	m_ImmediateContext->PSSetConstantBuffers(6, 1, &m_paramBuffer);

	// 入力レイアウト設定
	m_ImmediateContext->IASetInputLayout(m_VertexLayout[(int)SHADER_TYPE::Default]);

	// シェーダ設定
	m_ImmediateContext->VSSetShader(m_VertexShader[(int)SHADER_TYPE::Default], NULL, 0);
	m_ImmediateContext->PSSetShader(m_PixelShader[(int)SHADER_TYPE::Default], NULL, 0);



	// ライト無効化
	LIGHT light;
	light.Enable = false;
	SetLight(light);



	// マテリアル初期化
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	ImguiManager::Initialize(GetWindow(), m_D3DDevice, m_ImmediateContext);

}


void CRenderer::CreateDefaultShader()
{

	ID3D11VertexShader* vs;
	FILE* file;
	long int fsize;

	file = fopen("Asset/Shader/vertexShader.cso", "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_D3DDevice->CreateVertexShader(buffer, fsize, NULL, &vs);
	m_VertexShader.push_back(vs);

	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	ID3D11InputLayout* l;
	m_D3DDevice->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		&l);

	m_VertexLayout.push_back(l);

	delete[] buffer;

	FILE* file2;
	long int fsize2;

	file2 = fopen("Asset/Shader/pixelShader.cso", "rb");
	fsize2 = _filelength(_fileno(file2));
	unsigned char* buffer2 = new unsigned char[fsize2];
	fread(buffer2, fsize2, 1, file2);
	fclose(file2);
	ID3D11PixelShader* ps;
	m_D3DDevice->CreatePixelShader(buffer2, fsize2, NULL, &ps);

	m_PixelShader.push_back(ps);

	delete[] buffer2;
}

void CRenderer::CreateColorShader()
{
	ID3D11VertexShader* vs;
	FILE* file;
	long int fsize;

	file = fopen("Asset/Shader/ColorVS.cso", "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_D3DDevice->CreateVertexShader(buffer, fsize, NULL, &vs);
	m_VertexShader.push_back(vs);

	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	ID3D11InputLayout* l;
	m_D3DDevice->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		&l);

	m_VertexLayout.push_back(l);

	delete[] buffer;

	FILE* file2;
	long int fsize2;

	file2 = fopen("Asset/Shader/ColorPS.cso", "rb");
	fsize2 = _filelength(_fileno(file2));
	unsigned char* buffer2 = new unsigned char[fsize2];
	fread(buffer2, fsize2, 1, file2);
	fclose(file2);
	ID3D11PixelShader* ps;
	m_D3DDevice->CreatePixelShader(buffer2, fsize2, NULL, &ps);

	m_PixelShader.push_back(ps);

	delete[] buffer2;
}

void CRenderer::CreateTextureShader()
{

	ID3D11VertexShader* vs;
	FILE* file;
	long int fsize;

	file = fopen("Asset/Shader/unlitTextureVS.cso", "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_D3DDevice->CreateVertexShader(buffer, fsize, NULL, &vs);
	m_VertexShader.push_back(vs);

	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	ID3D11InputLayout* l;
	m_D3DDevice->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		&l);

	m_VertexLayout.push_back(l);

	delete[] buffer;

	FILE* file2;
	long int fsize2;

	file2 = fopen("Asset/Shader/UnlitTexturePS.cso", "rb");
	fsize2 = _filelength(_fileno(file2));
	unsigned char* buffer2 = new unsigned char[fsize2];
	fread(buffer2, fsize2, 1, file2);
	fclose(file2);
	ID3D11PixelShader* ps;
	m_D3DDevice->CreatePixelShader(buffer2, fsize2, NULL, &ps);

	m_PixelShader.push_back(ps);

	delete[] buffer2;
}

void CRenderer::CreateMorphingShader()
{

	ID3D11VertexShader* vs;
	FILE* file;
	long int fsize;

	file = fopen("Asset/Shader/vertexShader.cso", "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_D3DDevice->CreateVertexShader(buffer, fsize, NULL, &vs);
	m_VertexShader.push_back(vs);

	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	ID3D11InputLayout* l;
	m_D3DDevice->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		&l);

	m_VertexLayout.push_back(l);

	delete[] buffer;

	FILE* file2;
	long int fsize2;

	file2 = fopen("Asset/Shader/MorphingPS.cso", "rb");
	fsize2 = _filelength(_fileno(file2));
	unsigned char* buffer2 = new unsigned char[fsize2];
	fread(buffer2, fsize2, 1, file2);
	fclose(file2);
	ID3D11PixelShader* ps;
	m_D3DDevice->CreatePixelShader(buffer2, fsize2, NULL, &ps);

	m_PixelShader.push_back(ps);

	delete[] buffer2;
}

void CRenderer::Uninit()
{
	// オブジェクト解放
	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();

	for (int i = m_VertexLayout.size() - 1; i >= 0; i--)
	{
		m_VertexLayout[i]->Release();
	}
	m_VertexLayout.clear();

	for (int i = m_VertexShader.size() - 1; i >= 0; i--)
	{
		m_VertexShader[i]->Release();
	}
	m_VertexShader.clear();

	for (int i = m_PixelShader.size() - 1; i >= 0; i--)
	{
		m_PixelShader[i]->Release();
	}
	m_PixelShader.clear();

	m_ImmediateContext->ClearState();
	m_RenderTargetView->Release();
	m_SwapChain->Release();
	m_ImmediateContext->Release();
	m_D3DDevice->Release();

}



void CRenderer::Begin()
{
	// バックバッファクリア
	float ClearColor[4] = { 0.0f, 0.5f, 0.0f, 1.0f };
	m_ImmediateContext->ClearRenderTargetView(m_RenderTargetView, ClearColor);
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void CRenderer::End()
{
	m_SwapChain->Present(1, 0);
}




void CRenderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		m_ImmediateContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
	else
		m_ImmediateContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);

}

void CRenderer::SetWorldViewProjection2D()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	m_ImmediateContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	m_ImmediateContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	m_ImmediateContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);

}


void CRenderer::SetWorldMatrix(D3DXMATRIX* WorldMatrix)
{
	D3DXMATRIX world;
	D3DXMatrixTranspose(&world, WorldMatrix);
	m_ImmediateContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

void CRenderer::SetViewMatrix(D3DXMATRIX* ViewMatrix)
{
	D3DXMATRIX view;
	D3DXMatrixTranspose(&view, ViewMatrix);
	m_ImmediateContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

void CRenderer::SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix)
{
	D3DXMATRIX projection;
	D3DXMatrixTranspose(&projection, ProjectionMatrix);
	m_ImmediateContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}



void CRenderer::SetMaterial(MATERIAL Material)
{

	m_ImmediateContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);

}

void CRenderer::SetLight(LIGHT Light)
{

	m_ImmediateContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);

}

void CRenderer::SetCameraPosition(Vector3 CameraPosition)
{
	m_ImmediateContext->UpdateSubresource(m_CameraBuffer, 0, NULL, &D3DXVECTOR4(CameraPosition.x, CameraPosition.y, CameraPosition.z, 1.0f), 0, 0);
}

void CRenderer::SetParameter(Vector4 v)
{
	m_ImmediateContext->UpdateSubresource(m_paramBuffer, 0, NULL, &v, 0, 0);
}

void CRenderer::SetViewPort(D3D11_VIEWPORT* viewPort)
{
	m_ImmediateContext->RSSetViewports(1, viewPort);
}

void CRenderer::SetShader(SHADER_TYPE type)
{
	m_ImmediateContext->IASetInputLayout(m_VertexLayout[(int)type]);
	m_ImmediateContext->VSSetShader(m_VertexShader[(int)type], NULL, 0);
	m_ImmediateContext->PSSetShader(m_PixelShader[(int)type], NULL, 0);
}

void CRenderer::SetVertexShader(SHADER_TYPE type)
{
	m_ImmediateContext->VSSetShader(m_VertexShader[(int)type], NULL, 0);
}

void CRenderer::SetPixelShader(SHADER_TYPE type)
{
	m_ImmediateContext->PSSetShader(m_PixelShader[(int)type], NULL, 0);
}

void CRenderer::SetInputLayout(SHADER_TYPE type)
{
	m_ImmediateContext->IASetInputLayout(m_VertexLayout[(int)type]);
}