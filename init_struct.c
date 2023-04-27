#include "shell_hd.h"

/**
 * clear_info - initializes info_t struct
 * @_Info_: struct address
 */
void clear_info(info_t *_Info_)
{
	_Info_->arg = NULL;
	_Info_->argv = NULL;
	_Info_->d_path = NULL;
	_Info_->argc = 0;
}

/**
 * set_info - initializes info_t struct
 * @_Info_: struct address
 * @av: argument vector
 */
void set_info(info_t *_Info_, char **av)
{
	int i = 0;

	_Info_->fname = av[0];
	if (_Info_->arg)
	{
		_Info_->argv = strtow(_Info_->arg, " \t");
		if (!_Info_->argv)
		{

			_Info_->argv = malloc(sizeof(char *) * 2);
			if (_Info_->argv)
			{
				_Info_->argv[0] = _strdup(_Info_->arg);
				_Info_->argv[1] = NULL;
			}
		}
		for (i = 0; _Info_->argv && _Info_->argv[i]; i++)
			;
		_Info_->argc = i;

		replace_alias(_Info_);
		replace_vars(_Info_);
	}
}

/**
 * free_info - frees info_t struct fields
 * @_Info_: struct address
 * @all: true if freeing all fields
 */
void free_info(info_t *_Info_, int all)
{
	ffree(_Info_->argv);
	_Info_->argv = NULL;
	_Info_->d_path = NULL;
	if (all)
	{
		if (!_Info_->cmd_bufr)
			free(_Info_->arg);
		if (_Info_->env)
			free_list(&(_Info_->env));
		if (_Info_->history)
			free_list(&(_Info_->history));
		if (_Info_->alias)
			free_list(&(_Info_->alias));
		ffree(_Info_->environ);
			_Info_->environ = NULL;
		bfree((void **)_Info_->cmd_bufr);
		if (_Info_->read_fd > 2)
			close(_Info_->read_fd);
		_putchar(FLUSH_BUF);
	}
}
