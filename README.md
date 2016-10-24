# Chess Engine
This chess engine follows the standard UCI protocol. The program does not have a graphical user interface, but the engine can be installed and run on UCI-compliant chess GUIs such as [Arena](http://www.playwitharena.com/).

## How to Build

To build the code, your computer must have GNU Make and the g++ compiler. After downloading, simply run `Make` and use the created executables.

The only executable that the `Makefile` currently creates is `random.exe`, which plays random legal moves.

## How to Run

Running `random.exe` with no flags will open a command-line session in which UCI messages can be entered through STDIN and UCI responses are sent from STDOUT. A UCI-compliant chess GUI will run this executable and handle the I/O for you.

Running `random.exe --runtests` will run the engine's unit tests.
