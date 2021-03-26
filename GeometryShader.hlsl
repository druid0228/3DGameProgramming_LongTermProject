#include "Header.hlsli"

[maxvertexcount(18)]
void GS_RenderPointLightShadow(
	triangle float4 input[3] : SV_POSITION, 
	inout TriangleStream< GS_OUTPUT > outStream
)
{
	for (uint iFace = 0; iFace < 6; iFace++) {
		GS_OUTPUT output;

		output.RTIndex = iFace;

		for (int v = 0; v < 3; v++) {
			output.pos = mul(input[v], gmtxLightViewProj[iFace]);
			outStream.Append(output);
		}
		outStream.RestartStrip();
	}
}

/*========================================================================
* Directional Light Shadow �� Cascade Shadow Map�� ����� GS
* �ϴ��� �Ÿ� �������� �ʰ� (�װɷ� �Ǵ����� �� �𸣰�����) ��� ������ʿ� ���
* 
*=======================================================================*/
[maxvertexcount(9)]
void GS_RenderDirectionalLightShadow(
	triangle float4 input[3] : SV_POSITION,
	inout TriangleStream< GS_OUTPUT > outStream
)
{
	for (uint iFace = 0; iFace < 3; iFace++) {
		GS_OUTPUT output;

		output.RTIndex = iFace;

		for (int v = 0; v < 3; v++) {
			output.pos = mul(input[v], gmtxLightViewProj[iFace]);
			outStream.Append(output);
		}
		outStream.RestartStrip();
	}
}