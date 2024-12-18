#pragma once

#include "BanKEngine.h"
#include "Input.h"
#include "Renderer.h"

#include <learnopengl/shader.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>

class Bullet : public BanKBehavior 
{

public:

	static 	enum Team
	{
		Player = 0,
		Enemy
	};

	float lifespan = 0.64;
	float Speed = 0.8f;
	Model_Static* m_model;
	Collider_Capsule* mCollider_Capsule;
	int Team = Bullet::Player;

	Bullet(Model_Static* m_model) :m_model(m_model) {
	}

	void Init() {
		GameObject->Transform.wScale = glm::vec3(0.16f);
		mCollider_Capsule = GameObject->AddComponent(new Collider_Capsule);
		mCollider_Capsule->Radius = 0.05f;
		mCollider_Capsule->Height = 0.1f;
		mCollider_Capsule->Trigger = true;

	}

	void Update() {
		GameObject->Transform.wPosition += GameObject->Transform.getForwardVector()* Speed;

		lifespan -= Time.Deltatime;
		if (lifespan < 0) {
			GameObject->Destroy = true;
		}

		if (mCollider_Capsule->Event.isCollided) {
			//GameObject->Destroy = true; 
		}
	}

	void Render(Renderer& renderer) {

		Shader& shader2 = renderer.m_basicShader;
		shader2.use();
		shader2.setMat4("model", GameObject->Transform.modelMatrix);
		m_model->Draw(shader2);
	}
};

class Player : public BanKBehavior
{
public:

	struct Controls {
		bool MOVE_FWD = false;
		bool MOVE_BACK = false;
		bool MOVE_LFT = false;
		bool MOVE_RHT = false;
		bool ATK_1 = false;
		float TURN_X = 0;
		float TURN_Y = 0;
		void Update() {
			MOVE_FWD = Input::GetKey(GLFW_KEY_W);
			MOVE_BACK = Input::GetKey(GLFW_KEY_S);
			MOVE_LFT = Input::GetKey(GLFW_KEY_A);
			MOVE_RHT = Input::GetKey(GLFW_KEY_D);
			ATK_1 = Input::GetKey(GLFW_KEY_N);

			pair Mouse = Input::getMousePosChange();
			TURN_Y = Mouse.first;
			TURN_X = Mouse.second;
		}
	}Controls; 

	Player();
	void Update();
	void Render(Renderer& renderer);

	GameObj* Gun_OBJ;
	Model_Static* Gun_Model;
	glm::mat4 Gun_Matrix;
	const float Gun_AnimCooldownMax = 0.25f;
	float Gun_AnimCooldown = 0;
	const float Gun_CooldownMax = 0.1f;
	float Gun_Cooldown = 0;
	Model_Static* Bullet_Model;
	Collider_Capsule* mCollider_Capsule;

	GameObj* CamArea;
	GameObj* CamSocket;
	GameObj* CamLookat;
	GameObj* BODY_RotProbe;
	float BODY_RotProbe_TargetRot;
	GameObj* BODY;
	float BODY_TargetRot;

	void Init() {

		mCollider_Capsule = GameObject->AddComponent(new Collider_Capsule);

		CamArea = GameObject->CreateChild();
		CamArea->Transform.wPosition = glm::vec3(0, 1.25, 0);

			CamSocket = CamArea->CreateChild();
			CamSocket->Transform.wPosition = glm::vec3(-0.25, 0, -1)*2.0f;

			CamLookat = CamArea->CreateChild();
			CamLookat->Transform.wPosition = glm::vec3(-0.25, 0, 1)*99999.0f;
			CamLookat->Transform.wPosition.x += 0.05;
				


		BODY_RotProbe = GameObject->CreateChild();

			BODY = BODY_RotProbe->CreateChild();
			 
				Gun_OBJ = BODY->CreateChild();
				Gun_OBJ->Transform.wPosition = glm::vec3(-5, 7, 0) * 20.0f;
				Gun_OBJ->Transform.wRotation = glm::vec3(-90, 180, 0);
				Gun_OBJ->Transform.wScale = glm::vec3(3); 




		Gun_Model = new Model_Static("Assets/Models/AK47/OBJ/ak7finished.obj");
		Bullet_Model = new Model_Static("Assets/Models/Bullets/Bullets.obj");
	}

private:
	enum AnimState {
		IDLE = 1,
		IDLE_PUNCH,
		PUNCH_IDLE,
		IDLE_KICK,
		KICK_IDLE,
		IDLE_WALK,
		WALK_IDLE,
		WALK
	};

	enum AnimState charState = IDLE;
	float blendAmount = 0.0f;


	Model_Bone m_model;
	std::unique_ptr<Animator> m_animator;

	Animation idleAnimation;
	Animation walkAnimation;
	Animation runAnimation;
	Animation punchAnimation;
	Animation kickAnimation;

	glm::vec3 Velocity;


	bool Input = false;
	void Update_Behavior() {
		Controls.Update();

		Input = false;
		float Accel = Time.Deltatime * 32;

		Gun_Cooldown += Time.Deltatime;
		Gun_AnimCooldown -= Time.Deltatime;
		if (Controls.ATK_1) {
			BODY_RotProbe_TargetRot = CamArea->Transform.wRotation.y;
			BODY_TargetRot = 0;
			Gun_AnimCooldown = Gun_AnimCooldownMax; 

			if (Gun_Cooldown > Gun_CooldownMax) {
				Gun_Cooldown = 0;
				GameObj* BulletOBJ = GameObj::Create();
				BulletOBJ->Transform.wPosition = getDirectPosition(Gun_Matrix);
				BulletOBJ->Transform.LookAt(CamLookat->Transform.getWorldPosition());
				BulletOBJ->AddComponent(new Bullet(Bullet_Model));

				CamArea->Transform.wRotation.x -= B_irand(-2,5);
				CamArea->Transform.wRotation.y += B_irand(-5,5);
			}

		}
		else if(Gun_AnimCooldown<=0) {


			if (Controls.MOVE_FWD) {
				Input = true;
				Velocity += BODY_RotProbe->Transform.getForwardVector() * Accel;
				BODY_TargetRot = B_lerp(BODY_TargetRot,0, Time.Deltatime *9);
			}
			else if (Controls.MOVE_BACK)
			{
				Input = true;
				Velocity -= BODY_RotProbe->Transform.getForwardVector() * Accel;
				BODY_TargetRot = B_lerp(BODY_TargetRot, 180, Time.Deltatime * 9);
			}

			if (Controls.MOVE_LFT) {
				Input = true;
				Velocity += BODY_RotProbe->Transform.getLeftVector() * Accel;
				BODY_TargetRot = B_lerp(BODY_TargetRot, 90, Time.Deltatime * 9);
			}
			else if (Controls.MOVE_RHT)
			{
				Input = true;

				Velocity -= BODY_RotProbe->Transform.getLeftVector() * Accel;

				if (Controls.MOVE_BACK)
				{
					BODY_TargetRot = B_lerp(BODY_TargetRot, 270, Time.Deltatime * 9);
				}
				else
				{
					BODY_TargetRot = B_lerp(BODY_TargetRot, -90, Time.Deltatime * 9);
				}
			}
		}




		if (Input::GetKey(GLFW_KEY_E)) {
			Input = true;  
			Velocity.y += Accel;
		}
		else if (Input::GetKey(GLFW_KEY_Q)){
			Input = true;
			Velocity.y -= Accel;
		}


		if (Input::GetKey(GLFW_KEY_LEFT)) {
			CamArea->Transform.wRotation.y += 64 * Time.Deltatime;
		}
		else if (Input::GetKey(GLFW_KEY_RIGHT)) {
			CamArea->Transform.wRotation.y -= 64 * Time.Deltatime;
		}
		if (Input::GetKey(GLFW_KEY_UP)) {
			CamArea->Transform.wRotation.x -= 64 * Time.Deltatime;
		}
		else if (Input::GetKey(GLFW_KEY_DOWN)) {
			CamArea->Transform.wRotation.x += 64 * Time.Deltatime;
		}




		float MaxVel = 5;
		Velocity.x = B_clamp(Velocity.x, -MaxVel, MaxVel);
		Velocity.y = B_clamp(Velocity.y, -MaxVel, MaxVel);
		Velocity.z = B_clamp(Velocity.z, -MaxVel, MaxVel);
		Velocity = B_lerpVec3(Velocity, glm::vec3(0), Time.Deltatime * 4);
		GameObject->Transform.wPosition += Velocity * Time.Deltatime;



		CamArea->Transform.wRotation.y -= 0.5f * Controls.TURN_Y;
		CamArea->Transform.wRotation.x += 0.5f * Controls.TURN_X;
		CamArea->Transform.wRotation.x = B_clamp(CamArea->Transform.wRotation.x, -90, 90);


		if (Input) {
			BODY_RotProbe_TargetRot = CamArea->Transform.wRotation.y;
		}
		 


		float RotSpd_A = 10 * Time.Deltatime;
		float RotSpd_B = 12 * Time.Deltatime;
		BODY_RotProbe->Transform.wRotation.y = B_lerp(BODY_RotProbe->Transform.wRotation.y, BODY_RotProbe_TargetRot, RotSpd_A);
		BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y, BODY_TargetRot, RotSpd_B);
	}
};




#include "Input.h"
#include <GLFW/glfw3.h>

Player::Player()
	: m_model("Assets/Models/mixamo/steve.dae")
	, idleAnimation("Assets/Models/mixamo/Rifle Aiming Idle.dae", &m_model)
	, walkAnimation("Assets/Models/mixamo/walk.dae", &m_model)
	, runAnimation("Assets/Models/mixamo/Rifle Run.dae", &m_model)
	, punchAnimation("Assets/Models/mixamo/punch.dae", &m_model)
	, kickAnimation("Assets/Models/mixamo/kick.dae", &m_model)
{
	m_animator = std::make_unique<Animator>(&idleAnimation);

	std::map<std::string, BoneInfo>::iterator it;
	auto boneInfoMap = m_animator->m_CurrentAnimation->GetBoneIDMap();
	for (it = boneInfoMap.begin(); it != boneInfoMap.end(); it++) {
		cout << it->first
			<< " " << it->second.id << std::endl;
	}
}

void Player::Update()
{
	if (Input::GetKeyDown(GLFW_KEY_1))
		m_animator->PlayAnimation(&idleAnimation, NULL, 0.0f, 0.0f, 0.0f);
	if (Input::GetKeyDown(GLFW_KEY_2))
		m_animator->PlayAnimation(&walkAnimation, NULL, 0.0f, 0.0f, 0.0f);
	if (Input::GetKeyDown(GLFW_KEY_3))
		m_animator->PlayAnimation(&punchAnimation, NULL, 0.0f, 0.0f, 0.0f);
	if (Input::GetKeyDown(GLFW_KEY_4))
		m_animator->PlayAnimation(&kickAnimation, NULL, 0.0f, 0.0f, 0.0f);


	float blendRate = Time.Deltatime*4;
	switch (charState) {
	case IDLE:
		if (Input) {
			blendAmount = 0.0f;
			m_animator->PlayAnimation(&idleAnimation, &walkAnimation, m_animator->m_CurrentTime, 0.0f, blendAmount);
			charState = IDLE_WALK;
		}
		else if (Input::GetKey(GLFW_KEY_J)) {
			blendAmount = 0.0f;
			m_animator->PlayAnimation(&idleAnimation, &punchAnimation, m_animator->m_CurrentTime, 0.0f, blendAmount);
			charState = IDLE_PUNCH;
		}
		else if (Input::GetKey(GLFW_KEY_K)) {
			blendAmount = 0.0f;
			m_animator->PlayAnimation(&idleAnimation, &kickAnimation, m_animator->m_CurrentTime, 0.0f, blendAmount);
			charState = IDLE_KICK;
		}
		printf("idle \n");
		break;
	case IDLE_WALK:
		blendAmount += blendRate;
		m_animator->PlayAnimation(&idleAnimation, &runAnimation, m_animator->m_CurrentTime, m_animator->m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = m_animator->m_CurrentTime2;
			m_animator->PlayAnimation(&runAnimation, NULL, startTime, 0.0f, blendAmount);
			charState = WALK;
		}
		printf("idle_walk \n");
		break;
	case WALK:
		m_animator->PlayAnimation(&runAnimation, NULL, m_animator->m_CurrentTime, m_animator->m_CurrentTime2, blendAmount);
		if (!Input) {
			charState = WALK_IDLE;
		}
		printf("walking\n");
		break;
	case WALK_IDLE:
		blendAmount += blendRate;
		m_animator->PlayAnimation(&runAnimation, &idleAnimation, m_animator->m_CurrentTime, m_animator->m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = m_animator->m_CurrentTime2;
			m_animator->PlayAnimation(&idleAnimation, NULL, startTime, 0.0f, blendAmount);
			charState = IDLE;
		}
		printf("walk_idle \n");
		break;
	case IDLE_PUNCH:
		blendAmount += blendRate;
		m_animator->PlayAnimation(&idleAnimation, &punchAnimation, m_animator->m_CurrentTime, m_animator->m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = m_animator->m_CurrentTime2;
			m_animator->PlayAnimation(&punchAnimation, NULL, startTime, 0.0f, blendAmount);
			charState = PUNCH_IDLE;
		}
		printf("idle_punch\n");
		break;
	case PUNCH_IDLE:
		if (m_animator->m_CurrentTime > 0.7f) {
			blendAmount += blendRate;
			m_animator->PlayAnimation(&punchAnimation, &idleAnimation, m_animator->m_CurrentTime, m_animator->m_CurrentTime2, blendAmount);
			if (blendAmount > 0.9f) {
				blendAmount = 0.0f;
				float startTime = m_animator->m_CurrentTime2;
				m_animator->PlayAnimation(&idleAnimation, NULL, startTime, 0.0f, blendAmount);
				charState = IDLE;
			}
			printf("punch_idle \n");
		}
		else {
			// punching
			printf("punching \n");
		}
		break;
	case IDLE_KICK:
		blendAmount += blendRate;
		m_animator->PlayAnimation(&idleAnimation, &kickAnimation, m_animator->m_CurrentTime, m_animator->m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = m_animator->m_CurrentTime2;
			m_animator->PlayAnimation(&kickAnimation, NULL, startTime, 0.0f, blendAmount);
			charState = KICK_IDLE;
		}
		printf("idle_kick\n");
		break;
	case KICK_IDLE:
		if (m_animator->m_CurrentTime > 1.0f) {
			blendAmount += blendRate;
			m_animator->PlayAnimation(&kickAnimation, &idleAnimation, m_animator->m_CurrentTime, m_animator->m_CurrentTime2, blendAmount);
			if (blendAmount > 0.9f) {
				blendAmount = 0.0f;
				float startTime = m_animator->m_CurrentTime2;
				m_animator->PlayAnimation(&idleAnimation, NULL, startTime, 0.0f, blendAmount);
				charState = IDLE;
			}
			printf("kick_idle \n");
		}
		else {
			// punching
			printf("kicking \n");
		}
		break;
	}


	m_animator->UpdateAnimation(Time.Deltatime);


	Update_Behavior();
}


int BoneIdx = MixamoBone_RightHand;
void Player::Render(Renderer& renderer)
{


	//FinalBoneMatrix = Transform Matrix that will be applied to T pose
	//So the bone can move to desired position acoording to T pose
	//NOT where the bone is right now in 3D space
	//T pose in this case, will stay there as reference point
	//and will not be moved 

	Shader& shader = renderer.m_animShader;
	shader.use();

	vector<glm::mat4> transforms = m_animator->GetFinalBoneMatrices();
	for (int i = 0; i < transforms.size(); ++i)
		shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

	shader.setMat4("model", BODY->Transform.modelMatrix);
	m_model.Draw(shader);





	Shader& shader2 = renderer.m_basicShader;
	shader2.use();

	glm::mat4 mm_Parent = BODY->Transform.modelMatrix;
	glm::mat4 mm_Child  = Gun_OBJ->Transform.modelMatrix;
	glm::mat4 T_asLocal = transforms[BoneIdx];
	glm::mat4 T_asWorld = mm_Parent * T_asLocal * glm::inverse(mm_Parent);
	Gun_Matrix = T_asWorld * mm_Child;

	shader2.setMat4("model", Gun_Matrix);
	Gun_Model->Draw(shader2);


}
