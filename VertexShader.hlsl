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
*
* - ���� ��ȯ �Ŀ� ��������� �� ��ȯ, ���� ��ȯ ����� ��
* - position �� �Ѱ��ָ� ��. ���̸� ���Ŵϱ�.
*=======================================================================*/
VS_OUTPUT VS_RenderShadow(VS_INPUT input)
{
	VS_OUTPUT output;

	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(float4(output.positionW, 1.0f), gmtxLightSpaceVP);

	return output;
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
