#pragma once
#include <inttypes.h>
#define MAX_RESULT_STRING_LENGTH 32
#define EXPERIMENTS_COUNT 6
#define PARTITION_SIZES                                                        \
  { 5, 10, 20, 100, 500, 1000 }

struct borders {
  double left;
  double right;
};

static double integrate_rectangle_rule(struct borders *segment);

static double integrate_simpsons_rule(struct borders *segment);

static double integrate(struct borders *segment, uint16_t partition_size,
                        double (*method)(struct borders *));

static char **calculate_integrals(struct borders *segment, uint8_t results_len,
                                  uint16_t *partition_sizes);

static void free_results(char **calculation_results, unsigned int len);

static struct borders read_interval();
