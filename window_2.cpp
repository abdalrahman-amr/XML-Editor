#include "window_2.h"
#include "ui_window_2.h"
#include<QTextBlock>
#include <QMessageBox>
#include<iostream>
#include <stack>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "xml-tree.h"
#include"QFileDialog"
#include"Match_Pointer.hpp"

using namespace std;

QString my_text;
QString my_file_name;

window_2::window_2(QString text ,QString file_name ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::window_2)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(951, 541));

    //ui->textEdit->setTextColor(QColor( "red" ));
    ui->textEdit->setPlainText(text);
    my_text = text;
    my_file_name = file_name;
}



window_2::~window_2()
{
    delete ui;
}

vector <string> xml_vector(QTextDocument *doc) {
    vector<string> temp;
    QTextBlock tb;
    string line;
    int i = 0;
    for (;tb!=doc->end() ; i++) {
        tb = doc->findBlockByLineNumber(i);
        line = tb.text().toStdString();
        int a = 0;
        string line2 = "";
        while (line[a] == ' ')
        {
            a++;
        }
        for (int k = a; k < line.size(); k++) {
            line2 = line2 + line[k];
        }
        line = line2;
         temp.push_back(line);
    }

    return temp;

}

string formatting( vector <string> x) {


    string line;
    string formatted;
    int space_count = 0;
    for (int i = 0; i < x.size(); i++) {
        line = x[i];
        if (line[0] == '<') {
            if (line.find_first_of('<') != line.find_last_of('<')) {
                for (int j = 0; j < space_count; j++) {
                    formatted.append("      ");
                }
                formatted.append(line);
                formatted.append("\n");
            }
            else if (line[1] == '/') {
                space_count--;
                for (int j = 0; j < space_count; j++) {
                    formatted.append("      ");
                }
                formatted.append(line);
                formatted.append("\n");

            }
            else {
                for (int j = 0; j < space_count; j++) {
                    formatted.append("      ");
                }
                formatted.append(line);
                formatted.append("\n");
                space_count++;
            }
        }
        else {
            for (int j = 0; j < space_count; j++) {
                formatted.append("      ");
            }
            formatted.append(line);
            formatted.append("\n");
        }
    }
    return formatted;
}





void window_2::on_pushButton_clicked()
{

    QTextEdit edit;
    edit.setText(ui->textEdit->toPlainText());
    QTextDocument *doc = edit.document();

    vector<string> temp;
    string str;

    temp = xml_vector(doc);
    str= formatting(temp);
    QString qstr = QString::fromStdString(str);
    ui->textEdit_2->setPlainText(qstr);




}


void window_2::on_pushButton_2_clicked()
{


    XMLTree* treeNode = new XMLTree(my_text);
    QString out = treeNode->convertXMLFileIntoJSONFile();
    ui->textEdit_2->setPlainText(out);

}


void window_2::on_pushButton_3_clicked()
{
    QFile file(my_file_name);
        if (!file.open(QFile::ReadOnly|QFile::Text))
        {
            QMessageBox::warning(this,"..","can not open the file");
            return ;
        }
        QTextStream in(&file);
        QString text = in.readAll();
        QByteArray compressed=compression(text);
        QString fileName = QFileDialog::getSaveFileName(this,
                 tr("Save Address Book"), "",
                 tr("COMP (*.comp);;All Files ()"));
        QFile newDoc(fileName);
        if(newDoc.open(QIODevice::WriteOnly)){
            newDoc.write(compressed);
        }

        newDoc.close();
}


void window_2::on_pushButton_4_clicked()
{
    QTextEdit edit;
    edit.setText(ui->textEdit->toPlainText());
    QTextDocument *doc = edit.document();

    vector<string> temp;
    string str = "";

    temp = xml_vector(doc);
    for(int k = 0;k<temp.size();k++ ){
        str.append(temp[k]);
    }
    QString qstr = QString::fromStdString(str);
    ui->textEdit_2->setPlainText(qstr);
}


void window_2::on_pushButton_5_clicked()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Open File"), QString(),tr("Text Files (*.comp)")));
            char file_data;
            QByteArray arr;
            if(!file.open(QFile::ReadOnly))
            {
                QMessageBox::warning(this,"..","can not open the file");
                return;
            }

            while(!file.atEnd())
            {
              file.read(&file_data,sizeof(char));
              arr.push_back(file_data);
            }
            file.close();
        QString txt = decompression(arr);

        ui->textEdit_2->setPlainText(txt);
}


void window_2::on_pushButton_6_clicked()
{
    QString edit;
    edit = ui->textEdit_2->toPlainText();

    QString fileName = QFileDialog::getSaveFileName(this,
             tr("Output"), "",
             tr("XML (*.xml);;TXT(*.txt);;All Files ()"));
    QFile newDoc(fileName);
    if(newDoc.open(QIODevice::WriteOnly)){
        QTextStream out(&newDoc);
        out << edit;
    }

    newDoc.close();
}

