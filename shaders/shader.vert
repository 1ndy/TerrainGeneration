#version 330
#define PI 3.14159265358979323846

layout (location = 0) in vec3 pos;

out float vHeight;
out vec4 vColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float height_scalar;
uniform float noise_scalar;
uniform float x_offset;
uniform float z_offset;
uniform float steepness_exp;
uniform int resolution;

int screenWidth = 10;

float rand(vec2 c){
	return fract(sin(dot(c.xy ,vec2(84.9372,37.2378))) * 74358.5354);
}

float noise(vec2 p, float freq ){
	float unit = screenWidth/freq;
	vec2 ij = floor(p/unit);
	vec2 xy = mod(p,unit)/unit;
	//xy = 3.*xy*xy-2.*xy*xy*xy;
	xy = .5*(1.-cos(PI*xy));
	float a = rand((ij+vec2(0.,0.)));
	float b = rand((ij+vec2(1.,0.)));
	float c = rand((ij+vec2(0.,1.)));
	float d = rand((ij+vec2(1.,1.)));
	float x1 = mix(a, b, xy.x);
	float x2 = mix(c, d, xy.x);
	return mix(x1, x2, xy.y);
}

float pNoise(vec2 p, int res, float f){
	float persistance = .5;
	float n = 0.;
	float normK = 0.;
	float amp = 1.;
	int iCount = 0;
	for (int i = 0; i<50; i++){
		n+=amp*noise(p, f);
		f*=2.;
		normK+=amp;
		amp*=persistance;
		if (iCount == res) break;
		iCount++;
	}
	float nf = n/normK;
	return nf*nf*nf*nf;
}

 void main() {
	vec2 noise_pos = vec2(pos.x + noise_scalar * x_offset, pos.z + noise_scalar * z_offset);
    vHeight = height_scalar * pNoise(noise_pos, resolution, noise_scalar);
    vHeight = pow(vHeight, steepness_exp);
    gl_Position = projection * view * model * vec4(pos.x, vHeight, pos.z, 1.0);
    if(0.0f <= vHeight && vHeight <= 0.5f) {
        //water
        vColor = vec4(0.1f, 0.4f, 0.9f, 1.0f);
    } else if(0.5f < vHeight && vHeight <= 1.5f) {
        //beach
        vColor = vec4(0.6f, 0.6f, 0.3f, 1.0f);
    } else if(1.5f < vHeight && vHeight <= 24.0f) {
        //forest
        vColor = vec4(0.1f, 0.6f, 0.2f, 1.0f);
    } else if(24.0f < vHeight && vHeight <= 32.0f) {
        //desert
        vColor = vec4(0.7f, 0.7f, 0.4f, 1.0f);
    } else if(32.0f < vHeight && vHeight <= 50.0f) {
        //rocks
        vColor = vec4(0.35f, 0.35f, 0.35f, 1.0f);
    } else {
        //snow
        vColor = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    }
 }