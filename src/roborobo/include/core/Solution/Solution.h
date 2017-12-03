//
// Created by 40161642 on 29/06/2016.
//

#ifndef ROBOROBO_SOLUTION_H
#define ROBOROBO_SOLUTION_H

#include "core/Utilities/Map.h"
#include "core/Utilities/Stack.h"

class Solution {
private:
    std::vector<double> _genome;
    std::vector<double> _memome;
    double _bestFitness;
    double _fitnessSize;

public:
    Solution() {};
    // Copy Constructor
    Solution(const Solution& solution) { this->_genome = solution._genome; this->_memome = solution._memome; this->_bestFitness = solution._bestFitness; this->_fitnessSize = solution._fitnessSize; };
    Solution(std::vector<double> genome, std::vector<double> memome, double bestfitness) : _genome(genome), _memome(memome), _bestFitness(bestfitness) {}
    bool operator==(const Solution& other) { return (this->_genome == other._genome && this->_memome == other._memome); };
    static bool genomeComparator(const Solution& lhs, const Solution& rhs) { return lhs._genome == rhs._genome; };
    void setGenome(std::vector<double> genome) { _genome = genome; };
    std::vector<double>& getGemome() { return _genome; };

    std::string genomeToString() {
        std::ostringstream oss;
        for (int i = 0; i<_genome.size(); ++i) {
            oss << _genome[i];
        }
        return oss.str();
    }

    void setMemome(std::vector<double> memome) { _memome = memome; };
    std::vector<double>& getMemome() { return _memome; };

    void setBestFitness(double bestFitness) { _bestFitness = bestFitness; };
    double getBestFitness() { return _bestFitness; };

    void setFitnessSize(double fitnessSize) { _fitnessSize = fitnessSize; };
    double getFitnessSize() { return _fitnessSize; };
};

#endif //ROBOROBO_SOLUTION_H
