#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GridDisplay.h"
#include "SnowFlake.h"
#include "SudokuBoard.h"
#include "Solver.h"

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QThread>
#include <QGraphicsDropShadowEffect>


class GridDisplay;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static QMediaPlayer *soundtrack;
    static QMediaPlayer *sounds;
    static QMediaPlaylist *soundsPlaylist;


public slots:
    void quitGame();
    void changeTheme();
    void muteSoundtrack();
    void setVolume(int vol);

    void newGame();
    void newGrid();
    void loadNewGrid();
    void getSolution();
    void resetGrid();
    void submit();
    void enableSolutionButton(bool result);
    void enableSubmitButton(bool enable);
    void enableNewGridButton();
    void deleteMediaAtEnd(QMediaPlayer::State state);

private:
    Ui::MainWindow *ui;
    GridDisplay *_grid;
    SudokuBoard *_sudoku;
    QTimer *_timerNewGrid;


/// TO BE DONE:
/// - Use iterators most often instead of loop over indices
/// - Implement "Clue" feature
/// - Implement Sudoku editor
/// - Implement save/open feature

};


#endif // MAINWINDOW_H
