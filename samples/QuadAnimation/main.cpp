#include "rectGenerator.h"

int main( void )
{
    RectGenerator rg;
    rg.init("Cosas", 1024, 1024, key_callback, error_callback);

    rg.initScene();
    rg.generateQuads();

    while(!rg.updateScene())	// Check if the ESC key was pressed or the window was closed
    {
       usleep(100);
       rg.render();
    }

    rg.close();

    return 0;
}
