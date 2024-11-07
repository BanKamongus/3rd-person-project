#include "Application.h"
#include "Renderer.h"

int main()
{
    Application app;
    Renderer renderer;

    while (!app.WindowShouldClose())
    {
        app.ProcessInput();

        // Updating game logic


        // Rendering tasks

        app.SwapBuffers();
    }
}