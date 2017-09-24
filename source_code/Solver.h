#ifndef SOLVER_H
#define SOLVER_H

#include "SudokuBoard.h"

#include <QQueue>
#include <QThread>



class SolverBTS : public QThread
{
    Q_OBJECT

public:
    SolverBTS(SudokuBoard *sudoku);
    ~SolverBTS();

    bool BTS(); //BackTracking Search (BTS) algorithm
    QString selectUnassignedVariable(); //Return the next key to be assigned
    QString selectDomainValue(QString key); //Return the next domain value to be checked at the given key

private:
    SudokuBoard *_sudoku;
    bool _abort;

protected:
    void run(); //Overloads QThread::run() method

signals:
    void resultReady(bool result);

public slots:
    void stopCalculation();

};





class SolverAC3 : public QThread
{
    Q_OBJECT

public:
    SolverAC3(SudokuBoard *sudoku);
    ~SolverAC3();

    bool AC3(); //BackTracking Search (BTS) algorithm
    bool revise(QPair<QString, QString> keys); //Return true if the domain of keys.first has been revised

private:
    SudokuBoard *_sudoku;
    bool _abort;

protected:
    void run(); //Overloads QThread::run() method

signals:
    void resultReady(bool result);

public slots:
    void stopCalculation();

};



#endif // SOLVER_H
