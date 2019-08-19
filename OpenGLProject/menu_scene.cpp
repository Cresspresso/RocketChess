/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	menu_scene.cpp
**
**	Summary		:	Main Menu scene. Jumpboard to other scenes.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "entity_utils.hpp"

#include "singleton.hpp"
#include "screen.hpp"

#include "audio.hpp"
#include "toggle_music.hpp"
#include "input.hpp"

#include "debug_log.hpp"

#include "menu_scene.hpp"



namespace menu_scene
{
#pragma region Scene::Materials::Builder



	struct Scene::Materials::Builder : BasicMaterialsBuilder
	{
		Materials& materials;

		Builder(Materials& materials);

		void build() const;
	private:
		void initFocusedObject(PhongMaterial& material) const;
		void initOutline(PhongMaterial& material) const;
		void initFoggy(FogMaterial& material) const;
		void initFlyby(Quad4Material& material) const;
		void initRimLighting(RimLightingMaterial& material) const;
		void initReflection(ReflectionMaterial& material) const;
		void initInfoText(TextMaterial& material) const;
	};



	Scene::Materials::Builder::Builder(Materials& materials) :
		materials{ materials }
	{}



	void Scene::Materials::Builder::build() const
	{
		BasicMaterialsBuilder::build(materials);

		initFocusedObject(materials.focusedObject);
		initOutline(materials.outline);
		initFoggy(materials.foggy);
		initFlyby(materials.flyby);
		initReflection(materials.reflection);
		initRimLighting(materials.rimLighting);
		initInfoText(materials.infoText);

		for (int i = 0; i < materials.chessMaterials.size(); i++)
		{
			materials.chessMaterials[i].tex1 = resources.textures[static_cast<int>(TextureIndexer::BishopUS) + i];
		}
	}



	void Scene::Materials::Builder::initFocusedObject(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		//material.ambientColor = vec3(0.03f);
		material.lightColor = vec3(0.9f, 1, 1) * 1.f;
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
	}

	void Scene::Materials::Builder::initOutline(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::White];
		material.ambientColor = vec3(1, 0, 0);
		material.lightColor = vec3();
		material.lightSpecularStrength = 0;
		material.shininess = 1;
	}

	void Scene::Materials::Builder::initFoggy(FogMaterial& material) const
	{
		initFocusedObject(static_cast<PhongMaterial&>(material));

		material.fogStart = 4;
		material.fogRange = 0.5f;
	}



	void Scene::Materials::Builder::initFlyby(Quad4Material& material) const
	{
		material.tex1 = resources.textures[TextureIndexer::AwesomeFace];
	}



	void Scene::Materials::Builder::initRimLighting(RimLightingMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(1, 0, 0) * 0.2f;
		material.lightColor = vec3(0.9f, 1, 1) * 1.f;
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;

		material.rimExponent = 3;
		material.rimColor = { 1, 0, 1 };
	}



	void Scene::Materials::Builder::initReflection(ReflectionMaterial& material) const
	{
		material.skybox = materials.skybox.cubemap;// resources.cubemaps[CubeMapIndexer::Skybox];
	}



	void Scene::Materials::Builder::initInfoText(TextMaterial& material) const
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
		void initWorldCameraOverride(CameraOrbitEntity& entity) const;
		void initFocusedObject(SimpleEntity& entity) const;
		void initOutline(SimpleEntity& entity) const;
		void initFlyby(SimpleEntity& entity) const;
		void initRimLit(SimpleEntity& entity) const;
		void initReflective(SimpleEntity& entity) const;

		//void initModel(Model& model) const;

		// hud
		void initInfoText(TextRenderer& entity) const;
		void initButtonQuitOverride(MenuButtonEntity& entity) const;
		void initButtonToggleMusic(MenuButtonEntity& entity) const;
		void initButtonLoadSceneRobotron(MenuButtonEntity& entity) const;
		void initButtonLoadSceneLobby(MenuButtonEntity& entity) const;
		void initButtonLoadSceneSeek(MenuButtonEntity& entity) const;
		void initButtonLoadScenePursue(MenuButtonEntity& entity) const;
		void initButtonLoadSceneWandering(MenuButtonEntity& entity) const;
		void initButtonLoadSceneArrival(MenuButtonEntity& entity) const;
		void initButtonLoadSceneLeaderFollowing(MenuButtonEntity& entity) const;
		void initButtonLoadSceneMultiplayer(MenuButtonEntity& entity) const;
	};



	Scene::Builder::Builder(Scene& scene) :
		BasicSceneBuilder{ scene.materials },
		scene{ scene },
		entities{ scene.entities },
		materials{ scene.materials }
	{}



//	// static (file-local) macros
//
//#define LAMBDA(x) ([this](auto& e){ (x)(e); })
//
//#define MAKE_OBJ(member, initialiser)\
//	try {\
//		scene.member = makeUniqueObjectPtr<\
//			typename decltype(scene.member)::element_type\
//		>(LAMBDA(initialiser));\
//	} catch(...) { printException(); }\
//	do{}while(0)
//



	void Scene::Builder::build() const
	{
		BasicSceneBuilder::build(entities);

		initWorldCameraOverride(entities.cameraWorld);
		initButtonQuitOverride(entities.buttonMenu);

		initFocusedObject(entities.focusedObject);
		initOutline(entities.outline);
		initFlyby(entities.flyby);
		initRimLit(entities.rimLit);
		initReflective(entities.reflective);

		initInfoText(entities.infoText);
		initButtonToggleMusic(entities.buttonToggleMusic);

		initButtonLoadSceneRobotron(entities.loadSceneButtons[0]);
		initButtonLoadSceneLobby(entities.loadSceneButtons[1]);

		initButtonLoadSceneSeek(entities.loadSceneButtons[2]);
		initButtonLoadScenePursue(entities.loadSceneButtons[3]);
		initButtonLoadSceneWandering(entities.loadSceneButtons[4]);
		initButtonLoadSceneArrival(entities.loadSceneButtons[5]);
		initButtonLoadSceneLeaderFollowing(entities.loadSceneButtons[6]);

		for (int y = 0; y < 8; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				auto& button = entities.boardButtons[x + y * 8].button;

				// init collider transform
				{
					auto& ct = button.colliderTransform;
					ct.localScale = vec3(40, 40, 1);
				}

				// init background transform
				{
					auto& bgt = button.backgroundTransform;
					bgt.localScale = vec3(2.f * vec2(button.colliderTransform.localScale), 1);
				}

				// init background
				{
					auto& bg = button.background;
					bg.program = resources.programs[ProgramIndexer::Quad4].program;
					bg.mesh = &(resources.meshes[MeshIndexer::Quad]);
					bg.material = materialButtonBackground;// &(materials.buttonMenuBackground);
				}

				// init text renderer
				{
					auto& tx = button.text;
					initTextRenderer(tx); // &(materials.buttonMenuText);
					tx.scale = vec2(1);

					vec3 const buttonHalfSize = 0.5f * button.backgroundTransform.localScale;
					tx.position = vec2(-buttonHalfSize.x, -4);
				}

				button.transform.localPosition = glm::vec3(x * 100 - 200, y * 100 - 350, 0);

				// init text renderer
				{
					auto& tx = button.text;
					tx.text = std::to_string(x) + "," + std::to_string(y);
				}

				auto* pscene = &this->scene;
				// replace "load menu scene" action with "quit" action.
				button.onClickLeft.action = [pscene, x, y]
				{
					return pscene->onCellClicked(x, y);
				};
			}
		}

		auto const initPlayer = [this](bool isPlayer2) {
			int y = isPlayer2 ? 7 : 0;
			auto& board = scene.boardPieces;
			board[0 + y * 8] = board[7 + y * 8] = { ChessPiece::Rook, isPlayer2 };
			board[1 + y * 8] = board[6 + y * 8] = { ChessPiece::Knight, isPlayer2 };
			board[2 + y * 8] = board[5 + y * 8] = { ChessPiece::Bishop, isPlayer2 };
			board[3 + y * 8] = { ChessPiece::Queen, isPlayer2 };
			board[4 + y * 8] = { ChessPiece::King, isPlayer2 };
			y = isPlayer2 ? 6 : 1;
			for (int x = 0; x < 8; ++x)
			{
				board[x + y * 8] = { ChessPiece::Pawn, isPlayer2 };
			}
		};
		initPlayer(false);
		initPlayer(true);
	}



	void Scene::Builder::initWorldCameraOverride(CameraOrbitEntity& entity) const
	{
		entity.control.minRadius = 4;
		entity.control.maxRadius = 8;
	}

	void Scene::Builder::initFocusedObject(SimpleEntity& entity) const
	{
		auto& r = entity.renderer;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.material = &(materials.focusedObject);

		auto& t = entity.transform;
		t.localPosition = { 0, -0.5f, 0 };
		t.localScale = vec3(1);
	}

	void Scene::Builder::initOutline(SimpleEntity& entity) const
	{
		auto& r = entity.renderer;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.material = &(materials.outline);

		auto& t = entity.transform;
		t.localPosition = entities.focusedObject.transform.localPosition;
		t.localScale = 1.02f * entities.focusedObject.transform.localScale;
	}

	void Scene::Builder::initFlyby(SimpleEntity& entity) const
	{
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::Quad4].program;
		r.mesh = &(resources.meshes[MeshIndexer::Quad]);
		r.material = &(materials.flyby);

		auto& t = entity.transform;
		t.localPosition = { 0.5f, -0.5f, -2 };

		auto& rb = entity.rigidbody;
		rb.localVelocity = { -0.1f, 0.1f, 0 };
	}

	void Scene::Builder::initRimLit(SimpleEntity& entity) const
	{
		auto& r = entity.renderer;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		//r.program = resources.programs[ProgramIndexer::RimLighting].program;
		//r.material = &(materials.rimLighting);
		// DEBUG
		r.program = resources.programs[ProgramIndexer::LinearFog].program;
		r.material = &(materials.foggy);

		auto& t = entity.transform;
		t.localScale = vec3(0.5f);
		t.localRotation = quat(glm::radians(vec3(70, 70, 70)));
		t.localPosition = vec3(1, 1, -1);
	}

	void Scene::Builder::initReflective(SimpleEntity& entity) const
	{
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::Reflection].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.reflection);

		auto& t = entity.transform;
		t.localScale = vec3(0.5f);
		t.localRotation = quat(glm::radians(vec3(-70, -70, -70)));
		t.localPosition = vec3(-3.45f, 0, 0);
	}

	void Scene::Builder::initInfoText(TextRenderer& entity) const
	{
		initTextRenderer(entity, &(materials.infoText));

		// TODO update info text for any new features
		entity.text = "I/K/J/L to orbit camera.  M to toggle music.  RMB to play sfx.";
		entity.scale = vec2(1.2f);
	}

	void Scene::Builder::initButtonQuitOverride(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;

		// replace "load menu scene" action with "quit" action.
		button.onClickLeft.action = []
		{
			glutLeaveMainLoop();
			return RC_SUCCESS;
		};

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "Quit (ESC)";
		}
	}

	void Scene::Builder::initButtonToggleMusic(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('m');
		button.onClickLeft.hotkeys.push_back('M');
		button.onClickLeft.action = toggleMusicPaused;
	}

	namespace
	{
		auto makeSceneLoaderCallback(SceneIndexer i)
		{
			return [i]
			{
				if (isMusicPlaying())
				{
					HANDLE_ALL(stopMusic());
				}
				singleton::postLoadSceneEvent(i);

				return playSoundEffect(g_soundDing);
			};
		}
	}

	void Scene::Builder::initButtonLoadSceneSeek(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('3');
		button.onClickLeft.action = makeSceneLoaderCallback(SceneIndexer::Seek);

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "Seek (3)";
		}
	}

	void Scene::Builder::initButtonLoadScenePursue(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('4');
		button.onClickLeft.action = makeSceneLoaderCallback(SceneIndexer::Pursue);

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "Pursue (4)";
		}
	}

	void Scene::Builder::initButtonLoadSceneWandering(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('5');
		button.onClickLeft.action = makeSceneLoaderCallback(SceneIndexer::Wandering);

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "Wandering (5)";
		}
	}

	void Scene::Builder::initButtonLoadSceneArrival(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('6');
		button.onClickLeft.action = makeSceneLoaderCallback(SceneIndexer::Arrival);

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "Arrival (6)";
		}
	}
	
	void Scene::Builder::initButtonLoadSceneLeaderFollowing(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('7');
		button.onClickLeft.action = makeSceneLoaderCallback(SceneIndexer::LeaderFollowing);

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "Leader Following (7)";
		}
	}

	void Scene::Builder::initButtonLoadSceneRobotron(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('1');
		button.onClickLeft.action = makeSceneLoaderCallback(SceneIndexer::Robotron);

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "Robotron (1)";
		}
	}

	void Scene::Builder::initButtonLoadSceneLobby(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('2');
		button.onClickLeft.action = makeSceneLoaderCallback(SceneIndexer::Lobby);

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "Multiplayer (2)";
		}
	}



#pragma endregion ~Scene::Builder
#pragma region Scene



	ReturnCode Scene::onCellClicked(int x, int y)
	{
		if (selectedCoords)
		{
			if (*selectedCoords == ivec2(x, y))
			{
				selectedCoords.reset();
				return RC_SUCCESS;
			}

			else
			{
				isCurrentPlayerTwo = !isCurrentPlayerTwo;
				DEBUG_LOG("Next player's turn");
				return RC_SUCCESS;
			}
		}
		else
		{
			auto& piece = boardPieces[x + y * 8];
			bool const isPlayer2 = piece.isPlayer2;
			int const yForward = isPlayer2 ? -1 : 1;
			switch (piece.type) {
			case ChessPiece::Pawn:
			{
				auto& other = boardPieces[(x)+(y + yForward) * 8];
				if (other.type == ChessPiece::None)
				{
					other = piece;
					piece = { ChessPiece::None };
					return RC_SUCCESS;
				}
				//else if (other.isPlayer2 != piece.isPlayer2)
				//{
				//	// TODO gain money from capture
				//	other = piece;
				//	piece = { ChessPiece::None };
				//	return RC_SUCCESS;
				//}
				return RC_ERROR;
			}
			case ChessPiece::King:
			{
				auto& other = boardPieces[(x)+(y + yForward) * 8];
				if (other.type == ChessPiece::None)
				{
					other = piece;
					piece = { ChessPiece::None };
					return RC_SUCCESS;
				}
				else if (other.isPlayer2 != piece.isPlayer2)
				{
					// TODO gain money from capture
					other = piece;
					piece = { ChessPiece::None };
					return RC_SUCCESS;
				}
			}
			case ChessPiece::None:
			default:
				break;
			}
			return RC_ERROR;


			/*
			auto& piece = boardPieces[x + y * 8];
			if (piece.type == ChessPiece::None)
			{
				return RC_ERROR;
			}

			bool const isPlayer2 = piece.isPlayer2;
			if (isPlayer2)
			{
				return RC_ERROR;
			}

			selectedCoords = ivec2(x, y);
			return RC_SUCCESS;*/
		}
	}

	ReturnCode Scene::init()
	{
		materials.init();
		Builder{ *this }.build();
		return playMusic(); // start background music playing
	}



	ReturnCode Scene::update()
	{
		BEGIN_ANYALL();
		{
			if (getMouseButtonState(MOUSE_RIGHT) == InputState::DownFirst)
			{
				DO_ANYALL(playSoundEffect(g_soundThump));
			}

			auto& e = entities;

			e.cameraWorld.update();
			DO_ANYALL(e.focusedObject.update());
			DO_ANYALL(e.flyby.update());
			DO_ANYALL(e.rimLit.update());
			DO_ANYALL(e.reflective.update());

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
				//vec2 const offset = vec2(buttonHalfSize.x * 2, 0);
				vec2 const offset = vec2(0, -buttonHalfSize.y * 2);
				size_t i = 1;
				e.buttonToggleMusic.button.transform.localPosition = vec3(menuButtonPos + offset * static_cast<float>(i++), 0);
				for (auto& loadButton : e.loadSceneButtons)
				{
					loadButton.button.transform.localPosition = vec3(menuButtonPos + offset * static_cast<float>(i++), 0);
				}

				//e.infoText.position = menuButtonPos + vec2(0, -80);
				e.infoText.position = vec2(-hudHalfSize.x + 30, -hudHalfSize.y + 20);
			}
			DO_ANYALL(e.buttonToggleMusic.update());
			for (auto& loadButton : e.loadSceneButtons)
			{
				DO_ANYALL(loadButton.update());
			}
			for (auto& b : e.boardButtons)
			{
				DO_ANYALL(b.update());
			}
			DO_ANYALL(e.buttonMenu.update());



			// update mouse picking
			{
				// Screen pos
				glm::ivec2 const mousePos = getMousePos();
				glm::vec2 const normalizedScreenPos = glm::vec2(
					(2.0f * mousePos.x) / (float)screen::getWidth() - 1.0f,
					1.0f - (2.0f * mousePos.y) / (float)screen::getHeight()
				);
				// Screen pos to Proj space
				glm::vec4 const clipCoords = glm::vec4(
					normalizedScreenPos.x,
					normalizedScreenPos.y,
					-1,
					1
				);
				// Proj space to eye space
				glm::mat4 const invProjMat = glm::inverse(e.cameraWorld.camera.projection.projectionMatrix);
				glm::vec4 const eyeCoordsTemp = invProjMat * clipCoords;
				glm::vec4 const eyeCoords = glm::vec4(eyeCoordsTemp.x, eyeCoordsTemp.y, -1, 0);
				// eye space to world space
				glm::mat4 const invViewMat = glm::inverse(e.cameraWorld.camera.transform.viewMatrix);
				glm::vec4 const rayWorld = invViewMat * eyeCoords;
				glm::vec3 const rayDirection = glm::normalize(glm::vec3(rayWorld));

				// check intersection with other objects

				// ray vs sphere
				auto const& obj = e.focusedObject;
				auto const& s = obj.transform.localScale;

				glm::vec3 const rayVector = rayDirection * 10.0f;

				float const radius = std::max({ s.x, s.y, s.z });
				glm::vec3 const v = obj.transform.localPosition - e.cameraWorld.camera.transform.eye;
				float const a = glm::length2(rayVector);
				float const b = 2 * glm::dot(v, rayVector);
				float const c = glm::length2(v) - radius * radius;
				float const d = b * b - 4 * a * c;

				bool const doesIntersect = [&] {
					if (d > 0) {
						float const sr = std::sqrt(d);
						float const x1 = (-b - sr) / 2;
						float const x2 = (-b + sr) / 2;
						return true;
						// TODO check x1 and x2 if within range
					}
					else /*if (d <= 0)*/ {
						return false;
					}
				}();

				if (doesIntersect)
				{
					// hover effect
					auto* p = dynamic_cast<PhongMaterial*>(obj.renderer.material);
					if (p) {
						p->ambientColor = vec3(0, 0.5f, 0);
					}

					if (InputState::DownFirst == getMouseButtonState(GLUT_LEFT_BUTTON)) {
						DEBUG_LOG("You clicked the focused object!");
					}
				}
				else
				{
					// not hovered effect
					auto* p = dynamic_cast<PhongMaterial*>(obj.renderer.material);
					if (p) {
						p->ambientColor = vec3(0, 0, 0);
					}
				}
			}
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

		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 50, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT) - 100);

		glEnable(GL_DEPTH_CLAMP);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		{
			DO_ANYALL(e.skybox.render());



			DO_ANYALL(e.flyby.render());
			DO_ANYALL(e.rimLit.render());
			DO_ANYALL(e.reflective.render());



			glEnable(GL_STENCIL_TEST);

			// draw object with stencil fill
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			DO_ANYALL(e.focusedObject.render());

			// draw outline
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			DO_ANYALL(e.outline.render());

			glDisable(GL_STENCIL_TEST);
		}
		glDisable(GL_DEPTH_TEST);

		glDisable(GL_DEPTH_CLAMP);
		glDisable(GL_SCISSOR_TEST);

		return END_ANYALL();
	}



	ReturnCode Scene::renderHud()
	{
		auto& e = entities;

		e.cameraHud.recalculate();
		e.cameraHud.useForRendering();

		BEGIN_ANYALL();
		{
			DO_ANYALL(e.infoText.render());

			e.buttonToggleMusic.button.text.text = isMusicPaused()
				? "Music On [Off] (M)"
				: "Music [On] Off (M)";

			DO_ANYALL(e.buttonToggleMusic.render());

			for (auto& loadButton : e.loadSceneButtons)
			{
				DO_ANYALL(loadButton.render());
			}
			DO_ANYALL(e.buttonMenu.render());

			for (int y = 0; y < 8; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					auto& piece = boardPieces[x + y * 8];
					auto& button = e.boardButtons[x + y * 8];
					if (piece.type != ChessPiece::None)
					{
						button.button.text.text = stringLink(symbol(piece.type), " ", (piece.isPlayer2 ? "2" : "1"));
						button.button.background.material = &(materials.chessMaterials[GetPieceType(x, y)]);
						DO_ANYALL(button.render());
					}
				}
			}
		}
		return END_ANYALL();
	}

	int Scene::GetPieceType(int x, int y)
	{
		// return int in range 0..<12
		switch (this->boardPieces[x + y * 8].type)
		{
			case ChessPiece::Bishop:
				{
				return this->boardPieces[x + y * 8].isPlayer2
					? 1
					: 0;
				}
				
			case ChessPiece::King:
			{
				return this->boardPieces[x + y * 8].isPlayer2
					? 1
					: 0;
			}
		
			case ChessPiece::Knight:
			{
				return this->boardPieces[x + y * 8].isPlayer2
					? 1
					: 0;
			}
			case ChessPiece::Pawn:
			{
				return this->boardPieces[x + y * 8].isPlayer2
					? 1
					: 0;
			}
			case ChessPiece::Queen:
			{
				return this->boardPieces[x + y * 8].isPlayer2
					? 1
					: 0;
			}
			case ChessPiece::Rook:
			{
				return this->boardPieces[x + y * 8].isPlayer2
					? 1
					: 0;
			}
			case ChessPiece::None:
			{
				return (0);
			}
			default:
			{	break; }
		}
			//button.button.background.material = &(materials.chessMaterials;
		return x, y;
	}

#pragma endregion ~Scene
}
