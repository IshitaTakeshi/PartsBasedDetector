#ifndef ESTIMATOR_HPP_
#define ESTIMATOR_HPP_


#ifdef __cplusplus
extern "C" {
#endif


typedef struct point_t {
  unsigned int x;
  unsigned int y;
} point_t;


typedef struct candidate_t {
  unsigned int size;  //the number of parts
  point_t **parts;  // the coordinates of the parts
  float *confidence;  //the confidence scores of the parts
} candidate_t;


//structure to hold a candidate array
typedef struct candidates_t {
  unsigned int size;
  candidate_t **candidates;
} candidates_t;


void* create_estimator(char filename[]);
candidates_t *estimate(void *estimator, char filename[]);
void destroy_estimator(void *estimator);
void print_candidate(candidate_t *candidate);
void free_candidates(candidates_t* candidates);
void test_calling();
void test();


#ifdef __cplusplus
}
#endif

#endif /* ESTIMATOR_HPP_ */
