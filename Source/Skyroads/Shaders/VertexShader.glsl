#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

out vec3 frag_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 color;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;


void main() {
vec3 world_pos = (Model * vec4(v_position,1)).xyz;
vec3 world_normal = normalize( mat3(Model) * v_normal);

float ambientala=0.8;

vec3 L= normalize(light_position - world_pos);
vec3 V = normalize(eye_position - world_pos);
vec3 H = normalize(L+V);
vec3 R = reflect(L, world_normal);


float difuza =max(dot(world_normal,L),0);

float speculara=pow(max(dot(world_normal, H), 0), material_shininess);
float specularaCalc=difuza>0.0?speculara:0;


float d	= distance(world_pos,light_position);
float factorAtenuare= 1/(1 + d * 0.1);

vec3 out_color= color* ambientala + (difuza * vec3(0.5,0.5,0.5) + specularaCalc * material_ks) * factorAtenuare;
frag_color=out_color;

gl_Position= Projection * View * Model * vec4(v_position,1.0);
}