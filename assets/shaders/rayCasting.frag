#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vColor;

// Uniforms 
uniform sampler3D volTexID;
uniform sampler2D renderedTexture;
uniform vec2 windowSize;

// Fragment Color
out vec4 fragColor;

void main()
{
	vec4 Color = vec4(0.0f,0.0f,0.0f,1.0f);
	vec2 coord = (gl_FragCoord.xy-windowSize.y) / windowSize;
	vec3 rayDir = vec3(texture(renderedTexture,coord).xyz - vColor);
	vec3 rayIn = vColor;
	float D = length(rayDir);
	rayDir = normalize(rayDir);

	for(float i=0.0f;i<D;i+=1.0f/256){
	// Ai y Ci se consultan en la TF 
		Color.rgb += texture(volTexID,rayIn).r * vec3(texture(volTexID,rayIn).r) * Color.a;
		Color.a *= 1 - texture(volTexID,rayIn).r;
		if(1-Color.a >= 0.99f) break;
		rayIn += rayDir*1.0f/256;
	}
	Color.a = 1.0f;
	fragColor = Color;
}
