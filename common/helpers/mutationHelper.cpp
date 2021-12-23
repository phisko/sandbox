#include "mutationHelper.hpp"

// kengine
#include "data/NameComponent.hpp"
#include "meta/Get.hpp"
#include "meta/Size.hpp"
#include "helpers/meta/attributeHelper.hpp"

// project
#include "meta/GenomeComponent.hpp"

// putils
#include "BitFieldRef.hpp"
#include "rand.hpp"

namespace mutationHelper {
    namespace {
        void mutateImpl(void * data, size_t bytes) noexcept {
           const auto mutationIndex = putils::rand<size_t>(0, bytes * 8);

           putils::BitFieldRef bitField(data);
           bitField.toggle(mutationIndex);
       } 
    }

    void mutate(kengine::Entity & e, const kengine::Entity & type) noexcept {
        const auto comp = (char *)type.get<kengine::meta::Get>()(e);
        const auto size = type.get<kengine::meta::Size>().size;

        const auto & genome = type.get<meta::GenomeComponent>();
        if (genome.attributes.empty()) {
            mutateImpl(comp, size);
            return;
        }

        const auto mutatedAttributeIndex = putils::rand<size_t>(0, genome.attributes.size());
        const auto & mutatedAttribute = genome.attributes[mutatedAttributeIndex];

        const auto attributeInfo = kengine::meta::attributeHelper::findAttribute(type, mutatedAttribute.name);
        if (!attributeInfo) {
            const auto & typeName = type.get<kengine::NameComponent>();
			kengine_assert_failed("Unknown attribute '", mutatedAttribute.name, "' in '", typeName.name, "'");
            return;
        }

        mutatedAttribute.mutate(comp + attributeInfo->offset, attributeInfo->size, mutateImpl);
    }
}
