#define PI 3.14159265359
#define TWOPI 6.28318530718

// Description: FUNCTION PLOTTING
//
// It is always useful to see the plots of functions on cartesian
// coordinate system, to understand what they are doing precisely
//
// Let's plot some 1D functions!
//
// If y value is a function f of x value, the expression of their
// relation is: y = f(x)
// in other words, the plot of a function is all points
// that satisfy the expression: y-f(x)=0
// this set has 0 thickness, and can't be seen.
// Instead use the set of (x,y) that satisfy: -d < y-f(x) < d
// in other words abs(y-f(x)) < d
// where d is the thickness. (the thickness in in y direction)
// Because of the properties of absolute function, the condition
// abs(y-f(x)) < d is equivalent to the condition:
// abs(f(x) - y) < d
// We'll use this last one for function plotting. (in the previous one
// we have to negate the function that we want to plot)

////////////////////////////////////////////
//         Function declaration
////////////////////////////////////////////
void drawAxis( inout vec3 pixel, in vec2 r, in vec3 axesColor, in vec3 gridColor );


////////////////////////////////////////////
//         Function implementation
////////////////////////////////////////////
/*
* This function draw the axis
*/
void drawAxis( inout vec3 pixel, in vec2 r, in vec3 axesColor, in vec3 gridColor )
{       
    // Draw the grid lines
    // we used "const" because loop variables can only be manipulated
    // by constant expressions.
    const float tickWidth = 0.1;
    for(float i=-2.0; i<2.0; i+=tickWidth)
    {
        // "i" is the line coordinate.
        if(abs(r.x - i)<0.004) pixel = gridColor;
        if(abs(r.y - i)<0.004) pixel = gridColor;
    }
    // Draw the axes
    if( abs(r.x)<0.006 ) pixel = axesColor;
    if( abs(r.y)<0.007 ) pixel = axesColor;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Basic variables
    vec3 blackColor = vec3(0.0);
    vec3 grayColor = vec3(0.5);
    vec3 whiteColor = vec3(1.0);
    vec3 redColor = vec3(1.0, 0.0, 0.0);
    vec3 greenColor = vec3(0.0, 1.0, 0.0);
    vec3 blueColor = vec3(0.0, 0.0, 1.0);
   
    // Operation variables
    vec2 r = 2.0 * vec2( fragCoord.xy - 0.5*iResolution.xy ) / iResolution.y ; // x,y [-1.0, 1.0] with aspect Ratio
    vec2 rxy = 2.0 * vec2( fragCoord.xy - 0.5*iResolution.xy ) / iResolution.y ; // x,y [-1.0, 1.0]
    vec2 p = vec2(fragCoord.xy / iResolution.xy);    // Point this shader iteration
   
    // Output variables
    vec3 pixel = vec3(1.0);
   
    drawAxis(pixel, r, blueColor, grayColor);
   
    // Draw functions
    float x = r.x;
    float y = r.y;
   
    // Final draw
    fragColor = vec4(pixel, 1.0);
}
