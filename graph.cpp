#include "graph.h"
#include "ui_graph.h"

#include <QtCore/qdatetime.h>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

Graph::Graph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Graph)
{
    ui->setupUi(this);

    // Set the display format to show only the month
    ui->dateEditMonth->setDisplayFormat("MMMM yyyy");

    // Connect the date selection signal to the slot
    connect(ui->dateEditMonth, &QDateEdit::dateChanged, this, &Graph::onDateSelectionChanged);
}

Graph::~Graph()
{
    delete ui;
}

void Graph::on_btnGraph_clicked()
{
    // Retrieve data based on the selected month
    QDate selectedDate = ui->dateEditMonth->date();
    int selectedMonth = selectedDate.month();
    int selectedYear = selectedDate.year();

    // Modify the SQL query to filter data for the selected month
    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare("SELECT "
                  "SUM(CASE WHEN SUBSTR(DOS, 6, 2) = :selectedMonth THEN SellingPrice ELSE 0 END), "
                  "SUM(CASE WHEN SUBSTR(DOP, 6, 2) = :selectedMonth THEN PurchasePrice ELSE 0 END) "
                  "FROM stocks WHERE SUBSTR(DOS, 1, 4) = :selectedYear OR SUBSTR(DOP, 1, 4) = :selectedYear");
    query.bindValue(":selectedMonth", QString("%1").arg(selectedMonth, 2, 10, QChar('0')));
    query.bindValue(":selectedYear", QString::number(selectedYear));

    if (!query.exec())
    {
        qDebug() << "Query error:" << query.lastError().text();
        return;
    }

    if (query.next())
    {
        double sellingPrice = query.value(0).toDouble();
        double purchasePrice = query.value(1).toDouble();

        // Create a bar series and add data
        QBarSeries *series = new QBarSeries();

        QBarSet *sellingBarSet = new QBarSet("Selling Price");
        sellingBarSet->append(sellingPrice);
        sellingBarSet->setColor(Qt::green);  // Set color for selling price bar

        QBarSet *purchaseBarSet = new QBarSet("Purchase Price");
        purchaseBarSet->append(purchasePrice);
        purchaseBarSet->setColor(Qt::red);   // Set color for purchase price bar

        series->append(sellingBarSet);
        series->append(purchaseBarSet);

        // Create a chart and set the series
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Column Comparison");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        // Create value axis and add it to the chart
        QValueAxis *axisY = new QValueAxis();
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Create category axis and add it to the chart
        QCategoryAxis *axisX = new QCategoryAxis();
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        // Set category labels
        axisX->append("Selling Price", -0.25);
        axisX->append("Purchase Price", 0.5);

        // Create chart view and set the chart
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        // Set the chart view as the child of chartLayout widget
        ui->chartLayout->addWidget(chartView);
    }
    else
    {
        qDebug() << "No results found.";
    }
}

void Graph::onDateSelectionChanged(const QDate& date)
{
    // Clear the chart layout when the date selection changes
    QLayoutItem *child;
    while ((child = ui->chartLayout->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

    // Generate the graph for the selected date
    on_btnGraph_clicked();
}
