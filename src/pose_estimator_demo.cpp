#include "PoseEstimator.hpp"
#include "Candidate.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
  PoseEstimator estimator(argv[1]);
  vector<Candidate> candidates = estimator.estimate(argv[2]);
  candidates[0].parts();
}
