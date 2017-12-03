/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */



#ifndef SOCIALLEARNINGCONTROLLER_H
#define SOCIALLEARNINGCONTROLLER_H

#include <chrono>
#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Controllers/Controller.h"
#include "WorldModels/RobotWorldModel.h"
#include "neuralnetworks/MLP.h"
#include "Utilities/LogManager.h"
#include "Utilities/Stack.h"
#include "Utilities/Vector.h"
#include "core/Solution/Solution.h"

class SocialLearningController : public Controller
{
	private:
		//Neural::MLP *_mlp;
		Solution _solution;
		double _leftMotor = 0.0;
		double _rightMotor = 0.0;
		double _currentTransactional = 0.0;
		double _currentRotational = 0.0;
		double _sigma = 0.0;

		void runMotorUpdate(Solution& solution);
		double calculateFitness();
		Solution tournamentSelection(Vector<Solution> solutions);
		Solution uniformCrossOver(Solution &parent1, Solution &parent2);
		void uniformMutate(Solution &solution);
		void setNewSolution(const Solution &solution);
		bool elapsed(const std::chrono::high_resolution_clock::time_point& startTime, double seconds);
		void recover();
		std::vector<double> getNewGenome();
		std::vector<double> getNewUniformWeights();

	public:
		//Initializes the variables
		SocialLearningController( RobotWorldModel *__wm );

		Solution& getSolution() { return this->_solution; };
		void evaluate(Solution &solution);
		void evolution(Vector<Solution> &solutions);
		void step() override;
		void reset() override {};
		void individualLearning();
		void socialLearning(Solution&);
		void reEvaluation();
		void resetGenomeMemome();

};

#endif

