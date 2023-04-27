#include "shell_hd.h"

/**
 * _myexit - exits the shell
 * @_Info_: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if _Info_.argv[0] != "exit"
 */
int _myexit(info_t *_Info_)
{
	int exitcheck;

	if (_Info_->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = _erratoi(_Info_->argv[1]);
		if (exitcheck == -1)
		{
			_Info_->status = 2;
			print_err(_Info_, "Illegal number: ");
			_eputs(_Info_->argv[1]);
			_eputchar('\n');
			return (1);
		}
		_Info_->error_numb = _erratoi(_Info_->argv[1]);
		return (-2);
	}
	_Info_->error_numb = -1;
	return (-2);
}

/**
 * _mycd - changes the current directory of the process
 * @_Info_: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _mycd(info_t *_Info_)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!_Info_->argv[1])
	{
		dir = _getenv(_Info_, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _getenv(_Info_, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(_Info_->argv[1], "-") == 0)
	{
		if (!_getenv(_Info_, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(_Info_, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _getenv(_Info_, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(_Info_->argv[1]);
	if (chdir_ret == -1)
	{
		print_err(_Info_, "can't cd to ");
		_eputs(_Info_->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(_Info_, "OLDPWD", _getenv(_Info_, "PWD="));
		_setenv(_Info_, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes the current directory of the process
 * @_Info_: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myhelp(info_t *_Info_)
{
	char **arg_array;

	arg_array = _Info_->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
