/*
 *  Software License Agreement (BSD License)
 *
 *  Copyright (c) 2012, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  File:    Candidate.hpp
 *  Author:  Hilton Bristow
 *  Created: Jun 21, 2012
 */

#ifndef CANDIDATE_HPP_
#define CANDIDATE_HPP_
#include <algorithm>
#include <iostream>
#include <limits>
#include <opencv2/core/core.hpp>
#include "types.hpp"
#include "Rect3.hpp"
#include "Math.hpp"

/*! @class Candidate
 *  @brief detection candidate
 *
 * Candidate describes a storage class for object detection candidates. A single Candidate
 * represents a detection for a tree of parts. The candidate is parameterized by a cv::Rect
 * bounding box and detection confidence for each part
 */

class Candidate {
private:
	//! the coordinates of the parts
	std::vector<cv::Point> parts_;
	//! the confidence scores of the parts
	vectorf confidence_;
	//! the model component the candidate belongs to
	int component_;

public:
	Candidate() {}
	virtual ~Candidate() {}

	//! return the vector of parts
	const std::vector<cv::Point>& parts(void) const {
    return parts_;
  }

	//! return the vector of confidence scores
	const vectorf& confidence(void) const {
    return confidence_;
  }

	//! add a part score to the candidate, parameterized by a bounding box
  //and confidence value
	void addPart(cv::Point p, float confidence) {
    parts_.push_back(p);
    confidence_.push_back(confidence);
  }

	//! get the root score of the detection. Using for sorting
	float score(void) const {
    if(confidence_.size() > 0) {
      return confidence_[0];
    }
    return -std::numeric_limits<double>::infinity();
  }

	//! set the root score of the detection
	void setScore(float confidence) {
    if (confidence_.size() == 0) {
      confidence_.resize(1);
    }
    confidence_[0] = confidence;
  }

	//! set the candidate component
	void setComponent(int c) {
    component_ = c;
  }

	//! get the candidate component
	int component(void) {
    return component_;
  }

	//! rescale the parts
	void resize(const float factor) {
		for (unsigned int n = 0; n < parts_.size(); ++n) {
			parts_[n].y      *= factor;
			parts_[n].x      *= factor;
		}
	}

	//! descending comparison method for ordering objects of type Candidate
	static bool descending(Candidate c1, Candidate c2) {
    return c1.score() > c2.score();
  }

	/*! @brief Sort the candidates from best to worst, in place
	 *
	 * @param candidates the vector of candidates to sort
	 */
	static void sort(vectorCandidate& candidates) {
		std::sort(candidates.begin(), candidates.end(), descending);
	}
};

#endif /* CANDIDATE_HPP_ */
