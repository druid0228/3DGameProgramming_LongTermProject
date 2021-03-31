#include "Header.hlsli"

/*========================================================================
* PackGBufferPSO
*
* - ����ϰ� ���� ��ȯ, �� ��ȯ, ���� ��ȯ ��İ�
* - position, normal, texCoord
*=======================================================================*/
VS_OUTPUT VS_PackGBuffer(VS_INPUT input) {
	VS_OUTPUT output;

	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.uv = input.uv;

	return output;
}

/*========================================================================
* RenderShadowPSO
* VS_MulW_LV_LP
* TransformToWolrdLightViewProj ?
*
* - ���� ��ȯ �Ŀ� ��������� �� ��ȯ, ���� ��ȯ ����� ��
* - position �� �Ѱ��ָ� ��. ���̸� ���Ŵϱ�.
*=======================================================================*/
VS_OUTPUT VS_RenderShadow(VS_INPUT input)
{
	VS_OUTPUT output;

	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(float4(output.positionW, 1.0f), gmtxLightViewProj[0]);

	return output;
}


/*========================================================================
* RenderPointLightShadowPSO, VS_RenderDirectionalLightShadowPSO
* TransToWorldSpace �� �̸��� �ٲٴ°�?
* �ƴϸ� VS_MulW
*
* - ���� ��ȯ������ ����.
*=======================================================================*/
float4 VS_RenderPointLightShadow(VS_INPUT input) : SV_POSITION {

	float4 result = mul(float4(input.position, 1.0f), gmtxGameObject);

	return result;
}

/*========================================================================
* VS_RenderDirectionalLightShadowPSO
*
* - ���� ��ȯ������ ����.
*=======================================================================*/
float4 VS_RenderDirectionalLightShadow(VS_INPUT input) : SV_POSITION{

	float4 result = mul(float4(input.position, 1.0f), gmtxGameObject);
	//float4 result = mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView);

	return result;
}

/*========================================================================
* ColorFromGBufferPSO, AddLightPSO
*
* - ��� ��ũ���� �׸� ����. ���� ��ȯ�� ����. ���� �޽� ������ �̸� ����.
* - position, uv
*=======================================================================*/
VS_OUTPUT VS_FlatScreen(VS_INPUT input)
{
	VS_OUTPUT output;

	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = float4(output.positionW, 1.0f);
	output.uv = input.uv;

	return output;
}
