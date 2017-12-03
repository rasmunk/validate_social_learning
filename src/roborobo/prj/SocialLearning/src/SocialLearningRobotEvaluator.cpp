//
// Created by admin on 29-05-2016.
//

#include "SocialLearning/include/SocialLearningRobotEvaluator.h"
#include <random>

SocialLearningRobotEvaluator::SocialLearningRobotEvaluator(Controller *controller) : Evaluator(controller) {
    _socialController = static_cast<SocialLearningController *>(this->getEvaluationController());
    _worker = new std::thread(&SocialLearningRobotEvaluator::evaluateController, this);
    this->logSetup();
}

// Evolution requires a barrier to sync the clearing of the sharedGenomes map
SocialLearningRobotEvaluator::SocialLearningRobotEvaluator(Controller *controller,
                                                           std::map<std::string, std::shared_ptr<SharedDataStructure>> &sharedStorage,
                                                           Barrier *evolutionBarrier) : Evaluator(controller) {
    _socialController = static_cast<SocialLearningController *>(this->getEvaluationController());
    // If not found the variable will remain a nullptr -> used to evaluate whether the setting is enabled -> evolution and sociallearning
    try {
        _sharedGenomes = std::dynamic_pointer_cast<SharedVector<Solution>>(sharedStorage.at("gRobotUseEvolution"));
    } catch (std::out_of_range e) {
    }
    try {
        _sharedMemomes = std::dynamic_pointer_cast<SharedStack<Solution>>(sharedStorage.at("gRobotUseSocialLearning"));
    } catch (std::out_of_range e) {
    }
    _barrier = evolutionBarrier;
    _worker = new std::thread(&SocialLearningRobotEvaluator::evaluateController, this);
    this->logSetup();
}

SocialLearningRobotEvaluator::~SocialLearningRobotEvaluator() {
    std::cout << "Thread: " << _worker->get_id() << " Destroyed" << std::endl;
    _worker->join();
    this->_log->flush();
    this->_file.close();
    delete this->_log;
}

void SocialLearningRobotEvaluator::logSetup() {
    // Initialize Social logger
    std::time_t now = std::time(0);
    std::stringstream ss;
    ss << "logs/social_experiment_";
    ss << now << "_" << this->_worker->get_id();
    ss << ".csv";
    this->_file.open(ss.str());
    if (!this->_file) {
        std::cout << "[error] couldn't open log file " << ss.str() << std::endl;
    }
    this->_log = new LogManager();
    this->_log->setLogFile(this->_file);
}


void SocialLearningRobotEvaluator::evaluateController() {
    std::uniform_real_distribution<double> realDistribution(0.0, 1.0);
    std::ostringstream oss;
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    oss <<
    "id,generation,age,fitness,genome,individuallearning,sociallearning,memomeThreadshold,evolution,maxSpeed,fitnessSize,roboVersion\n";
    int eval = 0;
    while (eval <
           gNumberOfTotalEvals) {
         // if evolution is not enabled -> the same sensor setup where every robot has all it's sensors active
         // New memome storage
        Stack<Solution> sharedMemomes;
         // New Genome storage
        Vector<Solution> sharedGenomes;
        _socialController->evaluate(_socialController->getSolution());
        double rand;
        for (int i = 0; i < gRobotLifeTime; ++i) {
            rand = realDistribution(generator);
            // reevaluation
            if (rand <= 0.2) {
                _socialController->reEvaluation();
            } else {
                // Get new random number
                rand = realDistribution(generator);
                if (rand <= 0.3 && _sharedMemomes && sharedMemomes.size() > 0) { // social learning
                    Solution newestSolution = sharedMemomes.pop();
                    _socialController->socialLearning(newestSolution);
                } else {
                    // individual learning
                    _socialController->individualLearning();
                }
            }
            // Evolution
            // add current genome performance to the shared pool
            if (_sharedGenomes) {
                genomeSharing();
                _barrier->wait();
                // Clone the current SharedVector<T> -> implicit cast to base class Vector<T>
                Vector<Solution> currentGenomes = (*_sharedGenomes).clone();
                long long int genomeSize = currentGenomes.size();
                int genomeIndex;
                for (int j = 0; j < genomeSize; ++j) {
                    if (sharedGenomes.size() < gGenomeStorageCapacity) {
                        genomeIndex = sharedGenomes.getIndexOfCustomComparator(currentGenomes.getAt(j),
                                                                               &Solution::genomeComparator);
                        // If the genome already exists -> overwrite it with the new one in the currentGenomes vector
                        if (genomeIndex >= 0) {
                            sharedGenomes.setAt(genomeIndex, currentGenomes.getAt(j));
                        } else {
                            sharedGenomes.add(currentGenomes.getAt(j));
                        }
                    }
                }
                // Barrier point -> need to all be here before flusing the shared genome storage.
                _barrier->wait();
                (*_sharedGenomes).clear();
            }
            // social learning
            // Share the current weights of the neural network -> memomes
            if (_sharedMemomes) {
                memomeSharing();
                // Wait for all memomes to be shared
                _barrier->wait();
                Stack<Solution> currentMemomes = (*_sharedMemomes).clone();
                long long int memomeSize = currentMemomes.size();
                for (int j = 0; j < memomeSize; ++j) {
                    if (sharedMemomes.size() < gMemomeStorageCapacity) {
                        sharedMemomes.push(currentMemomes.pop());
                    }
                }
                _barrier->wait();
                // Clear the shared storage for next learning
                (*_sharedMemomes).clear();
            }

            // Log Fitness
            oss << _worker->get_id() << "," << eval << "," << i << "," <<
            _socialController->getSolution().getBestFitness() << "," <<
            _socialController->getSolution().genomeToString() << "," << "1" << "," << gRobotUseSocialLearning << "," <<
            gMemomeSharingThreshold << "," << gRobotUseEvolution << "," << gMaxTranslationalSpeed << "," <<
            _socialController->getSolution().getFitnessSize() << "," << gEvolutionVersion << "\n";
        }
        // Evolution
        // Before the next robot life.
        // prepare for next generation, tournament between current genomes.
        if (_sharedGenomes) {
            if (sharedGenomes.size() > 0) {
                //genome crossover with a new random memome
                _socialController->evolution(sharedGenomes);
            } else {
                // set a new genome and memome
                _socialController->resetGenomeMemome();
            }
        }
        // Write to log
        eval++;
    }
    // Save the log after the simluation has finished
    _log->write(oss.str());
    _log->flush();
    _file.close();
    _finishedFlag = true;
}

// adds or replace a genome with a newer version -> newest fitness
void SocialLearningRobotEvaluator::genomeSharing() {
    (*_sharedGenomes).add(_socialController->getSolution());
}

// push the current solution
void SocialLearningRobotEvaluator::memomeSharing() {
    if (_socialController->getSolution().getBestFitness() > gMemomeSharingThreshold) {
        (*_sharedMemomes).push(_socialController->getSolution());
    }
}