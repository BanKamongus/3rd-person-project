#include "Player.h"
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

void Player::Update(float dt)
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
		if (Input::GetKey(GLFW_KEY_UP)) {
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
		if (!Input::GetKey(GLFW_KEY_UP)) {
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


	m_animator->UpdateAnimation(dt);
}

void Player::Render(Shader& shader)
{
	auto transforms = m_animator->GetFinalBoneMatrices();
	for (int i = 0; i < transforms.size(); ++i)
		shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(.5f, .5f, .5f));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", model);
	m_model.Draw(shader);
}