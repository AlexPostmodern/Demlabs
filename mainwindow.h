#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serialnumberlineedit.h"
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:

};
#endif // MAINWINDOW_H
