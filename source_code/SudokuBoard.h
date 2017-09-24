#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include <QString>
#include <QVector>
#include <QList>
#include <QSet>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QTextStream>


class SudokuBoard
{

public:
    SudokuBoard();
    ~SudokuBoard();

    QMap<QString, QSet<QString>> initConstraints();
    void loadValues(QString values); //Load input Sudoku grid
    bool isCorrectInputFormat(QString input);
    void assign(QString key, QString value);
    bool isCorrectAssignment(QString key, QString value); //Check if assignment (key, value) is permitted or not
    QString getAssignment(); //Converts the current sudoku grid into a string of values

    QList<QString> getKeys();
    QString getKey(int index);
    QMap<QString, QString> getValues();
    QString getValue(QString key);
    QMap<QString, QSet<QString>> getConstraints();
    QSet<QString> getConstraints(QString key);
    QString getInputBoard();
    void setInputBoard(QString input);
    int getNbWrongValues();
    void setNbWrongValues(int nb);

    static QString rows;
    static QString columns;
    static QList<QString> cross(QString A, QString B);
    static QFile inputFile;
    static int nbLinesInputFile;


private:
    QList<QString> _keys;
    QMap<QString, QString> _values;
    QMap<QString, QSet<QString>> _constraints;
    QString _inputBoard; //Input Sudoku configuration
    int _nbWrongValues; //Number of wrong values submitted by the user

};


#endif // SUDOKUBOARD_H
