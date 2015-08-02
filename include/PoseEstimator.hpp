#include <iostream>
#include <cstdio>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Candidate.hpp"
#include "Visualize.hpp"
#include "PartsBasedDetector.hpp"


class PoseEstimator {
  public:
    PoseEstimator(const std::string& model_filename);
    std::vector<Candidate> estimate(const std::string& filename);
  private:
    PartsBasedDetector<float> pbd;
};
