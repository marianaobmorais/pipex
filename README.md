<p align="center">
	<img src="https://img.shields.io/github/last-commit/marianaobmorais/pipex?color=%2312bab9&style=flat-square"/>
</p>

# pipex

#### Finished in 2024-09-09

## About

This project explores two shell concepts: Redirections and Pipes. In order to make a C program that recreates the behaviour of shell pipes and rediretions, I had to learn about UNIX processes.  

The mandatory part consists of a simple program that takes 4 arguments:
```shell
./pipex file1 cmd1 cmd2 file2
```
That should behave like:
```shell
< file1 cmd1 | cmd2 > file2
```
And the bonus part, accepts `multiple` pipes as well as `heredoc`:
```shell
./pipex file1 cmd1 cmd2 cmd3 cmd4 file2
```
Should behave like:
```shell
< file1 cmd1 | cmd2 | cmd3 | cmd4 > file2
```
And
```shell
./pipex here_doc LIMITER cmd cmd1 file
```
Should behave like:
```shell
cmd << LIMITER | cmd1 >> file
```

The successful execution of a command inside a program results in the end of the program. In order to execute the commands in a pipeline, I had to keep my program running until it reached the last command. To address that, I wrote a while loop that forks the main process for every command to be executed in a child process. This way the commands would only terminate the child process, and the program – which is now the parent process – will be running until the last command in the pipeline. Like this:
```shell
while (i < argc - 1)
{
	if (pipe(fd) == -1)
		return (perror(PERR_PIPE), 1);
	pid = fork();
	if (pid == -1)
		return (perror(PERR_FORK), 1);
	if (pid == 0)
		child_process(args, i - 2 - is_heredoc(argv[1]), envp, fd);
	else
		parent_process(fd, pid, is_heredoc(argv[1]));
	i++;
}
```

## Build

Clone the repository: 
```shell
git clone https://github.com/marianaobmorais/pipex.git
```
Enter the clonned directory:
```shell
cd pipex
```
Run `make` to compile the program:
```shell
make bonus
```
## Usage
For one or multiple pipes, the input:
```shell
./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
```
Behaves like:
```shell
< file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2
```
For heredoc, the input:
```shell
./pipex here_doc LIMITER cmd cmd1 file
```
Behaves like:
```shell
cmd << LIMITER | cmd1 >> file
```
## The Norm

This project is written in accordance to the 42 School's "The Norm" (_Version 4_). To review the Norm, [click here](https://github.com/42School/norminette/blob/master/pdf/en.norm.pdf).

## Contributions

If you find any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.

### Message to students

If you are searching resources to learn more about your own pipex, I encourage you to turn to your peers and the function manuals. Do not implement any code you do not understand and cannot code from scratch.
