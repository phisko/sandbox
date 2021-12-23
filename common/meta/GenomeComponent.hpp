#pragma once

namespace meta {
    struct GenomeComponent {
        using Mutator = std::function<void(void * data, size_t size)>;
        struct Attribute {
            std::string name;
            size_t size;
            std::function<void(void* attribute, size_t size, const Mutator & mutator)> mutate = [](void* attribute, size_t size, const Mutator& mutator) {
                mutator(attribute, size);
            };
        };

        std::vector<Attribute> attributes; // If empty, all attributes are part of the genome
    };
}