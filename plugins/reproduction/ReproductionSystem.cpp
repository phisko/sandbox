
#include "kengine.hpp"
#include "helpers/pluginHelper.hpp"
#include "Export.hpp"

#include "functions/Execute.hpp"

#include "helpers/meta/attributeHelper.hpp"

#include "data/ReproductionComponent.hpp"
#include "Baby.hpp"

namespace {
    struct ReproductionSystem {
        static void init(kengine::Entity & e) noexcept {
            e += kengine::functions::Execute{ execute };
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