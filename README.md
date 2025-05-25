# C_Shell

## Description

C_Shell is a custom shell implementation written in C. It provides a command-line interface for interacting with the operating system, allowing users to execute commands, manage files, and run programs.

## Features

-   **Command Execution:** Executes commands entered by the user.
-   **Built-in Commands:** Includes built-in commands for common tasks such as:
    -   `cd`: Change directory
    -   `ls`: List directory contents
    -   `pwd`: Print working directory
    -   `exit`: Exit the shell
-   **Background Processes:** Supports running processes in the background.
-   **Job Control:** Allows users to manage and control running processes.
-   **Redirection:** Supports input and output redirection.
-   **Piping:** Allows chaining commands together using pipes.
-   **History:** Maintains a history of previously executed commands.
-   **Signal Handling:** Handles signals such as Ctrl+C and Ctrl+Z.

## Build Instructions

To build C\_Shell, you need a C compiler (such as GCC) and the `make` utility.

1.  Clone the repository:

    ```bash
    git clone <repository_url>
    cd C_Shell
    ```

2.  Compile the code:

    ```bash
    make
    ```

This will create an executable file named `c_shell`.

## Usage

To run C\_Shell, execute the compiled binary:

```bash
./c_shell
```

Once the shell is running, you can enter commands and interact with the system.

## Contributing

Contributions are welcome! If you find a bug or have a suggestion, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
