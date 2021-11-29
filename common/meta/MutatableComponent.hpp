#pragma once

namespace meta {
    struct MutatableComponent {
        std::vector<std::string> mutatableAttributes; // If empty, entire component is mutatable
    };
}