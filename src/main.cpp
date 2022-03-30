// stl
#include <thread>

#include "kengine.hpp"

// kengine data
#include "data/CameraComponent.hpp"
#include "data/CommandLineComponent.hpp"
#include "data/DebugGraphicsComponent.hpp"
#include "data/GraphicsComponent.hpp"
#include "data/TransformComponent.hpp"
#include "data/ViewportComponent.hpp"
#include "data/WindowComponent.hpp"

// kengine functions
#include "functions/Execute.hpp"

// kengine helpers
#include "helpers/imguiHelper.hpp"
#include "helpers/registerTypeHelper.hpp"
#include "helpers/MainLoop.hpp"

// kengine systems: log
#include "systems/log_file/LogFileSystem.hpp"
#include "systems/log_imgui/LogImGuiSystem.hpp"
#include "systems/log_stdout/LogStdoutSystem.hpp"
#include "systems/log_visual_studio/LogVisualStudioSystem.hpp"

// kengine systems: imgui
#include "systems/imgui_adjustable/ImGuiAdjustableSystem.hpp"
#include "systems/imgui_entity_editor/ImGuiEntityEditorSystem.hpp"
#include "systems/imgui_entity_selector/ImGuiEntitySelectorSystem.hpp"
#include "systems/imgui_tool/ImGuiToolSystem.hpp"

// kengine systems: rendering
#include "systems/model_creator/ModelCreatorSystem.hpp"
#include "systems/sfml/SFMLSystem.hpp"

// putils
#include "go_to_bin_dir.hpp"
#include "PluginManager.hpp"
#include "command_line_arguments.hpp"

// project
#include "meta/GenomeComponent.hpp"
#include "data/ReproductionComponent.hpp"

namespace {
    static void mutateColor(putils::NormalizedColor &color, const meta::GenomeComponent::Mutator &mutator) noexcept {
        putils::Color mutatable(toColor(color));
        mutator(&mutatable, sizeof(mutatable));
        color = toNormalizedColor(mutatable);
    }

    static void setupGenome() noexcept {
        using kengine::typeHelper::getTypeEntity;

        getTypeEntity<kengine::GraphicsComponent>() += meta::GenomeComponent{
                .attributes = {
                        {
                                .name = "color",
                                .mutate = [](void *attribute, size_t size,
                                             const meta::GenomeComponent::Mutator &mutator) noexcept {
                                    const auto color = (putils::NormalizedColor *) attribute;
                                    mutateColor(*color, mutator);
                                }
                        }
                }
        };

        getTypeEntity<kengine::DebugGraphicsComponent>() += meta::GenomeComponent{
                .attributes = {
                        {
                                .name = "elements",
                                .mutate = [](void *attribute, size_t size,
                                             const meta::GenomeComponent::Mutator &mutator) noexcept {
                                    const auto elements = (std::vector<kengine::DebugGraphicsComponent::Element> *) attribute;
                                    for (auto &element: *elements)
                                        mutateColor(element.color, mutator);
                                }
                        }
                }
        };
    }

    static void Window(kengine::Entity &e) {
        e += kengine::NameComponent{"Window"};
        e += kengine::WindowComponent{
                .name = "Sandbox"
        };
        e += kengine::CameraComponent{
                .frustum = {
                        .size = {16.f, 9.f, 0.f}
                }
        };
        e += kengine::ViewportComponent{};
    }

    struct Options {
        bool showWindow = false;
    };
}

#define refltype Options
putils_reflection_info {
    putils_reflection_attributes(
        putils_reflection_attribute(showWindow)
    );
};
#undef refltype

int main(int ac, char ** av) {
	putils::goToBinDir(av[0]);
    kengine::init(std::thread::hardware_concurrency());

    const auto args = putils::toArgumentVector(ac, av);
    const auto options = putils::parseArguments<Options>(args);
    kengine::entities += [&](kengine::Entity & e) {
        e += kengine::CommandLineComponent{ args };
    };

#if defined(_WIN32)
    if (!options.showWindow)
        ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	setupGenome();

    // log
    kengine::entities += kengine::LogStdoutSystem();

    // imgui
	kengine::entities += kengine::ImGuiAdjustableSystem();
	kengine::entities += kengine::ImGuiEntityEditorSystem();
	kengine::entities += kengine::ImGuiEntitySelectorSystem();
	kengine::entities += kengine::ImGuiToolSystem();

    // rendering
	kengine::entities += kengine::ModelCreatorSystem();
	kengine::entities += kengine::SFMLSystem();

	kengine::types::registerTypes();

	putils::PluginManager pm;
	pm.rescanDirectory("plugins", "loadKenginePlugin", kengine::getState());

	kengine::entities += Window;

	kengine::EntityID firstParentID;
	kengine::entities += [&](kengine::Entity & firstParent) {
		firstParentID = firstParent.id;

		firstParent += kengine::NameComponent{ "First parent" };
#if 0
		firstParent += kengine::GraphicsComponent{
			.appearance = "resources/textures/character.png",
			.color = putils::NormalizedColor{1.f, 1.f, 0.f}
		};
#else
		firstParent += kengine::DebugGraphicsComponent{
			.elements = {
				{
                    .color = putils::NormalizedColor{ 1.f, 1.f, 1.f, 0.5f },
					.type = kengine::DebugGraphicsComponent::Type::Box
				}
			}
		};
#endif
		firstParent += kengine::TransformComponent{};
	};

	kengine::EntityID secondParentID;
	kengine::entities += [&](kengine::Entity & secondParent) {
		secondParentID = secondParent.id;

		secondParent += kengine::NameComponent{ "Second parent" };
		secondParent += kengine::GraphicsComponent{
			.appearance = "resources/textures/character.png",
			.color = putils::NormalizedColor{ 0.f, 0.5f, 0.5f }
		};
		secondParent += kengine::TransformComponent{
			.boundingBox = {
				.position = { 1.f, 1.f, 1.f },
				.size = { 1.f, 1.f, 1.f }
			}
		};
	};

	kengine::entities[firstParentID] += ReproductionComponent{ .mate = secondParentID };
	kengine::entities[secondParentID] += ReproductionComponent{ .mate = firstParentID };

	kengine::mainLoop::timeModulated::run();

	return 0;
}
