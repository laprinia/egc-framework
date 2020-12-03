#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int hasSecondTexture;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	
	vec4 color = texture2D(texture_1, texcoord);
	vec4 color2 = texture2D(texture_2, texcoord);
	if(color.a<0.5f) {
	discard;
	}
	if(hasSecondTexture==1){
	out_color = mix(color, color2, 0.5f);
	}else {
	out_color=color;
	}
	
	
}