#include "GridDisplay.h"
#include "ui_GridDisplay.h"
#include <QDebug>


GridDisplay::GridDisplay(SudokuBoard *sudoku, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GridDisplay)
{
    ui->setupUi(this);

    this->_sudoku = sudoku;
    this->_nbAssignedCells = 0;

    /*//Define authorized cell values
    QRegExp re("[1-9]");
    QRegExpValidator *validator = new QRegExpValidator(re, this);

    //Initialize all cells of the Sudoku grid
    QList<QLineEdit*> cells = this->findChildren<QLineEdit*>(QString(), Qt::FindDirectChildrenOnly);
    for (int i = 0; i < cells.count(); i++)
    {
        cells[i]->setValidator(validator);
        QObject::connect(cells[i], SIGNAL(returnPressed()), this, SLOT(defocus()));
    }*/

    //Set signals/slots
    QList<QLineEdit*> cells = this->findChildren<QLineEdit*>(QString(), Qt::FindDirectChildrenOnly);
    for (QList<QLineEdit*>::const_iterator cell = cells.constBegin(); cell != cells.constEnd(); ++cell)
    {
        QObject::connect(*cell, SIGNAL(textChanged(QString)), this, SLOT(updateNbCellsFilled(QString)));
    }

}


GridDisplay::~GridDisplay()
{
    delete ui;
}


void GridDisplay::clearGrid()
{
    QList<QString> keys = this->_sudoku->getKeys();
    QLineEdit *cell;
    for (QList<QString>::const_iterator key = keys.constBegin(); key != keys.constEnd(); ++key)
    {
        cell = this->findChild<QLineEdit *>(*key, Qt::FindDirectChildrenOnly);
        cell->setStyleSheet("");
        cell->clear();
    }
}


void GridDisplay::updateNbCellsFilled(QString text)
{
    GridCell* cell = qobject_cast<GridCell*>(sender());
    if (!cell->getPreviousText().isEmpty() && !text.isEmpty())
    {
        cell->setPreviousText(text);
        return;
    }
    cell->setPreviousText(text);

    if (!text.isEmpty())
    {
        this->_nbAssignedCells++;
    }
    else
    {
        this->_nbAssignedCells--;
    }

    //qDebug() << "Nb of assigned cells = " << this->_nbAssignedCells;

    if (this->_nbAssignedCells == 81)
    {
        emit this->allCellsFilled(true);
    }
    else if (this->_nbAssignedCells == 80)
    {
        emit this->allCellsFilled(false);
    }
}


void GridDisplay::enterEvent(QEvent *event)
{
    MainWindow::soundsPlaylist->setCurrentIndex(0);

    QWidget::enterEvent(event);
}


void GridDisplay::loadValues(bool isNewGrid)
{
    if (this->_sudoku != 0)
    {
        QList<QString> keys = this->_sudoku->getKeys();
        QLineEdit *cell;
        this->_sudoku->setNbWrongValues(0); //To be used later on when the user clicks on the "Submit" button

        for (QList<QString>::const_iterator key = keys.constBegin(); key != keys.constEnd(); ++key)
        {
            cell = this->findChild<QLineEdit *>(*key, Qt::FindDirectChildrenOnly);

            //Reinitialization of cells in a new grid is generated
            if (isNewGrid)
            {
                cell->clear();
                cell->setReadOnly(false);
                cell->setStyleSheet("font: normal;");
            }

            if (this->_sudoku->getValue(*key).size() == 1)
            {
                if (isNewGrid)
                {
                    cell->setStyleSheet("color: black; font: bold;");
                }
                else //If the used clicks on "Solution" button, the incorrect cells appear in red
                {
                    if (!cell->text().isEmpty() && cell->text() != this->_sudoku->getValue(*key))
                    {
                        cell->clear(); //Used to keep correct value for GridCell::nbAssignedCells
                        cell->setStyleSheet("background-color: #e6897d; color: #b81e0b");
                        this->_sudoku->setNbWrongValues( this->_sudoku->getNbWrongValues() + 1 );
                    }
                }

                //Load cell value
                cell->setText(this->_sudoku->getValue(*key));

                //Set cell in read-only mode
                cell->setReadOnly(true);
            }
        }
    }
}


int GridDisplay::getNbAssignedCells()
{
    return this->_nbAssignedCells;
}








GridCell::GridCell(QWidget *parent) : QLineEdit(parent)
{
    //Define authorized values
    QRegExp re("[1-9]");
    QRegExpValidator *validator = new QRegExpValidator(re, this);

    //Initialize cell
    this->setValidator(validator);
    QObject::connect(this, SIGNAL(returnPressed()), this, SLOT(defocus()));

    this->_previousText = "";
}


GridCell::~GridCell()
{

}


void GridCell::enterEvent(QEvent *event)
{
    if (!this->isReadOnly())
    {
        MainWindow::sounds->play();
    }

    QLineEdit::enterEvent(event);
}


void GridCell::defocus()
{
    qApp->focusWidget()->clearFocus();
}


void GridCell::focusInEvent(QFocusEvent *event)
{
    if (!this->isReadOnly())
    {
        QLineEdit::focusInEvent(event);
        MainWindow::soundsPlaylist->setCurrentIndex(1);
        MainWindow::sounds->play();
    }
}


QString GridCell::getPreviousText()
{
    return this->_previousText;
}


void GridCell::setPreviousText(QString text)
{
    this->_previousText = text;
}
