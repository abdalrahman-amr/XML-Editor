#ifndef WINDOW_2_H
#define WINDOW_2_H

#include <QDialog>

namespace Ui {
class window_2;
}

class window_2 : public QDialog
{
    Q_OBJECT

public:
    explicit window_2(QString text,QString file_name , QWidget *parent = nullptr);
    ~window_2();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::window_2 *ui;
};

#endif // WINDOW_2_H
