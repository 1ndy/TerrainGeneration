#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in float vHeight[];
in vec4 vColor[];

out float height;
out vec4 color;
out vec3 normal;

void main() {


    vec4 vp0 = gl_in[0].gl_Position;
    vec4 vp1 = gl_in[1].gl_Position;
    vec4 vp2 = gl_in[2].gl_Position;
    vec3 v1 = vec3(vp1.x - vp0.x, vp1.y - vp0.y, vp1.z - vp0.z);
    vec3 v2 = vec3(vp2.x - vp0.x, vp2.y - vp0.y, vp2.z - vp0.z);
    normal = normalize(cross(v1, v2));

    gl_Position = gl_in[0].gl_Position;
    height = vHeight[0];
    color = vColor[0];
    EmitVertex();
    
    
    gl_Position = gl_in[1].gl_Position;
    height = vHeight[1];
    color = vColor[1];
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    height = vHeight[2];
    color = vColor[2];
    EmitVertex();

    EndPrimitive();
}