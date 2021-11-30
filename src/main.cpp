#include <thread>

#include "go_to_bin_dir.hpp"
#include "kengine.hpp"
#include "PluginManager.hpp"

#include "data/CameraComponent.hpp"
#include "data/DebugGraphicsComponent.hpp"
#include "data/GraphicsComponent.hpp"
#include "data/TransformComponent.hpp"
#include "data/ViewportComponent.hpp"
#include "data/WindowComponent.hpp"

#include "functions/Execute.hpp"

#include "helpers/imguiHelper.hpp"
#include "helpers/registerTypeHelper.hpp"
#include "helpers/MainLoop.hpp"

#include "systems/imgui_adjustable/ImGuiAdjustableSystem.hpp"
#include "systems/imgui_entity_editor/ImGuiEntityEditorSystem.hpp"
#include "systems/imgui_entity_selector/ImGuiEntitySelectorSystem.hpp"
#include "systems/imgui_tool/ImGuiToolSystem.hpp"
#include "systems/log_stdout/LogStdoutSystem.hpp"
#include "systems/model_creator/ModelCreatorSystem.hpp"
#include "systems/sfml/SFMLSystem.hpp"

#include "types/registerTypes.hpp"

#include "meta/MutatableComponent.hpp"

int main(int, char ** av) {
	putils::goToBinDir(av[0]);

#if defined(_WIN32) && defined(KENGINE_NDEBUG)
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	kengine::init(std::thread::hardware_concurrency());

	registerTypes();

	auto transformComponent = kengine::typeHelper::getTypeEntity<kengine::TransformComponent>();
	transformComponent += meta::MutatableComponent{
		{ "boundingBox.size" }
	};

	kengine::entities += kengine::ImGuiAdjustableSystem();
	kengine::entities += kengine::ImGuiEntityEditorSystem();
	kengine::entities += kengine::ImGuiEntitySelectorSystem();
	kengine::entities += kengine::ImGuiToolSystem();
	kengine::entities += kengine::LogStdoutSystem();

	kengine::entities += kengine::ModelCreatorSystem();
	kengine::entities += kengine::SFMLSystem();

	putils::PluginManager pm;
	pm.rescanDirectory("plugins", "loadKenginePlugin", kengine::getState());

	kengine::entities += [](kengine::Entity & window) {
		window += kengine::WindowComponent{
			.name = "Sandbox"
		};
	};

	kengine::entities += [](kengine::Entity & character) {
		character += kengine::GraphicsComponent{ .appearance = "resources/textures/character.png" };

		character += kengine::DebugGraphicsComponent{
			.elements = {
				{
					.pos = { 0.f, -1.f, 1.f },
					.color = { 1.f, 0.f, 0.f, .5f },
					.type = kengine::DebugGraphicsComponent::Type::Box
				},
				{
					.pos = { 0.f, 1.f, 1.f },
					.color = { 0.f, 1.f, 0.f, 1.f },
					.line = {.end = { 5.f, 5.f, 0.f }},
					.type = kengine::DebugGraphicsComponent::Type::Line
				},
				{
					.pos = { -1.f, 0.f, 1.f },
					.color = { 0.f, 0.f, 1.f, 1.f },
					.type = kengine::DebugGraphicsComponent::Type::Sphere
				},
			}
		};

		auto & transform = character.attach<kengine::TransformComponent>();
		auto & camera = character.attach(kengine::CameraComponent{
			.frustum = {
				.size = {16.f, 9.f, 0.f}
			}
		});
		character += kengine::ViewportComponent{};
		
		character += kengine::functions::Execute{
			[id = character.id, &transform, &camera](float deltaTime) {
				if (ImGui::Begin("Character")) {
					auto e = kengine::entities[id];
					kengine::imguiHelper::editEntity(e);
				}
				ImGui::End();

				camera.frustum.position = transform.boundingBox.position;
			}
		};
	};

	kengine::mainLoop::timeModulated::run();

	return 0;
}
