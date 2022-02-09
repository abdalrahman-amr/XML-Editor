#include "graph.h"
#include "window_2.h"
#include "ui_graph.h"
#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <QString>

using namespace std;
graph::graph(QString text ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graph)
{
    ui->setupUi(this);
    ui->textEdit->setPlainText(text);

}

graph::~graph()
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
vector <string> collecting(vector<string> temp) {

    vector<string> temp2;
    string after;


    for (auto x : temp) {

        if (x[0] == '<') {
            for (int i = 0; i < x.length(); i++) {

                if (x[i] == '<') {
                    after = x.replace(i, 1, "");
                    continue;
                }


                if (x[i] == '>') {
                    after = x.replace(i, 1, "");
                }
                if (x[i] == '/') {
                    after = x.replace(i, 1, "");
                    continue;
                }

            }
            temp2.push_back(after);
        }
        else {
            temp2.push_back(x);
        }
    }
    return temp2;

}

void graph::on_pushButton_clicked()
{
    ui->customPlot->clearItems();
    //ui->customPlot->clearPlottables();
    QTextEdit edit;
       edit.setText(ui->textEdit->toPlainText());
       QTextDocument *doc = edit.document();
       vector <string> s;

       vector<string> temp3;
       temp3 = xml_vector(doc);
       s = collecting(temp3);
       string id[50];
       string names[20];
       string fols[500];
       int i = 0;
       int j = 0;
       int z = 0;
       int fnum = 0;
       string temp;
       string temp2;

       while (i < s.size()) {
           if (s[i] == "user") {
               temp2 = s[i + 1];
               id[j] = temp2.substr(2, temp2.length() - 5);
               temp = s[i + 2];
               names[j] = temp.substr(4, temp.length() - 9);
               j++;
               while (s[i] != "/user"){

                   if (s[i] == "follower") {
                       fols[z] = s[i + 1][2];
                       fnum++;
                       z++;
                   }
                   i++;

               }
               fols[z] = '/';
               z++;

           }
           else {
               i++;
           }
       }
       int adjmat[50][50] = {0};
       int x = 0;
       int ones = 0;
       for (int a = 0; a < z + 1; a++)
       {
           for (int b = 0; b < j; b++)
           {
               if (fols[a] == "/") {
                   x++;
                   break;
               }
               if (fols[a] == id[b]) {
                   adjmat[b][x] = 1;
                   ones++;
               }

           }
       }

   int size = j;
   QCPItemText* textLabels[size];
   ui->customPlot->xAxis->setRange(0, floor(sqrt(size))+2);
   ui->customPlot->yAxis->setRange(0, ceil(size/floor(sqrt(size)))+2);
   // replot graph
    //ui->customPlot->replot();
    //ui->customPlot->update();

   for (int i = 0,j = 0 , k = 0;k<size;k++){
   textLabels[k] = new QCPItemText(ui->customPlot);
   textLabels[k]->position->setCoords(i+1, j+1); // place position at center/top of axis rect
   textLabels[k]->setText(QString::fromStdString(id[k]));
   textLabels[k]->setFont(QFont(font().family(), 16)); // make font a bit larger
   textLabels[k]->setPen(QPen(Qt::black)); // show black border around text
   if(i!=floor(sqrt(size))-1)
   {
   i++;
   }
   else
   {
    i = 0;
    j++;
   }

}

   // add the arrow:
   int k = 0;
   QCPItemLine *arrows[ones];
   for(int i = 0;i<size;i++){
       for(int j = 0 ;j<size;j++){
           if(adjmat[i][j]==1){
   arrows[k] = new QCPItemLine(ui->customPlot);
   arrows[k]->start->setParentAnchor(textLabels[j]->bottom);
   arrows[k]->end->setParentAnchor(textLabels[i]->bottom);
   arrows[k]->setHead(QCPLineEnding::esSpikeArrow);
   k++;
           }

       }
   }
ui->customPlot->replot();
}

