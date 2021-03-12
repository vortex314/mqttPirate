#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QWidget *widget = new QWidget();
    QGridLayout *layout = new QGridLayout();

    widget->setLayout(layout);
    setCentralWidget(widget);
 //   ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

