#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>
#include <vector>
#include <string>
#include "string.h"
namespace Ui {
class graph;
}

class graph : public QDialog
{
    Q_OBJECT

public:
    explicit graph( QString text , QWidget *parent = nullptr);
    ~graph();

public slots:
private slots:
    void on_pushButton_clicked();

private:
    Ui::graph *ui;
};

#endif // GRAPH_H
