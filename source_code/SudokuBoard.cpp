#include "SudokuBoard.h"


QList<QString> SudokuBoard::cross(QString A, QString B)
{
    QList<QString> result;
    QString key;

    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < B.size(); j++)
        {
            key = "";
            key.append(A[i]);
            key.append(B[j]);
            result.append(key);
        }
    }

    return result;
}

QString SudokuBoard::rows = "ABCDEFGHI";
QString SudokuBoard::columns = "123456789";

QFile SudokuBoard::inputFile("sudoku_input.txt");
int SudokuBoard::nbLinesInputFile = 0;


SudokuBoard::SudokuBoard()
{
    this->_keys = SudokuBoard::cross(SudokuBoard::rows, SudokuBoard::columns);

    for (int i = 0; i < this->_keys.size(); i++)
    {
        this->_values[this->_keys[i]] = "123456789";
    }

    this->_constraints = this->initConstraints();
    //qDebug() << "Contraints = " << this->_constraints;

    this->_inputBoard = "";
    this->_nbWrongValues = 0;

    //Get number of lines in the sudoku input file
    int count = 0;
    QString line;
    if (!SudokuBoard::inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "ERROR: Sudoku input cannot be opened";
    }
    QTextStream in(&SudokuBoard::inputFile);
    while (!in.atEnd())
    {
        line = in.readLine();
        count++;
    }
    SudokuBoard::inputFile.close();
    SudokuBoard::nbLinesInputFile = count;
    //qDebug() << "nb lines = " << SudokuBoard::nbLinesInputFile;
}


QMap<QString, QSet<QString>> SudokuBoard::initConstraints()
{
    QList<QString> lst;
    QString str;
    QList<QString> rs({"ABC", "DEF", "GHI"});
    QList<QString> cs({"123", "456", "789"});

    /// List of all individual rows, columns and 3x3 boxes present on the Sudoku board
    QList<QList<QString>> unitlist;

    //Columns
    for (int i = 0; i < SudokuBoard::columns.size(); i++)
    {
        str = SudokuBoard::columns[i];
        lst = SudokuBoard::cross(SudokuBoard::rows, str);
        unitlist.append(lst);
    }

    //Rows
    for (int i = 0; i < SudokuBoard::rows.size(); i++)
    {
        str = SudokuBoard::rows[i];
        lst = SudokuBoard::cross(str, SudokuBoard::columns);
        unitlist.append(lst);
    }

    //3x3 boxes
    for (int i = 0; i < rs.size(); i++)
    {
        for (int j = 0; j < cs.size(); j++)
        {
            lst = SudokuBoard::cross(rs[i], cs[j]);
            unitlist.append(lst);
        }
    }

    //qDebug() << "unitlist: " << unitlist;

    /// Associate each key with its related row, column and 3x3 box
    QMap<QString, QSet<QString>> units;
    QSet<QString> st;

    for (int i = 0; i < this->_keys.size(); i++)
    {
        lst.clear();
        for (int j = 0; j < unitlist.size(); j++)
        {
            if (unitlist[j].contains(this->_keys[i]))
            {
                lst += unitlist[j];
            }
        }
        st = lst.toSet();
        st.remove(this->_keys[i]);
        units[this->_keys[i]] = st;
    }

    //qDebug() << "units: " << units;

    return units;

}


void SudokuBoard::loadValues(QString values)
{
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i] == "0")
        {
            this->_values[this->_keys[i]] = "123456789";
        }
        else
        {
            this->_values[this->_keys[i]] = values[i];
        }
    }
}


bool SudokuBoard::isCorrectInputFormat(QString input)
{
    QString value;

    if (input.size() == 81)
    {
        for (int i = 0; i < input.size(); i++)
        {
            value = input[i]; //gives a QString (instead of QChar)

            if (value.toInt() < 1 && value.toInt() > 9)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}


void SudokuBoard::assign(QString key, QString value)
{
    this->_values[key] = value;
}


bool SudokuBoard::isCorrectAssignment(QString key, QString value)
{
    //If a number is already assigned to the cell
    if (this->_values[key].size() == 1)
    {
        return false;
    }

    //if this cell value is already existing in the same row, column or 3x3 box
    QSet<QString> cst = this->_constraints.value(key);
    QSet<QString>::const_iterator iter_key = cst.constBegin();
    while (iter_key != cst.constEnd())
    {
        if ( this->_values[*iter_key] == value )
        {
            return false;
        }
        ++iter_key;
    }
    return true;
}


QString SudokuBoard::getAssignment()
{
    QString assignment = "";
    QString key;
    for (int i = 0; i < this->_keys.size(); i++)
    {
        key = this->_keys[i];
        if (this->_values[key].size() > 1)
        {
            assignment += "0";
        }
        else
        {
            assignment += this->_values[key];
        }
    }
    return assignment;
}


QList<QString> SudokuBoard::getKeys()
{
    return this->_keys;
}


QString SudokuBoard::getKey(int index)
{
    return this->_keys[index];
}


QMap<QString, QString> SudokuBoard::getValues()
{
    return this->_values;
}


QString SudokuBoard::getValue(QString key)
{
    return this->_values[key];
}


QMap<QString, QSet<QString>> SudokuBoard::getConstraints()
{
    return this->_constraints;
}


QSet<QString> SudokuBoard::getConstraints(QString key)
{
    return this->_constraints[key];
}


QString SudokuBoard::getInputBoard()
{
    return this->_inputBoard;
}


void SudokuBoard::setInputBoard(QString input)
{
    this->_inputBoard = input;
}


int SudokuBoard::getNbWrongValues()
{
    return this->_nbWrongValues;
}


void SudokuBoard::setNbWrongValues(int nb)
{
    this->_nbWrongValues = nb;
}
