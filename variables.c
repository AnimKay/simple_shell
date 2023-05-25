#include "shell.h"

/**
 * is_chain - check if current character in buffer is a chain delimeter
 * @info: the parameter structure
 * @buf: the character buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter and 0 if not chain delimeter
 */
int is_chain(info_t *info, char *buf, size_t *p)
{
	size_t i = *p;

	if (buf[i] == '|' && buf[i + 1] == '|')
	{
		buf[i] = 0;
		i++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buf[i] == '&' && buf[i + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buf[i] == ';') /* found end of this command */
	{
		buf[i] = 0; /* replace semicolon with null */
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = i;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: nothing
 */
void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t a = *p;

	if (info->cmd_buf_type == CMD_AND)
	{
		if (info->status)
		{
			buf[i] = 0;
			a = len;
		}
	}
	if (info->cmd_buf_type == CMD_OR)
	{
		if (!info->status)
		{
			buf[i] = 0;
			a = len;
		}
	}

	*p = a;
}

/**
 * replace_alias - replaces an aliases in the indexed string
 * @info: the parameter struct
 *
 * Return: 1 if replaced and 0 if not replaced
 */
int replace_alias(info_t *info)
{
	int ch;
	list_t *node;
	char *a;

	for (ch = 0; ch < 10; ch++)
	{
		node = node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		a = _strchr(node->str, '=');
		if (!a)
			return (0);
		a = _strdup(a + 1);
		if (!a)
			return (0);
		info->argv[0] = a;
	}
	return (1);
}

/**
 * replace_vars - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(info_t *info)
{
	int k = 0;
	list_t *node;

	for (k = 0; info->argv[k]; k++)
	{
		if (info->argv[k][0] != '$' || !info->argv[k][1])
			continue;

		if (!_strcmp(info->argv[k], "$?"))
		{
			replace_string(&(info->argv[k]),
				_strdup(convert_number(info->status, 10, 0)));
			continue;
		}
		if (!_strcmp(info->argv[k], "$$"))
		{
			replace_string(&(info->argv[k]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(info->env, &info->argv[k][1], '=');
		if (node)
		{
			replace_string(&(info->argv[k]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		replace_string(&info->argv[k], _strdup(""));

	}
	return (0);
}

/**
 * replace_string - replace the string with a new one
 * @old: contains address of old string
 * @new: newly created string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
