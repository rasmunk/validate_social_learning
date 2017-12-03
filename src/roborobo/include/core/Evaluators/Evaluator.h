//
// Created by admin on 29-05-2016.
//

#ifndef ROBOROBO_EVALUATOR_H
#define ROBOROBO_EVALUATOR_H

#include "core/Controllers/Controller.h"

class Evaluator
{
private:
    Controller *_controller;
    Evaluator() : _controller(nullptr) {} // Make the class call the destructor if the delegated constructor fails
public:
    Evaluator(Controller *controller) : Evaluator() {
        _controller = controller;
    };
    Controller* getEvaluationController() { return this->_controller; };
    virtual bool finished() {};
};

#endif //ROBOROBO_EVALUATOR_H
