#version 450 core

#define FRONT 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3
#define TOP 4

out vec4 outColor;

in vec2 uv;

layout(binding=1) uniform usampler2DArray hemicube;

vec2 getRange(float coord){
	vec2 ranges[] =  {vec2(.0f, .25f), vec2(.25f,.75f), vec2(.75f,1.0f)};
	for (uint i = 0; i < 3; i++){
		if (ranges[i].x <= coord && ranges[i].y > coord){
			return ranges[i];
		}
	}
}

float clampAdv(float v, vec2 limits){
	return (v - limits.x) / (limits.y - limits.x);
}


int getFace(float x, float y){
	if (x < .25f){
		if (y < .25f){
			discard;
		} else if (y < .75f){
			return LEFT;
		} else {
			discard;
		}
	} else if (x < .75f){
		if (y < .25f){
			return BOTTOM;
		} else if (y < .75f){
			return FRONT;
		} else {
			return TOP;
		}
	} else {
		if (y < .25f){
			discard;
		} else if (y < .75f){
			return RIGHT;
		} else {
			discard;
		}
	}
}

vec2 transform(vec2 uv, uint face){
	switch (face){
		case RIGHT: {
			return vec2(uv.y, 1.0f - uv.x);
		}
		break;
		case BOTTOM: {
			return vec2(uv.x,uv.y + .5f);
		}
		break;
		case LEFT: {
			return vec2(1.0f - uv.y,uv.x + .5f);
		}
		break;
		case TOP: {
			return vec2(1.0f - uv.x, 1.0f - uv.y);
		}
		break;
		default: {
			return uv;
		}
		break;
	}
}

vec3 getCoords(vec2 uv){
	vec2 xy;
	uint z;
	vec2 rangeX = getRange(uv.x);
	vec2 rangeY = getRange(uv.y);
	xy = vec2((uv.x - rangeX.x) * 2.0f, (uv.y - rangeY.x) * 2.0f);
	z = getFace(uv.x, uv.y);
	return vec3(transform(xy,z),float(z));
}

void main() {
	uint id = texture(hemicube,getCoords(uv)).r;
	float r = float( (id >> 16) & uint(0xFF)) / 255.0f;
	float g = float( (id >> 8)  & uint(0xFF)) / 255.0f;
	float b = float(  id        & uint(0xFF)) / 255.0f;
	vec3 color = vec3(r,g,b);
	outColor = vec4(color, 1.0f);
}