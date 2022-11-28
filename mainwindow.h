#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPainter>
#include <QtDebug>
#include <QDebug>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <QUdpSocket>
#include <string>
#include <QtGlobal>
#include <QBuffer>
#include <QImageWriter>
#include <QTcpSocket>
#include <QFile>
#include <QIODevice>
#include <QAbstractSocket>
#include <QChar>


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


    QString ipFPGA;
    int portNumber;
    QUdpSocket *socket;

    QTcpSocket *socket1;
    QByteArray data;

    int imageHeight;
    int imageWidth;

    int brightValue=0;
    int contrastValue=0;


    QString file1;
    QString file2;
    QString file3;
    QImage img1;  //base img
    QImage img2;  //overLay img
    QImage img3; //working Img
    QImage img4;

    QString getipFPGA(){return ipFPGA;}
    int getportNumber(){return portNumber;}

    int getimageHeight(){return imageHeight;}
    int getimageWidth(){return imageWidth;}




    int getbrightValue(){return brightValue;}
    int getcontrastValue(){return contrastValue;}



    QString getFile1(){return file1;}
    QString getFile2(){return file2;}
    QString getFile3(){return file3;}

    QImage getimg1(){return img1;}
    QImage getimg2(){return img2;}
    QImage getimg3(){return img3;}
    QImage getimg4(){return img4;}

    int getsliderVal(){return sliderVal;}
    QImage overLay(QImage org, QImage over);
    QImage adjBright(QImage &pic,int brightness);
    QImage adjContrast(QImage &image, int factor);

    void valueSplitter();
    void sendToUDPServer(QString x, int port);

    void sendToTCPServer(QByteArray data,quint32 sizeImage);





private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void setsliderVal(int value){sliderVal = value;}


    void setbrightVal (int value){brightValue = value;}
     void setcontVal (int value){contrastValue = value;}




    void setipFPGA(QString ip) {ipFPGA = ip;}
    void setportNumber(int port) {portNumber = port;}

    void setimageHeight(int height){imageHeight = height;}
    void setimageWidth(int width){imageWidth = width;}


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



    void on_bn_Slider_sliderMoved(int position);

    void on_bn_Slider_2_sliderReleased();


    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    //void on_pushButton_16_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_16_clicked();

private:
    Ui::MainWindow *ui;
    //QUdpSocket *socket;

public slots:
   /*
    void connected();
    void disconnected();
    void readyRead();
    void onReadyRead();
*/
    void connectToHost();


};


#endif // MAINWINDOW_H
