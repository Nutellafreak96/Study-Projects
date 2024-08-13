#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "nn_struc.h"

#define e 2.71828



/**
 * @brief Computes the sigmoid function for a given input.
 *
 * The sigmoid function is defined as 1 / (1 + e^(-n)), where e is Euler's number.
 * It is commonly used in machine learning, particularly in logistic regression and neural networks.
 *
 * @param n The input value for which the sigmoid function is to be computed.
 * @return The computed sigmoid value, which will be in the range (0, 1).
 */

double sigmoid_function(double n);

/**
 * @brief Computes the derivative of the sigmoid function for a given input.
 *
 * The derivative of the sigmoid function is given by sigmoid(n) * (1 - sigmoid(n)).
 * This function uses the previously defined sigmoid_function to compute its value.
 * The derivative is used in various optimization algorithms in machine learning.
 *
 * @param n The input value for which the derivative of the sigmoid function is to be computed.
 * @return The computed derivative of the sigmoid function.
 */
double sigmoid_derived(double n);

/**
 * @brief Calculates the categorical cross-entropy loss for a multi-class classification problem.
 *
 * @param predicted_output The output layer of the neural network after the forward pass (softmax activated).
 * @param target_output The true labels for the data sample, in one-hot encoded format.
 * @param output_neurons The number of neurons in the output layer.
 * @return The calculated categorical cross-entropy loss.
 */
double cost_function(double *predicted_output, double *target_output, int num_output_neurons);

/**
 * @brief Performs forward propagation through the neural network.
 *
 * This function calculates the activations of each neuron in the hidden layers
 * and the output layer based on the input values and the network's weights.
 *
 * @param input_layer A 2D array representing the input layer (including bias).
 * @param hidden_layers A 3D array representing the hidden layers (including biases).
 * @param output_layer A 1D array representing the output layer.
 * @param input_neurons The number of neurons in the input layer (excluding bias).
 * @param hidden_neurons The number of neurons in each hidden layer (excluding bias).
 * @param hidden_layers The number of hidden layers.
 * @param output_neurons The number of neurons in the output layer.
 */
void forward_propagation(struct NeuralNetwork *network, int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons);

/* Functions necessary for the Back Propagation*/


/**
 * @brief Compute the error term (delta) for the output layer.
 * 
 * This function calculates the error term for each neuron in the output layer based on the 
 * predicted output and the target output. The error term is used in the backpropagation algorithm.
 * 
 * @param delta_output Pointer to an array where the output deltas will be stored.
 * @param output_layer Pointer to the array representing the output layer values.
 * @param target_output Pointer to the array representing the target output values.
 * @param output_neurons Number of neurons in the output layer.
 */
void compute_output_delta(double *delta_output, double *output_layer, double *target_output, int output_neurons);

/**
 * @brief Compute the error terms (deltas) for the hidden layers.
 * 
 * This function calculates the error terms for each neuron in the hidden layers based on the 
 * deltas of the next layer (which could be another hidden layer or the output layer). 
 * The error term is used in the backpropagation algorithm.
 * 
 * @param delta_hidden Pointer to a 2D array where the hidden layer deltas will be stored.
 * @param hidden_layers_array Pointer to the 3D array representing the hidden layers.
 * @param delta_output Pointer to the array representing the output layer deltas.
 * @param hidden_neurons Number of neurons in each hidden layer.
 * @param hidden_layers Number of hidden layers in the network.
 * @param output_neurons Number of neurons in the output layer.
 */
void compute_hidden_deltas(double **delta_hidden, double ***hidden_layers_array, double *delta_output, int hidden_neurons, int hidden_layers, int output_neurons);

/**
 * @brief Update the weights and biases of the neural network.
 * 
 * This function updates the weights and biases of the neural network based on the calculated
 * error terms (deltas) for the output layer and the hidden layers. The updates are performed
 * using the backpropagation algorithm and a specified learning rate.
 * 
 * @param network Pointer to the neural network structure.
 * @param delta_hidden Pointer to a 2D array containing the hidden layer deltas.
 * @param delta_output Pointer to the array containing the output layer deltas.
 * @param input_neurons Number of neurons in the input layer.
 * @param hidden_neurons Number of neurons in each hidden layer.
 * @param hidden_layers Number of hidden layers in the network.
 * @param output_neurons Number of neurons in the output layer.
 * @param learning_rate The learning rate used for updating the weights and biases.
 */
void update_weights(struct NeuralNetwork *network, double **delta_hidden, double *delta_output, int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons, double learning_rate);

/**
 * @brief Perform backpropagation on the neural network.
 * 
 * This function orchestrates the backpropagation process by computing the error terms (deltas)
 * for the output layer and hidden layers, and then updating the weights and biases of the network.
 * 
 * @param network Pointer to the neural network structure.
 * @param target_output Pointer to the array representing the target output values.
 * @param input_neurons Number of neurons in the input layer.
 * @param hidden_neurons Number of neurons in each hidden layer.
 * @param hidden_layers Number of hidden layers in the network.
 * @param output_neurons Number of neurons in the output layer.
 * @param learning_rate The learning rate used for updating the weights and biases.
 */
void back_propagation(struct NeuralNetwork *network, double *target_output, int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons, double learning_rate);


/**
 * @brief Train the neural network using the provided inputs and targets.
 *
 * This function performs training of the neural network by iterating through a given number of epochs.
 * For each epoch, it processes each sample by performing forward propagation, calculating the loss,
 * and then performing backpropagation to adjust the weights.
 *
 * @param network Pointer to the NeuralNetwork structure to be trained.
 * @param inputs 2D array of input samples, where each row corresponds to a sample and each column to a feature.
 * @param targets 2D array of target values, where each row corresponds to a sample and each column to a target value.
 * @param input_neurons Number of neurons in the input layer.
 * @param hidden_neurons Number of neurons in each hidden layer.
 * @param hidden_layers Number of hidden layers.
 * @param output_neurons Number of neurons in the output layer.
 * @param learning_rate The learning rate for training.
 * @param epochs The number of epochs to train the network.
 * @param number_of_samples The number of training samples.
 *
 * @note This function assumes that the network structure has been initialized and that the inputs and targets arrays are properly sized.
 *       The function prints the loss at each epoch to provide feedback on the training process.
 */
void train(struct NeuralNetwork *network, double **inputs, double **targets, int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons, double learning_rate, int epochs, int number_of_samples);