static const int    MAX_SAMPLES = 64;    // Maximum texture grabs
Texture2D mSrc;

cbuffer SampleCB :register(c0) {
	float4 g_avSampleOffsets[MAX_SAMPLES/2];
}

SamplerState s0 : register(s0);

// The per-color weighting to be used for luminance calculations in RGB order.
static const float3 LUMINANCE_VECTOR = float3(0.2125f, 0.7154f, 0.0721f);

float4 EncodeFloatRGBA(float v)
{
	float4 enc = float4(1.0f, 255.0f, 65025.0f, 16581375.0f) * v;
	enc = frac(enc);
	enc -= enc.yzww * float4(1 / 255.0f, 1 / 255.0f, 1 / 255.0f, 0);
	return enc;
}

float DecodeFloatRGBA(float4 rgba)
{
	return dot(rgba, float4(1, 1 / 255.0f, 1 / 65025.0f, 1 / 16581375.0f));
}

//-----------------------------------------------------------------------------                                
// Desc: Sample the luminance of the source image using a kernal of sample
//       points, and return a scaled image containing the log() of averages
//-----------------------------------------------------------------------------
float4 LumLogInitial
(
	in float4 PosH:SV_POSITION,
	in float2 Tex : TEXCOORD
	) : SV_TARGET
{
	float3 vSample = 0.0f;
	float  fLogLumSum = 0.0f;
	float2 avSampleOffsets[MAX_SAMPLES] = g_avSampleOffsets;
	for (int iSample = 0; iSample < 9; iSample++)
	{
		// Compute the sum of log(luminance) throughout the sample points
		vSample = mSrc.Sample(s0, Tex + avSampleOffsets[iSample]).xyz;
		fLogLumSum += log(dot(vSample, LUMINANCE_VECTOR) + 0.0001f);
	}

	// Divide the sum to complete the average
	fLogLumSum /= 9;
	#ifdef NO_SINGLE_CHANNEL_FLOAT
	return EncodeFloatRGBA(fLogLumSum);
	#else
	return fLogLumSum;
	#endif
}