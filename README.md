# Minishell

> A C implementation of a Unix shell inspired by **Bash**, developed as part of the 42 School curriculum.

Minishell recreates the essential behavior of a Unix shell by parsing user input, executing commands, managing processes, and handling redirections, pipes, environment variables, and built-in commands. The project provides a deep understanding of how operating systems interact with user-space programs through the POSIX API.

---

## 📖 About

The shell is one of the most fundamental tools in Unix-like operating systems. It acts as an interpreter between the user and the kernel, allowing commands to be executed, processes to be created, and input/output streams to be redirected.

This project recreates many of Bash's core features while exposing the internal mechanisms behind:

- Command parsing
- Process creation
- File descriptor management
- Environment variables
- Signal handling
- Inter-process communication

Rather than relying on existing shell implementations, Minishell builds these features from the ground up in C.

---

## 🚀 Learning Objectives

Through this project, the following concepts were developed:

- Unix process management
- Parsing and lexical analysis
- Process synchronization
- Pipes and file descriptor manipulation
- Signal handling
- Environment variable management
- Low-level systems programming

---

## 🛠 Technologies

- C
- POSIX API
- GNU Readline
- Makefile
- Unix/Linux

---

## 📂 Project Structure

```
.
├── include/        # Header files
├── src/            # Source files
├── libft/          # Custom C library
├── Makefile
└── README.md
```

*(Directory names may vary slightly depending on future updates.)*

---

## ⚙️ Compilation

Clone the repository:

```bash
git clone https://github.com/Santiago-Scheinig/minishell.git
cd minishell
```

Compile:

```bash
make
```

Clean object files:

```bash
make clean
```

Remove generated files:

```bash
make fclean
```

Rebuild:

```bash
make re
```

---

## ▶️ Usage

Launch the shell:

```bash
./minishell
```

Example session:

```bash
minishell$ pwd
/home/user/projects

minishell$ ls -la

minishell$ echo "Hello, world!"

minishell$ cat file.txt | grep error > output.txt
```

---

## ✨ Features

Depending on the implementation, the shell supports:

- Interactive prompt
- Command history (Readline)
- Execution of binaries via `PATH`
- Relative and absolute paths
- Built-in commands
- Pipes (`|`)
- Input/output redirections (`<`, `>`, `>>`)
- Here-documents (`<<`)
- Environment variable expansion (`$VAR`)
- Exit status (`$?`)
- Signal handling (`Ctrl-C`, `Ctrl-D`, `Ctrl-\`)

---

## 🔧 Built-in Commands

- `echo`
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

---

## 🧠 Key Concepts

### Lexer & Parser

Transforms raw user input into structured commands that the shell can execute.

### Process Management

Creates child processes using `fork()` and executes programs through `execve()`.

### Pipes

Allows commands to communicate by redirecting the output of one process into the input of another.

### Redirections

Manipulates file descriptors to redirect standard input and output using the Unix file descriptor model.

### Signal Handling

Implements shell behavior for keyboard interrupts and terminal signals while preserving a user-friendly interactive experience.

---

## 🎯 Skills Demonstrated

- Systems programming
- POSIX process management
- Parsing and command interpretation
- Inter-process communication (IPC)
- File descriptor management
- Shell architecture
- Memory management
- Debugging complex C applications

---

## 📚 References

- Bash Reference Manual
- POSIX System Calls
- GNU Readline Documentation
- Advanced Programming in the UNIX Environment (APUE)
- 42 Minishell project documentation :contentReference[oaicite:1]{index=1}

---

## 👨‍💻 Author

**Santiago Scheinig**

GitHub:

https://github.com/Santiago-Scheinig

---

## 🤖 AI Disclosure

This README was created with the assistance of **OpenAI ChatGPT** based on the project's purpose, repository structure, and common documentation practices. The source code and implementation remain the work of the project author.

---

## 📄 License

This repository currently does not include a software license. If you intend for others to use, modify, or distribute the project, consider adding an open-source license such as MIT or Apache 2.0.
