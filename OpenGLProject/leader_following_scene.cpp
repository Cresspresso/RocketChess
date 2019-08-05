/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	leader_following_scene.cpp
**
**	Summary		:	Demo Scene for Leader Following behaviour.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	12/06/2019
*/

#include "random.hpp"
#include "map_container.hpp"
#include "lambda.hpp"

#include "screen.hpp"
#include "input.hpp"
#include "time.hpp"

#include "basic_materials_builder.hpp"
#include "basic_scene_builder.hpp"

#include "leader_following_scene.hpp"



namespace leader_following_scene
{
#pragma region Scene::Materials::Builder



	struct Scene::Materials::Builder : BasicMaterialsBuilder
	{
		Materials& materials;

		Builder(Materials& materials);

		void build() const;
	private:
		void initCorner(RimLightingMaterial& material) const;
		void initMouseMover(PhongMaterial& material) const;
		void initLeader(PhongMaterial& material) const;
		void initFollower(PhongMaterial& material) const;
		void initTargetPos(PhongMaterial& material) const;

		void initTextInfo(TextMaterial& material) const;
	};



	Scene::Materials::Builder::Builder(Materials& materials) :
		materials{ materials }
	{}



	void Scene::Materials::Builder::build() const
	{
		BasicMaterialsBuilder::build(materials);

		initCorner(materials.corner);
		initMouseMover(materials.mouseMover);
		initLeader(materials.leader);
		initFollower(materials.follower);
		initTargetPos(materials.targetPos);

		initTextInfo(materials.textInfo);
	}



	void Scene::Materials::Builder::initCorner(RimLightingMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::CapguyWalk];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0.9f, 1, 1);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
		material.rimColor = vec3(0, 0, 1);
		material.rimExponent = 2;
	}

	void Scene::Materials::Builder::initMouseMover(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0.9f, 0, 0);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 128;
	}

	void Scene::Materials::Builder::initLeader(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(0.05f, 0.15f, 0.05f);
		material.lightColor = vec3(0.7f, 1, 0.7f);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
	}

	void Scene::Materials::Builder::initFollower(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0.9f, 1, 1);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
	}

	void Scene::Materials::Builder::initTargetPos(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0, 0.9f, 0.1f);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
	}

	void Scene::Materials::Builder::initTextInfo(TextMaterial& material) const
	{
		material.tint = vec3(0.9f);
	}



#pragma endregion ~Scene::Materials::Builder
#pragma region Scene::Materials



	void Scene::Materials::init()
	{
		Builder{ *this }.build();
	}



#pragma endregion ~Scene::Materials
#pragma region Scene::Builder



	struct Scene::Builder : public BasicSceneBuilder
	{
		Scene& scene;
		Entities& entities;
		Materials& materials;

		Builder(Scene& scene);

		void build() const;

	private:
		// world
		void initCorners(std::array<SimpleEntity, 4>& corners) const;
		void initCorner(SimpleEntity& entity) const;
		void initMouseMover(MouseMover& entity) const;
		void initLeader(Seeker& entity) const;
		void initFollower(Pursuer& entity) const;
		void initTargetPos(SimpleEntity& entity) const;
		// hud
		void initTextInfo(TextRenderer& entity) const;
	};



	Scene::Builder::Builder(Scene& scene) :
		BasicSceneBuilder{ scene.materials },
		scene{ scene },
		entities{ scene.entities },
		materials{ scene.materials }
	{}



	void Scene::Builder::build() const
	{
		BasicSceneBuilder::build(entities);

		initCorners(entities.corners);
		initMouseMover(entities.mouseMover);
		initLeader(entities.leader);
		for (auto& follower : entities.followers)
		{
			initFollower(follower);
		}
		initTargetPos(entities.targetPos);

		initTextInfo(entities.textInfo);
	}



	void Scene::Builder::initCorners(std::array<SimpleEntity, 4>& corners) const
	{
		constexpr float a = 5.5f;
		/*static auto const points = std::array{
			vec3(-a, 0, -a),
			vec3(a, 0, -a),
			vec3(a, 0, a),
			vec3(-a, 0, a),
		};*/
		static std::array<vec3, 4> const points = {
			vec3(-a, 0, 0),
			vec3(0, 0, -a),
			vec3(a, 0, 0),
			vec3(0, 0, a),
		};

		for (size_t i = 0; i < corners.size(); ++i)
		{
			auto& e = corners[i];
			initCorner(e);
			e.transform.localPosition = points[i];
			e.transform.localRotation = quat(glm::radians(vec3(0, i * 90, 0)));
		}
	}

	void Scene::Builder::initCorner(SimpleEntity& entity) const
	{
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::RimLighting].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.corner);

		auto& t = entity.transform;
		t.localScale = vec3(0.1f, 1, 0.1f);
	}

	void Scene::Builder::initMouseMover(MouseMover& entity) const
	{
		auto& r = entity.simple.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.mouseMover);

		auto& t = entity.simple.transform;
		t.localPosition = vec3(-4, 0, -2);
		t.localScale = vec3(0.1f, 0.8f, 0.1f);
	}

	void Scene::Builder::initLeader(Seeker& entity) const
	{
		entity.forceScale = 4;
		entity.maxForce = 3.1f;
		entity.maxSpeed = 2;
		entity.arrivingRadius = 1.5f;

		auto& r = entity.simple.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.leader);

		auto& t = entity.simple.transform;
		t.localPosition = vec3(-4, 0, -2);
		t.localScale = vec3(0.2f, 0.4f, 0.2f);
	}

	void Scene::Builder::initFollower(Pursuer& entity) const
	{
		entity.distanceScales.direction = 0;
		entity.distanceScales.velocity = 0;
		entity.targetScales.direction = 0.1f;
		entity.targetScales.velocity = 0;

		auto& s = entity.seeker;
		s.forceScale = 4;
		s.maxForce = 3.1f;
		s.maxSpeed = 2;
		s.arrivingRadius = 1.5f;

		auto& r = s.simple.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.follower);

		auto& t = s.simple.transform;
		t.localPosition = vec3(randomRealEx<float>(-1, 1), 0, randomRealEx<float>(-1, 1));
		t.localScale = vec3(0.2f, 0.4f, 0.2f);

		auto& rb = s.simple.rigidbody;
		rb.localVelocity = 0.1f * vec3(randomRealEx<float>(-1, 1), 0, randomRealEx<float>(-1, 1));
	}

	void Scene::Builder::initTargetPos(SimpleEntity& entity) const
	{
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.targetPos);

		auto& t = entity.transform;
		t.localScale = vec3(0.1f);
	}



	void Scene::Builder::initTextInfo(TextRenderer& entity) const
	{
		initTextRenderer(entity, &(materials.textInfo));

		entity.text = "Left Click to set leader target position.";
		entity.scale = vec2(1.2f);
	}



#pragma endregion ~Scene::Builder
#pragma region Scene



	void Scene::init()
	{
		materials.init();
		Builder{ *this }.build();
	}



	ReturnCode Scene::update()
	{
		BEGIN_ANYALL();
		{
			auto& e = entities;

			e.cameraWorld.update();
			e.cameraWorld.useForRendering();

			for (auto& corner : e.corners)
			{
				DO_ANYALL(corner.update());
			}

			DO_ANYALL(e.mouseMover.update());

			e.leader.targetPosition = e.mouseMover.simple.transform.localPosition;
			DO_ANYALL(e.leader.update());

			DO_ANYALL(updateFollowers());

			// hud

			// button horizontal layout at top-left corner of hud
			{
				vec2 const hudHalfSize = e.cameraHud.projection.calculateHalfSize();
				vec3 const buttonHalfSize = 0.5f
					* e.buttonMenu.button.backgroundTransform.localScale
					* e.buttonMenu.scaleHovered;
				vec2 const menuButtonPos = vec2(
					-hudHalfSize.x + buttonHalfSize.x,
					hudHalfSize.y - buttonHalfSize.y
				);
				e.buttonMenu.button.transform.localPosition = vec3(menuButtonPos, 0);
				//vec2 const offset = vec2(0, buttonHalfSize.y * 2);

				e.textInfo.position = vec2(-hudHalfSize.x + buttonHalfSize.x * 2 + 20, hudHalfSize.y - 60);
			}
			e.cameraHud.useForRendering();
			DO_ANYALL(e.buttonMenu.update());
		}
		return END_ANYALL();
	}



	ReturnCode Scene::render()
	{
		BEGIN_ANYALL();
		{
			DO_ANYALL(renderWorld());
			DO_ANYALL(renderHud());
		}
		return END_ANYALL();
	}



	ReturnCode Scene::renderWorld()
	{
		auto& e = entities;

		e.cameraWorld.recalculate();
		e.cameraWorld.useForRendering();

		BEGIN_ANYALL();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		{
			DO_ANYALL(e.skybox.render());

			for (auto& corner : e.corners)
			{
				DO_ANYALL(corner.render());
			}
			DO_ANYALL(e.mouseMover.render());
			DO_ANYALL(e.leader.render());
			for (auto& follower : e.followers)
			{
				DO_ANYALL(follower.render());
			}

			e.targetPos.transform.localPosition = calculateLeaderFollowPos();
			DO_ANYALL(e.targetPos.render());
		}
		glDisable(GL_DEPTH_TEST);

		return END_ANYALL();
	}



	ReturnCode Scene::renderHud()
	{
		auto& e = entities;

		e.cameraHud.recalculate();
		e.cameraHud.useForRendering();

		BEGIN_ANYALL();
		{
			DO_ANYALL(e.textInfo.render());
			DO_ANYALL(e.buttonMenu.render());
		}
		return END_ANYALL();
	}



	ReturnCode Scene::updateFollowers()
	{
		vec3 const followPos = calculateLeaderFollowPos();

		size_t const sz = entities.followers.size();
		if (sz <= 0)
		{
			return RC_SUCCESS;
		}
		// else...

		// get positions of followers last frame
		std::vector<vec3> const positions = container::map(entities.followers, LAM(follower, follower.seeker.simple.transform.localPosition));

		BEGIN_ANYALL();
		// for each follower...
		for (size_t i = 0; i < sz; ++i)
		{
			auto& follower = entities.followers[i];
			vec3 const& position = positions[i];



			// Separate from leader.
			vec3 const& otherPosition = entities.leader.simple.transform.localPosition;

			// get displacement.
			vec3 const toSelf = position - otherPosition;

			// if too close to leader...
			float const radius = 1.0f;
			if (toleranceZero2(toSelf, radius * radius))
			{
				// Evade the leader.
				follower.setEvade(true);
				follower.target.position = entities.leader.simple.transform.localPosition;
				follower.target.velocity = entities.leader.simple.rigidbody.localVelocity;
				DO_ANYALL(follower.update());
			}
			else if (sz == 1)
			{
				// Seek point behind leader.
				follower.seeker.targetPosition = followPos;
				follower.seeker.flee = false;
				DO_ANYALL(follower.seeker.update());
			}
			else
			{
				// get average direction from other followers to self.
				vec3 totalDir = vec3();
				size_t count = 0;
				for (size_t j = 0; j < sz; ++j)
				{
					// if other is self, skip.
					if (j == i) { continue; }

					vec3 const& otherPosition = positions[j];

					// get displacement.
					vec3 const toSelf = position - otherPosition;

					// if not close enough, skip.
					float const radius = 0.8f;
					if (!toleranceZero2(toSelf, radius * radius)) { continue; }

					// add to total.
					//vec3 const dir = normalizeOrElse(toSelf);
					vec3 const dir = toSelf / radius;
					totalDir += dir;
					++count;
				}

				// If there is at least one nearby fellow follower...
				if (count > 0)
				{
					// average the directions.
					vec3 const averageDir = totalDir / static_cast<float>(count);

					float const separationForce = 1;
					follower.seeker.targetPosition = position + averageDir * separationForce;
					follower.seeker.flee = false;
					DO_ANYALL(follower.seeker.update());
				}
				else
				{
					// Seek point behind leader.
					follower.seeker.targetPosition = followPos;
					follower.seeker.flee = false;
					DO_ANYALL(follower.seeker.update());
				}
			}

			// Clamp to XZ plane.
			follower.seeker.simple.transform.localPosition.y = 0;
		}
		return END_ANYALL();
	}

	vec3 Scene::calculateLeaderFollowPos() const
	{
		auto const& leaderTransform = entities.leader.simple.transform;
		vec3 const& leaderPosition = leaderTransform.localPosition;
		quat const& leaderRotation = leaderTransform.localRotation;

		float const followDistance = 1.8f;
		vec3 const followPos = leaderPosition + (leaderRotation * vec3(0, 0, 1)) * followDistance;
		return followPos;
	}



#pragma endregion ~Scene
}
