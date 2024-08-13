
#include "dataset.h"



// Load the Dataset
mnist_dataset* load_mnist(const char *image_filename, const char *label_filename) {
    //printf("Attempting to open image file: %s\n", image_filename);
    //printf("Attempting to open label file: %s\n", label_filename);
    FILE *image_file = fopen(image_filename, "rb");
    FILE *label_file = fopen(label_filename, "rb");

    if (!image_file || !label_file) {
        fprintf(stderr, "Error opening files.\n");
        if (image_file) fclose(image_file);
        if (label_file) fclose(label_file);
        return NULL;
    }

    uint32_t magic_number = 0, number_of_images = 0, number_of_labels = 0;
    uint32_t rows = 0, columns = 0;

    if (fread(&magic_number, sizeof(magic_number), 1, image_file) != 1 ||
        fread(&number_of_images, sizeof(number_of_images), 1, image_file) != 1 ||
        fread(&rows, sizeof(rows), 1, image_file) != 1 ||
        fread(&columns, sizeof(columns), 1, image_file) != 1 ||
        fread(&magic_number, sizeof(magic_number), 1, label_file) != 1 ||
        fread(&number_of_labels, sizeof(number_of_labels), 1, label_file) != 1) {
        fprintf(stderr, "Error reading file headers.\n");
        fclose(image_file);
        fclose(label_file);
        return NULL;
    }

    number_of_images = __builtin_bswap32(number_of_images);
    number_of_labels = __builtin_bswap32(number_of_labels);
    rows = __builtin_bswap32(rows);
    columns = __builtin_bswap32(columns);

    if (number_of_images != number_of_labels) {
        fprintf(stderr, "Number of images does not match number of labels.\n");
        fclose(image_file);
        fclose(label_file);
        return NULL;
    }

    mnist_dataset *dataset = malloc(sizeof(mnist_dataset));
    if (!dataset) {
        fprintf(stderr, "Memory allocation failed for dataset.\n");
        fclose(image_file);
        fclose(label_file);
        return NULL;
    }

    dataset->number_of_images = number_of_images;
    dataset->image_size = rows * columns;
    dataset->data = malloc(dataset->number_of_images * dataset->image_size);
    dataset->labels = malloc(dataset->number_of_images);

    if (!dataset->data || !dataset->labels) {
        fprintf(stderr, "Memory allocation failed for dataset data or labels.\n");
        free(dataset->data);
        free(dataset->labels);
        free(dataset);
        fclose(image_file);
        fclose(label_file);
        return NULL;
    }

    if (fread(dataset->data, dataset->image_size, dataset->number_of_images, image_file) != number_of_images ||
        fread(dataset->labels, 1, dataset->number_of_images, label_file) != number_of_images) {
        fprintf(stderr, "Error reading image or label data.\n");
        free(dataset->data);
        free(dataset->labels);
        free(dataset);
        fclose(image_file);
        fclose(label_file);
        return NULL;
    }

    fclose(image_file);
    fclose(label_file);

    return dataset;
}

// Free Memory used by the loaded Dataset
void free_mnist(mnist_dataset *dataset) {
    if (dataset) {
        free(dataset->data);
        free(dataset->labels);
        free(dataset);
    }
}

// Print the First Number of the Dataset with Binary Values
void print_first_number(mnist_dataset *dataset) {
    if (!dataset || dataset->number_of_images < 1) {
        fprintf(stderr, "Dataset is empty or not loaded properly.\n");
        return;
    }

    uint8_t *image = dataset->data;
    uint8_t label = dataset->labels[0];

    printf("Label: %d\n", label);
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28; j++) {
            printf("%c", image[i * 28 + j] ? '1' : '0');
        }
        printf("\n");
    }
}

// Function to convert a label to one-hot encoding
void convert_label_to_array(uint8_t label, double *one_hot, int num_classes) {
    if (!one_hot) {
        fprintf(stderr, "Invalid one-hot encoding array pointer.\n");
        return;
    }
    for (int i = 0; i < num_classes; i++) {
        one_hot[i] = (i == label) ? 1.0 : 0.0;
    }
}

// Function to convert raw MNIST image data to binary format
void convert_image_to_binary(const uint8_t *raw_image, double *binary_image, int size) {
    if (!raw_image || !binary_image) {
        fprintf(stderr, "Invalid image or binary image pointer.\n");
        return;
    }

    
    for (int i = 0; i < size; i++) {
        binary_image[i] = raw_image[i] > THRESHOLD ? 1.0 : 0.0;
    }
}

// Function to print the test sample as binary values
void print_binary_image(const double *image, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c", image[i] > 0.5 ? '1' : '0');
        if ((i + 1) % 28 == 0) {  // Assuming images are 28x28
            printf("\n");
        }
    }
}