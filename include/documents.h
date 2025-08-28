/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:14:33 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/28 19:49:42 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H
# include <sys/wait.h>
# include <stdlib.h>
# include <unistd.h>
# include "minishell.h"


/**
 * struct termios
 * {
 * tcflag_t	c_cflag;	Control flags
 * tcflag_t	c_lflag;	Locals flags
 * tcflag_t	c_iflag;	Enter flags
 * tcflag_t	c_oflag;	Out flags
 * cc_t		c_cc[NCCS];	Special caracters (como EOF, INTR, etc)
 * };
 * 
 * c_cflag (Control Flags):
 * Controls hardware-related terminal settings such as baud rate, character
 * size (number of bits per byte), parity, stop bits, and hardware
 * flow control.
 * These flags manage the physical and link-layer behavior of the serial line.
 * 
 * c_lflag (Local Flags):
 * Controls terminal features related to line discipline, input editing, signal
 * generation, and echoing. For example, enabling canonical mode
 * (line buffering), signal characters like Ctrl+C, or echoing typed characters
 * to the screen.
 * 
 * c_iflag (Input Flags):
 * Controls how input bytes are processed. This includes special handling for
 * carriage returns, line feeds, parity errors, software flow control (XON/XOFF)
 * and input translations.
 * 
 * c_oflag (Output Flags):
 * Controls output processing. This includes whether newline characters are
 * translated to carriage return + newline, output post-processing, and other
 * formatting features applied before characters are sent to the terminal.
 * 
 * c_cc (Control Characters Array):
 * Contains special control characters used by the terminal driver, like
 * EOF (End Of File), INTR (interrupt, e.g., Ctrl+C), QUIT (e.g., Ctrl+),
 * ERASE, KILL, and others. These define which keys generate these special
 * signals.
 * 
 * This is the struct of termios, if you type in the command secction with
 * with the command "ssty" and some of this flags, you can modify any
 * information inside the minishell for true or false.
 * 
 * Examples:
 * 
 * 1.
 * If you type "stty 9600" you set the baud rate to 9600 bits per second.
 * 
 * 2.
 * if you type "stty -echoctl" you disable printing control characters like
 * ^C when pressing Ctrl+C.
 * 
 * 3.
 * if you type "stty -ixon" you disable software flow control (XON/XOFF),
 * so Ctrl+S and Ctrl+Q don’t pause/resume output.
 * 
 * 4.
 * if you type "stty onlcr" you enable translating newline (\n) into carriage
 * return + newline (\r\n) on output.
 * 
 * 5.
 * If you type "stty intr ^C" you set the interrupt character to Ctrl+C, which
 * sends the SIGINT signal.
 * 
 * C_IFLAG
 * 
 * Flag	Meaning
 * 
 * IGNBRK	Ignore break condition
 * BRKINT	Signal interrupt on break
 * IGNPAR	Ignore characters with parity errors
 * PARMRK	Mark parity errors
 * ISTRIP	Strip character to 7 bits
 * INPCK	Enable input parity checking
 * INLCR	Map NL (newline) to CR (carriage return) on input
 * IGNCR	Ignore carriage return
 * ICRNL	Map CR to NL on input
 * IXON		Enable XON/XOFF flow control on output
 * 
 * 
 * C_OFLAG
 * 
 * Flag	Meaning
 * 
 * ONLRET	NL performs CR function
 * OPOST	Enable output processing
 * ONLCR	Map NL to CR-NL on output
 * OCRNL	Map CR to NL on output
 * ONOCR	No CR output at column 0
 * 
 * 
 * C_OFLAG
 * 
 * Flag	Meaning
 * 
 * CS5..CS8	Character size (5 to 8 bits)
 * CSTOPB	Send two stop bits
 * PARENB	Enable parity generation and detection
 * PARODD	Odd parity, else even
 * CSIZE	Character size mask
 * CREAD	Enable receiver
 * HUPCL	Hang up on last close
 * 
 * 
 * C_LFLAG
 * 
 * Flag	Meaning
 * 
 * ECHOCTL	Echo control characters as ^X
 * ICANON	Enable canonical mode (line buffering)
 * NOFLSH	Disable flush after interrupt or quit
 * ECHONL	Echo newline even if ECHO is off
 * ECHOE	Echo erase character as backspace
 * ECHOK	Echo kill (clear line)
 * ISIG		Enable signals (INTR, QUIT, [D]SUSP)
 * ECHO		Enable echoing of input characters
 * 
 * 
 * C_CC
 * 
 * Index	Meaning	Typical default
 * 
 * VERASE	Erase character	Backspace or DEL
 * VSTART	Start character (XON)	Ctrl+Q
 * VINTR	Interrupt character	Ctrl+C
 * VQUIT	Quit character	Ctrl+\
 * VKILL	Kill line character	Ctrl+U
 * VSTOP	Stop character (XOFF)	Ctrl+S
 * VEOF		End-of-file character	Ctrl+D
 * VEOL		End-of-line character (additional)	
 */

/**
 * struct sigaction
 * {
 *     sigset_t	sa_mask;			Signals to block during handler execution
 *     void		(*sa_sigaction)(int, siginfo_t *, void *); // Extended handler
 *     void		(*sa_handler)(int);	Pointer to the signal handling function
 *     int		sa_flags;			Flags controlling handler behavior
 * };
 * 
 * sa_handler:
 * - Function pointer that defines the basic signal handler.
 * - This function is called when the signal is delivered.
 * - You must assign either `sa_handler` or `sa_sigaction`, not both.
 * 
 * sa_sigaction:
 * - Alternative to `sa_handler` used for extended signal handling.
 * - Provides more context via a `siginfo_t` structure and a `ucontext_t`
 *   pointer.
 * - Only used when the `SA_SIGINFO` flag is set in `sa_flags`.
 * 
 * sa_mask:
 * - A set of signals that are **blocked during the execution** of the signal
 *   handler.
 * - Prevents the specified signals from interrupting the current handler.
 * - Use `sigemptyset()`, `sigaddset()` and related functions to manipulate it.
 * 
 * sa_flags:
 * Controls how the signal is handled. Common values include:
 * 
 * - SA_RESTART: Automatically restarts certain interrupted system calls.
 * - SA_SIGINFO: Use `sa_sigaction` instead of `sa_handler`.
 * - SA_NOCLDWAIT: Prevent zombie processes (for SIGCHLD).
 * - SA_NOCLDSTOP: Prevents receiving SIGCHLD when children stop.
 * - SA_NODEFER: Do not block the signal being handled.
 * 
 * To use `sigaction`:
 * 
 * 1. Declare and initialize a struct sigaction.
 * 2. Set the handler function.
 * 3. Set any desired flags (like SA_RESTART).
 * 4. Define signals to block during the handler using `sa_mask`.
 * 5. Register the signal with `sigaction(signum, &act, NULL);`
 * 
 * Example:
 * 
 * struct sigaction sa;
 * sa.sa_handler = my_handler;
 * sigemptyset(&sa.sa_mask);
 * sigaddset(&sa.sa_mask, SIGINT); // Block SIGINT during handler
 * sa.sa_flags = SA_RESTART;
 * sigaction(SIGINT, &sa, NULL);
 * 
 * Compared to signal():
 * - `sigaction` is more powerful and reliable.
 * - `signal()` is older and has less consistent behavior across systems.
 * 
 * Notes:
 * - If you're using `readline()`, `SA_RESTART` is usually necessary to avoid
 *   the prompt being broken by Ctrl+C or other interruptions.
 * - Always handle signals carefully to avoid race conditions or unsafe behavior.
 */

t_env	*add_env(t_env *head, char *new_env);

/**
 * Initializes the shell terminal and process group.
 * 
 * This function checks if STDIN is a terminal, sets the shell
 * to its own process group, assigns terminal control to it, and
 * configures terminal settings to show control characters (like ^C).
 * 
 * Steps performed:
 * 
 * - Checks if STDIN is a terminal (interactive shell).
 * - Sets the current process as the leader of its own process group.
 * - Takes control of the terminal with `tcsetpgrp`.
 * - Retrieves current terminal settings.
 * - Modifies terminal flags to enable ECHOCTL (shows ^C, ^\ on screen).
 * 
 * @note - ECHOCTL makes control characters like ^C visible in the prompt.
 * @note - If any system call fails, the function prints an error and exits.
 * 
 * @warning - Must be called once at shell startup before prompting the user.
 * 
 */

#endif
