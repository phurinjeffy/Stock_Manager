#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>
#include "mydb.h"

namespace Ui {
class Graph;
}

class Graph : public QDialog
{
    Q_OBJECT

public:
    explicit Graph(QWidget *parent = nullptr);
    ~Graph();
    void generateGraph();
    void onDateSelectionChanged(const QDate& date);
    void on_btnGraph_clicked();

private:
    Ui::Graph *ui;
};

#endif // GRAPH_H
