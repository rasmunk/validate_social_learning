/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */

#include "SocialLearning/include/SocialLearningController.h"
#include "SocialLearning/include/EAHelper.h"
#include <random>

SocialLearningController::SocialLearningController(RobotWorldModel *__wm) : Controller(__wm) {
    _sigma = gRobotInitialSigma;
    _solution.setGenome(getNewGenome());
    _solution.setMemome(getNewUniformWeights());
    _solution.setBestFitness(0.0);
    _solution.setFitnessSize(0);
}


void SocialLearningController::runMotorUpdate(Solution &solution) {
    _leftMotor = 0.0;
    _rightMotor = 0.0;
    double normalisedSensor;
    int offset = (solution.getMemome().size() / 2) + 1;
    for (size_t i = 0; i < _wm->_cameraSensorsNb; ++i) {
        if (solution.getGemome().at(i)) {
            // Normalised between [-1,1]i
            normalisedSensor = (((_wm->getDistanceValueFromCameraSensor(i) - 0) / (gSensorRange - 0)) - 0.5) * 2;
            _leftMotor += (solution.getMemome().at(i) * normalisedSensor);
            _rightMotor += (solution.getMemome().at(i + offset) * normalisedSensor);
        }
    }
    _leftMotor += solution.getMemome().at((solution.getMemome().size() / 2) - 1);
    _rightMotor += solution.getMemome().at(solution.getMemome().size() - 1);

    double leftActivation = tanh(_leftMotor);
    double rightActivation = tanh(_rightMotor);
    if (leftActivation > 1.0 || leftActivation < -1.0 || rightActivation > 1.0 || rightActivation < -1.0) {
        std::cout << "Warning: left activation = " << leftActivation << " right activation = " << rightActivation <<
        std::endl;
    }
    _leftMotor = (gMaxTranslationalSpeed * leftActivation);
    _rightMotor = (gMaxTranslationalSpeed * rightActivation);

    this->_currentTransactional = ((_leftMotor + _rightMotor) / 2.0);
    this->_currentRotational = ((_leftMotor) - (_rightMotor));
}

void SocialLearningController::evaluate(
        Solution &solution) // evaluates the performance of the current robot setup -> run for the number of seconds specified in the gNumberOfIndiEvals variable
{
    // recovery time
    recover();
    std::vector<double> fitnesses;
    std::chrono::high_resolution_clock::time_point totalStartTime = std::chrono::high_resolution_clock::now();
    int i = 0;
    while (!elapsed(totalStartTime, gEvaluationSeconds)) {
        runMotorUpdate(solution);
        if (i % 10 == 0) { // limit the size of the fitnesses vector
            fitnesses.push_back(calculateFitness());
        }
        ++i;
    }
    double accumulatedFitness = 0.0;
    for (unsigned int i = 0; i < fitnesses.size(); ++i) {
        accumulatedFitness += fitnesses.at(i);
    }
    double range =
            (fitnesses.size() * (gMaxTranslationalSpeed * 2.0)) - (fitnesses.size() * (-gMaxTranslationalSpeed * 2.0));
    double totalFitness = ((accumulatedFitness - (fitnesses.size() * (-gMaxTranslationalSpeed * 2.0))) / range) *
                          100.0; // percentage of maximum achiveable fitness.
    //std::cout << "Total Fitness: " << totalFitness << std::endl;
    solution.setFitnessSize(fitnesses.size());
    solution.setBestFitness(totalFitness);
}

double SocialLearningController::calculateFitness() {
    // calculate distance to nearest object -> find sensor with the closest object
    double normalisedVsens = 0.0;
    for (int i = 0; i < _wm->_cameraSensorsNb; ++i) {
        double value = fabs(((_wm->getDistanceValueFromCameraSensor(i) - gSensorRange) / (0 - gSensorRange)));
        if (normalisedVsens < value) {
            normalisedVsens = value;
        }
    }
    if (normalisedVsens > 1.0) {
        normalisedVsens = 1.0;
    }

    // Extracted from https://github.com/ci-group/Thymio_swarm/blob/master/original_code_ICES.py
    double normalisedRotationalSpeed = 0.0;
    if (_leftMotor > _rightMotor) {
        normalisedRotationalSpeed = ((_leftMotor - _rightMotor) / gMaxTranslationalSpeed);
    } else {
        normalisedRotationalSpeed = ((_rightMotor - _leftMotor) / gMaxTranslationalSpeed);
    }
    if (normalisedRotationalSpeed > 1.0) {
        normalisedRotationalSpeed = 1.0;
    }

    double translationalSpeed = (_leftMotor + _rightMotor);
    double fitness = (translationalSpeed * (1.0 - normalisedRotationalSpeed) * (1.0 - normalisedVsens));
    // gMaxtranlationalSpeed = a single motors maximum speed
    // range [2*gMaxTranslationalSpeed, 2*-gMaxTranslationalSpeed]
    return fitness;
}

Solution SocialLearningController::tournamentSelection(Vector<Solution> solutions) {
    if (solutions.size() == 0) {
        std::cout << "No solutions are available ot the tournament" << std::endl;
        std::cout << "Genome size: " << solutions.size() << std::endl;
        std::cout << "Tournament size: " << gTournamentSize << std::endl;
        exit(1);
    }

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> intDistribution(0, solutions.size() - 1);
    std::vector<Solution> tournament;
    for (unsigned int i = 0; i < gTournamentSize; ++i) {
        tournament.push_back(solutions[intDistribution(generator)]);
    }

    Solution bestSolution = tournament[0];
    for (int i = 1; i < tournament.size(); ++i) {
        if (tournament[i].getBestFitness() > bestSolution.getBestFitness()) {
            bestSolution = tournament[i];
        }
    }
    return bestSolution;
}

Solution SocialLearningController::uniformCrossOver(Solution &parent1, Solution &parent2) {
    std::vector<Solution> parents;
    parents.push_back(parent1);
    parents.push_back(parent2);
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> intDistribution(0, parents.size() - 1);

    Solution child;
    int selectedParent;
    for (int i = 0; i < parent1.getGemome().size(); ++i) {
        selectedParent = intDistribution(generator);
        child.getGemome().push_back(parents[selectedParent].getGemome()[i]);
    }
    return child;
}

void SocialLearningController::uniformMutate(Solution &solution) {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> realDistribution(0, 1);

    for (int i = 0; i < solution.getGemome().size(); ++i) {
        if (realDistribution(generator) < gMutationRate) {
            if (solution.getGemome()[i] == 1.0) {
                solution.getGemome()[i] = 0.0;
            } else {
                solution.getGemome()[i] = 1.0;
            }
        }
    }
}

bool SocialLearningController::elapsed(const std::chrono::high_resolution_clock::time_point &startTime,
                                       double seconds) {
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - startTime);
    return (elapsed.count() > seconds);
}

void SocialLearningController::recover() {
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    while (!elapsed(startTime, gRecoverySeconds));
}

void SocialLearningController::step() { // called by the underlying roborobo thread that moves the robots. -> adjust the desired translational value to take the current frame rate into account -> fixed number of pixels per second

    _wm->_desiredTranslationalValue = this->_currentTransactional;
    _wm->_desiredRotationalVelocity = this->_currentRotational;
}

void SocialLearningController::individualLearning() {
    // Save current weights if new once aren't better
    Solution currentSolution = Solution(_solution);
    Solution newSolution = Solution(_solution);

    // Mutate and enable new weights
    EAHelper::mutateMemome(newSolution.getMemome(), _sigma);
    evaluate(newSolution);
    if (newSolution.getBestFitness() > currentSolution.getBestFitness()) {
        _sigma = gRobotMinSigma;
        setNewSolution(newSolution);
    } else {
        setNewSolution(currentSolution);
        _sigma = (_sigma * gRobotSigmaIncrease);
        if (_sigma > gRobotMaxSigma) {
            _sigma = gRobotMaxSigma;
        }
    }
}

void SocialLearningController::socialLearning(Solution &lastSolution) {
    Solution currentSolution = Solution(_solution);
    Solution socialChallenger = Solution(_solution);

    for (int i = 0; i < lastSolution.getMemome().size(); ++i) {
        if (socialChallenger.getMemome()[i] != gDisabledWeightValue &&
            lastSolution.getMemome()[i] != gDisabledWeightValue) {
            socialChallenger.getMemome()[i] = lastSolution.getMemome()[i];
        }
    }

    evaluate(socialChallenger);
    if (socialChallenger.getBestFitness() > currentSolution.getBestFitness()) {
        _sigma = gRobotMinSigma;
        setNewSolution(socialChallenger);
    } else {
        setNewSolution(currentSolution);
        _sigma = (_sigma * gRobotSigmaIncrease);
        if (_sigma > gRobotMaxSigma) {
            _sigma = gRobotMaxSigma;
        }
    }
}

void SocialLearningController::reEvaluation() // reevaluates the fitness of the current setup
{
    Solution newSolution = Solution(_solution);
    evaluate(newSolution);
    this->_solution.setBestFitness((this->_solution.getBestFitness() * gRobotOldFitnessWeight) +
                                   (newSolution.getBestFitness() * (1.0 - gRobotOldFitnessWeight)));
}

void SocialLearningController::resetGenomeMemome() {
    _solution.setGenome(getNewGenome());
    _solution.setMemome(getNewUniformWeights());
}


void SocialLearningController::evolution(Vector<Solution> &solutions) {
    Solution winner = tournamentSelection(solutions);
    Solution currentSolution = Solution(_solution);
    Solution newSolution = uniformCrossOver(winner, currentSolution);
    uniformMutate(newSolution);
    setNewSolution(newSolution);
    _solution.setMemome(getNewUniformWeights());
}

std::vector<double> SocialLearningController::getNewUniformWeights() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> realDistribution((-1.0 * gWeightRangeValue), gWeightRangeValue);

    std::vector<double> weights;
    size_t x = 0;
    // fill out weights with 2x number of sensors +2 bias values
    for (int i = 0, ilen = _solution.getGemome().size() * 2; i < ilen; ++i) {
        if (x == _solution.getGemome().size()) {
            x = 0;
        }
        // Check wheather the sensor is enabled
        if (_solution.getGemome().at(x) == 1.0) {
            weights.push_back(realDistribution(generator));
        } else {
            weights.push_back(gDisabledWeightValue);
        }
        ++x;
    }
    // insert biases at the end and at the middle
    weights.push_back(realDistribution(generator));
    std::vector<double>::iterator it = weights.begin();
    weights.insert(it + (weights.size() / 2), realDistribution(generator));
    return weights;
}

std::vector<double> SocialLearningController::getNewGenome() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> realDistribution(0.0, 1.0);
    std::vector<double> newGenome;
    if (gRobotUseEvolution) { // random genome
        for (int i = 0; i < _wm->_cameraSensorsNb; ++i) {
            if (realDistribution(generator) < gDisabledSensorPercentage) {
                newGenome.push_back(0.0);
            } else {
                newGenome.push_back(1.0);
            }
        }
    }
    else { // every seonsor is active
        for (int i = 0; i < _wm->_cameraSensorsNb; ++i) {
            newGenome.push_back(1.0); // active
        }
    }
    return newGenome;
}

void SocialLearningController::setNewSolution(const Solution &solution) {
    _solution = solution;
}