#include "kengine.hpp"
#include "helpers/pluginHelper.hpp"
#include "Export.hpp"

// kengine
#include "data/AdjustableComponent.hpp"
#include "functions/Execute.hpp"
#include "meta/ForEachEntity.hpp"
#include "helpers/meta/attributeHelper.hpp"

// project
#include "meta/GenomeComponent.hpp"
#include "helpers/mutationHelper.hpp"

// putils
#include "rand.hpp"

namespace {
	float g_timeSinceLastMutation = 0.f;
	float g_timeBetweenMutations = 1.f;
	float g_mutationChance = 0.1f;
    int g_minMutationCount = 1;
    int g_maxMutationCount = 10;

    struct GenomeAlterationSystem {
        static void init(kengine::Entity & e) noexcept {
            e += kengine::functions::Execute{ execute };
            e += kengine::AdjustableComponent{
                "Genome alteration", {
                    { "Time between mutations", &g_timeBetweenMutations },
                    { "Mutation chance", &g_mutationChance },
                    { "Mutation count (min)", &g_minMutationCount },
                    { "Mutation count (max)", &g_maxMutationCount }
                }
            };
        }

        static void execute(float deltaTime) noexcept {
            g_timeSinceLastMutation += deltaTime;
            if (g_timeSinceLastMutation < g_timeBetweenMutations)
                return;
            g_timeSinceLastMutation -= g_timeBetweenMutations;

            for (const auto & [typeEntity, genome, forEachEntity] : kengine::entities.with<meta::GenomeComponent, kengine::meta::ForEachEntity>()) {
                forEachEntity([&](kengine::Entity e) {
                    if (putils::rand(0.f, 1.f) > g_mutationChance)
                        return;
                    const auto mutationCount = putils::rand(g_minMutationCount, g_maxMutationCount);
                    for (int i = 0; i < mutationCount; ++i)
                        mutationHelper::mutate(e, typeEntity);
                });
            }
        }
	};
}

EXPORT void loadKenginePlugin(void * state) noexcept {
    kengine::pluginHelper::initPlugin(state);
    kengine::entities += GenomeAlterationSystem::init;
}