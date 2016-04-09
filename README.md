PartsBasedDetector
------------------

This project implements a Parts Based Detector in C++, described in the
following paper:

    Yi Yang, Deva Ramanan, "Articulated Pose Estimation with
    Flexible Mixtures-of-Parts," CVPR 2011

## Setting environment
### Installing Octave

```
$sudo apt-get install libgraphicsmagick++-dev
$wget ftp.gnu.org/gnu/octave/octave-3.8.2.tar.gz
$tar xvf octave-3.8.2.tar.gz
$./configure
$make -j4
$sudo make install
```

### Installing packages

```
$octave
> pkg install control-2.8.2.tar.gz
> pkg install general-1.3.4.tar.gz
> pkg install signal-1.3.2.tar.gz
> pkg install image-2.2.2.tar.gz
```

The project has the following dependencies:

* Octave  REQUIRED (for learning)
* OpenCV  REQUIRED (for image processing)
* CMake   REQUIRED (for building)
* Doxygen OPTIONAL (for documentation)
* OpenMP  OPTIONAL (for multithreading)
* ROS     OPTIONAL (for publishing detections on a ROS topic)
* ECTO    OPTIONAL (for building and ECTO cell)

## Downloading dataset

```
$cd <PROJECT_ROOT>/pose/dataset
$./load_inria.sh
$octave load_fashionista.m
```

## Building
The project can be built in one of two modes:

* A standalone binary (for testing functionality)
* A shared library    (for use in existing applications)

To configure the project, set the options at the top of CMakeLists.txt
To build the project, follow the normal cmake routine from the root folder:

```
git clone git@github.com:IshitaTakeshi/PartsBasedDetector.git --recursive
cd PartsBasedDetector/cvmatio
mkdir build
cd build
cmake ..
make -j4
cd ..
mkdir build
cd build
cmake ..
make -j4
```

## Learning
The learning code is currently only in Octave/Matlab. This is because 
the detector supports a number of learning schema, and porting all of 
these to C++ is not practical at this time.
Please consult the README within the matlab/ directory for instructions
on training a model

This package is developed and maintained by Hilton Bristow, Willow Garage

### Generating models

```
$cd matlab
$octave compile.m
$octave demo_fashionista.m
```

### Detecting
```
cd build
./src/PartsBasedDetector ../matlab/fashionista.mat <path to image>
```
