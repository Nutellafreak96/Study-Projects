
/**
*@file nn_struc.c
*@brief Neural Network Structure Implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "nn_struc.h"

// Global variables for network Structure
//int num_hidden_layer=1;
//int input_neurons=28*28; //784
//int hiddden_neurons=98;
//int output_neurons=10;




/*
*
*
*
*
*  Function to initialize the neural network
*
*
*/



struct NeuralNetwork init_neural_network(int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons) 
{
    struct NeuralNetwork nn; // Neural network structure
    // Create and initialize input layer
    nn.input_layer = create_input_layer(input_neurons, hidden_neurons);
    // Create and initialize hidden layers
    nn.hidden_layers = create_hidden_layers(hidden_neurons, hidden_layers);
    // Create and initialize output layer
    nn.output_layer = create_output_layer(output_neurons);
    return nn; // Return initialized neural network structure
}

/*
* Function to generate radnom values for the inital weights
*/

double random_double(double min, double max) {
    // Fixed min and max values
    double min_num = min;
    double max_num = max;
    
    
    double rand_val = (double)rand() / RAND_MAX;
    
    
    return min_num + (rand_val * (max_num - min_num));
}




/*
*
*
*
* Functions to allocate Memory for the Layers of the Neural Network
*
*
*/




double **create_input_layer(int input_neurons, int hidden_neurons) 
{
    int total_neurons = input_neurons + 1; // Including bias neuron
    double **input_layer = (double **)malloc(total_neurons * sizeof(double *));
    if (input_layer == NULL) {
        return NULL; // Memory allocation failed
    }

    for (int i = 0; i < input_neurons; ++i) {
        input_layer[i] = (double *)malloc((hidden_neurons + 1) * sizeof(double)); // Allocate for weights to hidden layer
        if (input_layer[i] == NULL) {
            for (int j = 0; j < i; ++j) {
                free(input_layer[j]);
            }
            free(input_layer);
            return NULL;
        }
    }

    // Bias neuron only has connections to hidden layer neurons
    input_layer[input_neurons] = (double *)malloc(hidden_neurons * sizeof(double));
    if (input_layer[input_neurons] == NULL) {
        for (int i = 0; i < input_neurons; ++i) {
            free(input_layer[i]);
        }
        free(input_layer);
        return NULL;
    }


	// Initialize Weights Randomly between 0 and 1
    for (int i = 0; i < input_neurons; ++i) {
        for (int j = 0; j < hidden_neurons + 1; ++j) {
            input_layer[i][j] = random_double(-0.8,0.8); // Random value between 0 and 1
        }
    }
    for (int j = 0; j < hidden_neurons; ++j) {
        input_layer[input_neurons][j] = random_double(-0.2,0.2);
    }



    return input_layer;
}


double ***create_hidden_layers(int hidden_neurons, int hidden_layers) 
{
    double ***hidden_layers_array = (double ***)malloc(hidden_layers * sizeof(double **));
    if (hidden_layers_array == NULL) {
        return NULL; // Memory allocation failed
    }

    for (int layer = 0; layer < hidden_layers; ++layer) {
        hidden_layers_array[layer] = (double **)malloc((hidden_neurons + 1) * sizeof(double *));
        if (hidden_layers_array[layer] == NULL) {
            for (int i = 0; i < layer; ++i) {
                free(hidden_layers_array[i]);
            }
            free(hidden_layers_array);
            return NULL;
        }

        for (int i = 0; i < hidden_neurons; ++i) {
            hidden_layers_array[layer][i] = (double *)malloc((hidden_neurons + 1) * sizeof(double)); // Connecting to next layer
            if (hidden_layers_array[layer][i] == NULL) {
                for (int j = 0; j < i; ++j) {
                    free(hidden_layers_array[layer][j]);
                }
                free(hidden_layers_array[layer]);
                for (int k = 0; k < layer; ++k) {
                    for (int l = 0; l < hidden_neurons; ++l) {
                        free(hidden_layers_array[k][l]);
                    }
                    free(hidden_layers_array[k]);
                }
                free(hidden_layers_array);
                return NULL;
            }
        }

        // Bias neuron for the hidden layer
        hidden_layers_array[layer][hidden_neurons] = (double *)malloc(hidden_neurons * sizeof(double)); // Bias only has connections to next layer neurons
        if (hidden_layers_array[layer][hidden_neurons] == NULL) {
            for (int i = 0; i < hidden_neurons; ++i) {
                free(hidden_layers_array[layer][i]);
            }
            free(hidden_layers_array[layer]);
            for (int k = 0; k < layer; ++k) {
                for (int l = 0; l < hidden_neurons; ++l) {
                    free(hidden_layers_array[k][l]);
                }
                free(hidden_layers_array[k]);
            }
            free(hidden_layers_array);
            return NULL;
        }
    }


	// Initialize Weights Randomly between 0 and 1
    for (int layer = 0; layer < hidden_layers; ++layer) {
        for (int i = 0; i < hidden_neurons; ++i) {
            for (int j = 0; j < hidden_neurons + 1; ++j) { // +1 for bias weight
                hidden_layers_array[layer][i][j] = random_double(-0.8,0.8);
            }
        }
        // Bias neuron
        for (int j = 0; j < hidden_neurons; ++j) {
            hidden_layers_array[layer][hidden_neurons][j] =random_double(-0.2,0.2);
        }
    }




    return hidden_layers_array;
}



double *create_output_layer(int output_neurons) {
    double *output_layer = (double *)malloc(output_neurons * sizeof(double));
    if (output_layer == NULL) {
        return NULL;
    }

    // Initialize output neurons to 0
    for (int i = 0; i < output_neurons; ++i) {
        output_layer[i] = 0.0;
    }

    return output_layer;
}


/*
*
*
*
* Functions to free the allocated Memory for the Layers of the Neural Network
*
*
*/


void free_input_layer(double **input_layer, int input_neurons) 
{
    if (input_layer == NULL) {
        return; // Nothing to free
    }

    // Free memory for each input neuron
    for (int i = 0; i < input_neurons; ++i) {
        free(input_layer[i]);
    }

    // Free memory for the bias neuron
    free(input_layer[input_neurons]);

    // Free memory for the array of pointers
    free(input_layer);
}


void free_hidden_layers(double ***hidden_layers_array, int hidden_neurons, int hidden_layers) 
{
    if (hidden_layers_array == NULL) {
        return; // Nothing to free
    }

    // Free memory for each hidden layer
    for (int layer = 0; layer < hidden_layers; ++layer) {
        double **hidden_layer = hidden_layers_array[layer];
        if (hidden_layer != NULL) {
            // Free memory for each individual neuron in the hidden layer
            for (int i = 0; i < hidden_neurons; ++i) {
                free(hidden_layer[i]);
            }
            // Free memory for the bias neuron
            free(hidden_layer[hidden_neurons]);
            // Free memory for the array of pointers
            free(hidden_layer);
        }
    }

    // Free memory for the array of pointers to hidden layers
    free(hidden_layers_array);
}


void free_output_layer(double *output_layer) 
{
    if (output_layer == NULL) {
        return; // Nothing to free
    }

    // Free memory for the array
    free(output_layer);
}
