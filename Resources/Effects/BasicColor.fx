/**
	File: BasicColor.fx
	Date created: 2012-08-13

	A simple effect for rendering colored primitives
*/


/** Input layouts & Global variables */
struct VSINPUT
{
	float4 PositionL : POSITION;
	float4 Color : COLOR;
};

struct PSINPUT
{
	float4 PositionH : SV_POSITION;
	float4 Color : COLOR;
};

cbuffer cbPerFrame
{
	float4x4 gWVP;
};


/** Render states */
RasterizerState RasterState
{
	CullMode = None;
};



/** Shader implementation */
PSINPUT VS(VSINPUT input)
{
	PSINPUT output;

	output.PositionH = mul(input.PositionL, gWVP);
	output.Color = input.Color;

	return output;
}

float4 PS(PSINPUT input) : SV_TARGET0
{
	return input.Color;
}


/** Technique definitions */
technique11 StandardTechnique
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));

		SetRasterizerState(RasterState);
	}
}