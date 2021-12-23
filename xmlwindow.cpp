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






void xmlwindow::on_pushButton_2_clicked()
{

    QTextEdit edit;
    edit.setText(ui->textEdit->toPlainText());
    QTextDocument *doc = edit.document();
    int i = 0;
     vector<int> spaces;
     vector<string> temp;
     vector<string> original;
     QTextBlock tb;
     string line;
    for (;tb!=doc->end() ; i++) {
    tb = doc->findBlockByLineNumber(i);
    line = tb.text().toStdString();
    spaces.push_back(line.find_first_of('<'));
    original.push_back(line);

    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    temp.push_back(line);
}
    vector<int> total_errors = detect_error(temp, spaces);

    for(int i = 0;i<total_errors.size();i++){

        int num = total_errors[i] ;
        // if two tags in the same line then the problem is mismatching
        if(original[num].find_first_of('<') != original[num].find_last_of('<')){

           string s1 = original[num].substr(original[num].find_first_of('<'), original[num].find_first_of('>')-original[num].find_first_of('<')+1);
           string s2 = original[num].substr(temp[i].find_last_of('<'), original[num].find_last_of('>')-original[num].find_first_of('<')+1);
           string s  = original[num].replace(original[num].find_last_of('<'),s2.length(),s1.insert(1, "/"));
            original[num] = s;
        }
        // if only one tag
        else if (original[num].find_first_of('<') == original[num].find_last_of('<')) {
            string s1 = original[num].substr(original[num].find_first_of('<'), original[num].find_first_of('>')-original[num].find_first_of('<')+1);
            string s = original[num];
            s.erase(std::remove(s.begin(), s.end(), ' '), s.end());

           // if open
            if (s1[1]!='/'){
               // if open without close in the same line
                if((s.find_first_of('>') != s.size() - 1) || ((s.find_first_of('<') != 0))){
                    original[num].append( s1.insert(1,"/")) ;
                }

                // if open only without close not in the same line
                else if(!((s.find_first_of('>') != s.size() - 1) || ((s.find_first_of('<') != 0)))){
                    int k = num+1;
                    for(;i<original.size();k++){
                        if ((spaces[k]<=spaces[num])&&(spaces[k]!=-1)){

                            auto itPos1 = original.begin()+k;
                            auto itPos2 = spaces.begin()+k;
                            string w="";
                            original.insert(itPos1,w.append(spaces[num],' ')+ s1.insert(1,"/"));
                            spaces.insert(itPos2,spaces[num]);
                            for(int i = 0;i<total_errors[i];i++){
                                if(total_errors[i]>=k)total_errors[i]+=1;
                            }
                            break;
                        }

                        else if(k == original.size()-1){
                            auto itPos1 = original.begin()+k+1;
                            auto itPos2 = spaces.begin()+k+1;
                            string w="";
                            original.insert(itPos1,w.append(spaces[num],' ')+ s1.insert(1,"/"));
                            spaces.insert(itPos2,spaces[num]);
                            break;
                        }

                    }
                }

            }
            // if close
            else if(s1[1]=='/'){
                // if close without open in the same line
                if((s.find_first_of('>') != s.size() - 1) || ((s.find_first_of('<') != 0))){
                    original[num] = original[num].insert(original[num].find_first_not_of(" "),s1.erase(1,1));
                }

                // if close only without open not in the same line
                else if(!((s.find_first_of('>') != s.size() - 1) || ((s.find_first_of('<') != 0)))){
                    int k=num-1;
                    for(;k>=0;k--){
                        if ((spaces[k]<=spaces[num])&&(spaces[k]!=-1)){

                            auto itPos1 = original.begin()+k+1;
                            auto itPos2 = spaces.begin()+k+1;
                            string w="";
                            original.insert(itPos1,w.append(spaces[num],' ') + s1.erase(1,1));
                            spaces.insert(itPos2,spaces[num]);
                            for(int i = 0;i<total_errors[i];i++){
                                if(total_errors[i]>=k+1)total_errors[i]+=1;
                            }
                            break;
                        }

                        else if(k == 0){
                            auto itPos1 = original.begin()+k;
                            auto itPos2 = spaces.begin()+k;
                            string w="";
                            original.insert(itPos1,w.append(spaces[num],' ') + s1.erase(1,1));
                            spaces.insert(itPos2,spaces[num]);
                            for(int i = 0;i<total_errors[i];i++)total_errors[i]+=1;
                            break;
                        }

                    }

                }

            }

        }
    }
ui->textEdit_2->clear();
string app;
for(int i = 0;i< original.size();i++){
//ui->textEdit_2->append(QString::fromStdString(original[i]));//append(QString::fromStdString(original[i]));
if (original[i].find_first_not_of(' ')!= string::npos){
app.append(original[i]+"\n");
}
}
ui->textEdit_2->setPlainText(QString::fromStdString(app));
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

