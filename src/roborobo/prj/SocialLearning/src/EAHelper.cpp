//
// Created by 40161642 on 27/05/2016.
//

#include "SocialLearning/include/EAHelper.h"
#include "RoboroboMain/roborobo.h"

void EAHelper::mutateMemome(std::vector<double> &weights, double sigma)
{
    for (size_t i = 0, ilen = weights.size(); i < ilen; ++i)
    {
        if (weights[i] != gDisabledWeightValue) {
            weights[i] += getGaussianRand(0,1) * sigma;
            if (weights[i] > gWeightRangeValue) {
                weights[i] = gWeightRangeValue;
            }
            if (weights[i] < (-1*gWeightRangeValue)) {
                weights[i] = (-1*gWeightRangeValue);
            }
        }
    }
}