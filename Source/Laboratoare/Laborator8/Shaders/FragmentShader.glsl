#version 330

// TODO: get color value from vertex shader
in vec3 world_pos;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform bool isSpotlight;
uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

vec3 color;

void main()
{
	// TODO: define ambient light component
	float ambientala = 0.25;
	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L+V);
	vec3 R = reflect(-L, world_normal);

	// TODO: compute diffuse light component
	float difuza = max(dot(world_normal,L),0);

	// TODO: compute specular light component
	float speculara=pow(max(dot(world_normal, H), 0), material_shininess);
	float specularaCalc=difuza>0.0f?speculara:0;

	// TODO: compute light
	float d	= distance(world_pos,light_position);
	float factorAtenuare= 1/(1 + d * 0.2 + pow(d,2) * 0.5);

	//SPOTLIGHT

	if(isSpotlight)
	{
	float spot_light = dot(-L, light_direction);
	float cutoff = radians(30.0f);
	float spot_light_limit = cos(cutoff);

	float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
	float light_att_factor = pow(linear_att, 2);
	
	
	if (spot_light > spot_light_limit) 
		{
		color=object_color* ambientala + (difuza * material_kd + specularaCalc * material_ks) * light_att_factor;
		}
	}
	else
	{
	   color=object_color* ambientala + (difuza *material_kd + specularaCalc * material_ks) * factorAtenuare;
	}

	// TODO: write pixel out color
	out_color = vec4(color,1);

}