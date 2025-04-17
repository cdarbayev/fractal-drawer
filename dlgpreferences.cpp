#include "dlgpreferences.h"
#include "ui_dlgpreferences.h"

DlgPreferences::DlgPreferences(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgPreferences)
{
    ui->setupUi(this);
    QValidator *validator = new QDoubleValidator();
    ui->editCIm->setValidator(validator);
}

DlgPreferences::~DlgPreferences()
{
    delete ui;
}


void DlgPreferences::setX1(Real val){
    ui->editX1->setText(QString("%1").arg(val));
}


void DlgPreferences::setX2(Real val){
    ui->editX2->setText(QString("%1").arg(val));
}


void DlgPreferences::setY1(Real val){
    ui->editY1->setText(QString("%1").arg(val));
}


void DlgPreferences::setY2(Real val){
    ui->editY2->setText(QString("%1").arg(val));
}

Real DlgPreferences::getX1() const{
    return ui->editX1->text().toDouble();
}

Real DlgPreferences::getX2() const{
    return ui->editX2->text().toDouble();
}

Real DlgPreferences::getY1() const{
    return ui->editY1->text().toDouble();
}

Real DlgPreferences::getY2() const{
    return ui->editY2->text().toDouble();
}


void DlgPreferences::setIterations(unsigned int val){
    ui->sbIters->setValue(val);
}

unsigned int DlgPreferences::getIterations() const{
    return ui->sbIters->value();
}

void DlgPreferences::setCRe(Real val){
    ui->editCRe->setText(QString("%1").arg(val));
}

void DlgPreferences::setCIm(Real val){
    ui->editCIm->setText(QString("%1").arg(val));
}


Real DlgPreferences::getCRe() const{
    bool ok = true;
    Real res = ui->editCRe->text().toDouble(&ok);
    if(!ok){
        //QMessageBox
    }
    return res;
}

Real DlgPreferences::getCIm() const{
    return ui->editCIm->text().toDouble();
}


void DlgPreferences::setType(FractalType type){
    if(type == MANDELBROT){
        ui->rbMandelbrot->setChecked(true);
    } else{
        ui->rbJulia->setChecked(true);
    }
}

FractalType DlgPreferences::getType() const{
    if(ui->rbMandelbrot->isChecked()){
        return MANDELBROT;
    } else{
        return JULIA;
    }
}

void DlgPreferences::setExponent(unsigned int val){
    ui->sbExponent->setValue(val);
}

unsigned int DlgPreferences::getExponent() const{
    return ui->sbExponent->value();
}
