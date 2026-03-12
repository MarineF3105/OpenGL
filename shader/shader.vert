#version 330 core
layout(location = 0) in vec3 Pos;

uniform float xMove;
uniform vec3 Col;

void main()
{
 gl_Position = vec4(Pos.x+xMove,Pos.y,Pos.z,1.0);
}
