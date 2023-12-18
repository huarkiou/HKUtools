#include "MainWindow.h"

#include <QDoubleValidator>
#include <cmath>
#include <numbers>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui = new Ui::MainWindowClass();
    ui->setupUi(this);
    initializeUI();
}

MainWindow::~MainWindow() {}

void MainWindow::initializeUI() {
    // set up menubar
    connect(ui->action_01, SIGNAL(triggered(bool)), this, SLOT(changePage()));
    connect(ui->action_02, SIGNAL(triggered(bool)), this, SLOT(changePage()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutQt()));

    // set up page01
    ui->lineEdit_velocity_01->setText("1.0");
    ui->lineEdit_velocity_01->setValidator(new QDoubleValidator(this));
    ui->lineEdit_density_01->setText("1.205");
    ui->lineEdit_density_01->setValidator(new QDoubleValidator(this));
    ui->lineEdit_viscosity_01->setText("1.82e-5");
    ui->lineEdit_viscosity_01->setValidator(new QDoubleValidator(this));
    ui->lineEdit_length_01->setText("1.0");
    ui->lineEdit_length_01->setValidator(new QDoubleValidator(this));
    ui->lineEdit_yplus_01->setText("1.0");
    ui->lineEdit_yplus_01->setValidator(new QDoubleValidator(this));

    connect(ui->pushButton_calculate_01, SIGNAL(clicked()), this, SLOT(pushbutton_caculate_01_clicked()));

    // set up page02
    ui->lineEdit_mass_02->setText("1.0");
    ui->lineEdit_mass_02->setValidator(new QDoubleValidator(this));
    ui->lineEdit_stiffness_02->setText("100");
    ui->lineEdit_stiffness_02->setValidator(new QDoubleValidator(this));
    ui->lineEdit_damping_02->setText("0.1");
    ui->lineEdit_damping_02->setValidator(new QDoubleValidator(this));

    connect(ui->pushButton_calculate_02, SIGNAL(clicked()), this, SLOT(pushbutton_caculate_02_clicked()));
}

void MainWindow::changePage() {
    QObject* sd = sender();
    if (sd == ui->action_01) {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (sd == ui->action_02) {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::aboutQt() { qApp->aboutQt(); }

void MainWindow::pushbutton_caculate_01_clicked() {
    // input
    double inlet_velocity = ui->lineEdit_velocity_01->text().toDouble();
    double medium_density = ui->lineEdit_density_01->text().toDouble();
    double medium_viciosity = ui->lineEdit_viscosity_01->text().toDouble();
    double character_length = ui->lineEdit_length_01->text().toDouble();
    double yplus = ui->lineEdit_yplus_01->text().toDouble();
    double y1 = -1;

    // calculate
    double Re = medium_density * inlet_velocity * character_length / medium_viciosity;
    double Cf = 0.058 * std::pow(Re, -0.2);
    double tw = 0.5 * Cf * medium_density * std::pow(inlet_velocity, 2);
    double Ur = sqrt(tw / medium_density);
    y1 = yplus * medium_viciosity / Ur / medium_density;

    // output
    ui->lineEdit_Re_01->setText(QString::number(Re, 'g', 6));
    ui->lineEdit_y1_01->setText(QString::number(y1, 'g', 6));
}

void MainWindow::pushbutton_caculate_02_clicked() {
    // input
    double mass = ui->lineEdit_mass_02->text().toDouble();
    double stiffness = ui->lineEdit_stiffness_02->text().toDouble();
    double damping = ui->lineEdit_damping_02->text().toDouble();

    // calculate
    double damping_ratio = damping / (2 * std::sqrt(stiffness * mass));
    double circle_frequency = std::sqrt(stiffness / mass);
    double natural_frequency = circle_frequency / 2.0 / std::numbers::pi;
    double period = 1.0 / natural_frequency;

    // output
    ui->lineEdit_naturalfrequency_02->setText(QString::number(natural_frequency, 'g', 6));
    ui->lineEdit_period_02->setText(QString::number(period, 'g', 6));
    ui->lineEdit_circlefrequency_02->setText(QString::number(circle_frequency, 'g', 6));
    ui->lineEdit_dampingratio_02->setText(QString::number(damping_ratio, 'g', 6));
}
