#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    QSqlDatabase p_db;
    p_db = QSqlDatabase::addDatabase("QSQLITE");
    p_db.setDatabaseName("./northwind.db");
    if (p_db.open()) {
        QSqlQueryModel* p_model = new QSqlQueryModel;
        ui->tableView->verticalHeader()->setDefaultAlignment(Qt::AlignRight);
        p_model->setQuery("select * from 'Order Details Extended'");
        // p_model->select();
        ui->tableView->setModel(p_model);
        ui->tableView->setColumnWidth(2, 250);
        ui->tableView->setColumnWidth(7, 150);
        ui->tableView->setColumnWidth(4, 120);
        ui->tableView->setColumnWidth(5, 0);
        ui->tableView->setColumnWidth(6, 150);
        ui->tableView->setColumnWidth(8, 150);
    }

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(30);
    ui->tableWidget->setColumnCount(10);
    for (auto r = 0; r < 30; r++)
        for (auto c = 0; c < 10; c++)
            ui->tableWidget->setItem(r, c, new QTableWidgetItem(QString("Item %1,%2").arg(r).arg(c)));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_PDF_clicked()
{
    QTableView* tbl;
    if (ui->tabWidget->currentIndex() == 0)
        tbl = ui->tableView;
    else
        tbl = ui->tableWidget;

    QEcsReport ecsRep(tbl); // QEcsReport ecsRep(ui->tableView);
    ecsRep.Reset();
    ecsRep.setLogo(":/res/ecs.png", Qt::AlignLeft, 70, 70); // optional  >>  Default = Null
    ecsRep.setHeaderCBackColor(0xFFD9D2); // optional  >>  Default = 0xEDEDED >> Grey
    ecsRep.setPageSize(QPageSize::PageSizeId::A5); // optional  >>  Default = A4
    ecsRep.setOrientation(QPageLayout::Landscape); // optional  >>  Default = Portrait
    ecsRep.setPageNumberFormat("%p/%P"); // optional  >>  Default = Null
    ecsRep.setPageNumberFont(QFont("Arial", 9)); // optional  >>  Default "Arial",8
    ecsRep.setGrudFont(QFont("tahoma", 10)); // optional  >>  Default "Arial",10
    ecsRep.addText(pageHeader, "General Report", QFont("Tahoma", 18), Qt::AlignCenter);
    ecsRep.addText(pageHeader, "Date: 10/03/2013", QFont("Arial", 14), Qt::AlignCenter);
    ecsRep.addText(pageHeader, "Products Test", QFont("Arial", 10), Qt::AlignLeft);
    ecsRep.addText(reportFooter, "Total 500.00", QFont("Tahoma", 14), Qt::AlignCenter);
    ecsRep.addText(pageFooter, "Test Footer", QFont("Arial", 14), Qt::AlignLeft);
    ecsRep.addText(reportHeader, "Estishari Benghazi Libya Co.", QFont("Arial", 18), Qt::AlignCenter);
    ecsRep.addText(reportFooter, "Sign", QFont("Arial", 12), Qt::AlignRight);
    ecsRep.setLayoutCol(3, Qt::AlignCenter);
    ecsRep.ExportPDF("Report.pdf");
}

void MainWindow::on_pushButton_Previw_clicked()
{
    QTableView* tbl;
    if (ui->tabWidget->currentIndex() == 0)
        tbl = ui->tableView;
    else
        tbl = ui->tableWidget;

    QEcsReport ecsRep(tbl); // QEcsReport ecsRep(ui->tableView);
    ecsRep.Reset();
    ecsRep.setLogo(":/res/ECS_Logo_Word.png", Qt::AlignRight, 60, 130); // // optional  >>  Default = Null
    ecsRep.setHeaderCBackColor(0xD4FFDC); // optional  >>  Default = 0xEDEDED >> Grey
    ecsRep.setPageSize(QPageSize::A4); // optional  >>  Default = A4
    ecsRep.setOrientation(QPageLayout::Portrait); // optional  >>  Default = Portrait
    ecsRep.setPageNumberFormat("Page %p from %P ", Qt::AlignRight); // optional  >>  Default = Null
    ecsRep.setPageNumberFont(QFont("Arial", 9)); // optional  >>  Default "Arial",8
    ecsRep.setGrudFont(QFont("tahoma", 10)); // optional  >>  Default "Arial",10
    ecsRep.addText(pageHeader, "General Report", QFont("Tahoma", 18), Qt::AlignCenter);
    ecsRep.addText(pageHeader, "Date: 10/03/2013", QFont("Arial", 14), Qt::AlignCenter);
    ecsRep.addText(pageHeader, "Products Test", QFont("Arial", 10), Qt::AlignLeft);
    ecsRep.addText(reportFooter, "Total 500.00", QFont("Tahoma", 14), Qt::AlignCenter);
    ecsRep.addText(pageFooter, "Test Footer", QFont("Arial", 14), Qt::AlignLeft);
    ecsRep.addText(reportHeader, "Estishari Benghazi Libya Co.", QFont("Arial", 18), Qt::AlignCenter);
    ecsRep.addText(reportFooter, "Sign", QFont("Arial", 12), Qt::AlignRight);
    ecsRep.setLayoutCol(3, Qt::AlignCenter);
    ecsRep.Preview();
}
