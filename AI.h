#pragma once
#ifndef AI_H
#define AI_H

class AI {
public:
	AI();
	float huristic();
	float sigmoid(float x);
	float sigmoid_derivative(float x);
	float train(float * inputs, float * outputs,int  training_itteration);
	float predict(float inputs);
	char getMove(char AIID);
private:
	float synaptic_weights;
};

#endif