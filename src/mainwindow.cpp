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
    ui->lineEdit_mass_input_02->setValidator(new QDoubleValidator(this));
    ui->lineEdit_stiffness_input_02->setValidator(new QDoubleValidator(this));
    ui->lineEdit_damping_input_02->setValidator(new QDoubleValidator(this));
    ui->lineEdit_mass_displaced_02->setValidator(new QDoubleValidator(this));
    ui->lineEdit_mass_output_02->setValidator(new QDoubleValidator(this));

    connect(ui->comboBox_mass_input_02, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int i) {
        this->ui->label_mass_output_02->setText(qobject_cast<QComboBox*>(sender())->itemText(!i));
    });
    connect(ui->comboBox_damping_input_02, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int i) {
        this->ui->label_damping_output_02->setText(qobject_cast<QComboBox*>(sender())->itemText(!i));
    });
    connect(ui->comboBox_stiffness_input_02, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int i) {
        this->ui->label_stiffness_output_02->setText(qobject_cast<QComboBox*>(sender())->itemText(!i));
    });
    connect(ui->pushButton_calculate_02, &QPushButton::clicked, this, &MainWindow::pushbutton_caculate_02_clicked);

    // set up page03
    ui->lineEdit_gamma_03->setValidator(new QDoubleValidator(this));
    ui->lineEdit_Rg_03->setValidator(new QDoubleValidator(this));
    ui->lineEdit_Ma_03->setValidator(new QDoubleValidator(this));
    ui->lineEdit_T_input_03->setValidator(new QDoubleValidator(this));
    ui->lineEdit_p_input_03->setValidator(new QDoubleValidator(this));

    ui->label_p_output_03->setText(ui->comboBox_p_input_03->itemText(!ui->comboBox_p_input_03->currentIndex()));
    ui->label_T_output_03->setText(ui->comboBox_T_input_03->itemText(!ui->comboBox_T_input_03->currentIndex()));

    connect(ui->comboBox_gamma_03, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this](int i) { this->ui->label_Cp_03->setText(qobject_cast<QComboBox*>(sender())->itemText(!i)); });
    connect(ui->comboBox_p_input_03, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this](int i) { this->ui->label_p_output_03->setText(qobject_cast<QComboBox*>(sender())->itemText(!i)); });
    connect(ui->comboBox_T_input_03, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this](int i) { this->ui->label_T_output_03->setText(qobject_cast<QComboBox*>(sender())->itemText(!i)); });
    connect(ui->pushButton_calculate_03, &QPushButton::clicked, this, &MainWindow::pushbutton_caculate_03_clicked);

    // set up page04
    ui->lineEdit_viscosity0_04->setValidator(new QDoubleValidator(this));
    ui->lineEdit_T_04->setValidator(new QDoubleValidator(this));
    ui->lineEdit_T_susth_04->setValidator(new QDoubleValidator(this));
    ui->lineEdit_S_susth_04->setValidator(new QDoubleValidator(this));
    connect(ui->pushButton_calculate_04, &QPushButton::clicked, this, &MainWindow::pushbutton_caculate_04_clicked);
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
    double mass_input = ui->lineEdit_mass_input_02->text().toDouble();
    double mass_displaced = ui->lineEdit_mass_displaced_02->text().toDouble();
    double mass = 0., mass_ratio = 0., mass_output = 0.;
    if (ui->comboBox_mass_input_02->currentText().contains("*")) {
        mass_ratio = mass_input;
        mass = mass_ratio * mass_displaced;
        mass_output = mass;
    } else {
        mass = mass_input;
        mass_ratio = mass / mass_displaced;
        mass_output = mass_ratio;
    }

    double stiffness_input = ui->lineEdit_stiffness_input_02->text().toDouble();
    double stiffness = 0., circle_frequency = 0., frequency = 0., stiffness_output = 0.;
    if (ui->comboBox_stiffness_input_02->currentText().contains("k")) {
        stiffness = stiffness_input;
        circle_frequency = std::sqrt(stiffness / mass);
        frequency = circle_frequency / (2.0 * std::numbers::pi);
        stiffness_output = frequency;
    } else {
        frequency = stiffness_input;
        circle_frequency = frequency * 2.0 * std::numbers::pi;
        stiffness = std::pow(circle_frequency, 2.) * mass;
        stiffness_output = stiffness;
    }

    double damping_input = ui->lineEdit_damping_input_02->text().toDouble();
    double damping = 0., damping_ratio = 0., damping_output = 0.;
    if (ui->comboBox_damping_input_02->currentText().contains("c")) {
        damping = damping_input;
        damping_ratio = damping / (2 * std::sqrt(stiffness * mass));
        damping_output = damping_ratio;
    } else {
        damping_ratio = damping_input;
        damping = damping_ratio * (2 * std::sqrt(stiffness * mass));
        damping_output = damping;
    }

    double period = 1.0 / frequency;

    // output
    ui->lineEdit_mass_output_02->setText(QString::number(mass_output, 'g', precision));
    ui->lineEdit_damping_output_02->setText(QString::number(damping_output, 'g', precision));
    ui->lineEdit_stiffness_output_02->setText(QString::number(stiffness_output, 'g', precision));
    ui->lineEdit_period_02->setText(QString::number(period, 'g', precision));
    ui->lineEdit_circlefrequency_02->setText(QString::number(circle_frequency, 'g', precision));
}

void MainWindow::pushbutton_caculate_03_clicked() {
    double Rg = ui->lineEdit_Rg_03->text().toDouble();
    double Ma = ui->lineEdit_Ma_03->text().toDouble();

    double gamma = 0., Cp = 0.;
    if (ui->comboBox_gamma_03->currentText().contains("Cp")) {
        Cp = ui->lineEdit_gamma_03->text().toDouble();
        double tmp = Cp / Rg;
        gamma = tmp / (tmp - 1.);
        ui->lineEdit_Cp_03->setText(QString::number(gamma, 'g', precision));
    } else {
        gamma = ui->lineEdit_gamma_03->text().toDouble();
        Cp = gamma / (gamma - 1.) * Rg;
        ui->lineEdit_Cp_03->setText(QString::number(Cp, 'g', precision));
    }

    double T_input = ui->lineEdit_T_input_03->text().toDouble();
    double T_static = 0., T_total = 0., T_output = 0.;
    if (ui->comboBox_T_input_03->currentText().contains("total")) {
        T_total = T_input;
        T_static = T_total / (1 + (gamma - 1.) / 2. * Ma * Ma);
        T_output = T_static;
    } else {
        T_static = T_input;
        T_total = T_static * (1 + (gamma - 1.) / 2. * Ma * Ma);
        T_output = T_total;
    }
    ui->lineEdit_T_output_03->setText(QString::number(T_output, 'g', precision));

    double p_input = ui->lineEdit_p_input_03->text().toDouble();
    double p_static = 0., p_total = 0., p_output = 0.;
    if (ui->comboBox_p_input_03->currentText().contains("total")) {
        p_total = p_input;
        p_static = p_total / std::pow(1 + (gamma - 1.) / 2. * Ma * Ma, gamma / (gamma - 1.));
        p_output = p_static;
    } else {
        p_static = p_input;
        p_total = p_static * std::pow(1 + (gamma - 1.) / 2. * Ma * Ma, gamma / (gamma - 1.));
        p_output = p_total;
    }
    ui->lineEdit_p_output_03->setText(QString::number(p_output, 'g', precision));

    double soundspeed = std::sqrt(gamma * Rg * T_static);
    ui->lineEdit_soundspeed_03->setText(QString::number(soundspeed, 'g', precision));

    double velocity = Ma * soundspeed;
    ui->lineEdit_velocity_03->setText(QString::number(velocity, 'g', precision));
}

void MainWindow::pushbutton_caculate_04_clicked() {
    double mu0 = ui->lineEdit_viscosity0_04->text().toDouble();
    double Tsusth = ui->lineEdit_T_susth_04->text().toDouble();
    double Ssusth = ui->lineEdit_S_susth_04->text().toDouble();
    double T = ui->lineEdit_T_04->text().toDouble();

    double mu = mu0 * std::pow(T / Tsusth, 1.5) * (Tsusth + Ssusth) / (T + Ssusth);

    ui->lineEdit_viscosity_04->setText(QString ::number(mu, 'g', precision));
}
