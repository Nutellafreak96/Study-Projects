/**
 * @file nn_struc.h
 * @brief Header file for neural network structure functions.
 */

#ifndef NN_STRUC_H
#define NN_STRUC_H

// Struct to hold the neural network
struct NeuralNetwork {
    double **input_layer;
    double ***hidden_layers;
    double *output_layer;
};

/**
 * @brief Initialize the neural network.
 *
 * This function creates and initializes the neural network structure.
 *
 * @param input_neurons Number of neurons in the input layer (excluding bias).
 * @param hidden_neurons Number of neurons in each hidden layer.
 * @param hidden_layers Number of hidden layers.
 * @param output_neurons Number of neurons in the output layer.
 * @return Initialized neural network structure.
 */
struct NeuralNetwork init_neural_network(int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons);



/**
 * @brief Generate a random double between -0.5 and 0.5.
 *
 * This function generates a random double value within the range [-0.5, 0.5].
 * It uses the standard C library function rand() to obtain a random integer,
 * then scales and shifts this value to the desired range.
 *
 * @return A random double value between -0.5 and 0.5.
 *
 * @note Ensure that srand() is called before using this function to seed the random number generator,
 *       typically with srand(time(NULL)) to get different sequences of random numbers on each run.
 */
double random_double(double min, double max);



/**
 * @brief Create the input layer.
 *
 * @param input_neurons Number of neurons in the input layer (excluding bias).
 * @param hidden_neurons Number of neurons in the hidden layer.
 * @return Pointer to the allocated array for the input layer.
 */
double **create_input_layer(int input_neurons, int hidden_neurons);

/**
 * @brief Create an array of pointers for the hidden layers.
 *
 * @param hidden_neurons Number of neurons in each hidden layer.
 * @param hidden_layers Number of hidden layers.
 * @return Pointer to the allocated array of pointers to hidden layers.
 */
double ***create_hidden_layers(int hidden_neurons, int hidden_layers);

/**
 * @brief Create an array for the output layer.
 *
 * @param output_neurons Number of neurons in the output layer.
 * @return Pointer to the allocated array for the output layer.
 */
double *create_output_layer(int output_neurons);

/**
 * @brief Free memory allocated for the input layer.
 *
 * @param input_layer Pointer to the input layer.
 * @param input_neurons Number of neurons in the input layer (excluding bias).
 */
void free_input_layer(double **input_layer, int input_neurons);

/**
 * @brief Free memory allocated for the hidden layers.
 *
 * @param hidden_layers_array Pointer to the array of pointers to hidden layers.
 * @param hidden_neurons Number of neurons in each hidden layer.
 * @param hidden_layers Number of hidden layers.
 */
void free_hidden_layers(double ***hidden_layers_array, int hidden_neurons, int hidden_layers);

/**
 * @brief Free memory allocated for the output layer.
 *
 * @param output_layer Pointer to the output layer.
 */
void free_output_layer(double *output_layer);

#endif // NN_STRUC_H
