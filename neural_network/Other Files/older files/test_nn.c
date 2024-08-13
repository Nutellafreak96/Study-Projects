#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "nn_struc.h"
#include "nn_algo.h"

//testing image files
#define TEST_IMAGES "mnist files/t10k-images-idx3-ubyte"
#define TEST_LABELS "mnist files/t10k-labels-idx1-ubyte"

typedef struct {
    uint8_t *data;
    uint8_t *labels;
    int number_of_images;
    int image_size;
} mnist_dataset;

mnist_dataset* load_mnist(const char *image_filename, const char *label_filename) {
    
}

void free_mnist(mnist_dataset *dataset) {
    
}

// Function to print the values of neurons and arrays
void print_neural_network(const struct NeuralNetwork* nn, int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons)
{
    printf("Input Layer:\n");
    for (int i = 0; i <= input_neurons; ++i) {
        printf("Neuron %d: Bias = %.2f, ", i + 1, nn->input_layer[i][0]);
        for (int j = 1; j <= hidden_neurons; ++j) {
            printf("Weight to Hidden %d = %.2f, ", j, nn->input_layer[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Hidden Layers:\n");
    for (int layer = 0; layer < hidden_layers; ++layer) {
        printf("Hidden Layer %d:\n", layer + 1);
        for (int i = 0; i <= hidden_neurons; ++i) {
            printf("Neuron %d: Bias = %.2f, ", i + 1, nn->hidden_layers[layer][i][0]);
            for (int j = 1; j <= hidden_neurons; ++j) {
                printf("Weight to Hidden %d = %.2f, ", j, nn->hidden_layers[layer][i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    printf("Output Layer:\n");
    for (int i = 0; i < output_neurons; ++i) {
        printf("Neuron %d: Bias = %.2f\n", i + 1, nn->output_layer[i]);
    }
}

// Main function
int main()
{

    mnist_dataset *test_data = load_mnist(TEST_IMAGES, TEST_LABELS);
    if (!test_data) {
        fprintf(stderr, "Failed to load testing data.\n");
        return 1;
    // XOR inputs and targets
    double inputs[4][2] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    double targets[4][2] = {
        {1, 0},
        {0, 1},
        {0, 1},
        {1, 0}
    };

    // Convert inputs and targets to double pointers
    double *inputs_ptr[4];
    double *targets_ptr[4];
    for (int i = 0; i < 4; i++) {
        inputs_ptr[i] = inputs[i];
        targets_ptr[i] = targets[i];
    }

    // Define neural network parameters
    int input_neurons = 2;
    int hidden_neurons = 4;
    int hidden_layers = 2;
    int output_neurons = 2;
    double learning_rate = 0.0001;
    int epochs = 10000;
    int number_of_samples = 4;
    
    
    
    struct NeuralNetwork nn = init_neural_network(input_neurons, hidden_neurons, hidden_layers, output_neurons);
    
    print_neural_network(&nn, input_neurons, hidden_neurons, hidden_layers, output_neurons);
    
    train(&nn, inputs_ptr, targets_ptr, input_neurons, hidden_neurons, hidden_layers, output_neurons, learning_rate, epochs, number_of_samples);

    print_neural_network(&nn, input_neurons, hidden_neurons, hidden_layers, output_neurons);


    // Free memory
    free_input_layer(nn.input_layer, input_neurons);
    free_hidden_layers(nn.hidden_layers, hidden_neurons, hidden_layers);
    free_output_layer(nn.output_layer);
    free_mnist(test_data);
    return 0;
}
