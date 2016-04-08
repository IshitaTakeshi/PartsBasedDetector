#include <cstdio>
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FileStorageModel.hpp"
#include "MatlabIOModel.hpp"
#include "PoseEstimator.hpp"

using namespace std;
using namespace cv;


PoseEstimator::PoseEstimator(const string& model_filename) {
  boost::scoped_ptr<Model> model;

  string ext = boost::filesystem::path(model_filename).extension().string();
	if (ext.compare(".xml") == 0 || ext.compare(".yaml") == 0) {
		model.reset(new FileStorageModel);
	} else if (ext.compare(".mat") == 0) {
    model.reset(new MatlabIOModel);
  } else {
		printf("Unsupported model format: %s\n", ext.c_str());
		exit(-2);
  }

	bool ok = model->deserialize(model_filename);
	if (!ok) {
		printf("Error deserializing file\n");
		exit(-3);
	}

  pbd.distributeModel(*model);
}


//cv::Mat copyPixelArrayToMat(int[][][] pixelArray, int rows, int cols) {
//  const cv::Mat& image(rows, cols, CV_8UC3);
//  for(int y = 0; y < image.rows; y++){
//    for(int x = 0; x < image.cols; x++){
//      image.data[image.step[0]*y + image.step[1]* x + 0] = pixelArray[y][x][0];
//      image.data[image.step[0]*y + image.step[1]* x + 1] = pixelArray[y][x][1];
//      image.data[image.step[0]*y + image.step[1]* x + 2] = pixelArray[y][x][2];
//    }
//  }
//  return image;
//}

vector<Candidate> PoseEstimator::estimate(const string& image_filename) {
	Mat_<float> depth;
	Mat im = imread(image_filename);
  if (im.empty()) {
      printf("Image not found or invalid image format\n");
      exit(-4);
  }

	vector<Candidate> candidates;
	pbd.detect(im, depth, candidates);

  return candidates;
}



//vector<Candidate> PoseEstimator::estimate(int[][][] pixelArray,
//                                          int rows, int cols) {
//  ///3D pixelArray to cv::Mat
//  cv::Mat& image = copyPixelArrayToMat(pixelArray, rows, cols);
//  //TODO show image once here
//
//  vector<Candidate> candidates;
//  pbd.detect(image, candidates);
//
//  return candidates;
//}
