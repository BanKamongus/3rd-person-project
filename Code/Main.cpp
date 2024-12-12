#include "Application.h"
#include "Renderer.h"

#include "B_Player.h"
#include "B_Camera.h"

#include "BanKEngine.h"

 
int main()
{
    Application app;
    Renderer renderer;



    //Camera
    GameObj* CameraOBJ = GameObj::Create();
        CameraOBJ->Transform.wPosition = glm::vec3(0, 0, -3);
        B_Camera* Camera_Bhav = CameraOBJ->AddComponent(new B_Camera);

    //Player
    GameObj* PlayerOBJ = GameObj::Create();
        PlayerOBJ->Transform.wPosition = glm::vec3(0, 0, 0);
        Player* Player_Bhav = PlayerOBJ->AddComponent(new Player);

    //Scene
    GameObj* SceneOBJ = GameObj::Create();
        Model Model_Racetrack("Assets/Models/Racetrack/Racetrack.obj");
        SceneOBJ->Transform.wPosition = glm::vec3(0, -10, 0);
        SceneOBJ->Transform.wRotation = glm::vec3(0, 0, 0);
        SceneOBJ->Transform.wScale = glm::vec3(30.0f, 30.0f, 30.0f) * 2.5f;


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

        for (GameObj* EACH : sGameObjs) {
            EACH->Render(renderer.m_animShader);
        }


        Shader& Shader4Scene = renderer.m_pbrShader; 
        Shader4Scene.use(); 
        Shader4Scene.setMat4("projection", Camera_Bhav->GetProjectionMatrix());
        Shader4Scene.setMat4("view", Camera_Bhav->GetViewMatrix());
        Shader4Scene.setMat4("model", SceneOBJ->Transform.modelMatrix);
        //Shader4Scene.setMat4("normalMatrix", glm::transpose(glm::inverse(glm::mat3(SceneOBJ->Transform.modelMatrix))));
        //Model_Racetrack.Draw(Shader4Scene);



        app.SwapBuffers();
         

        /////////////////////// TEMPORARY WORKSPACE  ///////////
        float LerpSpeed = 8 * Time.Deltatime;
        CameraOBJ->Transform.wPosition = B_lerpVec3(CameraOBJ->Transform.wPosition, Player_Bhav->CamSocket->Transform.getWorldPosition(), LerpSpeed);
        Camera_Bhav->m_lookAt = B_lerpVec3(Camera_Bhav->m_lookAt, Player_Bhav->CamLookat->Transform.getWorldPosition(), LerpSpeed);
    }
}