#include <stdio.h>
#include <cstdio>
#include <iostream>
#include "estimator.h"
#include "PoseEstimator.hpp"


using namespace std;
using namespace cv;


void test_calling() {
  printf("called\n");
}


void *create_estimator(char filename[]) {
  return new PoseEstimator(string(filename));
}


point_t **parts_to_part_array(const vector<Point> &parts) {
  point_t **parts_ = (point_t**)malloc(sizeof(point_t*) * parts.size());
  for(unsigned int i = 0; i < parts.size(); i++) {
    point_t *part = (point_t*)malloc(sizeof(point_t));
    part->x = parts[i].x;
    part->y = parts[i].y;
    parts_[i] = part;
  }
  return parts_;
}


candidate_t *init_candidate(point_t **parts, float *confidence,
                            unsigned long size) {
  candidate_t *candidate = (candidate_t*)malloc(sizeof(candidate_t));
  candidate->size = size;
  candidate->parts = parts;
  candidate->confidence = confidence;
  return candidate;
}


float *vector_to_float_array(vector<float> &vector) {
  float *array = (float *)malloc(sizeof(float) * vector.size());
  for(unsigned int i = 0; i < vector.size(); i++) {
    array[i] = vector[i];
  }
  return array;
}


candidate_t *candidate_to_candidate_t(const Candidate &candidate) {
  const vector<Point> &parts = candidate.parts();
  point_t **parts_ = parts_to_part_array(parts);
  vector<float> confidence = candidate.confidence();

  float *confidence_ = vector_to_float_array(confidence);

  candidate_t *candidate_ = init_candidate(parts_, confidence_, parts.size());
  return candidate_;
}


candidates_t *candidates_to_candidates_t(const vector<Candidate> &candidates) {
  candidate_t **candidates_ =
      (candidate_t**)malloc(sizeof(candidate_t*)*candidates.size());
  for(unsigned int i = 0; i < candidates.size(); i++) {
    candidates_[i] = candidate_to_candidate_t(candidates[i]);
  }

  candidates_t *c = (candidates_t*)malloc(sizeof(candidates_t));

  c->candidates = candidates_;
  c->size = candidates.size();
  return c;
}


/* Estimate the pose given a file.*/
candidates_t *estimate(void *estimator, char filename[]) {
  PoseEstimator *e = static_cast<PoseEstimator*>(estimator);
  vector<Candidate> candidates = e->estimate(string(filename));
	Candidate::sort(candidates);
  return candidates_to_candidates_t(candidates);
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
