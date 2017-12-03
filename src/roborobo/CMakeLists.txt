cmake_minimum_required(VERSION 3.5)
project(roborobo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
## Dependency libraries
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS system)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find boost!")
endif(Boost_FOUND)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/SDL2/lib/cmake)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(EXTRA_LIBS ${EXTRA_LIBS} ${SDL2_LIBRARIES})


## Include paths
include_directories(prj)
include_directories(include)
include_directories(include/core)
include_directories(include/contrib)
include_directories(include/ext)

set(STATIC_FILES
        include/contrib/neuralnetworks/Elman.h
        include/contrib/neuralnetworks/LayeredNeuralNetwork.h
        include/contrib/neuralnetworks/MLP.h
        include/contrib/neuralnetworks/NeuralNetwork.h
        include/contrib/neuralnetworks/NeuralNetworkException.h
        include/contrib/neuralnetworks/Perceptron.h
        include/contrib/neuralnetworks/ReservoirNeuralNetwork.h
        include/contrib/zsu/func.h
        include/contrib/zsu/Properties.h
        include/contrib/zsu/StringUtils.h
        include/contrib/zsu/strip.h
        include/core/Evaluators/Evaluator.h
        include/core/Utilities/SharedMap.h
        include/core/Agents/Agent.h
        include/core/Agents/InspectorAgent.h
        include/core/Agents/Robot.h
        include/core/Config/GlobalConfigurationLoader.h
        include/core/Controllers/Controller.h
        include/core/Observers/AgentObserver.h
        include/core/Observers/Observer.h
        include/core/Observers/WorldObserver.h
        include/core/RoboroboMain/common.h
        include/core/RoboroboMain/main.h
        include/core/RoboroboMain/roborobo.h
        include/core/Utilities/CellularMap.h
        include/core/Utilities/ExtendedProperties.h
        include/core/Utilities/Geometry.h
        include/core/Utilities/Graphics.h
        include/core/Utilities/LogManager.h
        include/core/Utilities/Misc.h
        include/core/Utilities/SDLMain.h
        include/core/Utilities/Timer.h
        include/core/Utilities/Map.h
        include/core/Utilities/Stack.h
        include/core/Utilities/SharedVector.h
        include/core/Utilities/SharedStack.h
        include/core/World/CircleObject.h
        include/core/World/PhysicalObject.h
        include/core/World/SquareObject.h
        include/core/World/World.h
        include/core/WorldModels/RobotWorldModel.h
        include/core/WorldModels/WorldModel.h
        include/ext/Config/ConfigurationLoader.h
        include/ext/Config/SocialLearningConfigurationLoader.h
        include/ext/World/EnergyItem.h
        include/ext/World/GateObject.h
        include/ext/World/LandmarkObject.h
        include/ext/World/PhysicalObjectFactory.h
        include/ext/World/RoundObject.h
        include/ext/World/SwitchObject.h
        prj/SocialLearning/include/EAHelper.h
        prj/SocialLearning/include/SocialLearningAgentObserver.h
        prj/SocialLearning/include/SocialLearningController.h
        prj/SocialLearning/include/SocialLearningRobotEvaluator.h
        prj/SocialLearning/include/SocialLearningWorldObserver.h
        prj/SocialLearning/src/EAHelper.cpp
        prj/SocialLearning/src/SocialLearningAgentObserver.cpp
        prj/SocialLearning/src/SocialLearningController.cpp
        prj/SocialLearning/src/SocialLearningRobotEvaluator.cpp
        prj/SocialLearning/src/SocialLearningWorldObserver.cpp
        src/contrib/neuralnetworks/Elman.cpp
        src/contrib/neuralnetworks/LayeredNeuralNetwork.cpp
        src/contrib/neuralnetworks/MLP.cpp
        src/contrib/neuralnetworks/NeuralNetwork.cpp
        src/contrib/neuralnetworks/NeuralNetworkException.cpp
        src/contrib/neuralnetworks/Perceptron.cpp
        src/contrib/neuralnetworks/ReservoirNeuralNetwork.cpp
        src/contrib/zsu/Properties.cpp
        src/contrib/zsu/StringUtils.cpp
        src/core/Agent.cpp
        src/core/AgentObserver.cpp
        src/core/CellularMap.cpp
        src/core/CircleObject.cpp
        src/core/Controller.cpp
        src/core/ExtendedProperties.cpp
        src/core/Geometry.cpp
        src/core/Graphics.cpp
        src/core/InspectorAgent.cpp
        src/core/LogManager.cpp
        src/core/main.cpp
        src/core/Misc.cpp
        src/core/Observer.cpp
        src/core/PhysicalObject.cpp
        src/core/roborobo.cpp
        src/core/Robot.cpp
        src/core/RobotWorldModel.cpp
        src/core/SquareObject.cpp
        src/core/Timer.cpp
        src/core/World.cpp
        src/core/WorldModel.cpp
        src/core/WorldObserver.cpp
        src/ext/ConfigurationLoader.cpp
        src/ext/EnergyItem.cpp
        src/ext/GateObject.cpp
        src/ext/LandmarkObject.cpp
        src/ext/PhysicalObjectFactory.cpp
        src/ext/RoundObject.cpp
        src/ext/SocialLearningConfigurationLoader.cpp
        src/ext/SwitchObject.cpp
        include/core/Solution/Solution.h
        include/core/Utilities/Barrier.h
        src/core/Barrier.cpp
        include/core/Utilities/SharedDataStructure.h
        include/core/Utilities/Vector.h)

## Boost Dependencies
set(SOURCE_FILES ${STATIC_FILES})
add_executable(roborobo ${SOURCE_FILES})
target_link_libraries(roborobo ${EXTRA_LIBS})
install(TARGETS roborobo DESTINATION bin)