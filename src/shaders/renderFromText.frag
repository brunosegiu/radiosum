#version 430 core

out vec4 outColor;

in vec2 uv;

uniform usampler2DArray hemicube;

vec3 getLayer(vec2 uv){
	float x,y,z;
	if (uv.y < .5f){
		y = uv.y * 1.0f;
		if (uv.x < .33f){
			x = uv.x * 3.0f;
			z = 0.0f;
		} else if (uv.x <.66f){
			x = (uv.x - .33f) * 3.0f;
			z=  1.0f;
		}else{
			x = (uv.x - .66f)* 3.0f;
			z = 2.0f;
		}
	} else {
		y = (uv.y -.5f) * 1.0f;
		if (uv.x < .33f){
			x = uv.x * 3.0f;
			z = 3.0f;
		} else if (uv.x <.66f){
			x = (uv.x -.33f) * 3.0f;
			z = 4.0f;
		}else{
			x = 2.0f;
			z = 0.0f;
			y = 2.0f;
		}
	}
	return vec3(x,y,z);
}

void main() {
	uint id = texture(hemicube,getLayer(uv)).r;
	float r = float( (id >> 16) & uint(0xFF)) / 255.0f;
	float g = float( (id >> 8)  & uint(0xFF)) / 255.0f;
	float b = float(  id        & uint(0xFF)) / 255.0f;
	vec3 color = vec3(r,g,b);
	outColor = vec4(color, 1.0f);
}