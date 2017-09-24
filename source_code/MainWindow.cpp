#include "MainWindow.h"
#include "ui_MainWindow.h"


QMediaPlayer *MainWindow::soundtrack = 0;
QMediaPlayer *MainWindow::sounds = 0;
QMediaPlaylist *MainWindow::soundsPlaylist = 0;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set window title and icon
    this->setWindowTitle("Sudoku");
    this->setWindowIcon( QIcon("./images/sudoku_icon.png") );

    //Define Sudoku board (stores information about the current state of the Sudoku board)
    this->_sudoku = new SudokuBoard();

    //Define Sudoku grid
    this->_grid = new GridDisplay(this->_sudoku);
    this->ui->frame_gridDisplay->layout()->addWidget(this->_grid);

    //Initialize timer
    this->_timerNewGrid = new QTimer();

    //Set signals/slots
    QObject::connect(this->ui->button_newGame, SIGNAL(clicked()), this, SLOT(newGame()));
    QObject::connect(this->ui->button_blueTheme, SIGNAL(clicked()), this, SLOT(changeTheme()));
    QObject::connect(this->ui->button_redTheme, SIGNAL(clicked()), this, SLOT(changeTheme()));
    QObject::connect(this->ui->button_muteSoundtrack, SIGNAL(clicked()), this, SLOT(muteSoundtrack()));
    QObject::connect(this->ui->verticalSlider_soundtrack, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
    QObject::connect(this->ui->verticalSlider_sounds, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
    QObject::connect(this->ui->button_newGrid, SIGNAL(clicked()), this, SLOT(newGrid()));
    QObject::connect(this->ui->button_solution, SIGNAL(clicked()), this, SLOT(getSolution()));
    QObject::connect(this->ui->button_reset, SIGNAL(clicked()), this, SLOT(resetGrid()));
    QObject::connect(this->ui->button_submit, SIGNAL(clicked()), this, SLOT(submit()));
    QObject::connect(this->_grid, SIGNAL(allCellsFilled(bool)), this, SLOT(enableSubmitButton(bool)));

    //Set menu actions/slots
    QObject::connect(this->ui->action_quit, SIGNAL(triggered()), this, SLOT(quitGame()));

    //Initialize audio
    MainWindow::soundtrack = new QMediaPlayer;
    MainWindow::soundtrack->setVolume(75);
    MainWindow::soundtrack->setMedia(QUrl::fromLocalFile("./audio/soundtrack/blueTheme.mp3"));
    MainWindow::soundtrack->play();

    MainWindow::sounds = new QMediaPlayer;
    MainWindow::sounds->setVolume(75);
    MainWindow::soundsPlaylist = new QMediaPlaylist;
    MainWindow::soundsPlaylist->addMedia(QUrl::fromLocalFile("./audio/samples/blop_hoverCell.mp3"));
    MainWindow::soundsPlaylist->addMedia(QUrl::fromLocalFile("./audio/samples/button_click.mp3"));
    MainWindow::soundsPlaylist->addMedia(QUrl::fromLocalFile("./audio/samples/button_click_5.mp3"));
    MainWindow::soundsPlaylist->addMedia(QUrl::fromLocalFile("./audio/samples/button_click_3.mp3"));
    MainWindow::soundsPlaylist->addMedia(QUrl::fromLocalFile("./audio/samples/button_click_4.mp3"));
    MainWindow::soundsPlaylist->setCurrentIndex(0);
    MainWindow::soundsPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    MainWindow::sounds->setPlaylist(MainWindow::soundsPlaylist);

    //Initialize ui elements
    this->ui->button_muteSoundtrack->setIcon(QIcon("./images/audio_on.svg"));
    this->ui->widget_centralContent->setVisible(false);
    this->ui->button_solution->setEnabled(false);
    this->ui->button_reset->setEnabled(false);
    this->ui->button_clue->setEnabled(false);
    this->ui->button_submit->setEnabled(false);
    this->ui->button_blueTheme->setEnabled(false);
    this->ui->label_gameOver->setVisible(false);
    this->ui->label_nbErrors->setVisible(false);
    QGraphicsDropShadowEffect *borderEffect = new QGraphicsDropShadowEffect(this->ui->label_gameOver);
    borderEffect->setBlurRadius(0);
    borderEffect->setColor(Qt::black);
    borderEffect->setOffset(-2,-2);
    this->ui->label_gameOver->setGraphicsEffect(borderEffect);
    this->ui->widgetLayout_header->layout()->setAlignment(this->ui->button_reset, Qt::AlignBottom);
    this->ui->widgetLayout_header->layout()->setAlignment(this->ui->button_solution, Qt::AlignBottom);
    this->ui->widgetLayout_footer->layout()->setAlignment(this->ui->button_clue, Qt::AlignTop);
    this->ui->verticalLayout_rightSide->setAlignment(this->ui->button_muteSoundtrack, Qt::AlignCenter);

    //Initialise time for random number generation
    qsrand(time(0));

    //Load fonts
    QFontDatabase::addApplicationFont("./fonts/calibri.ttf");
    QFontDatabase::addApplicationFont("./fonts/chinyen.ttf");

    //Set default theme for the app
    QFile styleSheet("styleSheet_blue.qss");
    styleSheet.open(QFile::ReadOnly);
    QString styleSheetContent = QLatin1String(styleSheet.readAll());
    qApp->setStyleSheet(styleSheetContent);
    SnowFlake::generateSnow(this->ui->centralWidget);

}


MainWindow::~MainWindow()
{
    delete MainWindow::soundtrack;
    delete MainWindow::sounds;
    delete MainWindow::soundsPlaylist;
    delete this->_grid;
    delete this->_timerNewGrid;
    delete ui;
}


void MainWindow::quitGame()
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon("./images/sudoku_icon.png"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Close Sudoku game");
    msgBox.setText("Do you want to save the current game before closing?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int quit = msgBox.exec();
    if (quit == QMessageBox::Save)
    {
        //Save changes (to be done...)
        qApp->quit();
    }
    else if (quit == QMessageBox::Discard)
    {
        qApp->quit();
    }
}


void MainWindow::changeTheme()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString fileName;

    if (button == this->ui->button_blueTheme)
    {
        fileName = "styleSheet_blue.qss";
        MainWindow::soundtrack->setMedia(QUrl::fromLocalFile("./audio/soundtrack/blueTheme.mp3"));
        MainWindow::soundtrack->play();
        SnowFlake::generateSnow(this->ui->centralWidget);
        this->ui->button_blueTheme->setEnabled(false);
        this->ui->button_redTheme->setEnabled(true);
    }
    else if (button == this->ui->button_redTheme)
    {
        SnowFlake::stopSnow();
        fileName = "styleSheet_red.qss";
        MainWindow::soundtrack->setMedia(QUrl::fromLocalFile("./audio/soundtrack/redTheme.mp3"));
        MainWindow::soundtrack->play();
        this->ui->button_blueTheme->setEnabled(true);
        this->ui->button_redTheme->setEnabled(false);
    }
    else
    {
        fileName = "";
    }

    QFile styleSheet(fileName);
    if (styleSheet.open(QFile::ReadOnly))
    {
        QString styleSheetContent = QLatin1String(styleSheet.readAll());
        qApp->setStyleSheet(styleSheetContent);
    }

}


void MainWindow::muteSoundtrack()
{
    if (MainWindow::soundtrack->isMuted())
    {
        MainWindow::soundtrack->setMuted(false);
        MainWindow::sounds->setMuted(false);
        if (SnowFlake::sounds != 0)
        {
            SnowFlake::sounds->setMuted(false);
        }
        this->ui->button_muteSoundtrack->setIcon(QIcon("./images/audio_on.svg"));
        this->ui->verticalSlider_soundtrack->setEnabled(true);
        this->ui->verticalSlider_sounds->setEnabled(true);
    }
    else
    {
        MainWindow::soundtrack->setMuted(true);
        MainWindow::sounds->setMuted(true);
        if (SnowFlake::sounds != 0)
        {
            SnowFlake::sounds->setMuted(true);
        }
        this->ui->button_muteSoundtrack->setIcon(QIcon("./images/audio_off.svg"));
        this->ui->verticalSlider_soundtrack->setEnabled(false);
        this->ui->verticalSlider_sounds->setEnabled(false);
    }
}


void MainWindow::setVolume(int vol)
{
    QSlider* slider = qobject_cast<QSlider*>(sender());

    if (slider == this->ui->verticalSlider_soundtrack)
    {
        MainWindow::soundtrack->setVolume(vol);
    }
    else if (slider == this->ui->verticalSlider_sounds)
    {
        MainWindow::sounds->setVolume(vol);
        if (SnowFlake::sounds != 0)
        {
            SnowFlake::sounds->setVolume(vol);
        }
    }

    //Disable the mute button if all volumes are set to 0
    if ( MainWindow::soundtrack->volume() == 0 && MainWindow::sounds->volume() == 0 && this->ui->button_muteSoundtrack->isEnabled() )
    {
        this->ui->button_muteSoundtrack->setEnabled(false);
    }
    else if ( ( MainWindow::soundtrack->volume() > 0 || MainWindow::sounds->volume() > 0 ) && !this->ui->button_muteSoundtrack->isEnabled() )
    {
        this->ui->button_muteSoundtrack->setEnabled(true);
    }
}


void MainWindow::newGame()
{
    //Display Soduku and load a new grid
    this->ui->widget_startMenu->setVisible(false);
    this->ui->widget_centralContent->setVisible(true);
    this->loadNewGrid();

    //Play sound
    QMediaPlayer *soundNewGame = new QMediaPlayer();
    connect(soundNewGame, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(deleteMediaAtEnd(QMediaPlayer::State)));
    soundNewGame->setMedia(QUrl::fromLocalFile("./audio/samples/newGame.mp3"));
    soundNewGame->play();
}


void MainWindow::deleteMediaAtEnd(QMediaPlayer::State state)
{
    QMediaPlayer* media = qobject_cast<QMediaPlayer*>(sender());

    if (state == QMediaPlayer::StoppedState)
    {
        //qDebug() << "New game sound finished!";
        media->deleteLater();
    }
}


void MainWindow::newGrid()
{
    //Play click sound
    MainWindow::soundsPlaylist->setCurrentIndex(2);
    MainWindow::sounds->play();

    //Disable buttons
    this->ui->button_newGrid->setEnabled(false);
    this->ui->button_solution->setEnabled(false);
    this->ui->button_reset->setEnabled(false);
    this->ui->button_clue->setEnabled(false);

    this->ui->label_gameOver->setVisible(false);
    this->ui->label_nbErrors->setVisible(false);
    this->ui->widgetLayout_footer->setVisible(true);

    //Clear grid display
    this->_grid->clearGrid();

    //Wait for a fraction of a second to let the current running BTS solver aborting safely
    //and then load new grid values using this->loadNewGrid()
    QObject::connect(this->_timerNewGrid, SIGNAL(timeout()), this, SLOT(loadNewGrid()));
    this->_timerNewGrid->setSingleShot(true);
    this->_timerNewGrid->start(200);
}


void MainWindow::loadNewGrid()
{
    //Disconnect timer slot that has previously been connected in the newGrid() method
    QObject::disconnect(this->_timerNewGrid, SIGNAL(timeout()), this, SLOT(loadNewGrid()));

    //Default input if no external input file available
    QString input = "031260004400019000000004006000000050060008020000056300010900000028000001003000290";

    //Select a random line in the Sudoku input file
    if (SudokuBoard::inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        int rand_line;
        QString line;

        rand_line = qrand()%SudokuBoard::nbLinesInputFile + 1;

        QTextStream in(&SudokuBoard::inputFile);
        for (int i = 1; i <= rand_line; i++)
        {
            line = in.readLine();
        }
        SudokuBoard::inputFile.close();

        //Check the integrity of the input configuration
        if ( this->_sudoku->isCorrectInputFormat(line) )
        {
            input = line;
        }
        else
        {
            qDebug() << "ERROR: input Sudoku format is incorrect. Loading default configuration.";
        }
    }
    else
    {
        qDebug() << "ERROR: Sudoku input file cannot be opened. Loading default configuration.";
    }

    //Store input config
    this->_sudoku->setInputBoard(input);

    //Load input in the Sudoku instance and in the display grid
    this->_sudoku->loadValues(input);
    this->_grid->loadValues(true);

    //Start BTS solver in the background
    SolverBTS *solverThread = new SolverBTS(this->_sudoku);
    connect(solverThread, SIGNAL(resultReady(bool)), this, SLOT(enableSolutionButton(bool)));
    connect(solverThread, SIGNAL(finished()), solverThread, SLOT(deleteLater()));
    connect(this->ui->button_newGrid, SIGNAL(clicked(bool)), solverThread, SLOT(stopCalculation()));
    solverThread->start();

    //Reactivate "New grid" button
    this->ui->button_newGrid->setEnabled(true);
}


void MainWindow::getSolution()
{
    //Play click sound
    MainWindow::soundsPlaylist->setCurrentIndex(4);
    MainWindow::sounds->play();

    QObject::disconnect(this->_grid, SIGNAL(allCellsFilled(bool)), this, SLOT(enableSubmitButton(bool)));
    this->ui->button_solution->setEnabled(false);
    this->_grid->loadValues();
    this->ui->button_submit->setEnabled(false);
    this->ui->button_clue->setEnabled(false);
    QObject::connect(this->_grid, SIGNAL(allCellsFilled(bool)), this, SLOT(enableSubmitButton(bool)));
}


void MainWindow::enableSolutionButton(bool result)
{
    this->ui->button_solution->setEnabled(result);
    this->ui->button_reset->setEnabled(result);
    this->ui->button_clue->setEnabled(result);
}


void MainWindow::enableNewGridButton()
{
    this->ui->button_newGrid->setEnabled(true);
}


void MainWindow::enableSubmitButton(bool enable)
{
    if (enable)
    {
        this->ui->button_submit->setStyleSheet("background-color: #66dba0");
    }
    else
    {
        this->ui->button_submit->setStyleSheet("");
    }

    this->ui->button_submit->setEnabled(enable);
}


void MainWindow::resetGrid()
{
    //Play click sound
    MainWindow::soundsPlaylist->setCurrentIndex(3);
    MainWindow::sounds->play();

    //Save the solution (so it doesn't need to be computed again)
    QString solution = this->_sudoku->getAssignment();

    //Reset configuration
    this->_sudoku->loadValues(this->_sudoku->getInputBoard());
    this->_grid->loadValues(true);

    //Reload the already-calculated solution into the SudokuBoard instance only (not the GridDiplay)
    this->_sudoku->loadValues(solution);

    //Re-activate buttons
    this->ui->button_solution->setEnabled(true);
    this->ui->button_clue->setEnabled(true);
    this->ui->label_gameOver->setVisible(false);
    this->ui->label_nbErrors->setVisible(false);
    this->ui->widgetLayout_footer->setVisible(true);
}


void MainWindow::submit()
{
    //Play click sound
    MainWindow::soundsPlaylist->setCurrentIndex(2);
    MainWindow::sounds->play();

    //Load solution and compare it to the user's values
    this->ui->button_solution->setEnabled(false);
    this->_grid->loadValues();
    this->ui->button_submit->setEnabled(false);
    this->ui->widgetLayout_footer->setVisible(false);
    this->ui->label_gameOver->setVisible(true);

    //Play sound
    QMediaPlayer *soundGameOver = new QMediaPlayer();
    connect(soundGameOver, SIGNAL(stateChanged()), soundGameOver, SLOT(deleteLater()));

    //Display if the user won the game or not
    //QPalette pl = this->ui->label_gameOver->palette();
    if (this->_sudoku->getNbWrongValues() == 0)
    {
        qDebug() << "YOU WIN !!!";
        this->ui->label_gameOver->setText("YOU WIN !!");
        //pl.setColor(this->ui->label_gameOver->foregroundRole(), QColor("#41b11f"));
        this->ui->label_gameOver->setStyleSheet("color: #41b11f");
        soundGameOver->setMedia(QUrl::fromLocalFile("./audio/samples/win.mp3"));
    }
    else
    {
        qDebug() << "YOU'VE LOST! You made" << this->_sudoku->getNbWrongValues() << "mistakes.";
        this->ui->label_nbErrors->setVisible(true);
        this->ui->label_gameOver->setText("YOU'VE LOST!");
        this->ui->label_nbErrors->setText("You made " + QString::number(this->_sudoku->getNbWrongValues()) + " mistakes.");
        //pl.setColor(this->ui->label_gameOver->foregroundRole(), QColor("#c52323"));
        this->ui->label_gameOver->setStyleSheet("color: #c52323");
        soundGameOver->setMedia(QUrl::fromLocalFile("./audio/samples/loose.mp3"));
    }

    soundGameOver->play();
    //this->ui->label_gameOver->setPalette(pl);
}
