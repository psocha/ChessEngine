# Chess Engine
This chess engine follows the standard UCI protocol. The program does not have a graphical user interface, but the engine can be installed and run on UCI-compliant chess GUIs such as [Arena](http://www.playwitharena.com/).

## How to Build

To build the code, your computer must have GNU Make and the g++ compiler. After downloading, simply run `Make` and use the created executables.

The most recent version of the engine uses a min-max tree search with alpha-beta pruning. The engine evaluates chess positions based on material differences and some basic positional considerations. Other AIs for the engine (such as one that plays random moves) can be found in the `ai` folder.

## How to Run

Running `Engine` with no flags will open a command-line session in which UCI messages can be entered through STDIN and UCI responses are sent from STDOUT. A UCI-compliant chess GUI can run this executable and handle the I/O for you.

Running `Engine --runtests` will run the engine's unit tests.
