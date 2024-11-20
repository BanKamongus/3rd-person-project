#pragma once

#include "BanKEngine.h"
#include "Input.h"

#include <learnopengl/shader.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>

class Player : public BanKBehavior
{
public:

	Player();
	void Update();
	void Render(Shader& shader);

	GameObj* CamSocket;
	GameObj* CamLookat;
	GameObj* BODY;
	void Init() {

		CamSocket = GameObject->CreateChild();
		CamSocket->Transform.wPosition = glm::vec3(-0.2, 0.25, -1);

		CamLookat = GameObject->CreateChild();
		CamLookat->Transform.wPosition = glm::vec3(-0.2, 0.25, 0);

		BODY = GameObject->CreateChild();
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

	Model m_model;
	std::unique_ptr<Animator> m_animator;

	Animation idleAnimation;
	Animation walkAnimation;
	Animation runAnimation;
	Animation punchAnimation;
	Animation kickAnimation;

	glm::vec3 Velocity;


	bool Input = false;
	void Update_Control() {

		Input = false;
		float Accel = Time.Deltatime * 0.25f;
		float RotSpd = 8 * Time.Deltatime;
		if (Input::GetKey(GLFW_KEY_W)) {
			Input = true;
			Velocity += GameObject->Transform.getForwardVector() * Accel;
			BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y,0, RotSpd);
		}
		else if (Input::GetKey(GLFW_KEY_S))
		{
			Input = true;
			Velocity -= GameObject->Transform.getForwardVector() * Accel;
			BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y, 180, RotSpd);
		}

		if (Input::GetKey(GLFW_KEY_A)) {
			Input = true;
			Velocity += GameObject->Transform.getLeftVector() * Accel;
			BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y, 90, RotSpd);
		}
		else if (Input::GetKey(GLFW_KEY_D))
		{ 
			Input = true;
			Velocity -= GameObject->Transform.getLeftVector() * Accel;
			if(Input::GetKey(GLFW_KEY_S)) {
				BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y, 270, RotSpd);
			}
			else{
				BODY->Transform.wRotation.y = B_lerp(BODY->Transform.wRotation.y, -90, RotSpd);
			}
		}

		if (Input::GetKey(GLFW_KEY_E)) {
			Input = true;
			Velocity += Accel;
		}
		else if (Input::GetKey(GLFW_KEY_Q))
		{
			Input = true;
			Velocity.y -= Accel;
		}

		float MaxAccel = 4;
		Velocity = B_lerpVec3(Velocity, glm::vec3(0), Time.Deltatime * 4);
		Velocity.x = B_clamp(Velocity.x, -MaxAccel, MaxAccel);
		Velocity.y = B_clamp(Velocity.y, -MaxAccel, MaxAccel);
		Velocity.z = B_clamp(Velocity.z, -MaxAccel, MaxAccel);
		GameObject->Transform.wPosition += Velocity;
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

void Player::Render(Shader& shader)
{
	auto transforms = m_animator->GetFinalBoneMatrices();
	for (int i = 0; i < transforms.size(); ++i)
		shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

	// render the loaded model
	glm::mat4 model = BODY->Transform.modelMatrix;
	model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(.5f, .5f, .5f));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", model);
	m_model.Draw(shader);
}