#include "MainWindow.h"

#include <QDebug>
#include <QDoubleValidator>
#include <cmath>
#include <numbers>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui = new Ui::MainWindowClass();
    ui->setupUi(this);
    initializeUI();
    precision = 9;
}

MainWindow::~MainWindow() {}

void MainWindow::initializeUI() {
    // set up page01
    ui->lineEdit_velocity_01->setValidator(new QDoubleValidator(this));
    ui->lineEdit_density_01->setValidator(new QDoubleValidator(this));
    ui->lineEdit_viscosity_01->setValidator(new QDoubleValidator(this));
    ui->lineEdit_length_01->setValidator(new QDoubleValidator(this));
    ui->lineEdit_yplus_01->setValidator(new QDoubleValidator(this));

    connect(ui->pushButton_calculate_01, &QPushButton::clicked, this, &MainWindow::pushbutton_caculate_01_clicked);

    // set up page02
    ui->lineEdit_mass_02->setValidator(new QDoubleValidator(this));
    ui->lineEdit_stiffness_02->setValidator(new QDoubleValidator(this));
    ui->lineEdit_damping_02->setValidator(new QDoubleValidator(this));

    connect(ui->pushButton_calculate_02, &QPushButton::clicked, this, &MainWindow::pushbutton_caculate_02_clicked);

    // set up page03
    ui->lineEdit_gamma_03->setValidator(new QDoubleValidator(this));
    ui->lineEdit_Rg_03->setValidator(new QDoubleValidator(this));
    ui->lineEdit_Ma_03->setValidator(new QDoubleValidator(this));
    ui->lineEdit_T_total_03->setValidator(new QDoubleValidator(this));
    ui->lineEdit_p_total_03->setValidator(new QDoubleValidator(this));

    ui->label_p_static_03->setText(ui->comboBox_pressure_03->itemText(!ui->comboBox_pressure_03->currentIndex()));
    ui->label_T_static_03->setText(ui->comboBox_temperature_03->itemText(!ui->comboBox_temperature_03->currentIndex()));

    connect(ui->comboBox_pressure_03, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::comboBox_pressure_03_change);
    connect(ui->comboBox_temperature_03, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::comboBox_temperature_03_change);
    connect(ui->pushButton_calculate_03, &QPushButton::clicked, this, &MainWindow::pushbutton_caculate_03_clicked);
}

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
    ui->lineEdit_Re_01->setText(QString::number(Re, 'g', precision));
    ui->lineEdit_y1_01->setText(QString::number(y1, 'g', precision));
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
    ui->lineEdit_naturalfrequency_02->setText(QString::number(natural_frequency, 'g', precision));
    ui->lineEdit_period_02->setText(QString::number(period, 'g', precision));
    ui->lineEdit_circlefrequency_02->setText(QString::number(circle_frequency, 'g', precision));
    ui->lineEdit_dampingratio_02->setText(QString::number(damping_ratio, 'g', precision));
}

void MainWindow::comboBox_pressure_03_change(int i) {
    this->ui->label_p_static_03->setText(this->ui->comboBox_pressure_03->itemText(!i));
}

void MainWindow::comboBox_temperature_03_change(int i) {
    this->ui->label_T_static_03->setText(this->ui->comboBox_temperature_03->itemText(!i));
}

void MainWindow::pushbutton_caculate_03_clicked() {
    // input
    double gamma = ui->lineEdit_gamma_03->text().toDouble();
    double Rg = ui->lineEdit_Rg_03->text().toDouble();
    double Ma = ui->lineEdit_Ma_03->text().toDouble();

    double T_input = ui->lineEdit_T_total_03->text().toDouble();
    double T_static = 0., T_total = 0., T_output = 0.;
    if (ui->comboBox_temperature_03->currentText().contains("total")) {
        T_total = T_input;
        T_static = T_total / (1 + (gamma - 1.) / 2. * Ma * Ma);
        T_output = T_static;
    } else {
        T_static = T_input;
        T_total = T_static * (1 + (gamma - 1.) / 2. * Ma * Ma);
        T_output = T_total;
    }
    ui->lineEdit_T_static_03->setText(QString::number(T_output, 'g', precision));

    double p_input = ui->lineEdit_p_total_03->text().toDouble();
    double p_static = 0., p_total = 0., p_output = 0.;
    if (ui->comboBox_pressure_03->currentText().contains("total")) {
        p_total = p_input;
        p_static = p_total / std::pow(1 + (gamma - 1.) / 2. * Ma * Ma, gamma / (gamma - 1.));
        p_output = p_static;
    } else {
        p_static = p_input;
        p_total = p_static * std::pow(1 + (gamma - 1.) / 2. * Ma * Ma, gamma / (gamma - 1.));
        p_output = p_total;
    }
    ui->lineEdit_p_static_03->setText(QString::number(p_output, 'g', precision));

    double soundspeed = std::sqrt(gamma * Rg * T_static);
    ui->lineEdit_soundspeed_03->setText(QString::number(soundspeed, 'g', precision));

    double velocity = Ma * soundspeed;
    ui->lineEdit_velocity_03->setText(QString::number(velocity, 'g', precision));
}
