#include "Header.hlsli"
#include "Light.hlsli"

//float4 DeferredShading(VS_OUTPUT input) : SV_TARGET {
//	float3 worldPos			= WorldPosFromLinearDepth(input.uv);
//	float3 color			= gtxtColorMap.Sample(gSamplerState, input.uv).xyz;
//	float3 vNormal			= gtxtNormalMap.Sample(gSamplerState, input.uv).xyz * 2.0f - 1.0f;
//
//	float3 vToCam			= normalize(gvCameraPosition - worldPos);
//
//	//==============================================================================
//	float3 result = float3(0.1f, 0.1f, 0.1f);
//	//for (int i = 0; i < 64; i++) {
//	//	switch (gLightArray[i].type) {
//	//	case 1: result += CalcPointLight(gLightArray[i], color, worldPos, vNormal, vToCam); break;
//	//	case 2: result += CalcSpotLight(gLightArray[i], color, worldPos, vNormal, vToCam); break;
//	//	case 3: result += CalcDirectionalLight(gLightArray[i], color, vNormal, vToCam);	break;
//	//	default: break;
//	//	}
//	//}
//	return float4(color, 1.0f);
//	//==============================================================================
//
//}

/*========================================================================
* PackGBufferPSO
*
* - position, normal, texCoord�� �Ѿ��.
*=======================================================================*/
GBuffer PS_PackGBuffer(VS_OUTPUT input)
{
	GBuffer output;
	input.normalW = normalize(input.normalW);
	output.cColor = gtxtColorMap.Sample(gSamplerState, input.uv);
	output.cNormal = float4((input.normalW * 0.5 + 0.5), 1.0f);

	return output;
}

/*========================================================================
* RenderShadowPSO
*
* - Depth�� �� �����̹Ƿ� PS�� ����θ� ��. ���� ����� ���� �� �ҰŴϱ�.
*=======================================================================*/
void PS_RenderShadow(VS_OUTPUT input) {

}

/*========================================================================
* ColorFromGBufferPSO, DebugColorPSO
*
* - Color�� �о�� ����. �״�� �Ѹ��� �ȴ�.
*=======================================================================*/
float4 PS_ColorFromGBuffer(VS_OUTPUT input) : SV_TARGET{
	float3 color = gtxtColorMap.Sample(gSamplerState, input.uv).xyz;
	color -= 0.9;
	return float4(color, 1.0f);
}

/*========================================================================
* DebugDepthPSO
*
* - Depth�� �о�� ����. �״�� �Ѹ��� �ȴ�.
*=======================================================================*/
float4 PS_DepthFromGBuffer(VS_OUTPUT input) : SV_TARGET{
	float depth = gtxtDepthMap.Sample(gSamplerState, input.uv).r;
	depth = pow(depth, 5);

	return float4(depth.xxx, 1.0f);
}

/*========================================================================
* AddLightPSO
*
* - ������ ����ϱ� ���� GBuffer�� Normal�� Depth�� �о�� �Ѵ�.
*=======================================================================*/
float4 PS_AddLight(VS_OUTPUT input) : SV_TARGET{
	float3 vWorldPosition	= WorldPosFromLinearDepth(input.uv);
	float3 vNormal			= gtxtNormalMap.Sample(gSamplerState, input.uv).xyz * 2.0f - 1.0f;
	float3 vToEye			= normalize(gvCameraPosition - vWorldPosition);

	float3 result = float3(0.0f, 0.0f, 0.0f);
	// ����...
	switch (gLightType) {
	case 1: result += CalcPointLight(vWorldPosition, vNormal, vToEye); break;
	case 2: result += CalcSpotLight(vWorldPosition, vNormal, vToEye); break;
	case 3: result += CalcDirectionalLight(vNormal, vToEye);	break;
	default: break;
	}
	return float4(result, 1.0f);
}