/*****************************************************************************
**   Stereo Visual Odometry by combining point and line segment features	**
******************************************************************************
**																			**
**	Copyright(c) 2016, Ruben Gomez-Ojeda, University of Malaga              **
**	Copyright(c) 2016, MAPIR group, University of Malaga					**
**																			**
**  This program is free software: you can redistribute it and/or modify	**
**  it under the terms of the GNU General Public License (version 3) as		**
**	published by the Free Software Foundation.								**
**																			**
**  This program is distributed in the hope that it will be useful, but		**
**	WITHOUT ANY WARRANTY; without even the implied warranty of				**
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the			**
**  GNU General Public License for more details.							**
**																			**
**  You should have received a copy of the GNU General Public License		**
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.	**
**																			**
*****************************************************************************/

#pragma once

#include <future>
#include <thread>
#include <time.h>
using namespace std;

#include <opencv/cv.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/line_descriptor.hpp>
#include <opencv2/line_descriptor/descriptor.hpp>
using namespace cv;
using namespace cv::line_descriptor;

#include <eigen3/Eigen/Core>
using namespace Eigen;

#include <config.h>
#include <stereoFeatures.h>
#include <pinholeStereoCamera.h>
#include <auxiliar.h>

typedef Matrix<double,6,6> Matrix6d;
typedef Matrix<double,6,1> Vector6d;

namespace StVO{

class StereoFrame
{
public:

    StereoFrame();
    StereoFrame(const Mat img_l_, const Mat img_r_, const int idx_, PinholeStereoCamera* cam_ );
    StereoFrame(const Mat img_l_, const Mat img_r_, const Mat img_s_, const int idx_, PinholeStereoCamera* cam_ );
    ~StereoFrame();

    void extractStereoFeatures();
    void extractInitialStereoFeatures();
    void detectFeatures(Mat img, vector<KeyPoint> &points, Mat &pdesc, vector<KeyLine> &lines, Mat &ldesc, double min_line_length);
    void matchPointFeatures(BFMatcher* bfm, Mat pdesc_1, Mat pdesc_2, vector<vector<DMatch>> &pmatches_12);
    void matchLineFeatures(Ptr<BinaryDescriptorMatcher> bdm, Mat ldesc_1, Mat ldesc_2, vector<vector<DMatch>> &lmatches_12 );
    void matchLineFeaturesBFM(BFMatcher* bfm, Mat ldesc_1, Mat ldesc_2, vector<vector<DMatch>> &lmatches_12 );
    void pointDescriptorMAD( const vector<vector<DMatch>> matches, double &nn_mad, double &nn12_mad );
    void lineDescriptorMAD( const vector<vector<DMatch>> matches, double &nn_mad, double &nn12_mad );
    Mat  plotStereoFrame();

    int frame_idx;
    Mat img_l, img_r, img_s;
    Matrix4d Tfw;
    Matrix4d DT;

    Matrix6d Tfw_cov;
    Vector6d Tfw_cov_eig;
    double   entropy_first;

    Matrix6d DT_cov;
    Vector6d DT_cov_eig;
    double   err_norm;

    vector<PointFeature*> stereo_pt;
    vector<LineFeature*>  stereo_ls;

    Mat pdesc_l, pdesc_r, ldesc_l, ldesc_r;

    PinholeStereoCamera* cam;

};

}
