#include "minishell.h"

char	*remove_outer_quotes(const char *str)
{
	size_t	len;
	char	*result;

	if (!str)
		return (NULL);
	len = strlen(str);
	if (len >= 2
		&& ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '"' && str[len - 1] == '"')))
	{
		result = malloc(len - 1);
		if (!result)
			return (NULL);
		memcpy(result, str + 1, len - 2);
		result[len - 2] = '\0';
		return (result);
	}
	return (strdup(str));
}
