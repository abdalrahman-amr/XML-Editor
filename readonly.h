#ifndef READONLY_H
#define READONLY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class readonly; }
QT_END_NAMESPACE

class readonly : public QMainWindow
{
    Q_OBJECT

public:
    readonly(QWidget *parent = nullptr);
    ~readonly();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::readonly *ui;
};
#endif // READONLY_H
