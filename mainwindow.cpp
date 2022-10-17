#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    QImage fImage = QImage (org.size(), QImage::Format_RGB16); //create a QImage final  Instance based on size of orginal image and format to RBG16
    QPainter overlay(&fImage);  // CREATE QPainter instacne based final image. passed by referecne to ensure changes are applied
    QRect loc1(0,.0,400,300);   // create QrRect instance that will be used later for overlay
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
void MainWindow::on_pushButton_7_clicked()
{

    /** follows same procedure as previous buttons to load an image */
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.bmp)"));
    setFile3(filename);
    if (QString::compare(filename, QString()) != 0)
    {

        QImage image;
        bool valid = image.load(filename);
        if (valid)
        {
         ui->lbl_image_5->setPixmap(QPixmap::fromImage(image));
        setimg3(image);
        setimg4(image);
        }

    }
}


QImage MainWindow::adjBright(QImage &image,int brightness)
{

          /* Two Parameters, the image being adjusted and the factor of adjustemnt **/

            QImage origin = image;  /// make a copy of orginal image
            QColor oldColor; // create a QColor instance, holds RBG value
            int delta = brightness;   // copy brightness factor

           QImage * newImage = new QImage(image);// new image QImage Pointer, based on size of orginal image format chanegd to RBG32 so RGB values can be cha

            for(int y=0; y<newImage->height(); ++y)
            {
                // qDebug("Factir pixel : %d", y);
                for(int x=0; x<newImage->width(); ++x)
                {

                    //qDebug("Factir pixel : %d", x);
                    oldColor = origin.pixelColor(y,x);

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
    QImage origin = image;  /// make a copy of orginal image
    QColor oldColor; // create a QColor instance, holds RBG value
    int minintensity=255;
    int maxintensity =0;

   QImage  newImage = origin;// new image QImage Pointer, based on size of orginal image format chanegd to RBG32 so RGB values can be cha
    qDebug("Factir pixel : %d %d", newImage.height(), newImage.width());

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

    QImage copy= getimg3();
    QImage orgImg= adjBright(copy,position);
    setimg4(orgImg);
    ui->lbl_image_5->setPixmap(QPixmap::fromImage(orgImg));

}


void MainWindow::on_bn_Slider_2_sliderReleased()
{
    int value = ui->bn_Slider_2->value();
    qDebug("Factir pixel : %d", value);
    QImage copyBright = getimg3();
    QImage contImg= adjContrast(copyBright,value); // call fun
    ui->lbl_image_5->setPixmap(QPixmap::fromImage(contImg));
    contImg.save("Contrast.bmp"); // save the image
}

