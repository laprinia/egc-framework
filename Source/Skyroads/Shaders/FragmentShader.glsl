#version 330

in vec3 frag_color;
in vec3 world_position;
in vec3 world_normal;

layout(location = 0) out vec4 out_color;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform bool isPlayer;


void main()
{

float ambientala=0.8;

vec3 L= normalize(light_position - world_position);
vec3 V = normalize(eye_position - world_position);
vec3 H = normalize(L+V);
vec3 R = reflect(L, world_normal);


float difuza =max(dot(world_normal,L),0);

float speculara=pow(max(dot(world_normal, H), 0), material_shininess);
float specularaCalc=difuza>0.0?speculara:0;


float d	= distance(world_position,light_position);
float factorAtenuare= 1/(1 + d * 0.06);

vec3 res_color= frag_color* ambientala + (difuza * vec3(0.5,0.463,0.330) + specularaCalc * material_ks) * factorAtenuare;
out_color =isPlayer?vec4(world_normal,1): vec4(res_color,1);
}