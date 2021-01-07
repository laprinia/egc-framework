#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

out vec3 frag_color;
out vec3 world_position;
out vec3 world_normal;


uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 color;

void main() {


vec3 w_position=(Model * vec4(v_position,1)).xyz;
vec3 w_normal= normalize( mat3(Model) * v_normal);
world_position =w_position;
world_normal = w_normal;
frag_color=color;

gl_Position= Projection * View * Model * vec4(v_position,1.0);
}