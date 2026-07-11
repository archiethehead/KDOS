input_buffer times 255 db 0

welcome_message db "Welcome to KDOS Version ",VERSION, "!",CR,LF,0

help_message db CR,LF,"COMMAND-LIST:",CR,LF,"EXIT --> Shuts down your machine.",CR,LF,"HELP --> Outputs a list of commands.",CR,LF,"SYSINFO --> Outputs hardware information",CR,LF,"Note: Commands are not case-sensitive.",CR,LF,0

error db CR,LF,"ERROR: '",0
command_not_recognised db "' is not a recognised command :(",CR,LF,"Try 'HELP.'",CR,LF,0

memory db CR,LF,"MEMORY: ",0
kb db "KB",CR,LF,0

shell_commands:
	dw exit_str
	dw help_str
	dw sysinfo_str
	dw echo_str
	dw clrscr_str

exit_str db "exit",0
help_str db "help",0
sysinfo_str db "sysinfo",0
echo_str db "echo",0
clrscr_str db "clrscr",0

shell_command_function_pointers:
	dw shutdown
	dw help
	dw sysinfo
	dw echo
	dw clrscr
