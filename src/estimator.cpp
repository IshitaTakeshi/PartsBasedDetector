#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <assert.h>

#include "estimator.h"
#include "PoseEstimator.hpp"

using namespace std;
using namespace cv;


//check that functions defined in C code can be called from Julia
void test_calling() {
  printf("called\n");
}


void *create_estimator(char filename[]) {
  return new PoseEstimator(string(filename));
}


float *vector_to_float_array(vector<float> &vector) {
  float *array = (float *)malloc(sizeof(float) * vector.size());
  for(unsigned int i = 0; i < vector.size(); i++) {
    array[i] = vector[i];
  }
  return array;
}


void test_vector_to_float_array() {
  std::vector<float> v(3);
  v[0] = 34;
  v[1] = 23;
  v[2] = 21;
  float* array = vector_to_float_array(v);
  assert(array[0] == 34);
  assert(array[1] == 23);
  assert(array[2] == 21);
  free(array);
}


point_t **parts_to_part_array(const vector<Point> &parts) {
  point_t **array = (point_t**)malloc(sizeof(point_t*) * parts.size());
  for(unsigned int i = 0; i < parts.size(); i++) {
    array[i] = (point_t*)malloc(sizeof(point_t));
    array[i]->x = parts[i].x;
    array[i]->y = parts[i].y;
  }
  return array;
}


candidate_t *candidate_to_candidate_t(const Candidate &candidate) {
  candidate_t *c = (candidate_t*)malloc(sizeof(candidate_t));

  const vector<Point> &parts = candidate.parts();
  vector<float> confidence = candidate.confidence();

  c->size = parts.size();
  c->parts = parts_to_part_array(parts);
  c->confidence = vector_to_float_array(confidence);
  return c;
}


void print_candidate(candidate_t *candidate) {
  //printf("pointer to candidate: %p\n", (void *)(candidate));
  //printf("pointer to confidence: %p\n", (void *)(&(candidate->confidence)));
  for(unsigned int i = 0; i < candidate->size; i++) {
      printf("index: %2d  x: %4u  y: %4u  confidence: %.4f \n",
             i, candidate->parts[i]->x, candidate->parts[i]->y,
             candidate->confidence[i]);
  }
}


candidates_t *candidates_to_candidate_ts(const vector<Candidate> &candidates) {
  candidates_t *c = (candidates_t*)malloc(sizeof(candidates_t));

  int size = sizeof(candidate_t*)*candidates.size();
  c->candidates = (candidate_t**)malloc(size);
  for(unsigned int i = 0; i < candidates.size(); i++) {
    c->candidates[i] = candidate_to_candidate_t(candidates[i]);
  }

  c->size = candidates.size();

  printf("size in C       : %u\n", c->candidates[0]->size);
  printf("parts in C      : %p\n", (void *)c->candidates[0]->parts);
  printf("confidence in C : %p\n", (void *)c->candidates[0]->confidence);

  for(unsigned int i = 0; i < c->candidates[0]->size; i++) {
    printf("confidence[%2d] : %f is at %p\n", i,
           c->candidates[0]->confidence[i],
           (void *)&c->candidates[0]->confidence[i]);
  }

  printf("\n");
  //for(unsigned int i = 0; i < c->candidates[0]->size; i++) {
  //  printf("parts[%2d] : %p is at %p\n", i,
  //         (void *)(c->candidates[0]->parts[i]),
  //         (void *)(&(c->candidates[0]->parts[i])));
  //}
  return c;
}


/* Estimate the pose */
candidates_t *estimate(void *estimator, char filename[]) {
  PoseEstimator *e = static_cast<PoseEstimator*>(estimator);
  vector<Candidate> candidates = e->estimate(string(filename));
	Candidate::sort(candidates);
  return candidates_to_candidate_ts(candidates);
}


void free_candidate(candidate_t *candidate) {
  for(unsigned int i = 0; i < candidate->size; i++) {
    free(candidate->parts[i]);
  }

  free(candidate->parts);
  free(candidate->confidence);
  free(candidate);
}


void free_candidates(candidates_t* candidates) {
  for(unsigned int i = 0; i < candidates->size; i++) {
    free_candidate(candidates->candidates[i]);
  }
  free(candidates);
}


void destroy_estimator(void *estimator) {
  delete static_cast<PoseEstimator*>(estimator);
}


void test() {
  test_vector_to_float_array();
}
