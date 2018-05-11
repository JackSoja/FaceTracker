#include <FaceTracker/Tracker.h>
#include <opencv/highgui.h>
#include <iostream>

//=============================================================================

    void Draw(cv::Mat &image, cv::Mat &shape, cv::Mat &con, cv::Mat &tri, cv::Mat &visi) {
      int i, n = shape.rows / 2;
      cv::Point p1, p2;
      cv::Scalar c;

      //draw triangulation
      c = CV_RGB(0, 0, 0);
      for (i = 0; i < tri.rows; i++) {
        if (visi.at<int>(tri.at<int>(i, 0), 0) == 0 ||
            visi.at<int>(tri.at<int>(i, 1), 0) == 0 ||
            visi.at<int>(tri.at<int>(i, 2), 0) == 0)
          continue;
        p1 = cv::Point(shape.at<double>(tri.at<int>(i, 0), 0),
                       shape.at<double>(tri.at<int>(i, 0) + n, 0));
        p2 = cv::Point(shape.at<double>(tri.at<int>(i, 1), 0),
                       shape.at<double>(tri.at<int>(i, 1) + n, 0));
        cv::line(image,p1,p2,c);
        p1 = cv::Point(shape.at<double>(tri.at<int>(i, 0), 0),
                       shape.at<double>(tri.at<int>(i, 0) + n, 0));
        p2 = cv::Point(shape.at<double>(tri.at<int>(i, 2), 0),
                       shape.at<double>(tri.at<int>(i, 2) + n, 0));
         cv::line(image,p1,p2,c);
        p1 = cv::Point(shape.at<double>(tri.at<int>(i, 2), 0),
                       shape.at<double>(tri.at<int>(i, 2) + n, 0));
        p2 = cv::Point(shape.at<double>(tri.at<int>(i, 1), 0),
                       shape.at<double>(tri.at<int>(i, 1) + n, 0));
        cv::line(image,p1,p2,c);
      }
      //draw connections
      c = CV_RGB(0, 0, 255);
      for (i = 0; i < con.cols; i++) {
        if (visi.at<int>(con.at<int>(0, i), 0) == 0 ||
            visi.at<int>(con.at<int>(1, i), 0) == 0)
          continue;
        p1 = cv::Point(shape.at<double>(con.at<int>(0, i), 0),
                       shape.at<double>(con.at<int>(0, i) + n, 0));
        p2 = cv::Point(shape.at<double>(con.at<int>(1, i), 0),
                       shape.at<double>(con.at<int>(1, i) + n, 0));
         cv::line(image,p1,p2,c,1);
      }
      //draw points
      for (i = 0; i < n; i++) {
        if (visi.at<int>(i, 0) == 0)continue;
        p1 = cv::Point(shape.at<double>(i, 0), shape.at<double>(i + n, 0));
        c = CV_RGB(255, 0, 0); cv::circle(image,p1,2,c);
      }
      return;
    }


//=============================================================================
int main(int argc, const char** argv){

   // cv::Mat FaceTrack::getFace(cv::Mat frame) {
    
    char ftFile[256], conFile[256], triFile[256];
    std::strcpy(ftFile, "../model/face2.tracker");
    std::strcpy(conFile, "../model/face.con");
    std::strcpy(triFile, "../model/face.tri");
   // __android_log_print(ANDROID_LOG_DEBUG,"jni", "%s:%s:%s",ftFile,conFile,triFile);
    cv::Mat gray, im;
    FACETRACKER::Tracker model(ftFile);
    //__android_log_print(ANDROID_LOG_DEBUG,"jni", "函数调用");
    cv::Mat tri = FACETRACKER::IO::LoadTri(triFile);
    cv::Mat con = FACETRACKER::IO::LoadCon(conFile);

      int fpd = 1;
      bool fcheck = false;
      int nIter = 5;
      double clamp = 3, fTol = 0.01;
      bool failed = true;

      std::vector<int> wSize1(1);
      wSize1[0] = 7;
      std::vector<int> wSize2(3);
      wSize2[0] = 11;
      wSize2[1] = 9;
      wSize2[2] = 7;

      //track this image
      std::vector<int> wSize;
      if (failed)wSize = wSize2; else wSize = wSize1;
      if (model.Track(gray, wSize, fpd, nIter, clamp, fTol, fcheck) == 0) {
        int idx = model._clm.GetViewIdx();
        failed = false;
        Draw(im, model._shape, con, tri, model._clm._visi[idx]);
      } else {

      }
      return 0;
    }
