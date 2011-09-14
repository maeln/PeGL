#version 330

smooth in vec4 light_comp;
out vec4 outputColor;

void main()
{
    outputColor = light_comp;
}
