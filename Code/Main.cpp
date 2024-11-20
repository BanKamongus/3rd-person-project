#include "Application.h"
#include "Renderer.h"

#include "B_Player.h"
#include "B_Camera.h"

#include "BanKEngine.h"


int main()
{
    Application app;
    Renderer renderer;




    GameObj* CameraOBJ = GameObj::Create();
        CameraOBJ->Transform.wPosition = glm::vec3(0, 0, -3);
        B_Camera* Camera_Bhav = CameraOBJ->AddComponent(new B_Camera);

    GameObj* PlayerOBJ = GameObj::Create();
        PlayerOBJ->Transform.wPosition = glm::vec3(0, 0, 0);
        Player* Player_Bhav = PlayerOBJ->AddComponent(new Player);


    BanKEngine::Init();
    while (!app.WindowShouldClose())
    {
        BanKEngine::All_Update();
        ///////////////////////////////////


        app.ProcessInput();


        // Render
        renderer.Clear();
        renderer.m_animShader.use();

        renderer.m_animShader.setMat4("projection", Camera_Bhav->GetProjectionMatrix());
        renderer.m_animShader.setMat4("view", Camera_Bhav->GetViewMatrix());

        Player_Bhav->Render(renderer.m_animShader);

        app.SwapBuffers();



        /////////////////////// TEMPORARY WORKSPACE  ///////////
        float LerpSpeed = 5 * Time.Deltatime;
        //CameraOBJ->Transform.wPosition = B_lerpVec3(CameraOBJ->Transform.wPosition, Player_Bhav->CamSocket->Transform.getWorldPosition(), LerpSpeed);
        Camera_Bhav->m_lookAt = B_lerpVec3(Camera_Bhav->m_lookAt, PlayerOBJ->Transform.wPosition, LerpSpeed);
    }
}