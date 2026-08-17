#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QByteArray>
#include <QTimer>
#include <QFont>
#include <QDir>
#include <QCoreApplication>
#include <sstream>
#include <iostream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     bcam=new baslercamera;
     connect(bcam,SIGNAL(imageupdate(QImage)),this,SLOT(updata_grayimage_qlabel(QImage)));

     f_saveimg=false;
     f_savelaserimg=false;
     const QString outputRoot = QCoreApplication::applicationDirPath() + "/output";
     const QString laserDir = outputRoot + "/laser";
     const QString grayDir = outputRoot + "/gray";
     QDir().mkpath(laserDir);
     QDir().mkpath(grayDir);
     laser_img_path = (laserDir + "/").toStdString();
     gray_img_path = (grayDir + "/").toStdString();
     gray_count=1;
     laser_count=1;
}

MainWindow::~MainWindow()
{
    delete bcam;
    delete ui;
}

void MainWindow::updata_grayimage_qlabel(QImage img)
{

    bool inCloneImageData = true;
   // cout<<11<<endl;
    cv::Mat cvimg1 = bcam->QImage_to_cvMat( img, inCloneImageData );
//    cv::Mat cvimg;
//    bcam->QImage_to_cvMat1(img, cvimg);

    //cout<<22<<endl;

    cv::Mat cvimg;
    cv::Size m_showsize=cv::Size(640,480);
    cv::resize(cvimg1,cvimg,m_showsize);
   // cout<<cvimg.cols<<"  "<<cvimg.rows<<endl;
    if(!cvimg.empty())
    {
        if(f_saveimg)
        {

            std::ostringstream oss;
            oss <<gray_img_path<< gray_count;
            oss <<".bmp";
            std::string image_path = oss.str();
            std::cout << image_path << std::endl;
            cv::imwrite(image_path,cvimg1);
            gray_count++;
            f_saveimg=false;

        }

        if(f_savelaserimg)
        {
            std::ostringstream oss;
            oss <<laser_img_path<< laser_count;
            oss <<".bmp";
            std::string image_path = oss.str();
            std::cout << image_path << std::endl;
            cv::imwrite(image_path,cvimg1);
            laser_count++;
            f_savelaserimg=false;
        }



        cv::Mat m_show_img;
        cv::Size m_showsize=cv::Size(640,480);
        cv::resize(cvimg,m_show_img,m_showsize);
        QImage m_qqimg= bcam->Mat2QImage(m_show_img);
        ui->camera_gray_frame->setPixmap(QPixmap::fromImage(m_qqimg));


      //  QImage newImg = img.scaled(640, 480);
       // ui->camera_gray_frame->setPixmap(QPixmap::fromImage(newImg));
    }

}
void MainWindow::on_pushButton_stopcam_clicked()
{
    bcam->deleteAll();
}
void MainWindow::on_pushButton_camstart_clicked()
{
    // Leave serial_number empty to connect to the first available Basler camera.
    // Set a serial number here only when a specific device must be selected.
    bcam->serial_number = "";
    bcam->ConnectCamera();
    //set parameters  between ConnectCamera() and openCamera().
   //std::cout<<12<<std::endl;
    bcam->setWidth(bcam->m_camera,1400);  //1456
    bcam->setheight(bcam->m_camera,1050);  //1088
    bcam->setPacketSize(bcam->m_camera,8000);
    bcam->setInterPacketDelay(bcam->m_camera,1000);

    bcam->setFramerate(bcam->m_camera,10);
    bcam->setExposureTime(bcam->m_camera,25000);

    //digital IO  Control
//    string lineselector="Out1";
//     bcam->setLineSelector(bcam->m_camera,lineselector);
//    string linesource="ExposureActive";
//    bcam->setLineSource(bcam->m_camera,linesource);
//    bcam->setLineInverter(bcam->m_camera,true);
    bcam->openCamera();

}

void MainWindow::on_pushButton_setExposure_clicked()
{
    int exp= ui->lineEdit_Exposure->text().toInt();
    bcam->setExposureTime(bcam->m_camera,exp);
}

void MainWindow::on_pushButton_setframerate_clicked()
{
   double framerate= ui->lineEdit_Framerate->text().toDouble();
   bcam->setFramerate(bcam->m_camera,framerate);
}

void MainWindow::on_pushButton_getExposure_clicked()
{
    int exp=bcam->getExposureTime(bcam->m_camera);
    QString exp_s=QString::number(exp,10);
    ui->lineEdit_Exposure->setText(exp_s);
}

void MainWindow::on_pushButton_getFramerate_clicked()
{
     float framerate=bcam->getFramerate(bcam->m_camera);
     QString framerate_s=QString::number(framerate,10,2);
     ui->lineEdit_Framerate->setText(framerate_s);
}

void MainWindow::on_pushButton_savegrayimg_clicked()
{
    int exp= ui->lineEdit_Exposure_gray->text().toInt();
    bcam->setExposureTime(bcam->m_camera,exp);
    sleep(1);
    f_saveimg=true;
}


void MainWindow::on_pushButton_savelaserimg_clicked()
{
    int exp= ui->lineEdit_Exposure_laser->text().toInt();
    bcam->setExposureTime(bcam->m_camera,exp);
    sleep(1);
    f_savelaserimg=true;
}


void MainWindow::on_lineEdit_Exposure_cursorPositionChanged(int arg1, int arg2)
{

}


void MainWindow::on_lineEdit_Exposure_gray_cursorPositionChanged(int arg1, int arg2)
{

}

