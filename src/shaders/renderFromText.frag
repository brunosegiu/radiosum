#version 450 core

out vec4 outColor;

in vec2 uv;

layout(binding=1) uniform usampler2DArray hemicube;

vec3 getCoords(vec2 uv){
	float x,y,z;
	float ratio = 1.0f / 3.0f;
	if (uv.y < .5f){
		y = uv.y * 2.0f;
		if (uv.x < ratio){
			x = 2.0f;
			y = 2.0f;
			z = 10.0f;
		} else if (uv.x <  ratio * 2.0f){
			x = (uv.x - ratio) * 3.0f;
			z= 2.0f;
		}else{
			x = (uv.x - ratio * 2.0f)* 3.0f;
			z = 4.0f;
		}
	} else {
		y = (uv.y -.5f) * 2.0f;
		if (uv.x < ratio){
			x = uv.x * 3.0f;
			z = 3.0f;
		} else if (uv.x < ratio * 2.0f){
			x = (uv.x - ratio) * 3.0f;
			z = 0.0f;
		}else{
			x = (uv.x - ratio * 2.0f)* 3.0f;
			z = 1.0f;
		}
	}
	return vec3(x,y,z);
}

void main() {
	uint id = texture(hemicube,getCoords(uv)).r;
	float r = float( (id >> 16) & uint(0xFF)) / 255.0f;
	float g = float( (id >> 8)  & uint(0xFF)) / 255.0f;
	float b = float(  id        & uint(0xFF)) / 255.0f;
	vec3 color = vec3(r,g,b);
	outColor = vec4(color, 1.0f);
}