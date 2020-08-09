#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout();
    centralWidget->setLayout(layout);

    SerialNumberLineEdit* lineEdit=new SerialNumberLineEdit;

    layout->addWidget(new QPushButton("Take Focus"),1,0);
    layout->addWidget(lineEdit,0,0);

    setCentralWidget(centralWidget);
    setStyleSheet("#obj_MainWindow{background: #fff;}");
}

MainWindow::~MainWindow()
{
}

