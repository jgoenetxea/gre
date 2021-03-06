#version 330 core
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
  
  void main(){
     vec2 r = gl_FragCoord.xy / iResolution.xy;
     gl_FragColor.r = r.x;
     gl_FragColor.g = 0.0;
     gl_FragColor.b = r.y;
     gl_FragColor.a = 1.0;
  }

