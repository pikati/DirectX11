#pragma once
#include <vector>
#include "Engine.h"



// ���_�\����
struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};

// ���_�\����
struct VERTEX_LINE
{
	Vector3 Position;
	D3DXVECTOR4 Diffuse;
};


// �}�e���A���\����
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];//16byte���E�p
};

struct COLOR
{
	D3DXVECTOR4 color;
};
struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];//16byte���E�p
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
};


class CVertexBuffer;
class CIndexBuffer;
class CTexture;

enum class SHADER_TYPE : int
{
	Default,
	Color,
	Max
};

class CRenderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_D3DDevice;
	static ID3D11DeviceContext*    m_ImmediateContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;



	static std::vector<ID3D11VertexShader*>     m_VertexShader;
	static std::vector<ID3D11PixelShader*>      m_PixelShader;
	static std::vector<ID3D11InputLayout*>      m_VertexLayout;
	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static void CreateDefaultShader();
	static void CreateColorShader();


public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX * WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX * ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX * ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);

	static void SetShader(SHADER_TYPE type);
	static void SetVertexShader(SHADER_TYPE type);
	static void SetPixelShader(SHADER_TYPE type);
	static void SetInputLayout(SHADER_TYPE type);

	static ID3D11Device* GetDevice( void ){ return m_D3DDevice; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_ImmediateContext; }


};
