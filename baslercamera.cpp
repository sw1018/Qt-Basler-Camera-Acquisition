#include "baslercamera.h"



baslercamera::baslercamera()
{
    cameraInit();
     m_thread_flag=false;

            // m_cvimg_mat
}


void baslercamera::cameraInit()
{
     m_thread_flag=false;
    //Pylon::PylonAutoInitTerm autoInitTerm;
     //std::cout<<03<<std::endl;
    Pylon::PylonInitialize();
    //std::cout<<04<<std::endl;
    m_pTlFactory = &CTlFactory::GetInstance();
    //std::cout<<05<<std::endl;
    m_devices.clear();
    //std::cout<<06<<std::endl;
    m_pTlFactory->EnumerateDevices(m_devices);
    //std::cout<<07<<std::endl;
    if (m_devices.empty())
      {
          cout<<"no camera"<<endl;

      }

}


void baslercamera::ConnectCamera()
{
    //std::cout<<01<<std::endl;
    cameraInit();
    //std::cout<<02<<std::endl;
    if (serial_number == "")
        {
          // Create an instant camera object for the camera device found first.
          m_camera.Attach(CTlFactory::GetInstance().CreateFirstDevice());   //连接第一个设备
          cout<<"camera  first connect successful"<<endl;
        }
        else
        {
          // Look up the camera by its serial number
          for (size_t i=0; i<m_devices.size(); i++)
          {
            if (m_devices[i].GetSerialNumber().c_str() == serial_number)
            {
              m_camera.Attach(m_pTlFactory->CreateDevice(m_devices[i]));
              cout<<"camera connect successful"<<endl;
              break;
            }
          }
       }
    m_camera.RegisterConfiguration(new CAcquireContinuousConfiguration , RegistrationMode_ReplaceAll, Cleanup_Delete);
    m_camera.Open();
}

void baslercamera::openCamera()
{
    m_camera.StartGrabbing();

    m_thrd_1 = new std::thread(&baslercamera::Fun_1, this);
}

void baslercamera::Fun_1()
{
    //cvNamedWindow("point2",CV_WINDOW_AUTOSIZE);
    //m_camera.StartGrabbing();
    while (m_camera.IsGrabbing())
    {
           m_camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_Return);
           //GenApi::CIntegerPtr width(m_camera.GetNodeMap().GetNode("Width"));
          // GenApi::CIntegerPtr height(m_camera.GetNodeMap().GetNode("Height"));

           if (ptrGrabResult->GrabSucceeded())
           {


               GenApi::CEnumerationPtr m_pixelformat(m_camera.GetNodeMap().GetNode("PixelFormat"));

               // cout<<m_pixelformat->ToString()<<endl;

                if(m_pixelformat->ToString()=="Mono8")
                {
                    converter_.Convert(pylon_image_, ptrGrabResult);
                    mtx.lock();
                    //mutexPause.lock();
                   // cv::Mat  m_cvimg_rgb(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3);
                    m_cvimg_mat = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1,(uint8_t*)pylon_image_.GetBuffer());
                    mtx.unlock();
                    //mutexPause.unlock();

//                    m_qimage = Mat2QImage(m_cvimg_mat);
//                     emit imageupdate(m_qimage);


//                    cv::Mat m_show_img;
//                    cv::Size m_showsize=cv::Size(320,240);
//                    cv::resize(m_cvimg_mat,m_show_img,m_showsize);
//                    QImage m_qqimg= Mat2QImage(m_show_img);
                    m_qimage = Mat2QImage(m_cvimg_mat);
                    emit imageupdate(m_qimage);

                }
                else
                {
                    converter_.OutputPixelFormat=PixelType_BGR8packed;
                    converter_.Convert(pylon_image_, ptrGrabResult);
                     mtx.lock();
                     //mutexPause.lock();

                     m_cvimg_mat = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3,(uint8_t*)pylon_image_.GetBuffer());

                     cout<<1<<endl;
                     mtx.unlock();
                     //cv::resize(m_cvimg_rgb,m_cvimg_rgb,cv::Size(640,480),CV_INTER_CUBIC);
                     m_qimage = Mat2QImage(m_cvimg_mat);
                     cout<<2<<endl;
                     emit imageupdate(m_qimage);
//                     cout<<3<<endl;
                    // mutexPause.unlock();



                }
               //
           }
           else
           {
             cout<<"Error_: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription()<<endl;
           }

    if(m_thread_flag)
    {
        break;
    }


    }
}

QImage baslercamera::Mat2QImage(cv::Mat cvImg)
{
    cv::Mat TmpMat;
    QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {
        //qDebug()<<"rgb";
        cv::cvtColor(cvImg,cvImg,COLOR_BGR2RGB);
       // cout<<2<<endl;
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
         //cout<<3<<endl;
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }

    return qImg;
}
cv::Mat baslercamera::QImage_to_cvMat( const QImage &image, bool inCloneImageData) {
  switch ( image.format() )
  {
     // 8-bit, 4 channel
     case QImage::Format_RGB32:
     {
        cv::Mat mat( image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine() );
        return (inCloneImageData ? mat.clone() : mat);
     }

     // 8-bit, 3 channel
     case QImage::Format_RGB888:
     {
        if ( !inCloneImageData ) {
           cout << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";
        }
        cout<<111<<endl;
        QImage swapped = image.rgbSwapped();
        cout<<222<<endl;
        return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
     }

     // 8-bit, 1 channel
     case QImage::Format_Indexed8:
     {
        cv::Mat  mat( image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine() );

        return (inCloneImageData ? mat.clone() : mat);
     }

     default:
        qDebug("Image format is not supported: depth=%d and %d format\n", image.depth(), image.format());
        break;
  }

  return cv::Mat();
}


void baslercamera::QImage_to_cvMat1( const QImage &image, cv::Mat &img)
{
  switch ( image.format() )
  {

     switch(image.format())
     {

     case QImage::Format_RGB888:
     {
         //cv::Mat img = new cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());

         //cv::cvtColor(**pImg, img, CV_BGR2RGB);
         cv::Mat cvimg = cv::Mat(image.height(), image.width(), CV_8UC3,(void*)image.constBits(), image.bytesPerLine());
         img=img.clone();
         cv::cvtColor(img, img, COLOR_BGR2RGB);

         break;
     }
     case QImage::Format_Indexed8:
         cv::Mat cvimg = cv::Mat(image.height(), image.width(), CV_8UC3,(void*)image.constBits(), image.bytesPerLine());
         img=cvimg.clone();
         break;
     }
  }
}


cv::Mat baslercamera::GetGrabImageMat()
{
    mtx.lock();
    m_grabImg = m_cvimg_mat.clone();
    mtx.unlock();
    return m_grabImg;
}


void baslercamera::setWidth(CInstantCamera& camera,int width)
{
    string name ="Width";
    handle_basler_int_parameter(camera, name, width);
}
int baslercamera::getWidth(CInstantCamera& camera)
{
   string name ="Width";
   int width=get_basler_int_parameter(camera,name);
   return width;
}

void baslercamera::setheight(CInstantCamera& camera,int height)
{
    string name ="Height";
    handle_basler_int_parameter(camera, name,  height);
}
int baslercamera::getheight(CInstantCamera& camera)
{
    string name ="Height";
    int height=get_basler_int_parameter(camera,name);
    return height;
}


void baslercamera::setPacketSize(CInstantCamera& camera,int packetsize)
{
    string name ="GevSCPSPacketSize";
    handle_basler_int_parameter(camera, name,  packetsize);
}
void baslercamera::setInterPacketDelay(CInstantCamera& camera,int interpacketdelay)
{
    string name ="GevSCPD";
    handle_basler_int_parameter(camera, name,  interpacketdelay);
}

void baslercamera::setExposureTime(CInstantCamera& camera,float exp)
{
    string name ="ExposureTime";
    handle_basler_float_parameter(camera, name, exp);
}
int baslercamera::getExposureTime(CInstantCamera& camera)
{
    string name ="ExposureTime";


    int exp=get_basler_int_parameter(camera,name);
    return exp;
}

void baslercamera::setFramerate(CInstantCamera& camera,double framerate)
{
    string name="AcquisitionFrameRateEnable";
    bool frameenable=true;
    handle_basler_boolean_parameter(camera, name, frameenable);

    string name1="AcquisitionFrameRate";

    handle_basler_float_parameter(camera, name1, framerate);

}
float baslercamera::getFramerate(CInstantCamera& camera)
{
    string name ="AcquisitionFrameRate";

    float framerate=get_basler_float_parameter(camera,name);
    return framerate;
}

//Digital IO output
void baslercamera::setLineSelector(CInstantCamera& camera,string lineselector)
{
    string name ="LineSelector";
    handle_basler_enum_parameter(camera, name,  lineselector);
     //handle_basler_boolean_parameter(camera, name,lineselector);
}
void baslercamera::setLineSource(CInstantCamera& camera,string linesource)
{
    string name ="LineSource";
    handle_basler_enum_parameter(camera, name,  linesource);
}
void baslercamera::setLineInverter(CInstantCamera& camera,bool lineinverter)
{
    string name ="LineInverter";
    handle_basler_boolean_parameter(camera, name,lineinverter);
}


void baslercamera::handle_basler_boolean_parameter(CInstantCamera& camera, string name, bool value)

{
  INodeMap& nodemap = camera.GetNodeMap();
  try
  {
    cout<< "Setting boolean param  "<<name<<" to "<<value<<endl;

    CBooleanPtr this_node(nodemap.GetNode(name.c_str()));
    if (!IsWritable(this_node))
    {
      cout<<"Basler parameter '" << name << "' isn't writable or doesn't exist."<<endl;

      return;
    }
    this_node->SetValue(value);
  }
  catch (const GenericException& e)
  {
    cout<<"set boolean_parameter "<< e.GetDescription()<<endl;

  }
}


void baslercamera::handle_basler_int_parameter(CInstantCamera& camera, string name, int value)
{
  INodeMap& nodemap = camera.GetNodeMap();
  try
  {
      cout<< "Setting int param  "<<name<<" to "<<value<<endl;

    CIntegerPtr this_node(nodemap.GetNode(name.c_str()));
    if (!IsWritable(this_node))
    {
        cout<<"Basler int parameter '" << name << "' isn't writable or doesn't exist."<<endl;

      return;
    }
    this_node->SetValue(value);
  }
  catch (const GenericException& e)
  {
       cout<<"set int_parameter "<< e.GetDescription()<<endl;

  }
}

int baslercamera::get_basler_int_parameter(CInstantCamera& camera, string name)
{
  INodeMap& nodemap = camera.GetNodeMap();
  try
  {
     // cout<< "getting int param  "<<name<<endl;

    CIntegerPtr this_node(nodemap.GetNode(name.c_str()));

    if (!IsReadable(this_node))
    {
        cout<<"Basler int parameter '" << name << "' isn't readable or doesn't exist."<<endl;

      return -404;
    }
    int value=this_node->GetValue();
    return value;

  }
  catch (const GenericException& e)
  {
       cout<<"get int_parameter "<< e.GetDescription()<<endl;
  }

  return -404;
}



void baslercamera::handle_basler_float_parameter(CInstantCamera& camera, string name, double value)
{
  INodeMap& nodemap = camera.GetNodeMap();
  try
  {
      cout<< "Setting float param  "<<name<<" to "<<value<<endl;

    CFloatPtr this_node(nodemap.GetNode(name.c_str()));
    if (!IsWritable(this_node))
    {
         cout<<"Basler float parameter '" << name << "' isn't writable or doesn't exist."<<endl;

      return;
    }


    this_node->SetValue(value);
  }
  catch (const GenericException& e)
  {
      cout<<"set float_parameter "<< e.GetDescription()<<endl;

  }
}


float baslercamera::get_basler_float_parameter(CInstantCamera& camera, string name)
{
  INodeMap& nodemap = camera.GetNodeMap();

  try
  {
      cout<< "getting float param  "<<name<<endl;

    CFloatPtr this_node(nodemap.GetNode(name.c_str()));
    if (!IsReadable(this_node))
    {
         cout<<"Basler float parameter '" << name << "' isn't readable or doesn't exist."<<endl;

      return -404;
    }

    float value=this_node->GetValue();
    return value;

  }
  catch (const GenericException& e)
  {
      cout<<"get float_parameter "<< e.GetDescription()<<endl;
  }

  return -404.0f;
}

void baslercamera::handle_basler_enum_parameter(CInstantCamera& camera, string name, string value)
{
  INodeMap& nodemap = camera.GetNodeMap();
  try
  {
      cout<< "Setting enum param  "<<name<<" to "<<value<<endl;

    CEnumerationPtr this_node(nodemap.GetNode(name.c_str()));
    if (!IsWritable(this_node))
    {
         cout<<"Basler enum parameter '" << name << "' isn't writable or doesn't exist."<<endl;

      return;
    }//Output Line 1
    if (!IsAvailable(this_node->GetEntryByName(value.c_str())))
    {
      cout<<"enum_Valuer '" << value << "' isn't available for basler param '" << name << "'."<<endl;
      return;
    }
    this_node->FromString(value.c_str());
  }
  catch (const GenericException& e)
  {
      cout<<"set enum_parameter "<< e.GetDescription()<<endl;

  }
}


void baslercamera::stopAcquire()
{
    cout << "stop camera" << endl;

    try {
        if (m_camera.IsGrabbing())
        {
            m_camera.StopGrabbing();
        }
    } catch (GenICam::GenericException &e)
    {
        cout<<"Error_stopGrabbing: " << e.GetDescription() <<endl;
    }
}


void baslercamera::closeCamera()
{
    try {
        if (m_camera.IsOpen())
        {
            m_camera.Close();
            m_camera.DetachDevice();

        }
    } catch (GenICam::GenericException &e)
    {
        cout<<"Error_closeCamera: " << e.GetDescription() <<endl;
    }
}

void baslercamera::deleteAll()
{
     m_thread_flag = true;

     if (m_thrd_1 != nullptr)
     {
         if (m_thrd_1->joinable())
             m_thrd_1->join();
         delete m_thrd_1;
         m_thrd_1 = nullptr;
     }

     stopAcquire();
     m_grabImg.release();
}




baslercamera::~baslercamera()
{
    deleteAll();
//    if (m_thrd_1->joinable())
//       m_thrd_1->join();
    stopAcquire();
    closeCamera();
   // PylonTerminate();
}





void baslercamera::GrabOne()
{
        //CGrabResultPtr ResultGrab;
        // Start image grabbing if a camera device was created already
        if (m_camera.IsPylonDeviceAttached())
        {
            m_camera.GrabOne(5000, ptrGrabResult, TimeoutHandling_ThrowException);


            if (ptrGrabResult->GrabSucceeded())
            {

                converter_.Convert(pylon_image_, ptrGrabResult);
                GenApi::CEnumerationPtr m_pixelformat(m_camera.GetNodeMap().GetNode("PixelFormat"));

                // cout<<m_pixelformat->ToString()<<endl;

                 if(m_pixelformat->ToString()=="Mono8")
                 {
                      mtx.lock();
                    // mutexPause.lock();

                     m_cvimg_mat = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1,(uint8_t*)pylon_image_.GetBuffer());
                     mtx.unlock();
                     // mutexPause.unlock();

                     // m_qimage = Mat2QImage(m_cvimg_mat);
                     // emit imageupdate(m_qimage);

                 }
                 else
                 {
                      mtx.lock();
                      //mutexPause.lock();
                      m_cvimg_mat = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3,(uint8_t*)pylon_image_.GetBuffer());
                      mtx.unlock();
                     // mutexPause.unlock();

                 }
                //
            }
            else
            {
              cout<<"Error_: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription()<<endl;
            }

        }
}


cv::Mat baslercamera::GetGraboneImageMat()
{
    return m_cvimg_mat.clone();
}



