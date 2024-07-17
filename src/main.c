#include "main.h"
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static double integrate_rectangle_rule(struct borders *segment) {
  return (segment->right - segment->left) *
         sin((segment->left + segment->right) / 2);
}

static double integrate_simpsons_rule(struct borders *segment) {
  double border_sum = segment->right + segment->left;
  double border_diff = segment->right - segment->left;
  return border_diff / 6 *
         (sin(segment->left) + 4 * sin((border_sum) / 2) + sin(segment->right));
}

static double integrate(struct borders *segment, uint16_t partition_size,
                        double (*method)(struct borders *)) {
  double integral_value = 0.0;
  double part_length = (segment->right - segment->left) / partition_size;
  struct borders part = {segment->left, 0.0};
  for (uint16_t rectangle_num = 0; rectangle_num < partition_size;
       ++rectangle_num) {
    part.right = part.left + part_length;
    integral_value += method(&part);
    part.left = part.right;
  }
  return integral_value;
}

static char **calculate_integrals(struct borders *segment, uint8_t results_len,
                                  uint16_t *partition_sizes) {
  char **calculation_results = (char **)malloc(sizeof(char *) * results_len);
  if (!calculation_results) {
    fprintf(stderr, "Can't allocate memory for calculation results\n");
    return NULL;
  }
  for (uint8_t experiment_num = 0; experiment_num < results_len;
       ++experiment_num) {
    double integral_rectangle = integrate(
        segment, partition_sizes[experiment_num], integrate_rectangle_rule);
    double integral_simpson = integrate(
        segment, partition_sizes[experiment_num], integrate_simpsons_rule);
    calculation_results[experiment_num] =
        (char *)malloc(sizeof(char) * MAX_RESULT_STRING_LENGTH);
    if (!calculation_results[experiment_num]) {
      free_results(calculation_results, experiment_num);
      fprintf(stderr,
              "Can't allocate memory for result string in %d experiment\n",
              experiment_num);
      return NULL;
    }
    sprintf(calculation_results[experiment_num], "%d %.5f %.5f",
            partition_sizes[experiment_num], integral_rectangle,
            integral_simpson);
  }
  return calculation_results;
}

static void free_results(char **calculation_results, unsigned int len) {
  for (unsigned int experiment_num = 0; experiment_num < len;
       ++experiment_num) {
    free(calculation_results[experiment_num]);
  }
  free(calculation_results);
}

static struct borders read_interval() {
  struct borders segment;
  printf("Enter the left border of the interval: ");
  if (scanf("%lf", &(segment.left)) != 1) {
    fprintf(stderr, "Can't read the left border of the interval\n");
    exit(-1);
  }
  if (segment.left < 0) {
    fprintf(stderr, "Left border of the interval must be not less than 0\n");
    exit(-1);
  }
  printf("Enter the right border of the interval: ");
  if (scanf("%lf", &(segment.right)) != 1) {
    fprintf(stderr, "Can't read the right border of the interval\n");
    exit(-1);
  }
  if (segment.right > M_PI) {
    fprintf(stderr, "Right border of the interval must be not more than pi\n");
    exit(-1);
  }
  if (segment.left > segment.right) {
    fprintf(stderr,
            "Right border of the interval must be not less than left one\n");
    exit(-1);
  }
  return segment;
}

int main() {
  struct borders segment = read_interval();
  size_t experiments_count = 6;
  uint16_t partition_sizes[] = PARTITION_SIZES;
  char **calculation_results =
      calculate_integrals(&segment, experiments_count, partition_sizes);
  for (uint8_t experiment_num = 0; experiment_num < experiments_count;
       ++experiment_num) {
    printf("%s\n", calculation_results[experiment_num]);
  }
  free_results(calculation_results, experiments_count);
  return 0;
}
