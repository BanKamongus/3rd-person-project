#pragma once

#include "BanKEngine.h"
#include "Input.h"
#include "B_Player.h"

#include <learnopengl/shader.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>


class Enemy : public BanKBehavior
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
	}Controls;

	Enemy();   
	void Update();
	void Render(Shader& shader);

	Player* TargetPLR;
	GameObj* Gun_OBJ;
	int BoneIdx = MixamoBone_LeftHand;

	GameObj* BODY;
	void Init() {

			BODY = GameObject;
				Gun_OBJ = BODY->CreateChild();
				Gun_OBJ->Transform.wPosition = glm::vec3(4, 6, 0) * 20.0f;
				Gun_OBJ->Transform.wRotation = glm::vec3(0, 90, 0);
				Gun_OBJ->Transform.wScale = glm::vec3(20); 

	}

	void Start() {
		TargetPLR = sGetComponent_OfClass(TargetPLR);
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

	glm::vec3 TargetPos;
	float FollowSpeed = 0.5f;
	float Speed = 1.5;


	void Update_Behavior() {
		if (Time.Deltatime > 0.1) { return; }
		 
		if (TargetPLR) {
			TargetPos = B_lerpVec3(TargetPos,TargetPLR->GameObject->Transform.wPosition,Time.Deltatime* FollowSpeed);
			GameObject->Transform.LookAt(TargetPos);
			
			if (glm::distance(TargetPos, GameObject->Transform.wPosition) > 0.5) {
				//GameObject->Transform.wPosition += GameObject->Transform.getForwardVector() * Time.Deltatime * Speed;
			}
		}
	}
};




#include "Input.h"
#include <GLFW/glfw3.h>

Enemy::Enemy()
	: m_model("Assets/Models/mixamo/doozy/doozy.dae") 
	, idleAnimation("Assets/Models/mixamo/doozy/Fight Idle.dae", &m_model)
	, walkAnimation("Assets/Models/mixamo/doozy/Fight Idle.dae", &m_model)
	, runAnimation("Assets/Models/mixamo/doozy/Fight Idle.dae", &m_model)
	, punchAnimation("Assets/Models/mixamo/doozy/Fight Idle.dae", &m_model)
	, kickAnimation("Assets/Models/mixamo/doozy/Fight Idle.dae", &m_model)
{
	m_animator = std::make_unique<Animator>(&walkAnimation);
}

void Enemy::Update()
{
	if (Input::GetKeyDown(GLFW_KEY_1))
		m_animator->PlayAnimation(&idleAnimation, NULL, 0.0f, 0.0f, 0.0f);
	if (Input::GetKeyDown(GLFW_KEY_2))
		m_animator->PlayAnimation(&walkAnimation, NULL, 0.0f, 0.0f, 0.0f);
	if (Input::GetKeyDown(GLFW_KEY_3))
		m_animator->PlayAnimation(&punchAnimation, NULL, 0.0f, 0.0f, 0.0f);
	if (Input::GetKeyDown(GLFW_KEY_4))
		m_animator->PlayAnimation(&kickAnimation, NULL, 0.0f, 0.0f, 0.0f);

	m_animator->UpdateAnimation(Time.Deltatime);


	Update_Behavior();
}



void Enemy::Render(Shader& shader)
{

	vector<glm::mat4> transforms = m_animator->GetFinalBoneMatrices();
	for (int i = 0; i < transforms.size(); ++i)
		shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


	shader.setMat4("model", BODY->Transform.modelMatrix);
	m_model.Draw(shader);

}
