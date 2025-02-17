#pragma once

#include "GameObject.h"
#include <learnopengl/model_animation.h>
#include <learnopengl/animator.h>

class Enemy : public GameObject
{
public:
	Enemy()
		: m_model("Assets/Models/mixamo/doozy/doozy.dae")
		, idleAnimation("Assets/Models/mixamo/doozy/Fight Idle.dae", &m_model)
		, walkAnimation("Assets/Models/mixamo/doozy/Fight Idle.dae", &m_model)
		, runAnimation("Assets/Models/mixamo/doozy/Run.dae", &m_model)
		, punchAnimation("Assets/Models/mixamo/doozy/Fight Idle.dae", &m_model)
		, knockAnimation("Assets/Models/mixamo/doozy/Slipping.dae", &m_model)
	{
		m_animator = std::make_unique<Animator>(&idleAnimation);
		m_scale = glm::vec3(1.0f);
		m_position = glm::vec3(4, 0, 0);
	}

	virtual void Update(float dt) override
	{
		// DO AI STUFF


		m_animator->UpdateAnimation(dt);
	}

	virtual void Render(Renderer& renderer) override
	{
		Shader& shader = renderer.m_animShader;
		shader.use();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position)
			* glm::mat4(glm::quat(m_rotation))
			* glm::scale(glm::mat4(1.0f), m_scale);
		shader.setMat4("model", model);

		vector<glm::mat4> transforms = m_animator->GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
		m_model.Draw(shader);
	}

	Model m_model;
	std::unique_ptr<Animator> m_animator;

	Animation idleAnimation;
	Animation walkAnimation;
	Animation runAnimation;
	Animation punchAnimation;
	Animation knockAnimation;
};