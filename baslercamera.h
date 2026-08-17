#ifndef BASLERCAMERA_H
#define BASLERCAMERA_H
#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigECamera.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>

#include <vector>
#include <iostream>
#include <chrono>
#include <stdint.h>
#include <unistd.h>
#include <thread>
#include <mutex>
//#include <QThread>
//#include <QMutex>


#include "opencv2/opencv.hpp"

#include <QImage>
using namespace cv;
using namespace std;
using namespace Pylon;
using namespace GenApi;

//class baslercamera :public CImageEventHandler,public CConfigurationEventHandler
class baslercamera :public QObject
{
    Q_OBJECT
public:
    baslercamera();
    ~baslercamera();
        //cam initial
         void cameraInit();
        //connect cam
         void ConnectCamera();
         void openCamera();
         //get image
         cv::Mat GetGrabImageMat();
         //stop camera;
         void stopAcquire();
         void closeCamera();
         void deleteAll();


        /******************
         * set parameters    :set parameters  between ConnectCamera() and openCamera().
        *******************/
         void handle_basler_boolean_parameter(CInstantCamera& camera, string name, bool value);
         void handle_basler_int_parameter(CInstantCamera& camera, string name, int value);
         void handle_basler_float_parameter(CInstantCamera& camera, string name, double value);
         void handle_basler_enum_parameter(CInstantCamera& camera, string name, string value);

         int get_basler_int_parameter(CInstantCamera& camera, string name);
         float get_basler_float_parameter(CInstantCamera& camera, string name);

         //must stop set, set them in initial stage

         void setWidth(CInstantCamera& camera,int width);
         int getWidth(CInstantCamera& camera);

         void setheight(CInstantCamera& camera,int height);
         int getheight(CInstantCamera& camera);

         //ju xin zhen set
         void setPacketSize(CInstantCamera& camera,int packetsize);
         void setInterPacketDelay(CInstantCamera& camera,int interpacketdelay);

         //can running set
         void setExposureTime(CInstantCamera& camera,float exp);
         int getExposureTime(CInstantCamera& camera);

         void setFramerate(CInstantCamera& camera,double framerate);
         float getFramerate(CInstantCamera& camera);

         //Digital IO output
         void setLineSelector(CInstantCamera& camera,string lineselector);
         void setLineSource(CInstantCamera& camera,string linesource);
         void setLineInverter(CInstantCamera& camera,bool lineinverter);  //true: rising edge   false: falling edge


         void SetupCamera();
        //callback receive grab image
        //virtual void OnImageGrabbed(CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult);
        //image grap complete
         void GrabFinished();
        //device remove
        void DeviceRemoved();
        //camera remove
        //void cameraRemoveall();
        // grab one
        void GrabOne();
        //updata camera info
        void updateCameraInfo();
        // scan camera
        void RescanCamera();
        //get image (one)
        cv::Mat GetGraboneImageMat();





        void SetTriggerFunction();

    public:   //thread
        std::thread* m_thrd_1=nullptr;
        void Fun_1();
        std::mutex mtx;
        bool m_thread_flag;
       // QMutex mtx;
//    private slots:
//        void Fun_1();
    public:

        CGrabResultPtr ptrGrabResult;
        //camera
        CInstantCamera m_camera;
        //camera serial number
        string serial_number;
        //device list
        Pylon::DeviceInfoList_t m_devices;

        Pylon::CTlFactory *m_pTlFactory;






        CDeviceInfo  cameraInfo;
        //last img
        CGrabResultPtr m_LastGrabbedImage;
        //single image
        CGrabResultPtr ResultGrabone;
        // Will be used to lock the access.
        GenApi::CLock m_lock;
        cv::Mat m_cvimg_mat;
        cv::Mat m_grabImg;



  private:
       CImageFormatConverter converter_;
       CPylonImage pylon_image_;
      // EPixelType pixel_type_;

//qt
  public:     //qt
       QImage Mat2QImage(cv::Mat cvImg);
       cv::Mat QImage_to_cvMat( const QImage &image, bool inCloneImageData);
       void QImage_to_cvMat1( const QImage &image, cv::Mat &img);
       QImage m_qimage;

  signals:
    void imageupdate(QImage);


};

#endif // BASLERCAMERA_H
