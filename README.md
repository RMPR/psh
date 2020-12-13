# psh
![psh demo](https://github.com/RMPR/psh/raw/master/psh.gif)

This is a simple shell made for learning purpose.

# Usage

```
psh 
```
Opens a new prompt with a pattern defined in psh.cfg (if the file doesn't exist it will be created with the default pattern `>`). Think of it as a cheap equivalent of `$PS1`.
Now the Shell is working like any other shell, infinite loop waiting for user input starting execution after the user presses `\n`. 
It doesn't support piping and redirecting.

# Shell Built-ins
Essentially `cd`, `exit` and `help`

# Goodies
I don't know why but I feel like it. After each command the Shell will print `pss...sh` with the number of `s` corresponding to the number of characters read. That's it, the 
length of the command is proportional to the prompt size.
