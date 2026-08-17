#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chrono>
#include <string>

#include "baslercamera.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_camstart_clicked();
    void on_pushButton_stopcam_clicked();
    void updata_grayimage_qlabel(QImage img);
    void on_pushButton_setExposure_clicked();
    void on_pushButton_setframerate_clicked();
    void on_pushButton_getExposure_clicked();
    void on_pushButton_getFramerate_clicked();
    void on_pushButton_savegrayimg_clicked();
    void on_pushButton_savelaserimg_clicked();
    void on_lineEdit_Exposure_cursorPositionChanged(int arg1, int arg2);
    void on_lineEdit_Exposure_gray_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::MainWindow *ui;
    baslercamera *bcam;

    bool f_saveimg;
    bool f_savelaserimg;
    std::string laser_img_path;
    std::string gray_img_path;
    int gray_count;
    int laser_count;
};

#endif // MAINWINDOW_H
