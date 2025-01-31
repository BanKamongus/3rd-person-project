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
        Model_Static Model_3rdPersonUE("Assets/Models/UE3rd Person/3rdPerson Visual.obj");
        Model_Static Model_3rdPersonUE_Collider("Assets/Models/UE3rd Person/3rdPerson.obj");
        SceneOBJ->Transform.wPosition = glm::vec3(0, 0, 0);
        SceneOBJ->Transform.wRotation = glm::vec3(0, 0, 0);
        SceneOBJ->Transform.wScale = glm::vec3(1, 1, 1);

        PLR_Raycast_Init(SceneOBJ, SceneOBJ, Model_3rdPersonUE_Collider);

    //GameObj* ModelTestOBJ = GameObj::Create();
    //    Model_Static Model_Racetrack("Assets/Models/Racetrack/Racetrack.obj");
    //    ModelTestOBJ->Transform.wPosition = glm::vec3(0, -5, 0);
    //    ModelTestOBJ->Transform.wRotation = glm::vec3(0, 0, 0);
    //    ModelTestOBJ->Transform.wScale = glm::vec3(0.1, 0.1, 0.1);


 
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
 
        renderer.m_basicShader.use();
        renderer.m_basicShader.setMat4("projection", Camera_Bhav->GetProjectionMatrix());
        renderer.m_basicShader.setMat4("view", Camera_Bhav->GetViewMatrix());

        for (GameObj* EACH : sGameObjs) {
            EACH->Render(renderer);
        }



        /////////////////////// TEMPORARY WORKSPACE  ///////////

        renderer.m_basicShader.use();
        renderer.m_basicShader.setMat4("model", SceneOBJ->Transform.modelMatrix);
        Model_3rdPersonUE.Draw(renderer.m_basicShader);

        //renderer.m_basicShader.use();
        //renderer.m_basicShader.setMat4("model", ModelTestOBJ->Transform.modelMatrix);
        //Model_Racetrack.Draw(renderer.m_basicShader); 
        //Model_Racetrack.Draw(renderer.m_basicShader); 


        if (sGetComponent_OfClass(Player_Bhav)) {
                float LerpSpeed = 16 * Time.Deltatime;
                CameraOBJ->Transform.wPosition = B_lerpVec3(CameraOBJ->Transform.wPosition, Player_Bhav->CamSocket->Transform.getWorldPosition(), LerpSpeed);
                Camera_Bhav->m_lookAt = B_lerpVec3(Camera_Bhav->m_lookAt, Player_Bhav->CamLookat->Transform.getWorldPosition(), LerpSpeed);     

                PLR_Raycast_Update(Player_Bhav->GameObject, Player_Bhav);
        }
        else
        {
            PlayerOBJ = GameObj::Create();
            PlayerOBJ->Transform.wPosition = glm::vec3(-5, 1, -5);
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
      



        /////////////////////////////////////////////////////////////
        app.SwapBuffers();


    }
}