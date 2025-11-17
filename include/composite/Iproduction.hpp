#pragma once
class IProducteur
{
public:
    virtual ~IProducteur() = default;
    virtual float getProductionInstantanee() const = 0;
};