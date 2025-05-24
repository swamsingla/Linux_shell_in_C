# Custom Shell Implementation

## Project Overview

This project is a custom shell implemented in C, designed to simulate basic shell functionalities and offer several advanced features like process management, I/O redirection, piping, job control, and a custom configuration file. The shell supports built-in commands and handles system signals such as `Ctrl-C`, `Ctrl-Z`, and `Ctrl-D` for process management and shell exit.

## Features

1. **Custom Command Parsing:**
   - Supports sequential and background execution using `;` and `&`.
   - Allows executing multiple commands in a single line.

2. **Directory Navigation (`hop`):**
   - A custom implementation of the `cd` command.
   - Supports navigation using `hop`, including shortcuts for `~` (home directory), `..` (parent directory), and `-` (previous directory).

3. **Command History (`log`):**
   - Keeps a history of up to 15 commands.
   - Prevents duplicate entries.
   - Commands like `log purge` and `log execute <index>` allow clearing and executing commands from the log, respectively.

4. **File Listing (`reveal`):**
   - Lists files and directories in lexicographic order.
   - Supports flags `-a` (to show hidden files) and `-l` (for detailed information).
   - Color-coded output for directories, files, and executables.
   - Also works for weird kind of flags like -lalalal etc

5. **Foreground Process Time Reporting:**
   - Displays the time taken by any foreground process that runs for more than 2 seconds.
   Also works for neonate , iMan , fg

6. **Process Search (`seek`):**
   - Searches for files and directories within a specified or current directory.
   - Displays results with color-coded output (green for files, blue for directories).

7. **I/O Redirection:**
   - Supports `>`, `>>`, and `<` for output, appending output, and input redirection, respectively.
   - Appropriate error handling for missing files or invalid paths.
   - - **Assumption** -> in a valid command executing once (after tokenizing ';' , '&' and '|') , i am assuming we will get only one <  and only one '>>' or '>'

8. **Piping:**
   - Supports chaining commands with pipes (`|`).
   - Handles multiple pipes and invalid use cases gracefully.
   - **ASSUMPTIONS**
   - - I am assuming my pipe will get only one '&' for         background commands and that too at the end of the whole piped command.
   - - 

9. **Signal Handling:**
   - **Ctrl-C**: Interrupts the foreground process.
   - **Ctrl-D**: Terminates the shell after printing Logging Out...
   - **Ctrl-Z**: Stops the foreground process and pushes it to the background.
   It is also stopping background processes . Don't know why .Spent 3 days on it . Was not able to figure it out.
   - Custom `ping` command to send signals to processes.

10. **Background Process Management (`activities`):**
    - Lists all active and stopped processes started by the shell.
    - Displays the command name, PID, and current state (running or stopped).
    - I am using proclore to implement it . If getting status status S or R , then i am updating status to runnning if needed , if T then stopped, if Z , it means zombie process 


11. **Job Control (`fg` and `bg`):**
    - `fg <pid>` brings a background process to the foreground.
    - Background processes are managed and can be resumed using `bg <pid>`.

12. **Custom Configuration File (`.myshrc`):**
    - Supports user-defined aliases and functions.

## Usage

### Built-in Commands

- **hop [directory]**: Changes the current directory to the specified path. Supports `~`, `..`, and `-`.
- **reveal [directory] [-a] [-l]**: Lists the contents of the specified directory with optional flags for hidden files and detailed view.
- **log**: Displays the command history.
  - `log purge`: Clears the command history.
  - `log execute <index>`: Executes the command at the specified index in the history.
- **seek [target] [-d] [-f] [-e]**: Searches for directories, files, or executables based on the flags.
- **ping <pid> <signal_number>**: Sends a signal to the process with the specified PID.
- **fg <pid>**: Brings the specified background process to the foreground.
- **activities**: Lists all background processes started by the shell.

### I/O Redirection

- Use `>` to redirect output to a file.
- Use `>>` to append output to a file.
- Use `<` to read input from a file.

### Piping

- Chain commands using `|`. The output of one command becomes the input for the next.

### Signals

- **Ctrl-C**: Interrupt the current foreground process.
- **Ctrl-D**: Exit the shell.
- **Ctrl-Z**: Stop the foreground process and move it to the background.

### Building and Running

The project includes a Makefile with the following commands:

- **Build the shell**:
  ```bash
  make
  ```

- **Run the shell**:
  ```bash
  ./shell
  ```

- **Clean build files**:
  ```bash
  make clean
  ```

The Makefile handles compilation of all source files with appropriate flags (-Wall -Wextra) and creates an executable named 'shell'. The clean target removes all object files, the executable, and the command log file.


