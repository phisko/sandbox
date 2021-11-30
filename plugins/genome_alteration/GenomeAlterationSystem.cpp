#include "kengine.hpp"
#include "helpers/pluginHelper.hpp"
#include "Export.hpp"

#include "data/AdjustableComponent.hpp"

#include "functions/Execute.hpp"

#include "meta/ForEachEntity.hpp"
#include "meta/Get.hpp"
#include "meta/Size.hpp"

#include "helpers/meta/attributeHelper.hpp"

#include "meta/GenomeComponent.hpp"

#include "BitFieldRef.hpp"
#include "rand.hpp"

namespace {
	static float g_timeSinceLastMutation = 0.f;
	static float g_timeBetweenMutations = 1.f;
	static float g_mutationChance = 0.1f;

    struct GenomeAlterationSystem {
        static void init(kengine::Entity & e) noexcept {
            e += kengine::functions::Execute{ execute };
            e += kengine::AdjustableComponent{
                "Genome alteration", {
                    { "Time between mutations", &g_timeBetweenMutations },
                    { "Mutation chance", &g_mutationChance }
                }
            };
        }

        static void execute(float deltaTime) noexcept {
            g_timeSinceLastMutation += deltaTime;
            if (g_timeSinceLastMutation < g_timeBetweenMutations)
                return;
            g_timeSinceLastMutation -= g_timeBetweenMutations;

            for (const auto & [typeEntity, genome, get, size, forEachEntity] : kengine::entities.with<meta::GenomeComponent, kengine::meta::Get, kengine::meta::Size, kengine::meta::ForEachEntity>()) {
                forEachEntity([&](kengine::Entity e) {
                    if (putils::rand(0.f, 1.f) > g_mutationChance)
                        return;

                    const auto comp = (char*)get(e);
                    if (genome.attributes.empty())
                        mutate(comp, size.size);
                    else
                        mutateRandomAttribute(comp, typeEntity, genome);
                });
            }
        }

        static void mutateRandomAttribute(char * comp, const kengine::Entity & typeEntity, const meta::GenomeComponent & genome) noexcept {
            const auto mutatedAttributeIndex = putils::rand<size_t>(0, genome.attributes.size());
            const auto & mutatedAttribute = genome.attributes[mutatedAttributeIndex];

            const auto attributeInfo = kengine::meta::attributeHelper::findAttribute(typeEntity, mutatedAttribute.name);
            if (attributeInfo)
                mutatedAttribute.mutate(comp + attributeInfo->offset, attributeInfo->size, mutate);
        }

        static void mutate(void * data, size_t bytes) noexcept {
            const auto mutationIndex = putils::rand<size_t>(0, bytes * 8);

            putils::BitFieldRef bitField(data);
            bitField.toggle(mutationIndex);
        }
	};
}

EXPORT void loadKenginePlugin(void * state) noexcept {
    kengine::pluginHelper::initPlugin(state);
    kengine::entities += GenomeAlterationSystem::init;
}