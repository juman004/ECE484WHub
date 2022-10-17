#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPainter>
#include <QtDebug>
#include <algorithm>
#include <cstdlib>
#include <iostream>

//#include "opencv2/opencv.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int sliderVal;
    QString file1;
    QString file2;
    QString file3;
    QImage img1;
    QImage img2;
    QImage img3;
    QImage img4;

    QString getFile1(){return file1;}
    QString getFile2(){return file2;}
    QString getFile3(){return file3;}

    QImage getimg1(){return img1;}
    QImage getimg2(){return img2;}
    QImage getimg3(){return img3;}
    QImage getimg4(){return img4;}

    int getsliderVal(){return sliderVal;}
    QImage overLay(QImage org, QImage over);
    QImage adjBright(QImage &image,int brightness);
    QImage adjContrast(QImage &image, int factor);



private slots:
    void on_pushButton_clicked();



    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void setsliderVal(int value){sliderVal = value;}

    void setFile1(QString file){file1 = file;}
    void setFile2(QString file){file2 = file;}
    void setFile3(QString file){file3 = file;}

    void setimg1(QImage img){img1 = img;}
    void setimg2(QImage img){img2 = img;}
    void setimg3(QImage img){img3 = img;}
    void setimg4(QImage img){img4 = img;}

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_bn_Slider_sliderMoved(int position);

    void on_bn_Slider_2_sliderReleased();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
