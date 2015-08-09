#include "estimator.h"
#include <stdio.h>

int main(int argc, char** argv) {
    void* estimator = create_estimator(argv[1]);
    candidates_t* candidates = estimate(estimator, argv[2]);
    for(unsigned int i = 0; i < candidates->size; i++) {
        for(unsigned int j = 0; j < candidates->candidates[i]->size; j++) {
            printf("x: %4lu  y: %4lu  confidence: %.4f \n",
                    candidates->candidates[i]->parts[j]->x,
                    candidates->candidates[i]->parts[j]->y,
                    candidates->candidates[i]->confidence[j]);
        }
        printf("\n");
    }
    destroy_estimator(estimator);
    free_candidates(candidates);
}
