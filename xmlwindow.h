#ifndef XMLWINDOW_H
#define XMLWINDOW_H

#include <QDialog>

namespace Ui {
class xmlwindow;
}

class xmlwindow : public QDialog
{
    Q_OBJECT

public:
    explicit xmlwindow(QString text,QWidget *parent = nullptr);
    ~xmlwindow();

private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::xmlwindow *ui;
};

#endif // XMLWINDOW_H
