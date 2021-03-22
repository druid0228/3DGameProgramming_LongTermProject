#include "stdafx.h"
#include "Scene.h"
#include "Object.h"
#include "PipelineStateObject.h"
#include "Camera.h"
#include "Framework.h"
#include "Texture.h"
#include "Light.h"
#include "Importer.h"
#include "Vertex.h"

ID3D12RootSignature* Scene::CreateRootSignature()
{
	ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;
	D3D12_DESCRIPTOR_RANGE d3dDescriptorRange[6];

	d3dDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	d3dDescriptorRange[0].NumDescriptors = 1;
	d3dDescriptorRange[0].BaseShaderRegister = ROOTSIGNATURE_OBJECTS;
	d3dDescriptorRange[0].RegisterSpace = 0;
	d3dDescriptorRange[0].OffsetInDescriptorsFromTableStart = 0;

	d3dDescriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	d3dDescriptorRange[1].NumDescriptors = 1;
	d3dDescriptorRange[1].BaseShaderRegister = ROOTSIGNATURE_LIGHTS;
	d3dDescriptorRange[1].RegisterSpace = 0;
	d3dDescriptorRange[1].OffsetInDescriptorsFromTableStart = 0;

	d3dDescriptorRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	d3dDescriptorRange[2].NumDescriptors = 1;
	d3dDescriptorRange[2].BaseShaderRegister = ROOTSIGNATURE_COLOR_TEXTURE;
	d3dDescriptorRange[2].RegisterSpace = 0;
	d3dDescriptorRange[2].OffsetInDescriptorsFromTableStart = 0;

	d3dDescriptorRange[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	d3dDescriptorRange[3].NumDescriptors = 1;
	d3dDescriptorRange[3].BaseShaderRegister = ROOTSIGNATURE_NORMAL_TEXTURE;
	d3dDescriptorRange[3].RegisterSpace = 0;
	d3dDescriptorRange[3].OffsetInDescriptorsFromTableStart = 0;

	d3dDescriptorRange[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	d3dDescriptorRange[4].NumDescriptors = 1;
	d3dDescriptorRange[4].BaseShaderRegister = ROOTSIGNATURE_DEPTH_TEXTURE;
	d3dDescriptorRange[4].RegisterSpace = 0;
	d3dDescriptorRange[4].OffsetInDescriptorsFromTableStart = 0;

	d3dDescriptorRange[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	d3dDescriptorRange[5].NumDescriptors = 1;
	d3dDescriptorRange[5].BaseShaderRegister = ROOTSIGNATURE_SHADOW_TEXTURE;
	d3dDescriptorRange[5].RegisterSpace = 0;
	d3dDescriptorRange[5].OffsetInDescriptorsFromTableStart = 0;

	D3D12_ROOT_PARAMETER pd3dRootParameters[7];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = ROOTSIGNATURE_PASSCONSTANTS;
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[1].DescriptorTable.pDescriptorRanges = &d3dDescriptorRange[0];
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[2].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[2].DescriptorTable.pDescriptorRanges = &d3dDescriptorRange[1];
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[3].DescriptorTable.pDescriptorRanges = &d3dDescriptorRange[2];
	pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[4].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[4].DescriptorTable.pDescriptorRanges = &d3dDescriptorRange[3];
	pd3dRootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[5].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[5].DescriptorTable.pDescriptorRanges = &d3dDescriptorRange[4];
	pd3dRootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[6].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[6].DescriptorTable.pDescriptorRanges = &d3dDescriptorRange[5];
	pd3dRootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc[2];
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC) * 2);
	d3dSamplerDesc[0].Filter = D3D12_FILTER_ANISOTROPIC;
	d3dSamplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].MipLODBias = 0;
	d3dSamplerDesc[0].MaxAnisotropy = 1;
	d3dSamplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc[0].MinLOD = 0;
	d3dSamplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc[0].ShaderRegister = 0;
	d3dSamplerDesc[0].RegisterSpace = 0;
	d3dSamplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	d3dSamplerDesc[1].Filter = D3D12_FILTER_ANISOTROPIC;
	d3dSamplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	d3dSamplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	d3dSamplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	d3dSamplerDesc[1].MipLODBias = 0;
	d3dSamplerDesc[1].MaxAnisotropy = 16;
	d3dSamplerDesc[1].ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	d3dSamplerDesc[1].MinLOD = 0;
	d3dSamplerDesc[1].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc[1].ShaderRegister = 1;
	d3dSamplerDesc[1].RegisterSpace = 0;
	d3dSamplerDesc[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 2;
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc[0];
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	HRESULT isSuccess = D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &pd3dSignatureBlob, &pd3dErrorBlob);
	isSuccess = m_pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return pd3dGraphicsRootSignature;
}

void Scene::Init(Framework* pFramework, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	/*========================================================================
	* �ֿ� ���� �ʱ�ȭ
	*=======================================================================*/
	m_pd3dDevice		= pd3dDevice;
	m_pd3dCommandList	= pd3dCommandList;
	m_pd3dRootSignature = CreateRootSignature();

	/*========================================================================
	* ī�޶� ����
	*=======================================================================*/
	m_pCamera = new FollowCamera();
	m_pCamera->SetPosition(XMFLOAT3(0, 0, -2));
	m_pCamera->SetLookAt(XMFLOAT3(0, 0, 0));

	/*========================================================================
	* ��ũ���� �� ����
	*=======================================================================*/
	CreateDescriptorHeap();


	/*========================================================================
	* PassConstants ����
	*=======================================================================*/
	CreatePassInfoShaderResource();

	/*========================================================================
	* Pass 1 ���� ������Ʈ ������ �ε� �� ����
	*=======================================================================*/
	ObjectDataImporter objDataImporter;
	vector<OBJECT_DESC> vecObjDesc = objDataImporter.Load("Data/ObjectData.txt");
	MeshDataImporter meshDataImporter;


	for (int i = 0; i < vecObjDesc.size(); i++) {
		if (strcmp(vecObjDesc[i].modelPath.c_str(), "") != 0) {

			vector<MESH_DATA> vecMeshData = meshDataImporter.Load(vecObjDesc[i].modelPath.c_str());

			Object* tempObj = new Object(m_pd3dDevice, m_pd3dCommandList, m_d3dCbvCPUDescriptorStartHandle, m_d3dCbvGPUDescriptorStartHandle, vecMeshData);

			tempObj->Move(vecObjDesc[i].position);
			m_vecObject.push_back(tempObj);
		}
		else {
			Object* tempObj = new Object(m_pd3dDevice, m_pd3dCommandList, m_d3dCbvCPUDescriptorStartHandle, m_d3dCbvGPUDescriptorStartHandle);

			tempObj->Move(vecObjDesc[i].position);
			m_vecObject.push_back(tempObj);
		}
	}


	/*========================================================================
	* Pass 2 ���� ����� ������ ����
	*=======================================================================*/
	for (int i = 0; i < 3; i++) {
		DebugWindowObject* temp = new DebugWindowObject(m_pd3dDevice, m_pd3dCommandList, m_d3dCbvCPUDescriptorStartHandle, m_d3dCbvGPUDescriptorStartHandle);
		temp->Move(XMFLOAT3(-0.5f * i, 1, 0));

		m_vecDebugWindow.push_back(temp);
	}

	/*========================================================================
	* Pass 2 ���� ��ü ȭ�� �簢��
	*=======================================================================*/
	DebugWindowObject* tempScreen = new DebugWindowObject(m_pd3dDevice, m_pd3dCommandList, m_d3dCbvCPUDescriptorStartHandle, m_d3dCbvGPUDescriptorStartHandle, true);
	m_vecDebugWindow.push_back(tempScreen);



	/*========================================================================
	* �ؽ���
	*=======================================================================*/
	m_TextureMng = new TextureManager(m_pd3dDevice);
	m_TextureMng->AddDepthBufferTexture("GBuffer_Depth", m_pd3dDevice, 1000, 1000, m_d3dSrvCPUDescriptorStartHandle, m_d3dSrvGPUDescriptorStartHandle);
	m_TextureMng->AddRenderTargetTexture("GBuffer_Color", m_pd3dDevice, 1000, 1000, m_d3dSrvCPUDescriptorStartHandle, m_d3dSrvGPUDescriptorStartHandle);
	m_TextureMng->AddRenderTargetTexture("GBuffer_Normal", m_pd3dDevice, 1000, 1000, m_d3dSrvCPUDescriptorStartHandle, m_d3dSrvGPUDescriptorStartHandle);
	m_TextureMng->LoadFromFile("Tex_Test", L"test.dds", m_pd3dDevice, m_pd3dCommandList, m_d3dSrvCPUDescriptorStartHandle, m_d3dSrvGPUDescriptorStartHandle);

	/*========================================================================
	* ���� ����
	*=======================================================================*/
	LightDataImporter lightDataImporter;
	vector<LIGHT_DESC> vecLightDesc = lightDataImporter.Load("Data/LightData.txt");
	string shadow("ShadowMap_");
	m_LightMng = new LightManager();
	for (int i = 0; i < vecLightDesc.size(); i++) {
		switch (vecLightDesc[i].lightType)
		{
		case LightType::LIGHT_POINT:		m_LightMng->AddPointLight(vecLightDesc[i], m_pd3dDevice, m_pd3dCommandList, m_d3dCbvCPUDescriptorStartHandle, m_d3dCbvGPUDescriptorStartHandle);		break;
		case LightType::LIGHT_DIRECTIONAL:	m_LightMng->AddDirectionalLight(vecLightDesc[i], m_pd3dDevice, m_pd3dCommandList, m_d3dCbvCPUDescriptorStartHandle, m_d3dCbvGPUDescriptorStartHandle);	break;
		case LightType::LIGHT_SPOT:			m_LightMng->AddSpotLight(vecLightDesc[i], m_pd3dDevice, m_pd3dCommandList, m_d3dCbvCPUDescriptorStartHandle, m_d3dCbvGPUDescriptorStartHandle);			break;
		case LightType::LIGHT_NONE:
		default:
			break;
		}

		if (vecLightDesc[i].bIsShadow) {
			string temp = to_string(i);
			temp = shadow + temp;

			m_TextureMng->AddDepthBufferTexture(temp.c_str(), m_pd3dDevice, 512, 512, m_d3dSrvCPUDescriptorStartHandle, m_d3dSrvGPUDescriptorStartHandle);
			m_LightMng->SetShadowMapName(temp.c_str(), i);
		}

	}

	/*========================================================================
	* PSO ����
	*=======================================================================*/
	CreatePSO();
}

void Scene::RenderPass1()
{
	/*========================================================================
	* ��Ʈ �ñ״�ó, ��ũ���� ��, ���������� ������Ʈ, �޽� �������� ����
	*=======================================================================*/
	m_pd3dCommandList->SetGraphicsRootSignature(m_pd3dRootSignature);
	m_pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);
	m_pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	/*========================================================================
	* PassInfo ����
	*=======================================================================*/
	m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	m_pCamera->SetViewportsAndScissorRects(m_pd3dCommandList);
	UpdatePassInfoAboutCamera();

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbPassInfo->GetGPUVirtualAddress();
	m_pd3dCommandList->SetGraphicsRootConstantBufferView(ROOTSIGNATURE_PASSCONSTANTS, d3dGpuVirtualAddress);

	XMFLOAT4X4 texture = {
	0.5f,		0,		0,		0,
	0,		-0.5f,		0,		0,
	0,			0,	 1.0f,		0,
	0.5f,	 0.5f,		0,	 1.0f };
	UINT passIdx = 0;
	XMStoreFloat4x4(&m_pcbMappedPassInfo->m_xmf4x4TextureTransform, XMMatrixTranspose(XMLoadFloat4x4(&texture)));




	/*========================================================================
	* Pass 1. �޽� ���� To Color, Normal, Depth
	*=======================================================================*/
	D3D12_RESOURCE_BARRIER d3dResourceBarrier[3];
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	d3dResourceBarrier[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[0].Transition.pResource = m_TextureMng->GetTextureResource("GBuffer_Depth");
	d3dResourceBarrier[0].Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
	d3dResourceBarrier[0].Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	d3dResourceBarrier[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	d3dResourceBarrier[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[1].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[1].Transition.pResource = m_TextureMng->GetTextureResource("GBuffer_Color");
	d3dResourceBarrier[1].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	d3dResourceBarrier[1].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	d3dResourceBarrier[2].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[2].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[2].Transition.pResource = m_TextureMng->GetTextureResource("GBuffer_Normal");
	d3dResourceBarrier[2].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	d3dResourceBarrier[2].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier[2].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pd3dCommandList->ResourceBarrier(3, d3dResourceBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_TextureMng->GetDsvCPUHandle("GBuffer_Depth");
	m_pd3dCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle[2];
	rtvHandle[0] = m_TextureMng->GetRtvCPUHandle("GBuffer_Color");
	rtvHandle[1] = m_TextureMng->GetRtvCPUHandle("GBuffer_Normal");

	float pfClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_pd3dCommandList->ClearRenderTargetView(rtvHandle[0], pfClearColor, 0, NULL);
	m_pd3dCommandList->ClearRenderTargetView(rtvHandle[1], pfClearColor, 0, NULL);

	m_pd3dCommandList->OMSetRenderTargets(2, rtvHandle, FALSE, &dsvHandle);

	m_pd3dCommandList->SetPipelineState(m_uomPipelineStates["PackGBuffer"]);
	m_TextureMng->UseForShaderResource("Tex_Test", m_pd3dCommandList, ROOTSIGNATURE_COLOR_TEXTURE);
	for (int i = 0; i < m_vecObject.size(); i++) m_vecObject[i]->Render(m_pd3dCommandList);

	d3dResourceBarrier[0].Transition.pResource = m_TextureMng->GetTextureResource("GBuffer_Depth");
	d3dResourceBarrier[0].Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	d3dResourceBarrier[0].Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	d3dResourceBarrier[1].Transition.pResource = m_TextureMng->GetTextureResource("GBuffer_Color");
	d3dResourceBarrier[1].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier[1].Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	d3dResourceBarrier[2].Transition.pResource = m_TextureMng->GetTextureResource("GBuffer_Normal");
	d3dResourceBarrier[2].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier[2].Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	m_pd3dCommandList->ResourceBarrier(3, d3dResourceBarrier);

	/*========================================================================
	* Pass 1. ���� ����
	*=======================================================================*/
	m_pCamera->SetViewport(0, 0, 512, 512, 0.0f, 1.0f);
	m_pCamera->SetViewportsAndScissorRects(m_pd3dCommandList);
	m_pd3dCommandList->SetPipelineState(m_uomPipelineStates["RenderShadow"]);

	for (UINT i = 0; i < m_LightMng->GetNumLight(); i++) {
		if (m_LightMng->GetIsShadow(i)) {
			m_LightMng->SetShaderResource(m_pd3dCommandList, i);

			

			D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_TextureMng->GetDsvCPUHandle(m_LightMng->GetShadowMapName(i).c_str());
			m_pd3dCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
			m_pd3dCommandList->OMSetRenderTargets(0, NULL, TRUE, &dsvHandle);

			for (int i = 0; i < m_vecObject.size(); i++) m_vecObject[i]->Render(m_pd3dCommandList);
		}
	}
}
void Scene::RenderPass2()
{
	/*========================================================================
	* ��Ʈ �ñ״�ó, ��ũ���� ��, �޽� �������� ����
	* 
	* �ٵ� PrevRender���� �ߴ� �������� ������� ������?
	*=======================================================================*/
	m_pd3dCommandList->SetGraphicsRootSignature(m_pd3dRootSignature);				
	m_pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);			
	m_pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/*========================================================================
	* PassInfo ����
	*=======================================================================*/
	m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	m_pCamera->SetViewportsAndScissorRects(m_pd3dCommandList);

	/*========================================================================
	* Pass 2. ��ũ�� ����
	*=======================================================================*/
	m_pd3dCommandList->SetPipelineState(m_uomPipelineStates["ColorFromGBuffer"]);
	m_TextureMng->UseForShaderResource("GBuffer_Color", m_pd3dCommandList, ROOTSIGNATURE_COLOR_TEXTURE);
	m_vecDebugWindow[3]->Render(m_pd3dCommandList);


	/*========================================================================
	* Pass 2. ������ ����
	*=======================================================================*/
	m_pd3dCommandList->SetPipelineState(m_uomPipelineStates["AddLight"]);
	m_TextureMng->UseForShaderResource("GBuffer_Normal", m_pd3dCommandList, ROOTSIGNATURE_NORMAL_TEXTURE);
	m_TextureMng->UseForShaderResource("GBuffer_Depth", m_pd3dCommandList, ROOTSIGNATURE_DEPTH_TEXTURE);

	for (UINT i = 0; i < m_LightMng->GetNumLight(); i++) {
		m_LightMng->SetShaderResource(m_pd3dCommandList, i);

		if (m_LightMng->GetIsShadow(i)) {
			m_TextureMng->UseForShaderResource(m_LightMng->GetShadowMapName(i).c_str(), m_pd3dCommandList, ROOTSIGNATURE_SHADOW_TEXTURE);
		}

		m_vecDebugWindow[3]->Render(m_pd3dCommandList);
	}

	/*========================================================================
	* Pass 2. Debug Window ����
	*=======================================================================*/
	if (test) {
		m_pd3dCommandList->SetPipelineState(m_uomPipelineStates["DebugColor"]);
		m_TextureMng->UseForShaderResource("GBuffer_Color", m_pd3dCommandList, ROOTSIGNATURE_COLOR_TEXTURE);
		m_vecDebugWindow[0]->Render(m_pd3dCommandList);
		m_TextureMng->UseForShaderResource("GBuffer_Normal", m_pd3dCommandList, ROOTSIGNATURE_COLOR_TEXTURE);
		m_vecDebugWindow[1]->Render(m_pd3dCommandList);

		m_TextureMng->UseForShaderResource("GBuffer_Depth", m_pd3dCommandList, ROOTSIGNATURE_DEPTH_TEXTURE);
		m_pd3dCommandList->SetPipelineState(m_uomPipelineStates["DebugDepth"]);
		m_vecDebugWindow[2]->Render(m_pd3dCommandList);
	}



}
void Scene::Update(float fTimeElapsed)
{
	m_pCamera->Update(fTimeElapsed);

	m_fCurrentTime += fTimeElapsed;
	::memcpy(&m_pcbMappedPassInfo->m_xmfCurrentTime, &m_fCurrentTime, sizeof(float));
}
void Scene::Input(UCHAR* pKeyBuffer, float fTimeElapsed)
{
	if (pKeyBuffer[KeyCode::_W] & 0xF0) { m_pCamera->MoveForward(2); }
	if (pKeyBuffer[KeyCode::_A] & 0xF0) { m_pCamera->MoveLeft(2); }
	if (pKeyBuffer[KeyCode::_S] & 0xF0) { m_pCamera->MoveBackward(2); }
	if (pKeyBuffer[KeyCode::_D] & 0xF0) { m_pCamera->MoveRight(2); }
	if (pKeyBuffer[KeyCode::_R] & 0xF0) { m_pCamera->MoveUp(2); }
	if (pKeyBuffer[KeyCode::_F] & 0xF0) { m_pCamera->MoveDown(2); }
	if (pKeyBuffer[KeyCode::_Q] & 0xF0) { m_pCamera->Rotate(0, -50 * fTimeElapsed, 0); }
	if (pKeyBuffer[KeyCode::_E] & 0xF0) { m_pCamera->Rotate(0, 50 * fTimeElapsed, 0); }
	if (pKeyBuffer[KeyCode::_Z] & 0xF0) { m_pCamera->Rotate(50 * fTimeElapsed, 0, 0); }
	if (pKeyBuffer[KeyCode::_X] & 0xF0) { m_pCamera->Rotate(-50 * fTimeElapsed, 0, 0); }

	if (pKeyBuffer[KeyCode::_1] & 0xF0) {
		m_pCamera->SetPosition(XMFLOAT3(0, 0, -5));
		m_pCamera->SetLookAt(XMFLOAT3(0, 0, 0));

	}
	if (pKeyBuffer[KeyCode::_2] & 0xF0) {
		m_pCamera->SetPosition(XMFLOAT3(0, 3, -5));
		m_pCamera->SetLookAt(XMFLOAT3(0, 3, 0));

	}
	if (pKeyBuffer[KeyCode::_3] & 0xF0) {
		m_pCamera->SetPosition(XMFLOAT3(0, 0, 5));
		m_pCamera->SetLookAt(XMFLOAT3(0, 0, 0));
	}
	if (pKeyBuffer[KeyCode::_4] & 0xF0) {
		m_pCamera->SetPosition(XMFLOAT3(5, 0, 0));
		m_pCamera->SetLookAt(XMFLOAT3(0, 0, 0));
	}

	if (pKeyBuffer[KeyCode::_N] & 0xF0) { test = true; }
	if (pKeyBuffer[KeyCode::_M] & 0xF0) { test = false; }



}

void Scene::CreatePSO()
{
	PackGBufferPSO PackGBufferPso = PackGBufferPSO(m_pd3dDevice, m_pd3dRootSignature);
	m_uomPipelineStates["PackGBuffer"] = PackGBufferPso.GetPipelineState();

	RenderShadowPSO RenderShadowPso = RenderShadowPSO(m_pd3dDevice, m_pd3dRootSignature);
	m_uomPipelineStates["RenderShadow"] = RenderShadowPso.GetPipelineState();

	ColorFromGBufferPSO ColorFromGBufferPso = ColorFromGBufferPSO(m_pd3dDevice, m_pd3dRootSignature);
	m_uomPipelineStates["ColorFromGBuffer"] = ColorFromGBufferPso.GetPipelineState();

	AddLightPSO AddLightPso = AddLightPSO(m_pd3dDevice, m_pd3dRootSignature);
	m_uomPipelineStates["AddLight"] = AddLightPso.GetPipelineState();

	DebugColorPSO DebugColorPso = DebugColorPSO(m_pd3dDevice, m_pd3dRootSignature);
	m_uomPipelineStates["DebugColor"] = DebugColorPso.GetPipelineState();

	DebugDepthPSO DebugDepthPso = DebugDepthPSO(m_pd3dDevice, m_pd3dRootSignature);
	m_uomPipelineStates["DebugDepth"] = DebugDepthPso.GetPipelineState();
}
void Scene::UpdatePassInfoAboutCamera()
{
	XMFLOAT4X4 xmf4x4Temp;

	xmf4x4Temp = m_pCamera->GetViewMatrix();
	XMStoreFloat4x4(&m_pcbMappedPassInfo->m_xmf4x4CameraView,			XMMatrixTranspose(XMLoadFloat4x4(&xmf4x4Temp)));
	xmf4x4Temp = Matrix4x4::Inverse(xmf4x4Temp);
	XMStoreFloat4x4(&m_pcbMappedPassInfo->m_xmf4x4CameraViewInv,		XMMatrixTranspose(XMLoadFloat4x4(&xmf4x4Temp)));

	xmf4x4Temp = m_pCamera->GetProjectionMatrix();
	XMStoreFloat4x4(&m_pcbMappedPassInfo->m_xmf4x4CameraProjection,		XMMatrixTranspose(XMLoadFloat4x4(&xmf4x4Temp)));
	xmf4x4Temp = Matrix4x4::Inverse(xmf4x4Temp);
	XMStoreFloat4x4(&m_pcbMappedPassInfo->m_xmf4x4CameraProjectionInv,	XMMatrixTranspose(XMLoadFloat4x4(&xmf4x4Temp)));

	::memcpy(&m_pcbMappedPassInfo->m_xmf3CameraPosition, &m_pCamera->GetPosition(), sizeof(XMFLOAT3));


}
void Scene::CreatePassInfoShaderResource()
{
	UINT ncbElementBytes = ((sizeof(CB_PASS_INFO) + 255) & ~255); //256�� ���
	m_pd3dcbPassInfo = ::CreateBufferResource(m_pd3dDevice, m_pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);


	D3D12_GPU_VIRTUAL_ADDRESS		d3dGpuVirtualAddress;
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;

	if (nullptr != m_pd3dcbPassInfo) {
		m_pd3dcbPassInfo->Map(0, NULL, (void**)&m_pcbMappedPassInfo);
		d3dGpuVirtualAddress = m_pd3dcbPassInfo->GetGPUVirtualAddress();
		d3dCBVDesc.SizeInBytes = ncbElementBytes;
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress;
		m_pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, m_d3dCbvCPUDescriptorStartHandle);

		m_d3dCbvCPUDescriptorStartHandle.ptr += gnCbvSrvDescriptorIncrementSize;
	}

	m_d3dCbvGPUPassInfoHandle = m_d3dCbvGPUDescriptorStartHandle;
	m_d3dCbvGPUDescriptorStartHandle.ptr += gnCbvSrvDescriptorIncrementSize;

}
void Scene::CreateDescriptorHeap() 
{
	// heap ������ �Ϸ��� �̸� �����ڸ� �� ���� ������ �� �˾ƾ� �ϱ� ������
	// obj ������ �߿������� srv�� �� texture ������ �߿���..
	// �̸� �� �� ������ �� �� ������ �װ� ���صΰ� �� ������ �ϰڴµ�
	// ���� �������� ���ҽ� �ε�&��ε带 �Ѵٸ�?
	// �׷� ��쿡�� ���� �̸� �ѵ��� �ɾ�ΰ� ����� �͵� ����� �� ����
	// (���� ���, obj�� �� ��, ����ü�� �� ��, �ؽ��Ĵ� �� �� ������ �дٴ���)
	// ����, ������ ����. ������Ʈ�� ����ü, ���� ���� �� ���ļ� 1,024�� ������ �� ��.
	// �ؽ��ĵ� 128�� ������ ����� ��.
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = MAXNUMCBV + MAXNUMSRV;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;

	HRESULT result = m_pd3dDevice->CreateDescriptorHeap(
		&d3dDescriptorHeapDesc,
		__uuidof(ID3D12DescriptorHeap),
		(void**)&m_pd3dCbvSrvDescriptorHeap);
	HRESULT reason = m_pd3dDevice->GetDeviceRemovedReason();

	m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * MAXNUMCBV);
	m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * MAXNUMCBV);

}

void Scene::Release()
{
	if (m_pd3dcbPassInfo)
	{
		m_pd3dcbPassInfo->Unmap(0, NULL);
		m_pd3dcbPassInfo->Release();
	}
}
