#include "AI.h"

AI::AI() {
	// seed the randomizer
	// create an nx1 matrix
	// assign values from range of -#inputs/2 to +#inputs/2 with mean of 0
	// set the synaptic_weights = for example: 2 * rand[#inputs][2] - 1
}
float AI::huristic() {
	// return HP of AI - HP of player <- we are optimizing for this
	return 0;
}
float AI::sigmoid(float x) {
	// activation function : sugested a sigmoid describing an s shaped curve
	// pass weited sum of inputs through this function to get a probability
	// We are trying to normalize them between 0 to 1
	// return 1 / 1+(exp(x))
	// exp: calculate the exponential
	return 0;
}
float AI::sigmoid_derivative(float x) {
	// calculates the gradient
	// return x * (1-x)
	return 0;
}
float AI::train(float * inputs, float * outputs, int  training_itteration) {
	// maximize the huristic
	// for i in training_itteration
	// pass the training set through the network
	// predictedOutput = AI::predict(inputs)
	// calculate error
	//error = outputs - predictedOutput;
	//multiply error by inputs and gradient of sigmoid curve
	// adjustment = dot(transpose(input), error * sigmoid_derivative(output))
	// synaptic_weights += adjustment;
	return 0;
}
float AI::predict(float inputs) {
	// pass inputs through nural network
	// return sigmoid(dot(inputs,synaptic_weights))
	return 0;
}
char AI::getMove(char AIID) {
	char output = 0;
	switch (AIID)
	{
	case 'a':
	case 'A':
		output = 'w';
		break;
	case 't':
	case 'T':
		output = '8';
		break;
	}
	return output;
}