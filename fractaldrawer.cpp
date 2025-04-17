#include "fractaldrawer.h"
#include <QLabel>
#include <QScrollArea>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QDebug>
#include <QEventLoop>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>

#include "fractals.h"
#include "dlgpreferences.h"

FractalDrawer::FractalDrawer(QWidget *parent)
    : QMainWindow(parent), x1(-2), x2(2), y1(-2), y2(2),
    w(1680), h(1050), exponent(2), iters(64), type(MANDELBROT), cRe(-0.75), cIm()

{
    setupForm();
    setupActions();
    loadSettings();
}


void FractalDrawer::closeEvent(QCloseEvent *evt){
    saveSettings();
    if(!actDraw->isEnabled() && QMessageBox::question(this, tr("Really quit?"), tr("Do you really wish to quit and interrupt drawing?"))
        == QMessageBox::StandardButton::No){
        evt->ignore();
        return;
    }
    evt->accept();
}

void FractalDrawer::loadSettings(){
    QSettings settings("FNSPE", "FractalDrawer2K5");
    x1 = settings.value("geometry/x1", x1).toDouble();
    x2 = settings.value("geometry/x2", x2).toDouble();
    y1 = settings.value("geometry/y1", y1).toDouble();
    y2 = settings.value("geometry/y2", y2).toDouble();
    iters = settings.value("geometry/iters", iters).toUInt();
    exponent = settings.value("fractal/exponent", exponent).toInt();
    cRe = settings.value("fractal/cRe", cRe).toDouble();
    cIm = settings.value("fractal/cIm", cIm).toDouble();
    type = (FractalType)settings.value("fractal/type", MANDELBROT).toUInt();
}

void FractalDrawer::saveSettings(){
    QSettings settings("FNSPE", "FractalDrawer2K5");
    settings.setValue("geometry/x1", x1);
    settings.setValue("geometry/x2", x2);
    settings.setValue("geometry/y1", y1);
    settings.setValue("geometry/y2", y2);
    settings.setValue("geometry/iters", iters);
    settings.setValue("fractal/exponent", exponent);
    settings.setValue("fractal/cRe", cRe);
    settings.setValue("fractal/cIm", cIm);
    settings.setValue("fractal/type", type);
}

void FractalDrawer::setupForm(){
    lblImage = new QLabel;
    lblImage->setSizeIncrement(QSizePolicy::Ignored,
                               QSizePolicy::Ignored);
    scrollArea = new QScrollArea;
    scrollArea->setWidget(lblImage);
    this->setCentralWidget(scrollArea);
    this->resize(w, h);
    this->setWindowTitle(tr("Simple fractal drawer"));
}

void FractalDrawer::setupActions(){
    QIcon iconDraw = QIcon::fromTheme("applicationsgraphics",
                                      QIcon(":/icons/draw.svg"));
    actDraw = new QAction(tr("&Draw"));
    actDraw->setStatusTip(tr("Draws fractal", "status tip"));
    actDraw->setToolTip(tr("Draws fractal", "tool tip"));
    actDraw->setShortcut(tr("Ctrl+D", "Shortcut for the Draw action"));
    actDraw->setIcon(iconDraw);

    QIcon iconAbort = QIcon::fromTheme("process-stop",
                                       QIcon(":/icons/abort.svg"));
    actAbort = new QAction(tr("&Abort"));
    actAbort->setStatusTip(tr("Aborts calculation"));
    actAbort->setToolTip(tr("Aborts calculation"));
    actAbort->setShortcut(QKeySequence::Cancel);
    actAbort->setEnabled(false);
    actAbort->setIcon(iconAbort);

    QIcon iconQuit = QIcon::fromTheme("application-exit", QIcon(":/icons/quit.svg"));
    actQuit = new QAction(tr("&Quit"));
    actQuit->setToolTip(tr("&Quit"));
    actQuit->setShortcut(QKeySequence::Quit);
    actQuit->setToolTip(tr("Exits the fractal drawer application"));
    actQuit->setStatusTip(tr("Exits the fractal drawer application"));
    actQuit->setIcon(iconQuit);

    QIcon iconPreferences = QIcon::fromTheme("preferences-system",
                                             QIcon(":/icons/preferences.svg"));
    actPreferences = new QAction(tr("&Preferences..."));
    actPreferences->setToolTip(tr("Configures Fractal Drawer"));
    actPreferences->setStatusTip(tr("Configures Fractal Drawer"));
    actPreferences->setShortcut(QKeySequence::Preferences);
    actPreferences->setIcon(iconPreferences);

    mnuFractal = new QMenu(tr("&Fractal"));
    mnuFractal->addAction(actDraw);
    mnuFractal->addAction(actAbort);
    mnuFractal->addSeparator();
    mnuFractal->addAction(actQuit);

    mnuTools = new QMenu("&Tools");
    mnuTools->addAction(actPreferences);

    this->menuBar()->addMenu(mnuFractal);
    this->menuBar()->addMenu(mnuTools);

    toolBar = new QToolBar(tr("Main toolbar"));
    toolBar->addAction(actDraw);
    toolBar->addAction(actAbort);
    toolBar->addSeparator();
    toolBar->addAction(actPreferences);
    this->addToolBar(toolBar);

    this->setupConnections();
    this->statusBar()->showMessage(tr("Ready"), 2000);
}


void FractalDrawer::setupConnections(){
    connect(actDraw, SIGNAL(triggered()), this, SLOT(draw()));
    connect(actAbort, SIGNAL(triggered()), this, SLOT(abort()));
    connect(actQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actPreferences, SIGNAL(triggered()), this, SLOT(configure()));
}

void FractalDrawer::configure(){
    DlgPreferences dialog;
    dialog.setX1(x1);
    dialog.setX2(x2);
    dialog.setY1(y1);
    dialog.setY2(y2);
    dialog.setIterations(iters);
    dialog.setCRe(cRe);
    dialog.setCIm(cIm);
    dialog.setExponent(exponent);
    dialog.setType(type);
    if(dialog.exec() == QDialog::Accepted){
        x1 = dialog.getX1();
        x2 = dialog.getX2();
        y1 = dialog.getY1();
        y2 = dialog.getY2();
        iters=dialog.getIterations();
        cRe = dialog.getCRe();
        cIm = dialog.getCIm();
        exponent = dialog.getExponent();
        type = dialog.getType();
    }
}

void FractalDrawer::abort(){
    aborted = true;
}

void FractalDrawer::draw(){
    actDraw->setEnabled(false);
    actAbort->setEnabled(true);
    aborted = false;
    QImage image(w, h, QImage::Format_Mono);
    int color;
    Real x, y;
    Real dx, dy;

    statusBar()->showMessage(tr("Drawing..."), 2000);

    dx = (x2 - x1) / w;
    dy = (y2 - y1) / h;
    //std::cout << dx << " " << dy << std::endl;
    x = x1;
    QEventLoop loop;
    for(unsigned int i = 0; i < w; i++){
        y = y1;
        for(unsigned int j = 0; j < h; j++){
            switch(type){
            case MANDELBROT:
                color = mandelbrot(x, y, iters, exponent);
                break;
            case JULIA:
                color = julia(x, y, cRe, cIm, iters);
                break;
            }
            image.setPixel(i, j, color);
            y = y + dy;
            //std::cout << i << " " << j << std::endl;
        }
        x = x + dx;
        statusBar()->showMessage(tr("Calculating in progress %1%").arg(i*100/w));
        loop.processEvents(QEventLoop::AllEvents);
        if(aborted){
            break;
        }
    }
    if(aborted){
        statusBar()->showMessage(tr("Aborted!"), 2000);
    } else{
        lblImage->setPixmap(QPixmap::fromImage(image));
        lblImage->adjustSize();
        statusBar()->showMessage(tr("Drawing completed"), 2000);
    }
    actDraw->setEnabled(true);
    actAbort->setEnabled(false);
}

FractalDrawer::~FractalDrawer() {}
