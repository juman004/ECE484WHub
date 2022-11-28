#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
 //   socket = new QUdpSocket(this);
    setipFPGA("192.168.0.16");
    setportNumber(1234);

    setimageHeight(200);
    setimageWidth(200);

    QString height;
    QString width;

    height.setNum(getimageHeight());
    width.setNum(getimageWidth());
    ui->label_17->setText(height);
    ui->label_18->setText(width);

    ui->label_6->setText(getipFPGA());
    QString temp;
    temp.setNum(getportNumber());
    ui->label_9->setText(temp);
   // socket -> bind(QHostAddress(getipFPGA()),getportNumber());
    data.resize(4);
    // TCP Socket Initalization
    socket1= new QTcpSocket (this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.bmp)")); //get file path from user
    setFile1(filename);    // save file path incase needed for later
    if (QString::compare(filename, QString()) != 0)  // make sure filepath is valid
    {

        QImage image;  // create qImage instance
        bool valid = image.load(filename);  //load file path into QImage, true if succesful


        if (valid)  // if successful
        {
        ui->label_2->setScaledContents(true); // ensures that the pictures scales to the lable
         ui->label_2->setPixmap(QPixmap::fromImage(image));  //display the image
         setimg1(image); // save QImage to data structure

        }

    }
}


void MainWindow::on_pushButton_2_clicked()
{

    /** Same procedure as pushbutton one */
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.bmp)"));
    setFile2(filename);
    if (QString::compare(filename, QString()) != 0)
    {

        QImage image;
        bool valid = image.load(filename);
        if (valid)
        {
        ui->label_3->setScaledContents(true);
        ui->label_3->setPixmap(QPixmap::fromImage(image));
        setimg2(image);
        }

    }
}
QImage MainWindow::overLay(QImage org, QImage over)
{

    QImage fImage = QImage (org.size(), QImage::Format_Grayscale8); //create a QImage final  Instance based on size of orginal image and format to RBG16
    QPainter overlay(&fImage);  // CREATE QPainter instacne based final image. passed by referecne to ensure changes are applied
    QRect loc1(0,0,400,300);   // create QrRect instance that will be used later for overlay
    overlay.drawImage(0,0,org);  // draw the orginal image onto the QPainter objetc
    overlay.drawImage(loc1,over);  // Draw the rectangle over the Qpainter object and fill that rectangle with overlay image
    bool save =fImage.save("test.bmp");  // save the overlay image on computer
    Q_ASSERT(save); // make sure it saved
    return fImage; // return the final image

}

void MainWindow::on_pushButton_3_clicked()  // overlay button
{

    QImage final= overLay(getimg1(),getimg2());  // pass overlay fucntion the two images which will retun the final overlay image
    ui->label->setScaledContents(true);          // set label to scaled so entire picture fits
    ui->label->setPixmap(QPixmap::fromImage(final));  // display finaloverlay image in label

}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1); // go to next task
}


void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0); // go to task one

}


void MainWindow::on_pushButton_6_clicked()
{
        close();
}

// load task 2 image



QImage MainWindow::adjBright(QImage &pic,int brightness)
{

          /* Two Parameters, the image being adjusted and the factor of adjustemnt **/
    //qDebug("Image Dimension: [ %d, %d ]", pic.height(), pic.width());
           // QImage temp2 ("test.bmp");
            QImage image= pic.scaled(512,512, Qt::IgnoreAspectRatio, Qt::FastTransformation);  /// make a copy of orginal image
            QColor oldColor; // create a QColor instance, holds RBG value
            int delta = brightness;   // copy brightness factor

           QImage * newImage = new QImage(image);// new image QImage Pointer, based on size of orginal image format chanegd to RBG32 so RGB values can be cha
           bool temp = newImage->isNull();
           qDebug("Is image valid? %d", temp);
           qDebug("Image Dimension: [ %d, %d ]", newImage->height(), newImage->width());




            for(int y=0; y<newImage->height(); ++y)
            {
                // qDebug("Factir pixel : %d", y);
                for(int x=0; x<newImage->width(); ++x)
                {

                    //qDebug("Factir pixel : %d", x);
                    oldColor = image.pixelColor(y,x);

                    int newcolorVal = oldColor.value() + delta;
                    if (newcolorVal > 255)
                    {

                        newcolorVal = 255;
                    }
                    if (newcolorVal < 0)
                    {

                        newcolorVal = 0;
                    }
                    //qDebug("Factir pixel : %d", colorVal);

                   // r = oldColor.red() + delta;

                   // g = oldColor.green() +delta;

                   // b = oldColor.blue() + delta;

                   newImage->setPixel(y,x,newcolorVal);

                }

            }

            return *newImage;


}
QImage MainWindow::adjContrast(QImage &image, int factor)
{
    QImage origin = image.scaled(512,512, Qt::IgnoreAspectRatio, Qt::FastTransformation);;  /// make a copy of orginal image
    QColor oldColor; // create a QColor instance, holds RBG value
    int minintensity=255;
    int maxintensity =0;

   QImage  newImage = origin;// new image QImage Pointer, based on size of orginal image format chanegd to RBG32 so RGB values can be cha
    //qDebug("Factir pixel : %d %d", newImage.height(), newImage.width());

   for(int y=0; y<newImage.height(); ++y)  // go throuhg pixesl and find max and min intensity
   {
       // qDebug("Factir pixel : %d", y);
       for(int x=0; x<newImage.width(); ++x)
       {

           //qDebug("Factir pixel : %d", x);
           oldColor = origin.pixelColor(y,x);

           int intenseVal = oldColor.value();

            minintensity= std::min(minintensity,intenseVal);
           maxintensity= std::max(maxintensity,intenseVal);
       }

    }
    if (factor > 0)   // if factor is postive ensure contrast equation doesn't pass 255 or 0
    {
        if ((minintensity-factor) < 0 )
        {
            minintensity=0;
        }
        else
        {
             minintensity= minintensity-factor;
        }
        if ((maxintensity= maxintensity+factor) > 255)
        {

            maxintensity=255;
        }
       else
        {
            maxintensity= maxintensity+factor;
        }
    }

   if (factor < 0)  //if factor is negative
   {


       minintensity = minintensity -factor;
       maxintensity= maxintensity-factor;

   }



   for(int y=0; y<newImage.height(); ++y)
   {

       for(int x=0; x<newImage.width(); ++x)
       {


            oldColor = origin.pixelColor(y,x);

            double intenseVal = oldColor.value();
            double top = intenseVal-minintensity;
            double bottom = maxintensity - minintensity;
            double newIntensity = 255* (top/bottom); // contrast adjuster based on the range of pixel intensities
            if (newIntensity < 0)  // make sure the new intensity is not out of bounds
            {
                newIntensity = 0;
          }
            if (newIntensity > 255)
            {
                newIntensity=255;
            }
            newImage.setPixel(y,x,newIntensity); // set the new pixel
       }

    }


    return newImage; // return the image

}




void MainWindow::on_bn_Slider_sliderMoved(int position)
{

    setbrightVal(position);
   //valueSplitter();
    QImage copy= getimg3();


    // QImage copy("test.bmp");

    QImage orgImg= adjBright(copy,position);
   // sendBrightnessToFPGA();
    setimg4(orgImg);
    ui->lbl_image_5->setPixmap(QPixmap::fromImage(orgImg));
     QImage temp = orgImg.scaled(getimageWidth(),getimageHeight(),Qt::IgnoreAspectRatio);
     temp.save("test.bmp"); // save the image

}


void MainWindow::on_bn_Slider_2_sliderReleased()
{
    int value = ui->bn_Slider_2->value();
    setcontVal(value);
   // valueSplitter();
    qDebug("Factir pixel : %d", value);
    QImage copyBright = getimg4();
    QImage contImg= adjContrast(copyBright,value); // call fun
     //sendBrightnessToFPGA();
    ui->lbl_image_5->setPixmap(QPixmap::fromImage(contImg));
     QImage temp = contImg.scaled(getimageWidth(),getimageHeight(),Qt::IgnoreAspectRatio);
    temp.save("test.bmp"); // save the image
}
void MainWindow::sendToUDPServer(QString x, int port)
{
    QHostAddress q;
    q.setAddress(x);
    socket->writeDatagram(data,q,port);
 }




 void MainWindow::valueSplitter()
 {

    int brightTemp = getbrightValue();
    int contTemp = getcontrastValue();
    qDebug("Values  : %d %d", brightTemp, contTemp);
    std::string temp = std::to_string(brightTemp);
    std::string temp1 = std::to_string(contTemp);

    char bright[] = "00";
    char contrast[] = "00";

    if ((brightTemp>= 0) && (brightTemp <= 9) && (contTemp>= 0) && (contTemp <= 9) )
    {
       bright[0] = '0';
       bright[1] = temp[0];
       contrast[0]= '0';
       contrast[1] =temp1[0];
    }

   else
    {

        if (temp.size()==1 && temp1.size() ==2)
        {
            qDebug("I am here");
            bright[0] = '0';
            bright[1] = temp[0];
            contrast[0]= temp1[0];
            contrast[1] =temp1[1];
            qDebug("bright : %s ", bright);
            qDebug("contrast: %s ", contrast);
        }
        if (temp1.size()==1 && temp.size() == 2)
        {
            bright[0] = temp[0];
            bright[1] = temp[1];
            contrast[0]= '0';
            contrast[1] =temp1[0];
        }
        if (temp.size() == 2 && temp1.size()== 2)
         {
        bright[0] = temp[0];
        bright[1] = temp[1];
        contrast[0]= temp1[0];
        contrast[1] =temp1[1];

        }

      }
    data[0] = bright[0];
    data[1] = bright[1];
    data[2] = contrast[0];
    data[3] = contrast[1];
    //QString test(data);
    //qDebug("dataArrya : %s ", test.toStdString().c_str());
    sendToUDPServer(getipFPGA(),getportNumber());

 }

void MainWindow::on_pushButton_8_clicked()
{
    ui->label_6->setText(ui->lineEdit->text());
    setipFPGA(ui->lineEdit->text());


}

void MainWindow::on_pushButton_9_clicked()
{
    ui->label_9->setText(ui->lineEdit_2->text());
    setportNumber(ui->lineEdit_2->text().toUInt());

}


void MainWindow::on_pushButton_10_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.bmp)")); //get file path from user
    setFile1(filename);    // save file path incase needed for later
    if (QString::compare(filename, QString()) != 0)  // make sure filepath is valid
    {

        QImage image;  // create qImage instance
        bool valid = image.load(filename);  //load file path into QImage, true if succesful


        if (valid)  // if successful
        {
        ui->label_10->setScaledContents(true); // ensures that the pictures scales to the lable
         ui->label_10->setPixmap(QPixmap::fromImage(image));  //display the image
         setimg1(image); // save QImage to data structure

        }

    }




}


void MainWindow::on_pushButton_11_clicked()
{
    /** Same procedure as pushbutton one */
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.bmp)"));
    setFile2(filename);
    if (QString::compare(filename, QString()) != 0)
    {

        QImage image;
        bool valid = image.load(filename);
        if (valid)
        {
        ui->label_11->setScaledContents(true);
        ui->label_11->setPixmap(QPixmap::fromImage(image));
        setimg2(image);
        }

    }





}


void MainWindow::on_pushButton_12_clicked()
{

}
 void MainWindow::on_pushButton_13_clicked()  //load org button
 {
     QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.bmp)"));
     setFile1(filename);
     if (QString::compare(filename, QString()) != 0)
     {

         QImage image;
         bool valid = image.load(filename);
         if (valid)
         {
         ui->label_13->setScaledContents(true);
         ui->label_13->setPixmap(QPixmap::fromImage(image));
         setimg1(image);  //orgina load image copy
         setimg3(image); // if for brightness
         setimg4(image); // if for contrast algorthim
        // QRect loc1(0,.0,800,400);
         QImage temp = image.scaled(getimageWidth(),getimageWidth(),Qt::IgnoreAspectRatio);

         QSize size = temp.size();
          qDebug("size of size array: [%d], [%d]", size.rheight(), size.rwidth());
         temp.save("test.bmp");
         }

     }

     ui->lbl_image_5->setScaledContents(true);
     ui->lbl_image_5->setPixmap(QPixmap::fromImage(getimg1()));



 }

void MainWindow::on_pushButton_14_clicked()  //load second img and overlay them
{

    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.bmp)"));
    setFile2(filename);
    if (QString::compare(filename, QString()) != 0)
    {
        QImage image;
        bool valid = image.load(filename);
        if (valid)
        {
        //ui->label_14->setScaledContents(true);
       // ui->label_14->setPixmap(QPixmap::fromImage(image));
        setimg2(image); // copy of unedited overlay
        //setimg3(image);
        }
    }
    QImage overLayImg = overLay(getimg1(),getimg2());
    ui->label_14->setScaledContents(true);
    ui->label_14->setPixmap(QPixmap::fromImage(overLayImg));

    QImage temp = overLayImg.scaled(getimageWidth(),getimageHeight(),Qt::IgnoreAspectRatio);
    //QSize size = temp.size();
    // qDebug("size of size array: [%d], [%d]", size.rheight(), size.rwidth());
     temp.save("test.bmp");


    QImage realOverlay("test.bmp", "BMP");
   // QImage realOverlay;
   // realOverlay.load("test.bmp",QImage::Format_Grayscale8);




    setimg3(realOverlay);
    setimg4(realOverlay);

    ui->lbl_image_5->setScaledContents(true);
    ui->lbl_image_5->setPixmap(QPixmap::fromImage(realOverlay));

}
void MainWindow::on_pushButton_15_clicked()   //send img button
{

    QFile* img = new QFile("test.bmp");  // Create QFile from Image that needs to be sent
    connectToHost();  //connect to host will print debug statment if not conencted
    if(img->exists()==true)
    {
        qDebug("Image Found!!!");
        data.clear();     //clear byte array holding img data
        img->open(QIODevice::ReadOnly);   //open img to read data
        data = img->readAll();  //read raw data from img
        quint32 size = data.toBase64().size();  // raw data size of img in base64
        sendToTCPServer(data.toBase64(),size);  //send data in base64 to server
        img->close();   // close Img
    }
}

void MainWindow::sendToTCPServer(QByteArray data, quint32 sizeImage)
{
    if (socket1->state() == QAbstractSocket::ConnectedState)
    {
        socket1->write(data);
        qDebug("SUCCESS: Image Sent of Size: %d", sizeImage);
        qDebug("-----------------------------------");

    }
    else
    {
        qDebug("ERROR: Image Did Not Send");
        qDebug("-----------------------------------");

    }
}
void MainWindow::connectToHost()
{
    socket1->connectToHost(QHostAddress(getipFPGA()),getportNumber());
    if (socket1->waitForConnected())
    {
        qDebug("-----------------------------------");
        qDebug("CONNECTED TO SERVER");
    }
    else
    {
        qDebug("NOT CONNECTED");
    }
}

void MainWindow::on_pushButton_7_clicked()
{
     ui->label_17->setText(ui->lineEdit_3->text());
        setimageHeight(ui->lineEdit_3->text().toUInt());
}


void MainWindow::on_pushButton_16_clicked()
{

    ui->label_18->setText(ui->lineEdit_4->text());
     setimageWidth(ui->lineEdit_4->text().toUInt());


}

