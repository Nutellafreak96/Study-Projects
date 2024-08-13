/**
 * @file dataset.h
 * @brief Header file for handling MNIST dataset operations.
 * 
 * This header file defines the structures and functions used to load,
 * manage, and process the MNIST dataset for training and testing machine
 * learning models.
 */

#ifndef DATASET_H
#define DATASET_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>

/** @brief Path to the MNIST training images file. */
#define TRAIN_IMAGES "/home/kevin/Desktop/mpt24-bnf-kh-aa/Project-Code/mnist_files/train-images.idx3-ubyte"

/** @brief Path to the MNIST training labels file. */
#define TRAIN_LABELS "/home/kevin/Desktop/mpt24-bnf-kh-aa/Project-Code/mnist_files/train-labels.idx1-ubyte"

/** @brief Path to the MNIST testing images file. */
#define TEST_IMAGES  "/home/kevin/Desktop/mpt24-bnf-kh-aa/Project-Code/mnist_files/t10k-images.idx3-ubyte"

/** @brief Path to the MNIST testing labels file. */
#define TEST_LABELS  "/home/kevin/Desktop/mpt24-bnf-kh-aa/Project-Code/mnist_files/t10k-labels.idx1-ubyte"

/** @brief Threshold value for converting image pixels to binary. */
#define THRESHOLD 127  ///< Adjustable threshold for binary conversion.

 /**
  * @struct mnist_dataset
  * @brief Structure to hold MNIST dataset information.
  * 
  * This structure contains the data and labels of the MNIST dataset, as
  * well as metadata about the dataset.
  */
typedef struct {
    uint8_t *data;          /**< Pointer to the image data. */
    uint8_t *labels;        /**< Pointer to the labels. */
    int number_of_images;   /**< Total number of images in the dataset. */
    int image_size;         /**< Size of each image (number of pixels). */
} mnist_dataset;

/**
 * @brief Loads the MNIST dataset from the specified files.
 * 
 * This function reads the image and label files, parses the MNIST binary
 * format, and loads the data into an `mnist_dataset` structure.
 * 
 * @param[in] image_filename Path to the file containing image data.
 * @param[in] label_filename Path to the file containing label data.
 * @return Pointer to an `mnist_dataset` structure containing the loaded data,
 *         or NULL if there was an error.
 */
mnist_dataset* load_mnist(const char *image_filename, const char *label_filename);

/**
 * @brief Frees the memory allocated for an MNIST dataset.
 * 
 * This function frees the memory used by an `mnist_dataset` structure, including
 * the image data and labels.
 * 
 * @param[in] dataset Pointer to the `mnist_dataset` structure to be freed.
 */
void free_mnist(mnist_dataset *dataset);

/**
 * @brief Prints the first image and its label from the dataset.
 * 
 * This function prints the label of the first image and a textual representation
 * of the image itself (using '1' for pixels and '0' for background).
 * 
 * @param[in] dataset Pointer to the `mnist_dataset` structure containing the data to be printed.
 */
void print_first_number(mnist_dataset *dataset);

/**
 * @brief Converts a raw image from MNIST dataset to binary format based on a threshold.
 * 
 * @param[in] raw_image Pointer to an array of raw pixel values from the MNIST dataset image.
 * @param[out] binary_image Pointer to an array where the binary image will be stored.
 * @param[in] size The number of pixels in the image (e.g., 784 for a 28x28 image).
 */
void convert_image_to_binary(const uint8_t *raw_image, double *binary_image, int size);

/**
 * @brief Converts a numerical label to one-hot encoded format.
 * 
 * @param[in] label The numerical label representing the class (e.g., 0 for the digit '0').
 * @param[out] one_hot Pointer to an array where the one-hot encoded vector will be stored.
 * @param[in] num_classes The total number of classes (e.g., 10 for digits 0-9).
 */
void convert_label_to_array(uint8_t label, double *one_hot, int num_classes);

/**
 * @brief Prints a binary image representation.
 * 
 * This function prints an image where each pixel is represented as '1' if its value is greater than 0.5 and '0' otherwise.
 * The function assumes the image has a square shape, e.g., 28x28 pixels.
 * 
 * @param image Pointer to the array of image pixel values (binary).
 * @param size Total number of pixels in the image.
 */
void print_binary_image(const double *image, int size);

#endif // DATASET_H
