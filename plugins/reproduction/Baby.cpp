#include "Baby.hpp"

// kengine
#include "data/GraphicsComponent.hpp"
#include "data/NameComponent.hpp"
#include "data/TransformComponent.hpp"
#include "meta/Copy.hpp"
#include "meta/Get.hpp"
#include "meta/Has.hpp"
#include "meta/Size.hpp"
#include "helpers/meta/attributeHelper.hpp"

// project
#include "meta/GenomeComponent.hpp"
#include "helpers/mutationHelper.hpp"

// putils
#include "BitFieldRef.hpp"
#include "rand.hpp"

namespace {
    void mixImpl(const void * src, void * dst, size_t size) noexcept {
        putils::BitFieldRef dstBits(dst);
        const putils::ConstBitFieldRef srcBits(src);

        // Set every other bit to this parent's "chromosome" (the other bit came from the first parent)
        for (size_t i = 0; i < size * 8; ++i)
            if (i % 2)
                dstBits.set(i, srcBits.get(i));
    }
    
    void mix(void * baby, const void * secondParentComp, const kengine::Entity & type, const kengine::meta::Size & size, const meta::GenomeComponent & genome) noexcept {
        if (genome.attributes.empty()) {
            mixImpl(secondParentComp, baby, size.size);
            return;
        }

        for (const auto & attribute : genome.attributes) {
            const auto info = kengine::meta::attributeHelper::findAttribute(type, attribute.name);
            if (info) {
                const auto babyAttribute = (char *)baby + info->offset;
                const auto parentAttribute = (char *)secondParentComp + info->offset;

                // Get mutatable data for baby
                attribute.mutate(babyAttribute, info->size, [&](void * babyData, size_t) {
                    // Get mutatable data for parent
                    attribute.mutate(parentAttribute, info->size, [&](void * parentData, size_t dataSize) {
                        // Mix mutatable datas
                        mixImpl(parentData, babyData, dataSize);
                    });
                });
            }
        }
    }
}

void Baby::operator()(kengine::Entity & baby) const noexcept {
    static size_t babyNumber = 0;
    baby += kengine::NameComponent{ kengine::NameComponent::string("Baby%zu", babyNumber++) };
    baby += kengine::TransformComponent{
        .boundingBox = {
            .position = { 2.f + babyNumber, 2.f + babyNumber, 2.f + babyNumber },
            .size = { 1.f, 1.f, 1.f }
        }
    };

    for (const auto& [type, has, get, copy, size, genome] : kengine::entities.with<kengine::meta::Has, kengine::meta::Get, kengine::meta::Copy, kengine::meta::Size, meta::GenomeComponent>()) {
        if (has(firstParent)) {
            copy(firstParent, baby);
            if (has(secondParent))
                mix(get(baby), get(secondParent), type, size, genome);
        }
        else if (has(secondParent))
            copy(secondParent, baby);
        else
            continue;

        extern int g_minMutationCount;
        extern int g_maxMutationCount;
        const auto mutationCount = putils::rand(g_minMutationCount, g_maxMutationCount);
        for (int i = 0; i < mutationCount; ++i)
            mutationHelper::mutate(baby, type);
    }
}
