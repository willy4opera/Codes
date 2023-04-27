#include "shell_hd.h"

/**
 * hsh - main shell loop
 * @_Info_: the parameter & return _Info_ struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *_Info_, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(_Info_);
		if (sh_interactive(_Info_))
			_puts("$ ");
		_eputchar(FLUSH_BUF);
		r = get_input(_Info_);
		if (r != -1)
		{
			set_info(_Info_, av);
			builtin_ret = find_builtin(_Info_);
			if (builtin_ret == -1)
				fnd_cmd(_Info_);
		}
		else if (sh_interactive(_Info_))
			_putchar('\n');
		free_info(_Info_, 0);
	}
	write_history(_Info_);
	free_info(_Info_, 1);
	if (!sh_interactive(_Info_) && _Info_->status)
		exit(_Info_->status);
	if (builtin_ret == -2)
	{
		if (_Info_->error_numb == -1)
			exit(_Info_->status);
		exit(_Info_->error_numb);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @_Info_: the parameter & return _Info_ struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_builtin(info_t *_Info_)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(_Info_->argv[0], builtintbl[i].type) == 0)
		{
			_Info_->count_line++;
			built_in_ret = builtintbl[i].func(_Info_);
			break;
		}
	return (built_in_ret);
}

/**
 * fnd_cmd - finds a command in PATH
 * @_Info_: the parameter & return _Info_ struct
 *
 * Return: void
 */
void fnd_cmd(info_t *_Info_)
{
	char *d_path = NULL;
	int i, k;

	_Info_->d_path = _Info_->argv[0];
	if (_Info_->linecount_flag == 1)
	{
		_Info_->count_line++;
		_Info_->linecount_flag = 0;
	}
	for (i = 0, k = 0; _Info_->arg[i]; i++)
		if (!is_delimeter(_Info_->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	d_path = fnd_path(_Info_, _getenv(_Info_, "d_path="), _Info_->argv[0]);
	if (d_path)
	{
		_Info_->d_path = d_path;
		fork_cmd(_Info_);
	}
	else
	{
		if ((sh_interactive(_Info_) || _getenv(_Info_, "d_path=")
			|| _Info_->argv[0][0] == '/') && is_cmd(_Info_, _Info_->argv[0]))
			fork_cmd(_Info_);
		else if (*(_Info_->arg) != '\n')
		{
			_Info_->status = 127;
			print_err(_Info_, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @_Info_: the parameter & return _Info_ struct
 *
 * Return: void
 */
void fork_cmd(info_t *_Info_)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(_Info_->d_path, _Info_->argv, get_environ(_Info_)) == -1)
		{
			free_info(_Info_, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(_Info_->status));
		if (WIFEXITED(_Info_->status))
		{
			_Info_->status = WEXITSTATUS(_Info_->status);
			if (_Info_->status == 126)
				print_err(_Info_, "Permission denied\n");
		}
	}
}
