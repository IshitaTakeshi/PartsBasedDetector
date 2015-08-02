#include <cstdio>
#include <iostream>
#include "estimator.h"
#include "PoseEstimator.hpp"


using namespace std;
using namespace cv;

#include <stdio.h>
void test_calling() {
  printf("called\n");
}

void* create_estimator(char filename[]) {
  return new PoseEstimator(string(filename));
}


parts_t* parts_to_parts_t(const vector<Point> &parts) {
  part_t **parts_ = (part_t**)malloc(sizeof(part_t*) * parts.size());
  for(int i = 0; i < parts.size(); i++) {
    part_t *part = (part_t*)malloc(sizeof(part_t));
    part->x = parts[i].x;
    part->y = parts[i].y;
    parts_[i] = part;
  }

  parts_t *p = (parts_t*)malloc(sizeof(parts_t));
  p->parts = parts_;
  p->size = parts.size();
  return p;
}


candidate_t *init_candidate(parts_t *parts, float *confidence) {
  candidate_t *candidate = (candidate_t*)malloc(sizeof(candidate_t));
  candidate->parts = parts;
  candidate->confidence = confidence;
}


candidate_t *candidate_to_candidate_t(const Candidate &candidate) {
  const vector<Point> &parts = candidate.parts();
  parts_t *parts_ = parts_to_parts_t(parts);

  vector<float> confidence = candidate.confidence();

  //pointer to the vector object
  return init_candidate(parts_, &confidence[0]);
}


candidates_t *candidates_to_candidates_t(const vector<Candidate> &candidates) {
  candidate_t **candidates_ =
      (candidate_t**)malloc(sizeof(candidate_t*)*candidates.size());
  for(int i = 0; i < candidates.size(); i++) {
    candidates_[i] = candidate_to_candidate_t(candidates[i]);
  }
  candidates_t *c = (candidates_t*)malloc(sizeof(candidates_t));
  c->candidates = candidates_;
  c->size = candidates.size();
  return c;
}


candidates_t *estimate(void *estimator, char filename[]) {
  PoseEstimator *e = static_cast<PoseEstimator*>(estimator);
  const vector<Candidate> &candidates = e->estimate(string(filename));
  return candidates_to_candidates_t(candidates);
}


void free_parts(parts_t *parts) {
  for(int i = 0; i < parts->size; i++) {
    free(parts->parts[i]);
  }
  free(parts);
}


void free_candidate(candidate_t *candidate) {
  free_parts(candidate->parts);
  free(candidate);
}

void free_candidates(candidates_t* candidates) {
  for(int i = 0; i < candidates->size; i++) {
    free_candidate(candidates->candidates[i]);
  }
  free(candidates);
}


void destroy_estimator(void *estimator) {
  delete static_cast<PoseEstimator*>(estimator);
}
