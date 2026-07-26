#ifndef D3dSurfInfo_h
#define D3dSurfInfo_h

#include <d3d9.h>
#include <d3d11.h>

/*
	// Example usage:
	D3DSURFINFO info;
	info.m_lSize = sizeof(info);
	surf.GetDriverInfo(&info);
*/

typedef struct D3DSURFINFO 
{
	DWORD m_lSize; // must be set to sizeof(D3DSURFINFO)
	int m_nD3DVersion; // 8, 9 or 11
	union
	{
		IDirect3D9* m_pD3D9;
		ID3D11DeviceContext* m_pD3D11DevCtx;
	};
	union
	{
		IDirect3DDevice9* m_pD3D9Device;
		ID3D11Device* m_pD3D11Device;
	};
	union
	{
		IDirect3DTexture9* m_pD3D9Texture;
		ID3D11ShaderResourceView** m_pD3D11SRV; // dereference to retrieve shader resource view, might be NULL though until you do blits
	};
	int m_dwPixelShaderVersion;
	int m_dwVertexShaderVersion; // not used
	int m_dwMaxTextureWidth;
	int m_dwMaxTextureHeight;
	// DX11 exclusive (info not 100% confirmed):
	// -----
	LPVOID ppD3D11RenderTargetTexture;
	LPVOID ppD3D11RenderTargetView;
	LPVOID txtContext;
	// -----
} D3DSURFINFO;

#endif // D3dSurfInfo_h
