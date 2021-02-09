# Text-Editor

This is a command-line text editor that allows a user to open files for editing and saving text. It uses the C library functions in termios.h for the same. 

It uses advisory locks such that two simultaneous accesses to the same file using the editor throw an advisory warning on the one who opens the file that already is advisory locked. However, the access thereafter is not blocked. The user who receives the warning can choose to edit the file (regardless of race conditions).
