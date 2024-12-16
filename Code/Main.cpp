#include "Application.h"
#include "Renderer.h"

#include "Model_DAEstatic.h"
#include <learnopengl/model.h>

#include "B_Player.h"
#include "B_Camera.h"
#include "B_Enemy.h"

#include "BanKEngine.h"

 
int main()
{
    Application app;
    Renderer renderer;
    glfwSwapInterval(0);



    //Camera
    GameObj* CameraOBJ = GameObj::Create();
        CameraOBJ->Transform.wPosition = glm::vec3(0, 0, -3);
        B_Camera* Camera_Bhav = CameraOBJ->AddComponent(new B_Camera);

    //Player
    GameObj* PlayerOBJ = GameObj::Create();
        PlayerOBJ->Transform.wPosition = glm::vec3(0, 0, 0);
        Player* Player_Bhav = PlayerOBJ->AddComponent(new Player);
        PlayerOBJ->AddComponent(new Collider_Capsule);

    //Scene
    GameObj* SceneOBJ = GameObj::Create();
        //Model_Static Model_Racetrack("Assets/Models/AK47/OBJ/ak7finished.obj");
        SceneOBJ->Transform.wPosition = glm::vec3(30, 0, 30);
        SceneOBJ->Transform.wRotation = glm::vec3(0, 0, 0);
        SceneOBJ->Transform.wScale = glm::vec3(10.0f, 10.0f, 10.0f);



     GameObj* Enemy01 = GameObj::Create();
        Enemy01->Transform.wPosition = glm::vec3(1, 0, 0);
        Enemy01->AddComponent(new Enemy);
        Enemy01->AddComponent(new Collider_Capsule);

     //GameObj* Enemy02 = GameObj::Create();
     //   Enemy02->Transform.wPosition = glm::vec3(-1, 0, 0);
     //   Enemy02->AddComponent(new Enemy);
     //   Enemy02->AddComponent(new Collider_Capsule);
     //    




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


        SceneOBJ->Transform.wPosition = glm::vec3(0); 
        Shader& Shader4Static = renderer.m_basicShader; 
        Shader4Static.use();
        Shader4Static.setMat4("projection", Camera_Bhav->GetProjectionMatrix());
        Shader4Static.setMat4("view", Camera_Bhav->GetViewMatrix());

        for (GameObj* EACH : sGameObjs) {
            EACH->Render(renderer);
        }




        app.SwapBuffers();
         

        /////////////////////// TEMPORARY WORKSPACE  ///////////
        float LerpSpeed = 16 * Time.Deltatime;
        CameraOBJ->Transform.wPosition = B_lerpVec3(CameraOBJ->Transform.wPosition, Player_Bhav->CamSocket->Transform.getWorldPosition(), LerpSpeed);
        Camera_Bhav->m_lookAt = B_lerpVec3(Camera_Bhav->m_lookAt, Player_Bhav->CamLookat->Transform.getWorldPosition(), LerpSpeed);
    }
}