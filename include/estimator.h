#ifndef ESTIMATOR_HPP_
#define ESTIMATOR_HPP_


#ifdef __cplusplus
extern "C" {
#endif


typedef struct point_t {
  unsigned long x;
  unsigned long y;
} point_t;


typedef struct candidate_t {
  unsigned long size;
  point_t **parts;
  float *confidence;
} candidate_t;


typedef struct candidates_t {
  unsigned long size;
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
