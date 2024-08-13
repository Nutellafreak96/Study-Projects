#include "nn_algo.h"



double sigmoid_function(double n){return (1 / (1 + exp(-n)));}

double sigmoid_derived(double n){return ( (1-sigmoid_function(n)) * sigmoid_function(n));}


double cost_function(double *predicted_output, double *target_output, int num_output_neurons) {
    double loss = 0.0;
    for (int i = 0; i < num_output_neurons; i++) {
        // Clip predicted probabilities to avoid log(0)
        double clipped_prob = fmax(1e-15, predicted_output[i]); 
        loss += -target_output[i] * log(clipped_prob);
    }
    return loss;
}

void forward_propagation(struct NeuralNetwork *network, int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons) {
    
    double **input_layer = network->input_layer;
    double ***hidden_layers_array = network->hidden_layers; // Change the variable name
    double *output_layer = network->output_layer;
    

    // Calculate Hidden Layer Values
    for (int layer = 0; layer < hidden_layers; layer++) {
        for (int neuron = 0; neuron < hidden_neurons; neuron++) {
            double weighted_sum = 0.0;
            for (int prev_neuron = 0; prev_neuron < (layer == 0 ? input_neurons : hidden_neurons); prev_neuron++) {
                double prev_value = layer == 0 ? input_layer[prev_neuron][0] : hidden_layers_array[layer - 1][prev_neuron][0]; // Use the corrected variable name
                double weight = layer == 0 ? input_layer[prev_neuron][neuron + 1] : hidden_layers_array[layer - 1][prev_neuron][neuron + 1]; // Use the corrected variable name
                weighted_sum += prev_value * weight;
            }
            // Add bias
            if (layer == 0) {
                weighted_sum += input_layer[input_neurons][neuron];
            } else {
                weighted_sum += hidden_layers_array[layer - 1][hidden_neurons][neuron]; // Use the corrected variable name
            }

            // Apply your sigmoid activation function
            hidden_layers_array[layer][neuron][0] = sigmoid_function(weighted_sum); // Use the corrected variable name
        }
    }
   
    // Calculate Output Layer Values (using your sigmoid activation)
    for (int neuron = 0; neuron < output_neurons; neuron++) {
        double weighted_sum = 0.0;
        for (int prev_neuron = 0; prev_neuron < hidden_neurons; prev_neuron++) {
            weighted_sum += hidden_layers_array[hidden_layers - 1][prev_neuron][0] * hidden_layers_array[hidden_layers - 1][prev_neuron][neuron + 1];  // Use the corrected variable name
        }
        // Add bias
        weighted_sum += hidden_layers_array[hidden_layers - 1][hidden_neurons][neuron]; // Use the corrected variable name

        // Apply your sigmoid activation function 
        output_layer[neuron] = sigmoid_function(weighted_sum);
    }
}


/* Functions necessary for the Back Propagation*/


// Compute output delta (error term)
void compute_output_delta(double *delta_output, double *output_layer, double *target_output, int output_neurons) {
    for (int i = 0; i < output_neurons; i++) {
        double error = output_layer[i] - target_output[i];
        delta_output[i] = error * sigmoid_derived(output_layer[i]);
    }
}

// Compute hidden layer deltas (error terms)
void compute_hidden_deltas(double **delta_hidden, double ***hidden_layers_array, double *delta_output, int hidden_neurons, int hidden_layers, int output_neurons) {
    for (int layer = hidden_layers - 1; layer >= 0; layer--) {
        for (int i = 0; i < hidden_neurons; i++) {
            double error = 0.0;
            if (layer == hidden_layers - 1) { // Last hidden layer
                for (int j = 0; j < output_neurons; j++) {
                    error += delta_output[j] * hidden_layers_array[layer][i][j + 1];
                }
            } else { // Other hidden layers
                for (int j = 0; j < hidden_neurons; j++) {
                    error += delta_hidden[layer + 1][j] * hidden_layers_array[layer][i][j + 1];
                }
            }
            delta_hidden[layer][i] = error * sigmoid_derived(hidden_layers_array[layer][i][0]);
        }
    }
}

// Update weights and biases
void update_weights(struct NeuralNetwork *network, double **delta_hidden, double *delta_output, int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons, double learning_rate) {
    double **input_layer = network->input_layer;
    double ***hidden_layers_array = network->hidden_layers;
    double *output_layer = network->output_layer;

    // Update weights for output layer
    for (int i = 0; i < output_neurons; i++) {
        for (int j = 0; j < hidden_neurons; j++) {
            hidden_layers_array[hidden_layers - 1][j][i + 1] -= learning_rate * delta_output[i] * hidden_layers_array[hidden_layers - 1][j][0];
        }
        // Update bias
        hidden_layers_array[hidden_layers - 1][hidden_neurons][i] -= learning_rate * delta_output[i];
    }

    // Update weights for hidden layers
    for (int layer = hidden_layers - 1; layer >= 0; layer--) {
        for (int i = 0; i < hidden_neurons; i++) {
            for (int j = 0; j < (layer == 0 ? input_neurons : hidden_neurons); j++) {
                if (layer == 0) {
                    input_layer[j][i + 1] -= learning_rate * delta_hidden[layer][i] * input_layer[j][0];
                } else {
                    hidden_layers_array[layer - 1][j][i + 1] -= learning_rate * delta_hidden[layer][i] * hidden_layers_array[layer - 1][j][0];
                }
            }
            // Update bias
            if (layer == 0) {
                input_layer[input_neurons][i] -= learning_rate * delta_hidden[layer][i];
            } else {
                hidden_layers_array[layer - 1][hidden_neurons][i] -= learning_rate * delta_hidden[layer][i];
            }
        }
    }
}

// Backpropagation wrapper function
void back_propagation(struct NeuralNetwork *network, double *target_output, int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons, double learning_rate) {
    // Allocate memory for the error terms (deltas)
    double **delta_hidden = (double **)malloc(hidden_layers * sizeof(double *));
    for (int i = 0; i < hidden_layers; i++) {
        delta_hidden[i] = (double *)malloc(hidden_neurons * sizeof(double));
    }
    double *delta_output = (double *)malloc(output_neurons * sizeof(double));

    // Compute deltas
    compute_output_delta(delta_output, network->output_layer, target_output, output_neurons);
    compute_hidden_deltas(delta_hidden, network->hidden_layers, delta_output, hidden_neurons, hidden_layers, output_neurons);

    // Update weights
    update_weights(network, delta_hidden, delta_output, input_neurons, hidden_neurons, hidden_layers, output_neurons, learning_rate);

    // Free allocated memory for deltas
    for (int i = 0; i < hidden_layers; i++) {
        free(delta_hidden[i]);
    }
    free(delta_hidden);
    free(delta_output);
}

// Train function
void train(struct NeuralNetwork *network, double **inputs, double **targets, int input_neurons, int hidden_neurons, int hidden_layers, int output_neurons, double learning_rate, int epochs, int number_of_samples) {
    for (int epoch = 0; epoch < epochs; epoch++) {
        double total_loss = 0.0;
        for (int sample = 0; sample < number_of_samples; sample++) {
            // Set input values
            for (int i = 0; i < input_neurons; i++) {
                network->input_layer[i][0] = inputs[sample][i];
            }

            // Forward propagation
            forward_propagation(network, input_neurons, hidden_neurons, hidden_layers, output_neurons);

            // Calculate loss
            double loss = cost_function(network->output_layer, targets[sample], output_neurons);
            total_loss += loss;

            // Backpropagation
            back_propagation(network, targets[sample], input_neurons, hidden_neurons, hidden_layers, output_neurons, learning_rate);
        }
        if((epoch == 0) || ((epoch%1000)==0))
        {
            printf("Epoch: %d, Loss: %f\n", epoch, total_loss / number_of_samples);
        }
    }
}