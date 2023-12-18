#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QtWidgets/QMainWindow>

#include "ui_MainWindow.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private:
    void initializeUI();

   private slots:
    void changePage();
    void aboutQt();
    void pushbutton_caculate_01_clicked();
    void pushbutton_caculate_02_clicked();

   private:
    Ui::MainWindowClass *ui;
};

#endif