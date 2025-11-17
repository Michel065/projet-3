#include "strategie/ProductionPolynomiale.hpp"

ProductionPolynomiale::ProductionPolynomiale(float p00, float p10, float p01,
                                             float p11, float p20, float p02,
                                             float p21, float p12, float p03): 
    m_p00(p00),m_p10(p10),m_p01(p01),m_p11(p11),
    m_p20(p20),m_p02(p02),m_p21(p21),m_p12(p12),
    m_p03(p03)
    {

}

float ProductionPolynomiale::compute(float x, float y) const
{
    float x2 = x * x;
    float y2 = y * y;
    float y3 = y2 * y;

    return
        m_p00 +
        m_p10 * x +
        m_p01 * y +
        m_p11 * x * y +
        m_p20 * x2 +
        m_p02 * y2 +
        m_p21 * x2 * y +
        m_p12 * x * y2 +
        m_p03 * y3;
}
