#include "Application.h"
#include "Renderer.h"
#include "Player.h"

int main()
{
    Application app;
    Renderer renderer;

    Player player;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!app.WindowShouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        app.ProcessInput();

        // Update game logic
        player.Update(deltaTime);


        // Render
        renderer.Clear();
        renderer.m_animShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)1200 / (float)800, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0.0), glm::vec3(0, 1, 0));

        renderer.m_animShader.setMat4("projection", projection);
        renderer.m_animShader.setMat4("view", view);

        player.Render(renderer.m_animShader);

        app.SwapBuffers();
    }
}