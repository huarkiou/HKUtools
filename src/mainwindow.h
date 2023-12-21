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
    void pushbutton_caculate_01_clicked();
    void pushbutton_caculate_02_clicked();
    void pushbutton_caculate_03_clicked();
    void comboBox_pressure_03_change(int i);
    void comboBox_temperature_03_change(int i);

   private:
    Ui::MainWindowClass *ui;
    int precision;
};

#endif