#include "shell_hd.h"

/**
 * _myhistory - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @_Info_: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myhistory(info_t *_Info_)
{
	print_list(_Info_->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @_Info_: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(info_t *_Info_, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(_Info_->alias),
		get_node_index(_Info_->alias, node_starts_with(_Info_->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @_Info_: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t *_Info_, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(_Info_, str));

	unset_alias(_Info_, str);
	return (add_node_end(&(_Info_->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - mimics the alias builtin (man alias)
 * @_Info_: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myalias(info_t *_Info_)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (_Info_->argc == 1)
	{
		node = _Info_->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; _Info_->argv[i]; i++)
	{
		p = _strchr(_Info_->argv[i], '=');
		if (p)
			set_alias(_Info_, _Info_->argv[i]);
		else
			print_alias(node_starts_with(_Info_->alias, _Info_->argv[i], '='));
	}

	return (0);
}
