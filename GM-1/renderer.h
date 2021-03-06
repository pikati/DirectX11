#pragma once
#include <vector>
#include "main.h"
#include "Engine.h"

// 頂点構造体
struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};

// 頂点構造体
struct VERTEX_LINE
{
	Vector3 Position;
	Vector4 Diffuse;
};


// マテリアル構造体
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];//16byte境界用
};

struct COLOR
{
	D3DXVECTOR4 color;
};
struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];//16byte境界用
	Vector4		Direction;
	Vector4		Diffuse;
	Vector4		Ambient;
};

struct POINTLIGHT 
{
	Vector4 position;
	Vector4 attenuation;
	Vector4 diffuse;
};

struct Parameter
{
	Vector4 param;
};


class CVertexBuffer;
class CIndexBuffer;
class CTexture;

enum class SHADER_TYPE : int
{
	Default,
	Color,
	Texture,
	Morphing,
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
	static ID3D11Buffer*			m_CameraBuffer;
	static ID3D11Buffer*			m_paramBuffer;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static void CreateDefaultShader();
	static void CreateColorShader();
	static void CreateTextureShader();
	static void CreateMorphingShader();

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
	static void SetViewPort(D3D11_VIEWPORT* viewPort);
	static void SetCameraPosition(Vector3 CameraPosition);
	static void SetParameter(Vector4 v);

	static void SetShader(SHADER_TYPE type);
	static void SetVertexShader(SHADER_TYPE type);
	static void SetPixelShader(SHADER_TYPE type);
	static void SetInputLayout(SHADER_TYPE type);

	static ID3D11Device* GetDevice( void ){ return m_D3DDevice; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_ImmediateContext; }


};
