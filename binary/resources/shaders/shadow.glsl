struct ShadowMap {
	sampler2D shadowMap;
	vec2 			shadowMapSize;
	float 		reduceLightBleedingAmount;
	float 		minVariance;
};

float linstep(float min, float max, float v) {
	return clamp((v-min)/(max-min), 0.0, 1.0);
}

float ReduceLightBleeding(float pmax, float Amount) {
	return linstep(Amount, 1, pmax);
}

float chebychev(vec2 moments, float compare, float reduceLightBleedingAmout, float minVariance) {

	float p = step(compare, moments.x);
	
	// Calcualte the variance.
	float variance = max(moments.y - moments.x * moments.x, minVariance);

	float d = compare - moments.x;
	float pmax = variance / (variance + d*d);
	pmax = ReduceLightBleeding(pmax, reduceLightBleedingAmout);

	return min(max(p, pmax), 1.0);
}

vec2 CalcParallaxTexCoords(sampler2D dispMap, mat3 tbnMatrix, vec3 directionToEye, vec2 texCoords, float scale, float bias) {
	return texCoords.xy + (directionToEye * tbnMatrix).xy * (texture(dispMap, texCoords.xy).r * scale + bias);
}

float random(vec3 seed, int i) {
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float varianceShadow(ShadowMap shadow, vec2 uv, float compare) {
	vec2 moments = texture(shadow.shadowMap, uv).xy;
	return chebychev(moments, compare,shadow.reduceLightBleedingAmount,  shadow.minVariance);

}

float texture2DCompare(ShadowMap shadow, vec2 uv, float compare) {
	float depth = texture(shadow.shadowMap, uv).x;
	return step(compare, depth);
}

float texture2DShadowLerp(ShadowMap shadow, vec2 uv, float compare) {
	vec2 texelSize = vec2(1.0)/shadow.shadowMapSize;
	vec2 f = fract(uv*shadow.shadowMapSize+0.5);
	vec2 centroidUV = floor(uv*shadow.shadowMapSize+0.5)/shadow.shadowMapSize;

	float lb = texture2DCompare(shadow, centroidUV+texelSize*vec2(0.0, 0.0), compare);
	float lt = texture2DCompare(shadow, centroidUV+texelSize*vec2(0.0, 1.0), compare);
	float rb = texture2DCompare(shadow, centroidUV+texelSize*vec2(1.0, 0.0), compare);
	float rt = texture2DCompare(shadow, centroidUV+texelSize*vec2(1.0, 1.0), compare);
	float a = mix(lb, lt, f.y);
	float b = mix(rb, rt, f.y);
	float c = mix(a, b, f.x);
	return c;
}

float PCF(ShadowMap shadow, vec2 uv, float compare) {
	float result = 0.0;
	for(int x=-2; x<=2; x++) {
		for(int y=-2; y<=2; y++) {
			vec2 off = vec2(x,y)/shadow.shadowMapSize;
			result += texture2DCompare(shadow, uv+off, compare);
		}
	}
	return result/25.0;
}

float PCFLinearInterpolation(ShadowMap shadow, vec2 uv, float compare) {
	float result = 0.0;
	for(int x=-1; x<=1; x++) {
		for(int y=-1; y<=1; y++) {
			vec2 off = vec2(x,y)/shadow.shadowMapSize;
			result += texture2DShadowLerp(shadow, uv+off, compare);
		}
	}
	return result/9.0;
}


bool inRange(float val, float offset) {
	return ((val-offset) >= 0.0) && ((val+offset) <= 1.0);
}

float calculateVisibility(ShadowMap shadow, float ndotl, mat4 depthBiasMatrix, vec4 worldPos) {
	float visibility = 1.0;
	//
	// Do the shadow mapping here.
	//
	vec4 shadowUV =  depthBiasMatrix * worldPos;

	float cosTheta = clamp(ndotl, 0,1);
	float bias = 0.006*tan(acos(cosTheta));
	bias = clamp(bias, 0,0.01);


	if(inRange(shadowUV.x, 0.002) && inRange(shadowUV.y, 0.003) && inRange(shadowUV.z, bias)) {
	//	visibility = texture2DCompare(shadow, shadowUV.xy, shadowUV.z-bias);										// Basic Shadow Mapping.
		visibility = varianceShadow(shadow, shadowUV.xy, shadowUV.z);
	//visibility = texture2DShadowLerp(shadow, shadowUV.xy, shadowUV.z-bias); //  Interpolation.
	//visibility = PCF(shadow, shadowUV.xy, shadowUV.z-bias);									// PCF.
	//visibility = PCFLinearInterpolation(shadow, shadowUV.xy, shadowUV.z - bias);
	}

	return clamp(visibility, 0.3, 1.0);
}
