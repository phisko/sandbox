#pragma once

#include "kengine.hpp"

struct Baby {
    const kengine::Entity & firstParent;
    const kengine::Entity & secondParent;
    
    void operator()(kengine::Entity & baby) const noexcept;
};