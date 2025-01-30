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
    Doozy::Load();
    Steve::Load();


    //Camera
    GameObj* CameraOBJ = GameObj::Create();
        CameraOBJ->Transform.wPosition = glm::vec3(0, 0, -3);
        B_Camera* Camera_Bhav = CameraOBJ->AddComponent(new B_Camera);

    //Player
    GameObj* PlayerOBJ = nullptr;
        Player* Player_Bhav = nullptr;

    //Scene
    GameObj* SceneOBJ = GameObj::Create();
        Model_Static Model_Racetrack("Assets/Models/Racetrack/Racetrack.obj");
        SceneOBJ->Transform.wPosition = glm::vec3(0, 0, 0);
        SceneOBJ->Transform.wRotation = glm::vec3(0, 0, 0);
        SceneOBJ->Transform.wScale = glm::vec3(10.0f, 10.0f, 10.0f);



 
    float SpawnTimer_Gun = 0;
    float SpawnTimer_Gun_MAX = 4;



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

 
        Shader& Shader4Static = renderer.m_basicShader; 
        Shader4Static.use();
        Shader4Static.setMat4("projection", Camera_Bhav->GetProjectionMatrix());
        Shader4Static.setMat4("view", Camera_Bhav->GetViewMatrix());

        for (GameObj* EACH : sGameObjs) {
            EACH->Render(renderer);
        }


        Shader4Static.use();
        Shader4Static.setMat4("model", SceneOBJ->Transform.modelMatrix);
        Model_Racetrack.Draw(Shader4Static);


        app.SwapBuffers();
         

        /////////////////////// TEMPORARY WORKSPACE  ///////////

        if (sGetComponent_OfClass(Player_Bhav)) {
                float LerpSpeed = 16 * Time.Deltatime;
                CameraOBJ->Transform.wPosition = B_lerpVec3(CameraOBJ->Transform.wPosition, Player_Bhav->CamSocket->Transform.getWorldPosition(), LerpSpeed);
                Camera_Bhav->m_lookAt = B_lerpVec3(Camera_Bhav->m_lookAt, Player_Bhav->CamLookat->Transform.getWorldPosition(), LerpSpeed);     
        }
        else
        {
            PlayerOBJ = GameObj::Create();
            PlayerOBJ->Transform.wPosition = glm::vec3(0, 0, 0);
            Player_Bhav = PlayerOBJ->AddComponent(new Player);

            TargetPLR = Player_Bhav;
        }

        SpawnTimer_Gun += Time.Deltatime;
        if (SpawnTimer_Gun> SpawnTimer_Gun_MAX && GunCount <5) {
            SpawnTimer_Gun = 0;
            GameObj* GUN = GameObj::Create();
            GUN->Transform.wPosition = glm::vec3(B_frand(-10,10), 0, B_frand(-10, 10));
            GUN->AddComponent(new Gun);
        }

        EnemyMax += Time.Deltatime*0.025;
        if (EnemyCount< EnemyMax) {
            GameObj* Enemy01 = GameObj::Create();
            Enemy01->Transform.wPosition = glm::vec3(B_frand(-10, 10), 0, B_frand(-10, 10));
            Enemy01->AddComponent(new Enemy);
        }
      
    }
}