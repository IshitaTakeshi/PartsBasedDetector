#include <stdio.h>
#include <stdlib.h>

#include "estimator.h"


int main(int argc, char** argv) {
    //test();

    if(argc < 3) {
        printf("Usage: PartsBasedDetector1 model_file image_file\n");
        exit(0);
    }

    void* estimator = create_estimator(argv[1]);
    candidates_t* candidates = estimate(estimator, argv[2]);
    print_candidate(candidates->candidates[0]);

    destroy_estimator(estimator);
    free_candidates(candidates);
}
