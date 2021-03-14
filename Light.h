#pragma once

/*
Light�� �ϳ��� �ø� �� ���� ���� Scene���� ���̴� Light���� ��Ƽ�
�� �迭�� ����ְ� �� �迭�� �÷��� ��..
Light �ϳ��ϳ��� �� light�� �ʿ��� �������� ���� �ְ�..
enable ���� �༭... ���� light�� ��� �ϴ°�? ��......
�̰� �ϴ� ���߿� �����ϰ� ���� Scene�� �����ϴ� ��� Light��...
�迭�� �� �����Ӹ��� �ְ�... �ø���....

�׷����� LightUploader�� LightManager�� �����...
Light�� �߰��ϰų�? ���ų�? �Ÿ��� ����? 
��� ����Ʈ�� ��ġ�� ��ȸ�ؼ�??
�ָ� ����??
������ �Ѱ�??
*/

typedef enum LIGHTTYPE {
	LIGHT_POINT			= (UINT)1,
	LIGHT_SPOT			= (UINT)2,
	LIGHT_DIRECTIONAL	= (UINT)3
};
struct CB_LIGHT_INFO {
	XMFLOAT3	color;
	float		falloffStart;
	XMFLOAT3	direction;
	float		falloffEnd;
	XMFLOAT3	position;
	float		spotPower;
	UINT		type;
	bool		enable;
	bool		padding0;
	short		padding1;
	XMFLOAT2	padding2 = XMFLOAT2(0,0);
};

class LightManager {
public:
	void Update(XMFLOAT3 camPos);
	void UploadLightInfoToGpu(ID3D12GraphicsCommandList* pd3dCommandList);

	void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, 
		D3D12_CPU_DESCRIPTOR_HANDLE& cbvCpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& cbvGpuHandle);

	UINT AddPointLight(
		XMFLOAT3 color		= XMFLOAT3(1.0f, 0.0f, 0.0f),
		XMFLOAT3 pos		= XMFLOAT3(0,0,0),
		XMFLOAT2 falloff	= XMFLOAT2(300.0f, 500.0f));
	UINT AddDirectionalLight(
		XMFLOAT3 color		= XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT3 dir		= XMFLOAT3(0, -1, 0));
	UINT AddSpotLight(
		XMFLOAT3 color		= XMFLOAT3(0.0f, 0.0f, 1.0f),
		XMFLOAT3 pos		= XMFLOAT3(0, 0, -300),
		XMFLOAT3 dir		= XMFLOAT3(0, 0, 1),
		XMFLOAT2 falloff	= XMFLOAT2(200.0f, 500.0f));

	void SetColor(UINT idx, const XMFLOAT3 color)		{ m_vLightInfo[idx].color = color; }
	void SetPos(UINT idx, const XMFLOAT3 pos)			{ m_vLightInfo[idx].position = pos; }
	void SetFalloff(UINT idx, const XMFLOAT2 falloff)	{ m_vLightInfo[idx].falloffStart = falloff.x; m_vLightInfo[idx].falloffEnd = falloff.y; }
	void SetDir(UINT idx, const XMFLOAT3 dir)			{ m_vLightInfo[idx].direction = dir; }

private:
	UINT						m_nMaxLight = 64;
	D3D12_GPU_DESCRIPTOR_HANDLE m_d3dCbvGPUDescriptorHandle;
	ID3D12Resource*				m_pd3dCBResource;
	CB_LIGHT_INFO*				m_pCBMappedlightInfo;

	vector<CB_LIGHT_INFO>		m_vLightInfo;

};