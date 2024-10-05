#version 330 core

out vec4 FragColor;

in vec3 blockColor;

void main()
{
   //FragColor = vec4(159.0/255.0, 142.0/255.0, 255.0/255.0, 1.0);
   FragColor = vec4(blockColor, 1.0);
}
