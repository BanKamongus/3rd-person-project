#pragma once

#include "BanKEngine.h"
#include "Input.h"

#include <learnopengl/shader.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>

class Bullet : public BanKBehavior 
{
public:
	float Speed = 0.05f;
	Model_Anim* m_model;

	Bullet(Model_Anim* m_model) :m_model(m_model) {

	}

	void Update() {
		GameObject->Transform.wPosition += GameObject->Transform.getForwardVector()* Speed;
	}

	void Render(Shader& shader) {
		shader.setMat4("model", GameObject->Transform.modelMatrix);
		m_model->Draw(shader);
	}
};

class Player : public BanKBehavior
{
public:

	Player();
	void Update();
	void Render(Shader& shader);

	GameObj* Gun_OBJ;
	//Model* Gun_Model;

	GameObj* CamArea;
	GameObj* CamSocket;
	GameObj* CamLookat;
	GameObj* BODY_RotProbe;
	GameObj* BODY;
	void Init() {

		CamArea = GameObject->CreateChild();
		CamArea->Transform.wPosition = glm::vec3(0, 1.25, 0);

			CamSocket = CamArea->CreateChild();
			CamSocket->Transform.wPosition = glm::vec3(-0.25, 0, -1)*2.0f;

			CamLookat = CamArea->CreateChild();
			CamLookat->Transform.wPosition = glm::vec3(-0.25, 0, 1)*2.0f;
				


		BODY_RotProbe = GameObject->CreateChild();

			BODY = BODY_RotProbe->CreateChild();

				Gun_OBJ = BODY->CreateChild();
				Gun_OBJ->Transform.wPosition = glm::vec3(4, 6, 0) * 20.0f;
				Gun_OBJ->Transform.wRotation = glm::vec3(0, 90, 0);
				Gun_OBJ->Transform.wScale = glm::vec3(20); 




		//Gun_Model = new Model("Assets/Models/AK47/OBJ/ak7finished.obj");
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
	float blendRate = 0.055f;

	Model_Anim m_model;
	std::unique_ptr<Animator> m_animator;

	Animation idleAnimation;
	Animation walkAnimation;
	Animation runAnimation;
	Animation punchAnimation;
	Animation kickAnimation;

	glm::vec3 Velocity;


	bool Input = false;
	bool InputPrev = false;
	void Update_Control() {

		Input = false;
		float Accel = Time.Deltatime * 0.25f;
		float RotSpd = 8 * Time.Deltatime;
		if (Input::GetKey(GLFW_KEY_W)) {
			Input = true;
			Velocity += CamArea->Transform.getForwardVector() * Accel;
			BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y,0, RotSpd);
		}
		else if (Input::GetKey(GLFW_KEY_S))
		{
			Input = true;
			Velocity -= CamArea->Transform.getForwardVector() * Accel;
			BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y, 180, RotSpd);
		}

		if (Input::GetKey(GLFW_KEY_A)) {
			Input = true;
			Velocity += CamArea->Transform.getLeftVector() * Accel;
			BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y, 90, RotSpd);
		}
		else if (Input::GetKey(GLFW_KEY_D))
		{ 
			Input = true;

			Velocity -= CamArea->Transform.getLeftVector() * Accel;

			if(Input::GetKey(GLFW_KEY_S)) 
			{	BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y, 270, RotSpd);	} 
			else
			{	BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y, -90, RotSpd);		}
		}



		if (Input) {
			BODY_RotProbe->Transform.wRotation.y = B_lerp(BODY_RotProbe->Transform.wRotation.y, CamArea->Transform.wRotation.y,Time.Deltatime*8);
		}
		InputPrev = Input;


		if (Input::GetKey(GLFW_KEY_E)) {
			Input = true;
			Velocity.y += Accel;
		}
		else if (Input::GetKey(GLFW_KEY_Q)){
			Input = true;
			Velocity.y -= Accel;
		}

		float MaxAccel = 4;
		Velocity = B_lerpVec3(Velocity, glm::vec3(0), Time.Deltatime * 4);
		Velocity.x = B_clamp(Velocity.x, -MaxAccel, MaxAccel);
		Velocity.y = B_clamp(Velocity.y, -MaxAccel, MaxAccel);
		Velocity.z = B_clamp(Velocity.z, -MaxAccel, MaxAccel);
		GameObject->Transform.wPosition += Velocity;





		if (Input::GetKey(GLFW_KEY_LEFT)) {
			CamArea->Transform.wRotation.y -= 150 * Time.Deltatime;
		}
		else if (Input::GetKey(GLFW_KEY_RIGHT)) {
			CamArea->Transform.wRotation.y += 150 * Time.Deltatime;
		}

		pair Mouse = Input::getMousePosChange();
		float Mx = Mouse.first;
		float My = Mouse.second; 
		CamArea->Transform.wRotation.y -= 0.5f * Mx;
		CamArea->Transform.wRotation.x += 0.5f * My;
		CamArea->Transform.wRotation.x = B_clamp(CamArea->Transform.wRotation.x, -80, 80);


		if (Input::GetKeyDown(GLFW_KEY_N)) {
			GameObj* BulletOBJ = GameObj::Create();
			BulletOBJ->Transform.wPosition = GameObject->Transform.getWorldPosition();
			BulletOBJ->Transform.wRotation = CamArea->Transform.wRotation;
			BulletOBJ->AddComponent(new Bullet(&m_model));
		}
	}
};




#include "Input.h"
#include <GLFW/glfw3.h>

Player::Player()
	: m_model("Assets/Models/mixamo/steve.dae")
	, idleAnimation("Assets/Models/mixamo/idle.dae", &m_model)
	, walkAnimation("Assets/Models/mixamo/walk.dae", &m_model)
	, runAnimation("Assets/Models/mixamo/run.dae", &m_model)
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
		blendAmount = fmod(blendAmount, 1.0f);
		m_animator->PlayAnimation(&idleAnimation, &walkAnimation, m_animator->m_CurrentTime, m_animator->m_CurrentTime2, blendAmount);
		if (blendAmount > 0.9f) {
			blendAmount = 0.0f;
			float startTime = m_animator->m_CurrentTime2;
			m_animator->PlayAnimation(&walkAnimation, NULL, startTime, 0.0f, blendAmount);
			charState = WALK;
		}
		printf("idle_walk \n");
		break;
	case WALK:
		m_animator->PlayAnimation(&walkAnimation, NULL, m_animator->m_CurrentTime, m_animator->m_CurrentTime2, blendAmount);
		if (!Input) {
			charState = WALK_IDLE;
		}
		printf("walking\n");
		break;
	case WALK_IDLE:
		blendAmount += blendRate;
		blendAmount = fmod(blendAmount, 1.0f);
		m_animator->PlayAnimation(&walkAnimation, &idleAnimation, m_animator->m_CurrentTime, m_animator->m_CurrentTime2, blendAmount);
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
		blendAmount = fmod(blendAmount, 1.0f);
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
			blendAmount = fmod(blendAmount, 1.0f);
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
		blendAmount = fmod(blendAmount, 1.0f);
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
			blendAmount = fmod(blendAmount, 1.0f);
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


	Update_Control();
}


int BoneIdx = MixamoBone_LeftHand;
void Player::Render(Shader& shader)
{
	//FinalBoneMatrix = Transform Matrix that will be applied to T pose
	//So the bone can move to desired position acoording to T pose
	//NOT where the bone is right now in 3D space
	//T pose in this case, will stay there as reference point
	//and will not be moved 
	vector<glm::mat4> transforms = m_animator->GetFinalBoneMatrices();
	for (int i = 0; i < transforms.size(); ++i)
		shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


	shader.setMat4("model", BODY->Transform.modelMatrix);
	m_model.Draw(shader);



	if (Input::GetKeyDown(GLFW_KEY_UP)) {
		BoneIdx++;
		BoneIdx = B_clampLoop(BoneIdx,0, transforms.size() - 1);
		cout << endl
			<< "TotalCount : " << transforms.size() << " | "
			<< "Current    : " << BoneIdx
			;
	}
	else if (Input::GetKeyDown(GLFW_KEY_DOWN)) {
		BoneIdx--;
		BoneIdx = B_clampLoop(BoneIdx, 0, transforms.size() - 1);
		cout << endl
			<< "TotalCount : " << transforms.size() << " | "
			<< "Current    : " << BoneIdx
			;
	}


	glm::mat4 mm_Parent = BODY->Transform.modelMatrix;
	glm::mat4 mm_Child  = Gun_OBJ->Transform.modelMatrix;
	glm::mat4 T_asLocal = transforms[BoneIdx];
	glm::mat4 T_asWorld = mm_Parent * T_asLocal * glm::inverse(mm_Parent);

	shader.setMat4("model", T_asWorld * mm_Child );
	m_model.Draw(shader); 
	//Gun_Model->Draw(shader);

}
