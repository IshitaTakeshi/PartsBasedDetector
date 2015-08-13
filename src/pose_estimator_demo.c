#include "estimator.h"
#include <stdio.h>

int main(int argc, char** argv) {
    void* estimator = create_estimator(argv[1]);
    candidates_t* candidates = estimate(estimator, argv[2]);

    for(unsigned int i = 0; i < candidates->candidates[0]->size; i++) {
        printf("x: %4lu  y: %4lu  confidence: %.4f \n",
                candidates->candidates[0]->parts[i]->x,
                candidates->candidates[0]->parts[i]->y,
                candidates->candidates[0]->confidence[i]);
    }
    printf("\n");

    destroy_estimator(estimator);
    free_candidates(candidates);
}
