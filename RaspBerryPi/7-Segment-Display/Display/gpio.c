#include "gpio.h"

int export_pin(int num) {
    FILE* export = fopen(EXPORT, "w");
    if (export == NULL) {
        printf("Failed to open the export file\n");
        exit(-1);
    }
    fprintf(export, "%d", num);
    fclose(export);
    return 0;
}

int unexport_pin(int num) {
    FILE* unexport = fopen(UNEXPORT, "w");
    if (unexport == NULL) {
        printf("Failed to open the unexport file\n");
        exit(-1);
    }
    fprintf(unexport, "%d", num);
    fclose(unexport);
    return 0;
}

int set_direction(char *mode, int num) {
    char direction_file_path[1024];
    snprintf(direction_file_path, sizeof(direction_file_path), "/sys/class/gpio/gpio%d/direction", num);
    FILE* direction = fopen(direction_file_path, "w");
    if (direction == NULL) {
        printf("Failed to open the direction file\n");
        exit(-1);
    }
    fputs(mode, direction);
    fclose(direction);
    return 0;
}

int set_value(int val, int num) {
    char value_file_path[1024];
    snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio/gpio%d/value", num);
    FILE* value = fopen(value_file_path, "w");
    if (value == NULL) {
        printf("Failed to open the value file\n");
        exit(-1);
    }
    fprintf(value, "%d", val);
    fclose(value);
    return 0;
}