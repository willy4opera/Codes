#include "shell_hd.h"

/**
 * is_cmd - determines if a file is an executable command
 * @_Info_: the _Info_ struct
 * @d_path: d_path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_cmd(info_t *_Info_, char *d_path)
{
	struct stat st;

	(void)_Info_;
	if (!d_path || stat(d_path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dup_chars - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *dup_chars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
	buf[k] = 0;
	return (buf);
}

/**
 * fnd_path - finds this cmd in the PATH string
 * @_Info_: the _Info_ struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *fnd_path(info_t *_Info_, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *d_path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (is_cmd(_Info_, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			d_path = dup_chars(pathstr, curr_pos, i);
			if (!*d_path)
				_strcat(d_path, cmd);
			else
			{
				_strcat(d_path, "/");
				_strcat(d_path, cmd);
			}
			if (is_cmd(_Info_, d_path))
				return (d_path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
