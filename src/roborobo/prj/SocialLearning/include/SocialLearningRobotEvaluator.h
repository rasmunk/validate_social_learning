//
// Created by admin on 29-05-2016.
//

#ifndef ROBOROBO_SOCIALLEARNINGROBOTEVALUATOR_H
#define ROBOROBO_SOCIALLEARNINGROBOTEVALUATOR_H

#include "core/Evaluators/Evaluator.h"
#include "Utilities/SharedMap.h"
#include "Utilities/SharedStack.h"
#include "Utilities/SharedVector.h"
#include "SocialLearningController.h"
#include "core/Utilities/Barrier.h"
#include "core/Solution/Solution.h"
#include <thread>
#include <atomic>

class SocialLearningRobotEvaluator : public Evaluator {
private:
    std::thread *_worker;
    Barrier *_barrier;
    SocialLearningController *_socialController;
    std::shared_ptr<SharedVector<Solution>> _sharedGenomes;
    std::shared_ptr<SharedStack<Solution>> _sharedMemomes;
    LogManager *_log;
    std::ofstream _file;
    bool _finishedFlag = false;
    void logSetup();

public:
    SocialLearningRobotEvaluator(Controller *controller);
    SocialLearningRobotEvaluator(Controller *controller, std::map<std::string, std::shared_ptr<SharedDataStructure>> &sharedStorage, Barrier *evolutionBarrier);
    ~SocialLearningRobotEvaluator();

    void evaluateController();
    void genomeSharing();
    void memomeSharing();
    virtual bool finished() { return this->_finishedFlag; } // check whether the thread has finished execution at this moment.
};

#endif //ROBOROBO_SOCIALLEARNINGROBOTEVALUATOR_H
