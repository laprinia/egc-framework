#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 frag_color;

void main()
{
	// TODO: compute world space vectors
	vec3 world_pos = (Model * vec4(v_position,1)).xyz;
	vec3 world_normal = normalize( mat3(Model) * v_normal);

	// TODO: define ambient light component
	float ambientala = 0.25;

	vec3 L= normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L+V);
	vec3 R = reflect(-L, world_normal);

	// TODO: compute diffuse light component
	//float diffuse_light = 0;
	float difuza =max(dot(world_normal,L),0);

	// TODO: compute specular light component
	
	float speculara=pow(max(dot(world_normal, H), 0), material_shininess);
	float specularaCalc=difuza>0.0f?speculara:0;

	// TODO: compute light
	float d	= distance(world_pos,light_position);
	float factorAtenuare= 1/(1 + d * 0.2 + pow(d,2) * 0.5);
	// TODO: send color light output to fragment shader
	vec3 out_color= object_color* ambientala + (difuza * vec3(0.5f,0,1) + specularaCalc * material_ks) * factorAtenuare;
	frag_color=out_color;
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
