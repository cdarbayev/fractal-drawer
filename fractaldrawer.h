#ifndef FRACTALDRAWER_H
#define FRACTALDRAWER_H

#include <QMainWindow>
#include "datatype.h"

class QScrollArea;
class QLabel;
class QAction;
class QMenu;
class QToolBar;
class QCloseEvent;

class FractalDrawer : public QMainWindow
{
    Q_OBJECT
    Real x1, x2, y1, y2;
    unsigned int w, h; //width, height
    unsigned int exponent;
    unsigned int iters;
    FractalType type;
    Real cRe, cIm;
    bool aborted;
    QLabel *lblImage;
    QScrollArea *scrollArea;
    QMenu *mnuFractal;
    QMenu *mnuTools;
    QToolBar *toolBar;
    QAction *actDraw;
    QAction *actAbort;
    QAction *actQuit;
    QAction *actPreferences;
    void setupForm();
    void setupActions();
    void setupConnections();
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *evt);
public:
    FractalDrawer(QWidget *parent = nullptr);
    ~FractalDrawer();
public slots:
    void draw();
    void abort();
    void configure();
};
#endif // FRACTALDRAWER_H
