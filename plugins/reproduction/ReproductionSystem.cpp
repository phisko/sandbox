#include "kengine.hpp"
#include "helpers/pluginHelper.hpp"
#include "Export.hpp"

// kengine
#include "data/AdjustableComponent.hpp"
#include "functions/Execute.hpp"
#include "helpers/meta/attributeHelper.hpp"

// project
#include "data/ReproductionComponent.hpp"
#include "Baby.hpp"

int g_minMutationCount = 10;
int g_maxMutationCount = 20;

namespace {
    struct ReproductionSystem {
        static void init(kengine::Entity & e) noexcept {
            e += kengine::functions::Execute{ execute };
            e += kengine::AdjustableComponent{
                "Reproduction", {
                    { "Mutation count (min)", &g_minMutationCount },
                    { "Mutation count (max)", &g_maxMutationCount }
                }
            };
        }

        static void execute(float deltaTime) noexcept {
            for (auto [e, reproduction] : kengine::entities.with<ReproductionComponent>()) {
                reproduce(e, kengine::entities[reproduction.mate]);
                e.detach<ReproductionComponent>();
            }
        }

        static void reproduce(const kengine::Entity & e, const kengine::Entity & mate) noexcept {
            kengine::entities += Baby{ e, mate };
        }
    };
}

EXPORT void loadKenginePlugin(void * state) noexcept {
    kengine::pluginHelper::initPlugin(state);
    kengine::entities += ReproductionSystem::init;
}