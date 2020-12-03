#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int hasSecondTexture;
uniform float elapsedTime;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
    vec2 newTexCoord;
	// TODO : calculate the out_color using the texture2D() function
	if(elapsedTime >= 0)
	{
	newTexCoord = vec2(texcoord.x + elapsedTime/10, texcoord.y);
	} else {
	newTexCoord = vec2(texcoord.x, texcoord.y);
	}
	
	vec4 color = texture2D(texture_1, newTexCoord);
	vec4 color2 = texture2D(texture_2, newTexCoord);
	if(color.a<0.5f) {
	discard;
	}
	if(hasSecondTexture==1){
	out_color = mix(color, color2, 0.5f);
	}else {
	out_color=color;
	}
	
	
}