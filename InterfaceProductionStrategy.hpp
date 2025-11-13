#pragma once

/*
pour utiliser un pattern
*/

class InterfaceProductionStrategy
{
public:
    virtual ~InterfaceProductionStrategy() = default;
    virtual float compute(float x, float y) const = 0;
};
