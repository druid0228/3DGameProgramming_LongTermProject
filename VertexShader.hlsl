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
* AnimatedPackGBufferPSO
*
* - ����ϰ� ���� ��ȯ, �� ��ȯ, ���� ��ȯ ��İ�
* - position, normal, texCoord
*=======================================================================*/
VS_OUTPUT VS_AnimatedWVP(VS_INPUT input) {
	VS_OUTPUT output;

	float weights[4] = { input.weight[0], input.weight[1], input.weight[2], input.weight[3] };

	float3 normal = normalize(input.normal);
	float3 posL		= 0;
	float3 normalL	= 0;
	float3 tangentL = 0;

	for (int i = 0; i < 4; ++i) {
		posL		+= weights[i] * mul(float4(input.position, 1.0f), gmtxAnimation[input.boneIdx[i]]).xyz;
		normalL		+= weights[i] * mul(normal, (float3x3)gmtxAnimation[input.boneIdx[i]]);
		tangentL	+= weights[i] * mul(input.tangent, (float3x3)gmtxAnimation[input.boneIdx[i]]);
	}

	posL.x *= -1;
	normalL.x *= -1;

	output.positionW	= (float3)mul(float4(posL, 1.0f), gmtxGameObject);
	output.position		= mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.normalW		= mul(normalL, (float3x3)gmtxGameObject);
	output.tangentW		= mul(tangentL, (float3x3)gmtxGameObject);
	output.uv			= input.uv;

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
VS_OUTPUT VS_RenderSpotLightShadowObject(VS_INPUT input)
{
	VS_OUTPUT output;

	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(float4(output.positionW, 1.0f), gmtxLightViewProj[0]);

	return output;
}
VS_OUTPUT VS_RenderSpotLightShadowAnimatedObject(VS_INPUT input)
{
	VS_OUTPUT output;

	float weights[4] = { input.weight[0], input.weight[1], input.weight[2], input.weight[3] };

	float3 posL = 0;

	for (int i = 0; i < 4; ++i) {
		posL += weights[i] * mul(float4(input.position, 1.0f), gmtxAnimation[input.boneIdx[i]]).xyz;
	}
	posL.x *= -1;

	output.positionW = (float3)mul(float4(posL, 1.0f), gmtxGameObject);
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
float4 VS_RenderPointLightShadowAnimatedObject(VS_INPUT input) : SV_POSITION{
	float weights[4] = { input.weight[0], input.weight[1], input.weight[2], input.weight[3] };
	float3 posL = 0;
	for (int i = 0; i < 4; ++i) {
		posL += weights[i] * mul(float4(input.position, 1.0f), gmtxAnimation[input.boneIdx[i]]).xyz;
	}
	posL.x *= -1;

	float4 result = mul(float4(posL, 1.0f), gmtxGameObject);

	return result;
}

/*========================================================================
* VS_RenderDirectionalLightShadowPSO
*
* - ���� ��ȯ������ ����.
*=======================================================================*/
float4 VS_RenderDirectionalLightShadow(VS_INPUT input) : SV_POSITION{

	float4 result = mul(float4(input.position, 1.0f), gmtxGameObject);

	return result;
}
float4 VS_RenderDirectionalLightShadowAnimatedObject(VS_INPUT input) : SV_POSITION{
	float weights[4] = { input.weight[0], input.weight[1], input.weight[2], input.weight[3] };
	float3 posL = 0;
	for (int i = 0; i < 4; ++i) {
		posL += weights[i] * mul(float4(input.position, 1.0f), gmtxAnimation[input.boneIdx[i]]).xyz;
	}
	posL.x *= -1;

	float4 result = mul(float4(posL, 1.0f), gmtxGameObject);

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
