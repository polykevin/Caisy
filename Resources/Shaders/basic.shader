#shader vertex
#version 330 core
layout (location = 0) in vec2 vertex;
out vec2 a_uv;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(vertex.xy, 0.0, 1.0);
    a_uv = vertex.xy;
}  

#shader fragment
#version 330 core

in vec2 a_uv;
out vec4 color;

uniform vec3 Color;
uniform float Time;

void main()
{    
    vec2 dimensions = vec2(1280.0, 720.0);
    vec2 coords = a_uv * dimensions;
    float radius = 50.0;
     if (length(coords - vec2(0)) < radius || 
         length(coords - vec2(0, dimensions.y)) < radius || 
         length(coords - vec2(dimensions.x, 0)) < radius ||
         length(coords - dimensions) < radius) { 
         discard; 
     } 
     float test = abs(sin(Time))*3.0; 
     color = vec4(Color, test);  
}  
