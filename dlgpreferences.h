#ifndef DLGPREFERENCES_H
#define DLGPREFERENCES_H

#include <QDialog>

#include "datatype.h"

namespace Ui {
class DlgPreferences;
}

class DlgPreferences : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPreferences(QWidget *parent = nullptr);
    ~DlgPreferences();
    void setX1(Real val);
    Real getX1() const;
    void setX2(Real val);
    Real getX2() const;
    void setY1(Real val);
    Real getY1() const;
    void setY2(Real val);
    Real getY2() const;
    void setIterations(unsigned int val);
    unsigned int getIterations() const;
    Real getCRe() const;
    void setCRe(Real val);
    Real getCIm() const;
    void setCIm(Real val);
    void setType(FractalType val);
    FractalType getType() const;
    void setExponent(unsigned int val);
    unsigned int getExponent() const;
private:
    Ui::DlgPreferences *ui;
};

#endif // DLGPREFERENCES_H
