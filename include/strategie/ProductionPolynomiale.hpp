#pragma once
#include "InterfaceProductionStrategy.hpp"

class ProductionPolynomiale : public InterfaceProductionStrategy
{
public:
    ProductionPolynomiale(float p00, float p10, float p01,
                          float p11, float p20, float p02,
                          float p21, float p12, float p03);

    float compute(float x, float y) const override;

private:
    float m_p00, m_p10, m_p01, m_p11;
    float m_p20, m_p02, m_p21, m_p12, m_p03;
};
