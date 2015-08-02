#ifndef ESTIMATOR_HPP_
#define ESTIMATOR_HPP_


#ifdef __cplusplus
extern "C" {
#endif


typedef struct part_t {
  int x;
  int y;
} part_t;


typedef struct parts_t {
  unsigned int size;
  part_t **parts;
} parts_t;


typedef struct candidate_t {
  parts_t *parts;
  float *confidence;
} candidate_t;


typedef struct candidates_t {
  unsigned int size;
  candidate_t **candidates;
} candidates_t;


void* create_estimator(char filename[]);
candidates_t *estimate(void *estimator, char filename[]);
void destroy_estimator(void *estimator);
void free_candidates(candidates_t* candidates);
void test_calling();


#ifdef __cplusplus
}
#endif

#endif /* ESTIMATOR_HPP_ */
