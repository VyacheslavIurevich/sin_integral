#pragma once
#include <inttypes.h>
#define MAX_RESULT_STRING_LENGTH 32
#define EXPERIMENTS_COUNT 6
#define PARTITION_SIZES                                                        \
  { 5, 10, 20, 100, 500, 1000 }

typedef struct borders {
  double left;
  double right;
} borders_t;

static double integrate_rectangle_rule(borders_t *segment);

static double integrate_simpsons_rule(borders_t *segment);

static double integrate(borders_t *segment, uint16_t partition_size,
                        double (*method)(borders_t *));

static char **calculate_integrals(borders_t *segment, uint8_t results_len,
                                  uint16_t *partition_sizes);

static void free_results(char **calculation_results, unsigned int len);

static borders_t read_interval();
