#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "nn_algo.h"
#include "nn_struc.h"

#define TRAIN_IMAGES "mnist files/train-images-idx3-ubyte"
#define TRAIN_LABELS "mnist files/train-labels-idx1-ubyte"
#define TEST_IMAGES  "mnist files/t10k-images-idx3-ubyte"
#define TEST_LABELS  "mnist files/t10k-labels-idx1-ubyte"

typedef struct {
    uint8_t *data;
    uint8_t *labels;
    int number_of_images;
    int image_size;
} mnist_dataset;

mnist_dataset* load_mnist(const char *image_filename, const char *label_filename) {
    FILE *image_file = fopen(image_filename, "rb");
    FILE *label_file = fopen(label_filename, "rb");

    if (!image_file || !label_file) {
        fprintf(stderr, "Error opening files.\n");
        return NULL;
    }

    uint32_t magic_number = 0, number_of_images = 0, number_of_labels = 0;
    uint32_t rows = 0, columns = 0;

    fread(&magic_number, sizeof(magic_number), 1, image_file);
    fread(&number_of_images, sizeof(number_of_images), 1, image_file);
    fread(&rows, sizeof(rows), 1, image_file);
    fread(&columns, sizeof(columns), 1, image_file);

    fread(&magic_number, sizeof(magic_number), 1, label_file);
    fread(&number_of_labels, sizeof(number_of_labels), 1, label_file);

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
    dataset->number_of_images = number_of_images;
    dataset->image_size = rows * columns;
    dataset->data = malloc(dataset->number_of_images * dataset->image_size);
    dataset->labels = malloc(dataset->number_of_images);

    fread(dataset->data, dataset->image_size, dataset->number_of_images, image_file);
    fread(dataset->labels, 1, dataset->number_of_images, label_file);

    fclose(image_file);
    fclose(label_file);

    return dataset;
}

void free_mnist(mnist_dataset *dataset) {
    if (dataset) {
        free(dataset->data);
        free(dataset->labels);
        free(dataset);
    }
}

/**
*@brief Create num arrays dynamiclly
*@param num_arr: Number of arrays to create
*@param num_size: Size of each array
*@return pointer to an array of pointers to the created arrays
*/

int **create_arrays(int num_arr,int num_size)
{
	int **arrays = (double **)malloc(num_arr * sizeof(double *));
	
	//Check for allocation failure
	if(arrays == NULL)
	{
		//Memory allocation failed
		printf("Failed to allocate memory for arrays-n");
		return NULL;
	}

	//Create each array
	for(int i = 0; i< num; ++i)
	{
		arrays[i]=(double*)malloc(num_size * sizeof(double)); //Create an array
		if(arrays[i] == NULL)
		{
			//Memory allocation failed, clean up prevoiusly allocated memory
			for (int j = 0; j < i; ++j)
			{
				free(arrays[j]);
			}
			free(arrays);
			return NULL;
		}
	}
	return arrays;
}

/**
*@brief Free memory allocated for arrays
*@param pointer to a pointer
*@param
*/
void free_arrays()
{
	if(arrays=NULL) {return;}

	//free memory for each array
	for (int i=0;i<num;++i)
	{
		free(arrays[i]);
	}

	//Free memory for the array of pointers
	free(arrays);
}

