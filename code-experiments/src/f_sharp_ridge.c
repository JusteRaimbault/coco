#include <assert.h>
#include <math.h>

#include "coco.h"
#include "coco_problem.c"

static double f_sharp_ridge_raw(const double *x, const size_t number_of_variables) {

  static const double alpha = 100.0;
  size_t i = 0;
  double result;

  assert(number_of_variables > 1);

  result = 0.0;
  for (i = 1; i < number_of_variables; ++i) {
    result += x[i] * x[i];
  }
  result = alpha * sqrt(result) + x[0] * x[0];

  return result;
}

static void f_sharp_ridge_evaluate(coco_problem_t *self, const double *x, double *y) {
  assert(self->number_of_objectives == 1);
  y[0] = f_sharp_ridge_raw(x, self->number_of_variables);
}

static coco_problem_t *f_sharp_ridge(const size_t number_of_variables) {

  coco_problem_t *problem = coco_problem_allocate_from_scalars("sharp ridge function",
      f_sharp_ridge_evaluate, NULL, number_of_variables, -5.0, 5.0, 0.0);
  coco_problem_set_id(problem, "%s_d%04lu", "sharp_ridge", number_of_variables);

  /* Compute best solution */
  f_sharp_ridge_evaluate(problem, problem->best_parameter, problem->best_value);
  return problem;
}

/* TODO: Deprecated functions below are to be deleted when the new ones work as they should */


static void deprecated__f_sharp_ridge_evaluate(coco_problem_t *self, const double *x, double *y) {
  static const double alpha = 100.0;
  size_t i;
  assert(self->number_of_variables > 1);
  assert(self->number_of_objectives == 1);

  y[0] = 0.0;
  for (i = 1; i < self->number_of_variables; ++i) {
    y[0] += x[i] * x[i];
  }
  y[0] = alpha * sqrt(y[0]) + x[0] * x[0];
}

static coco_problem_t *deprecated__f_sharp_ridge(const size_t number_of_variables) {
  size_t i, problem_id_length;
  coco_problem_t *problem = coco_problem_allocate(number_of_variables, 1, 0);
  problem->problem_name = coco_strdup("sharp ridge function");
  /* Construct a meaningful problem id */
  problem_id_length = (size_t) snprintf(NULL, 0, "%s_%02lu", "sharp_ridge", (long) number_of_variables);
  problem->problem_id = coco_allocate_memory(problem_id_length + 1);
  snprintf(problem->problem_id, problem_id_length + 1, "%s_%02lu", "sharp_ridge", (long) number_of_variables);

  problem->number_of_variables = number_of_variables;
  problem->number_of_objectives = 1;
  problem->number_of_constraints = 0;
  problem->evaluate_function = deprecated__f_sharp_ridge_evaluate;
  for (i = 0; i < number_of_variables; ++i) {
    problem->smallest_values_of_interest[i] = -5.0;
    problem->largest_values_of_interest[i] = 5.0;
    problem->best_parameter[i] = 0.0;
  }
  /* Calculate best parameter value */
  deprecated__f_sharp_ridge_evaluate(problem, problem->best_parameter, problem->best_value);
  return problem;
}
