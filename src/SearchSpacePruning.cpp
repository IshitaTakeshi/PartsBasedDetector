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
 *  File:    SearchSpacePruning.cpp
 *  Author:  Hilton Bristow
 *  Created: Aug 1, 2012
 */

#include "nms.hpp"
#include "Candidate.hpp"
#include "SearchSpacePruning.hpp"
#include "Math.hpp"
#include <limits>
#include <iostream>
using namespace cv;
using namespace std;

template<typename T>
void SearchSpacePruning<T>::filterResponseByDepth(vector2DMat& pdfs, const vector<Size>& fsizes, const Mat& depth, const vectorf& scales, const float X, const float fx) {

	const unsigned int N  = pdfs.size();
	const unsigned int F  = pdfs[0].size();

#ifdef _OPENMP
	#pragma omp parallel for
#endif
	for (unsigned int nf = 0; nf < N*F; ++nf) {
		const unsigned int n = nf / F;
		const unsigned int f = nf % F;

		// create a mask of plausible depths given the object size
		// and the scale of the image
		Mat sdepth;
		resize(depth, sdepth, pdfs[n][f].size());

		// calculate the depth of the part in real-world coordinates,
		// given the 3d width of the part, the focal length of the camera,
		// and the width of the part in the image
		float Z = fx*X/scales[n];
		Z += fsizes[0].height;
	}
}


// declare all specializations of the template (this must be the last declaration in the file)
template class SearchSpacePruning<float>;
template class SearchSpacePruning<double>;
