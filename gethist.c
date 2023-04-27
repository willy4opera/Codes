#include "shell_hd.h"

/**
 * get_history_file - gets the history file
 * @_Info_: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_history_file(info_t *_Info_)
{
	char *buf, *dir;

	dir = _getenv(_Info_, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(FILE_HST) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, FILE_HST);
	return (buf);
}

/**
 * write_history - creates a file, or appends to an existing file
 * @_Info_: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(info_t *_Info_)
{
	ssize_t fd;
	char *filename = get_history_file(_Info_);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = _Info_->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(FLUSH_BUF, fd);
	close(fd);
	return (1);
}

/**
 * read_history - reads history from file
 * @_Info_: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_history(info_t *_Info_)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(_Info_);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_history_list(_Info_, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(_Info_, buf + last, linecount++);
	free(buf);
	_Info_->count_hist = linecount;
	while (_Info_->count_hist-- >= MAX_HST)
		delete_node_at_index(&(_Info_->history), 0);
	re_num_hist(_Info_);
	return (_Info_->count_hist);
}

/**
 * build_history_list - adds entry to a history linked list
 * @_Info_: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, count_hist
 *
 * Return: Always 0
 */
int build_history_list(info_t *_Info_, char *buf, int linecount)
{
	list_t *node = NULL;

	if (_Info_->history)
		node = _Info_->history;
	add_node_end(&node, buf, linecount);

	if (!_Info_->history)
		_Info_->history = node;
	return (0);
}

/**
 * re_num_hist - renumbers the history linked list after changes
 * @_Info_: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int re_num_hist(info_t *_Info_)
{
	list_t *node = _Info_->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (_Info_->count_hist = i);
}
