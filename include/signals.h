/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:14:33 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/05 16:53:24 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H
# include "minishell.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <stdio.h>

/**
 * struct termios
 * {
 * tcflag_t	c_cflag;   // Control flags
 * tcflag_t	c_lflag;   // Locals flags
 * tcflag_t	c_iflag;   // Enter flags
 * tcflag_t	c_oflag;   // Out flags
 * cc_t		c_cc[NCCS];// Special caracters (como EOF, INTR, etc)
 * };
 * 
 * c_cflag (Control Flags):
 * Controls hardware-related terminal settings such as baud rate, character size
 * (number of bits per byte), parity, stop bits, and hardware flow control.
 * These flags manage the physical and link-layer behavior of the serial line.
 * 
 * c_lflag (Local Flags):
 * Controls terminal features related to line discipline, input editing, signal
 * generation, and echoing. For example, enabling canonical mode (line buffering),
 * signal characters like Ctrl+C, or echoing typed characters to the screen.
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


t_body	*config_minishell(t_body *minishell);

void	recive_signals(t_body *minishell);

#endif
