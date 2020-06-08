#version 330

in float height;
in vec4 color;
in vec3 normal;

out vec4 fcolor;


struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    vec3 direction;
};


uniform DirectionalLight dirlight;


vec4 calcLightByDirection(DirectionalLight light) {
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    float diffuseFactor = max(dot(normalize(normal), -normalize(light.direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);

    return ambientColor + diffuseColor;
}


void main() {
    if(0.0f <= height && height <= 0.5f) {
        //water
        fcolor = vec4(0.1f, 0.4f, 0.9f, 1.0f);
    } else if(0.5f < height && height <= 1.5f) {
        //beach
        fcolor = vec4(0.6f, 0.6f, 0.3f, 1.0f);
    } else if(1.5f < height && height <= 24.0f) {
        //forest
        fcolor = vec4(0.1f, 0.6f, 0.2f, 1.0f);
    } else if(24.0f < height && height <= 32.0f) {
        //desert
        fcolor = color;//vec4(0.7f, 0.7f, 0.4f, 1.0f);
    } else if(32.0f < height && height <= 50.0f) {
        //rocks
        fcolor = color;//vec4(0.35f, 0.35f, 0.35f, 1.0f);
    } else {
        //snow
        fcolor = color;
    }
    fcolor *= calcLightByDirection(dirlight);
}