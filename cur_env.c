#include "shell_hd.h"

/**
 * _myenv - prints the current environment
 * @_Info_: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int _myenv(info_t *_Info_)
{
	print_list_str(_Info_->env);
	return (0);
}

/**
 * _getenv - gets the value of an environ variable
 * @_Info_: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_getenv(info_t *_Info_, const char *name)
{
	list_t *node = _Info_->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenv - Initialize a new environment variable,
 *             or modify an existing one
 * @_Info_: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _mysetenv(info_t *_Info_)
{
	if (_Info_->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(_Info_, _Info_->argv[1], _Info_->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - Remove an environment variable
 * @_Info_: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myunsetenv(info_t *_Info_)
{
	int i;

	if (_Info_->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= _Info_->argc; i++)
		_unsetenv(_Info_, _Info_->argv[i]);

	return (0);
}

/**
 * populate_env_list - populates env linked list
 * @_Info_: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int populate_env_list(info_t *_Info_)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	_Info_->env = node;
	return (0);
}
