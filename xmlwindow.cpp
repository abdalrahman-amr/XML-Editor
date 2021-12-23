#include "xmlwindow.h"
#include "ui_xmlwindow.h"
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

QString my_text_2;
xmlwindow::xmlwindow(QString text,QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::xmlwindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(951, 480));

    my_text_2=text;

    //ui->textEdit->setTextColor(QColor( "red" ));
    ui->textEdit->setPlainText(text);
    ui->textEdit_2->setReadOnly(true);
}

xmlwindow::~xmlwindow()
{
    delete ui;
}

vector <int> tag_index;

bool matching(string a, string b) {
    return (a.insert(1, "/") == b);
}


vector <string> getTags(vector<string>temp)  {
    vector<string> tags;
    tag_index.clear();
    for (int i = 0; i < temp.size(); i++) {

        // if empty space or data then don't push
        if ((temp[i] == "")||(temp[i].find('<') == -1))continue;

        // if only one tag push
        else if (temp[i].find_first_of('<') == temp[i].find_last_of('<')) {
            tags.push_back(temp[i].substr(temp[i].find_first_of('<'), temp[i].find_first_of('>') + 1));
            tag_index.push_back(i);
        }
        // if two tags push two tags
        else if(temp[i].find_first_of('<') != temp[i].find_last_of('<')) {
            string s1 = temp[i].substr(temp[i].find_first_of('<'), temp[i].find_first_of('>') + 1);
            string s2 = temp[i].substr(temp[i].find_last_of('<'), temp[i].find_last_of('>'));
            tags.push_back(s1);
            tags.push_back(s2);
            tag_index.push_back(i);
            tag_index.push_back(i);
        }
}
    return tags;
}

vector<int>  detect_error(vector<string>  temp, vector<int> spaces) {
    stack<string> openTag;
    stack<string> closeTag;
    stack<int> open_errors;
    stack<int> close_errors;
    vector<int> total_errors;
    vector <string>  tags;
    tags = getTags(temp);
    for (int i = 0; i < tags.size(); i++) {
        // if open tag push into stack
        if (tags[i][1] != '/') {
            openTag.push(tags[i]);
            open_errors.push(tag_index[i]);
        }
        // if close tag push into another stack and compare
        else if (tags[i][1] == '/') {
            closeTag.push(tags[i]);
            close_errors.push(tag_index[i]);
            // if both matching pop them from stack
            if (openTag.size() && matching(openTag.top(), tags[i])) { openTag.pop(); closeTag.pop(); open_errors.pop(); close_errors.pop(); }
            // if not matching and still size of open tags > 0
            else if (openTag.size()) {
                    // check close with each tag less priority than him(or higher indentation than him)
                for (;((!(matching(openTag.top(), tags[i]))) && (openTag.size() > 0)&& (spaces[close_errors.top()] < spaces[open_errors.top()] ));) {
                    total_errors.push_back(open_errors.top());
                    openTag.pop();
                    open_errors.pop();
                }
                // if it matching pop them from stack
                if ((matching(openTag.top(), tags[i])&&(openTag.size() > 0))) { openTag.pop(); closeTag.pop(); open_errors.pop(); close_errors.pop(); }
                // if not matching then this closed tag cannot have an open tag
                else {
                    total_errors.push_back(close_errors.top());
                    closeTag.pop();
                    close_errors.pop();
                }
            }
        }
    }
// take all the remaining tags
    if (closeTag.size() > 0) {

    for (; closeTag.size() > 0;) {
        total_errors.push_back(close_errors.top());
        closeTag.pop();
        close_errors.pop();
    }
            }
    if (openTag.size() > 0) {

        for (; openTag.size() > 0;) {
            total_errors.push_back(open_errors.top());
            openTag.pop();
            open_errors.pop();
        }
    }
return total_errors;
}

void highlight(int begin , int end , QTextEdit *edit1){
    QTextCharFormat fmt;
    fmt.setBackground(Qt::red);
    QTextCursor cursor(edit1->document());
    cursor.setPosition(begin, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    cursor.setCharFormat(fmt);
}

void xmlwindow::on_pushButton_clicked()
{
    ui->textEdit->setTextBackgroundColor(QColor( "white" ));
    ui->textEdit->setPlainText(ui->textEdit->toPlainText());
    QTextEdit edit;

    edit.setText(ui->textEdit->toPlainText());
    QTextDocument *doc = edit.document();
    int i = 0;
    int sum = 0;
    vector<int> begin;
     vector<int> spaces;
     vector<string> temp;
     QTextBlock tb;
     string line;
     begin.push_back(0);
    for (;tb!=doc->end() ; i++) {
    tb = doc->findBlockByLineNumber(i);
    line = tb.text().toStdString();
    sum = sum + line.size()+1;
    begin.push_back(sum);
    spaces.push_back(line.find_first_of('<'));
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    temp.push_back(line);
}
    vector<int> total_errors = detect_error(temp, spaces);
 if(total_errors.size()==0){
 ui->label->setStyleSheet("QLabel {color : green; }");
 ui->label->setText("NO Errors found");
}
 else{
     ui->label->setStyleSheet("QLabel {color : red; }");
     ui->label->setText(QString::number(total_errors.size())+" Errors found");
 }


 for(int i = 0; i<total_errors.size();i++){

   highlight(begin[total_errors[i]], begin[total_errors[i]+ 1]-1 , ui->textEdit);

 }

}

void xmlwindow::on_pushButton_3_clicked()
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

