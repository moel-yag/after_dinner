/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:41:36 by moel-yag          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:36 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			*expand_arg(const char *arg, t_env *env);

volatile sig_atomic_t	*ft_sigint_track(void)
{
	static volatile sig_atomic_t	signal;

	return (&signal);
}

void	ft_sigint_handler(int sig)
{
	(void)sig;
	*ft_sigint_track() = 1;
}

int	ft_getc(FILE *stream)
{
	char	c;

	c = EOF;
	(void)(stream);
	if (*ft_sigint_track())
		return (EOF);
	if (read(STDIN_FILENO, &c, 1) <= 0)
		return (EOF);
	return (c);
}

char	*handle_heredoc(char *delimiter)
{
	bool	flag;

	char *(file), *(line), *(tmp), *(expanded);
	flag = true;
	if (delimiter[0] == '\'' || delimiter[0] == '\"')
	{
		delimiter = remove_outer_quotes(delimiter);
		printf("%s\n", delimiter);
		flag = false;
	}
	(void)(flag);
	file = NULL;
	line = NULL;
	rl_getc_function = ft_getc;
	*ft_sigint_track() = 0;
	signal(SIGINT, ft_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (!line && !*ft_sigint_track())
			ft_putstr_fd(HEREDOC_MSG, 2);
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		if (flag && line)
		{
			// expanded = NULL;
			expanded = expand_arg(line, g_data.env_list);
			if (expanded)
			{
				free(line);
				line = ft_strdup(expanded);
			}
		}
		tmp = ft_strjoin3(file, line, "\n");
		free(file);
		file = tmp;
		free(line);
	}
	return (free(line), rl_getc_function = rl_getc, signal(SIGINT, SIG_IGN)
		, file);
}
