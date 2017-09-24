
# Sudoku Game with Artificial Intelligence
## Author : Matthieu Kieffer

### Description
This is my first game application!<br/> 
This Sudoku game comes with a fully-customizable interactive graphical user interface, and self-solving artificial intelligence via a CSP algorithm (Constraint Satisfaction Problem).<br/>
Also don't forget to turn the volume on ;)

### How to install the game
To install the application, download all files included in the "Executable" folder, and double-click on Sudoku.exe<br/><br/>
IMPORTANT: all .dll dependencies, plugin folders (imageformats/, mediaservice/, platforms/ and playlistformats/) and the sudoku_input.txt file are mandatory and should stay in the same folder as Sudoku.exe. The .qss files and the audio/, fonts/ and images/ folders are optional but should also be downloaded for an optimal user experience.<br/>
The game should look like this (tested on Windows 7 & Windows 10):
<br/><br/>
![Main window - blue theme](https://user-images.githubusercontent.com/25090342/30785561-c9d811cc-a168-11e7-975a-f9d89324bdbd.PNG "Main window - blue theme")
![Main window - red theme](https://user-images.githubusercontent.com/25090342/30785562-c9d82ce8-a168-11e7-9cab-b1e1443f13ab.PNG "Main window - red theme")

### How the artificial intelligence solver works
The CSP solver starts in the background as soon as the user loads a new Sudoku grid. Once the calculation terminates, the user can use the "Reset" button to reset the current grid, or use the "Solution" button to display the computed solution. If the user fills the whole grid, the "Submit" button becomes available. Just click on it and see if you've fully completed the game! If not, the mistaken cells are shown to the user.

### How to customize the game
The game design can be fully customized through the use of the .qss files that are included within the project.
If you have some basic knowlege of CSS or QSS (Qt style sheets), you can edit the files "styleSheet_blue.qss" and "styleSheet_red.qss" and make your own theme (background image, button styles, colours, ...).<br/>
If you are a C++/Qt programmer you can also easily modify the MainWindow::changeTheme() method and add additional buttons into MainWindow.ui if you want to have more than 2 default themes. The addition of custom fonts and sounds must also be done programmatically in MainWindow.cpp.

### Details about the implementation
Technologies: C++ / Qt

...

(more details coming soon)

### Next features (coming soon)
- Implementation of the "Clue" button, which helps the user by automatically filling a blank or incorrect cell on the grid<br/>
- Add save/open fonctionality to save the current game and finishing it later
- Add a Sudoku editor to create your own Sudoku input grid and add it to the list of existing input configurations (the CSP solver checks if the created configuration has a solution) 
- Improve the efficiency of the CSP algorithm (currently it is a simple backtracking search with basic heuristics)
- Add animation to the red theme
- Adding more themes
