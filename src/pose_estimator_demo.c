#include "estimator.h"

int main(int argc, char** argv) {
    void* estimator = create_estimator(argv[1]);
    candidates_t* candidates = estimate(estimator, argv[2]);
    destroy_estimator(estimator);
    free_candidates(candidates);
}
