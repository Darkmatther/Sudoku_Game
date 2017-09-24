#ifndef GRIDDISPLAY_H
#define GRIDDISPLAY_H

#include <QWidget>
#include <QLineEdit>
#include <QDebug>

#include "MainWindow.h"
#include "SudokuBoard.h"


namespace Ui {
class GridDisplay;
}

class GridDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit GridDisplay(SudokuBoard *sudoku = 0, QWidget *parent = 0);
    ~GridDisplay();

    void enterEvent(QEvent *event);

    void loadValues(bool isNewGrid = false); //Load/display current state of the Sudoku board into the grid
    void clearGrid();

    int getNbAssignedCells();

public slots:
    void updateNbCellsFilled(QString text);


private:
    Ui::GridDisplay *ui;

    SudokuBoard *_sudoku;
    int _nbAssignedCells;


signals:
    void allCellsFilled(bool);

};




class GridCell : public QLineEdit
{

    Q_OBJECT

public:
    explicit GridCell(QWidget *parent = 0);
    ~GridCell();

    void enterEvent(QEvent *event);

    QString getPreviousText();
    void setPreviousText(QString text);

private:
    QString _previousText;

public slots:
    void defocus();
    void focusInEvent(QFocusEvent *event);

};


#endif // GRIDDISPLAY_H
