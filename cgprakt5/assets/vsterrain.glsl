#version 400

layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;
uniform vec3 Scaling;

void main()
{
    vec4 newVertexPos = VertexPos;
    newVertexPos.x *= Scaling.x;
    newVertexPos.y *= Scaling.y;
    newVertexPos.z *= Scaling.z;
    
    vec4 newVertexNormal = VertexNormal;
    newVertexNormal.x /= Scaling.x;
    newVertexNormal.y /= Scaling.y;
    newVertexNormal.z /= Scaling.z;
    newVertexNormal = normalize(newVertexNormal);

    Position = (ModelMat * newVertexPos).xyz;
    Normal = (ModelMat * vec4(newVertexNormal.xyz,0)).xyz;
    Texcoord = VertexTexcoord;
    gl_Position = ModelViewProjMat * newVertexPos;
}
