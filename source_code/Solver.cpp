#include "Solver.h"


SolverAC3::SolverAC3(SudokuBoard *sudoku)
{
    this->_sudoku = sudoku;
    this->_abort = false;
}


SolverAC3::~SolverAC3()
{

}


bool SolverAC3::AC3()
{
    QQueue<QPair<QString, QString>> queue;

    /// Add all possible combination of keys to the queue
    QList<QString> keys = this->_sudoku->getKeys();
    QList<QString>::const_iterator key;

    QSet<QString> cst;
    QSet<QString>::const_iterator key_cst;

    QPair<QString, QString> pair;

    for (key = keys.constBegin(); key != keys.constEnd(); ++key)
    {
        if (this->_abort)
        {
            return false;
        }

        cst = this->_sudoku->getConstraints(*key);

        for (key_cst = cst.constBegin(); key_cst != cst.constEnd(); ++key_cst)
        {
            if (this->_abort)
            {
                return false;
            }

            pair.first = *key;
            pair.second = *key_cst;
            queue.enqueue(pair);
        }
    }

    //Print queue
    /*for (QQueue<QPair<QString, QString>>::const_iterator q = queue.constBegin(); q != queue.constEnd(); ++q)
    {
        qDebug() << "queue (AC3) = (" << q->first << ", " << q->second << ")";
    }*/

    /// Dequeue pairs of keys and revise domain values of the related cells
    QPair<QString, QString> new_pair;

    while (!queue.isEmpty() && !this->_abort)
    {
        pair = queue.dequeue();

        if (this->revise(pair))
        {
            if (this->_sudoku->getValue(pair.first).size() == 0)
            {
                return false;
            }

            cst = this->_sudoku->getConstraints(pair.first);
            for (key_cst = cst.constBegin(); key_cst != cst.constEnd(); ++key_cst)
            {
                if (*key_cst != pair.second && !this->_abort)
                {
                    new_pair.first = *key_cst;
                    new_pair.second = pair.first;
                    queue.enqueue(new_pair);
                }
                else if (this->_abort)
                {
                    return false;
                }
            }
        }
    }

    /// Check if assignment is complete (i.e. every cell of the Sudoku board is filled)
    for (key = keys.constBegin(); key != keys.constEnd(); ++key)
    {
        if (this->_abort)
        {
            return false;
        }

        if (this->_sudoku->getValue(*key).size() > 1)
        {
            return false; //Sudoku not solved :(
        }
    }

    return true; //Sudoku solved :)
}


bool SolverAC3::revise(QPair<QString, QString> keys)
{
    bool revised = false;
    QString value_first = this->_sudoku->getValue(keys.first);
    QString value_second = this->_sudoku->getValue(keys.second);

    //If there is more than one possible value for keys.second then we can always find a correct assignment for "keys"
    if (value_second.size() > 1)
    {
        return revised;
    }

    for (int i = 0; i < value_first.size(); i++)
    {        
        if (this->_abort)
        {
            return false;
        }

        if (value_second == value_first[i] && !this->_abort)
        {
            this->_sudoku->assign(keys.first, value_first.replace(value_first[i], ""));
            revised = true;
        }
    }

    return revised;
}


void SolverAC3::run()
{
    bool result = this->AC3();

    emit resultReady(result);
}


void SolverAC3::stopCalculation()
{
    this->_abort = true;
    //qDebug() << "Abort current process";
}







SolverBTS::SolverBTS(SudokuBoard *sudoku)
{
    this->_sudoku = sudoku;
    this->_abort = false;
}


SolverBTS::~SolverBTS()
{

}


bool SolverBTS::BTS()
{
    if (!this->_abort)
    {
        bool isSolved = true;
        bool result;

        QList<QString> keys = this->_sudoku->getKeys();

        QString next_key;
        QString ordered_values;

        //if assignment is complete, return True
        for (QList<QString>::const_iterator key = keys.constBegin(); key != keys.constEnd(); ++key)
        {
            if (this->_abort)
            {
                return false;
            }

            if (this->_sudoku->getValue(*key).size() > 1)
            {
                isSolved = false;
            }
        }
        if (isSolved)
        {
            return true;
        }

        //Choose next cell to be assigned (heuristics = minimum remaining value)
        next_key = this->selectUnassignedVariable();

        //Choose next value to be checked in this cell (order = least constraining value)
        ordered_values = this->selectDomainValue(next_key);

        //Apply forward checking algorithm
        for (QString::const_iterator value = ordered_values.constBegin(); value != ordered_values.constEnd(); ++value)
        {
            if (this->_abort)
            {
                return false;
            }

            if (this->_sudoku->isCorrectAssignment(next_key, *value) && !this->_abort)
            {
                this->_sudoku->assign(next_key, *value);
                result = this->BTS();
                if (result)
                {
                    return result;
                }
            }

            if (!this->_abort)
            {
                this->_sudoku->assign(next_key, ordered_values);
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}


QString SolverBTS::selectUnassignedVariable()
{
    //Initialization
    QString min_key = "A1";
    int min_remaining_values = 10;
    int nb_remaining_values;

    //Find the key that has the minimum number of remaining values (> 1)
    QList<QString> keys = this->_sudoku->getKeys();
    for (QList<QString>::const_iterator key = keys.constBegin(); key != keys.constEnd(); ++key)
    {
        if (this->_abort)
        {
            return min_key;
        }

        nb_remaining_values = this->_sudoku->getValue(*key).size();
        if (nb_remaining_values > 1 && nb_remaining_values < min_remaining_values)
        {
            min_remaining_values = nb_remaining_values;
            min_key = *key;
        }
    }

    return min_key;
}


QString SolverBTS::selectDomainValue(QString key)
{
    //for the moment, only returns the list of values in the original order.
    //Later it should return the same list but with modified order w.r.t. 'least constraining value' criteria
    return this->_sudoku->getValue(key);
}


void SolverBTS::run()
{
    bool result = this->BTS();

    emit resultReady(result);
}


void SolverBTS::stopCalculation()
{
    this->_abort = true;
    //qDebug() << "Abort current process";
}
