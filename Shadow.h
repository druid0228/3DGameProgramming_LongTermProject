#pragma once
/*
* ������� ��� ����.
* 
* ����: �׸��ڸ� �帮�� ������ ��������� �־�� �Ѵ�.(dir: 3��, point: 6��, spot: 1��)
* ����: �ٵ� ���� �н� �� ���� ���� ����� �ִ� 64�� �ؾ� �ϰ� ���̴� ���ҽ��� �ϳ��ۿ� �� �ø���.
* �̷�: ��������� Texture2DArray�� ���� 64���� �� ���� �ø� �� ���� ������?
* 
* 
* 
* 
*/
class DepthTexture;

constexpr int shadowMapWidth = 256;

class Shadow
{
public:
	void Init(ID3D12Device* pd3dDevice, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
	void RenderShadow(ID3D12GraphicsCommandList* pd3dCommandList, UINT shadowIdx) {
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

		pd3dCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
		pd3dCommandList->OMSetRenderTargets(0, nullptr, TRUE, &dsvHandle);
	}

private:
	UINT					m_nMaxShadowMap = 64;
	DepthTexture*			m_pDepthStencilTexture;
	ID3D12DescriptorHeap*	m_pd3dDsvDescriptorHeap;
};

