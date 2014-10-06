// -----------------------------------------------------------------------------
// Microfacet Specular BRDF (Cook-Torrance)
// No optimization at all. At the moment just to test.
//
// -----------------------------------------------------------------------------

#define M_PI 3.1415926535897932384626433832795

//
// Normal Distribution Functions (NDF)
//

// DBlinn(m)=1πα2(n⋅m)(2α2−2)
float d_blinnPhong(float alpha, vec3 n, vec3 m) {
	float term = (1.0/(M_PI*alpha*alpha)) * pow(dot(n,m), (2.0/alpha*alpha - 2));
	return term;
}

// DBeckmann(m)=1πα2(n⋅m)4exp((n⋅m)2−1α2(n⋅m)2)
float d_beckmann(float alpha, vec3 n, vec3 m) {
	float term = 1.0/((M_PI*alpha*alpha)*pow(dot(n,m),4))*exp((pow(dot(n,m), 2) - 1.0)/(alpha*alpha * pow(dot(n,m), 2)));
	return term;
}

// Trowbridge-Reitz DGGX(m)=α2π((n⋅m)2(α2−1)+1)2
float d_ggx(float alpha, float ndoth) {
	float a2 		= alpha * alpha;
	float t			= (ndoth*a2 - ndoth)*ndoth + 1;
	return (a2 / (M_PI * t*t));
}

// DGGXaniso(m)=1/παxαy * 1/((x⋅m)2α2x+(y⋅m)2α2y+(n⋅m)2)2
float d_aniso_ggx(float alpha_x, float alpha_y, float ndoth, vec3 h, vec3 x, vec3 y) {
	float axy 		= alpha_x * alpha_y;
	float xdoth		= max(0.0f, dot(x,h));
	float ydoth		= max(0.0f, dot(y,h));
	float t				= (xdoth * xdoth) / (alpha_x*alpha_x) + (ydoth*ydoth) / (alpha_y*alpha_y) + ndoth*ndoth;
	return (1.0/ (M_PI*axy*t*t));
}






//
// Geometric Shadowing
//

// GImplicit(l,v,h)=(n⋅l)(n⋅v)
float g_Implicit(float alpha, float ndotv, float ndotl) {
	return ndotl*ndotv;
}

// GNeumann(l,v,h)=(n⋅l)(n⋅v)max(n⋅l,n⋅v)
float g_Neumann(float alpha, float ndotv, float ndotl) {
	return ndotl*ndotv/max(ndotv, ndotl);
}

// GCook−Torrance(l,v,h)=min(1,2(n⋅h)(n⋅v)v⋅h,2(n⋅h)(n⋅l)v⋅h)
float g_CookTorrance(float alpha, float ndotv, float ndotl, float ndoth, float vdoth) {
	float m = min(2.0*ndoth*ndotv/vdoth, 2.0*ndoth*ndotl/vdoth);
	return min(1.0, m);
}

// GKelemen(l,v,h)=(n⋅l)(n⋅v)(v⋅h)2
float g_Kelemen(float alpha, float ndotv, float ndotl, float ndoth) {
	return (ndotl*ndotv/(ndoth*ndoth));
}

// Beckmann
float g_BeckmannF(float alpha, float ndotm) {
	float c = ndotm/(alpha*sqrt(1.0 - ndotm*ndotm));
	if(c < 1.6) {
		return (3.535*c + 2.181*pow(c,2))/(1.0 + 2.276*c + 2.577*pow(c,2));
	}
	return 1.0;
}

float g_Beckmann(float alpha, float ndotv, float ndotl) {
	return g_BeckmannF(alpha, ndotv) * g_BeckmannF(alpha, ndotl);
}


// Schlick-Beckmann
float g_SchlickBeckmann(float alpha, float ndotv, float ndotl) {
	float k = alpha*sqrt(2.0/M_PI);
	float t1 = (ndotv/(ndotv*(1.0 - k) + k));
	float t2 = (ndotl/(ndotl*(1.0 - k) + k));
	return t1*t2;
}






//
// Fresnel
//

// None
vec3 f_None(vec3 f0, float vdoth) {
	return f0;
}

// Schlick(v,h)=F0+(1−F0)(1−(v⋅h))5
vec3 f_Schlick(vec3 f0, float vdoth) {
	float t = pow((1.0 - vdoth), 5);
	return(f0 + (1.0 - f0)*t);
}

