#version 130

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 fragColor;

// Values that stay constant for the whole mesh.
uniform sampler2D iChannel0;
uniform vec3 iResolution;		// Viewport resolution (in pixels)
uniform float iGlobalTime;		// shader playback time (in seconds)
uniform vec4 iMouse;			// mouse pixel coords. xy: current (if MLB down), zw: click
uniform vec4 iDate;				// (year, month, day, time in seconds)
uniform float iSampleRate;		// sound sample rate (i.e., 44100)

uniform vec3 iColour;

void main(){

	// Output color = color of the texture at the specified UV
	fragColor = iColour;
}
