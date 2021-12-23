#include "readonly.h"
#include "ui_readonly.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QFile>
#include<QTextStream>
#include "xmlwindow.h"
#include"window_2.h"

readonly::readonly(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::readonly)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(600, 491));
}

readonly::~readonly()
{
    delete ui;
}


void readonly::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"open xml file","c://");
    //QMessageBox::information(this,"...",file_name);
    QFile file(file_name);

       if(!file.open(QFile::ReadOnly|QFile::Text)){

       QMessageBox::information(this,"...",file_name);
       }

        QTextStream in(&file);
        QString text = in.readAll();
        file.close();
        xmlwindow x(text) ;
        x.setModal(true);
        x.exec();
}


void readonly::on_pushButton_2_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"open xml file","c://");
    //QMessageBox::information(this,"...",file_name);
    QFile file(file_name);

       if(!file.open(QFile::ReadOnly|QFile::Text)){

       QMessageBox::information(this,"...",file_name);
       }

        QTextStream in(&file);
        QString text = in.readAll();
        file.close();
        window_2 x(text,file_name) ;
        x.setModal(true);
        x.exec();
}

