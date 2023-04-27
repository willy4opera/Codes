#include "shell_hd.h"

/**
 * get_environ - returns the string array copy of our environ
 * @_Info_: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_environ(info_t *_Info_)
{
	if (!_Info_->environ || _Info_->env_changed)
	{
		_Info_->environ = list_to_strings(_Info_->env);
		_Info_->env_changed = 0;
	}

	return (_Info_->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @_Info_: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenv(info_t *_Info_, char *var)
{
	list_t *node = _Info_->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			_Info_->env_changed = delete_node_at_index(&(_Info_->env), i);
			i = 0;
			node = _Info_->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (_Info_->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @_Info_: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setenv(info_t *_Info_, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = _Info_->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			_Info_->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(_Info_->env), buf, 0);
	free(buf);
	_Info_->env_changed = 1;
	return (0);
}
