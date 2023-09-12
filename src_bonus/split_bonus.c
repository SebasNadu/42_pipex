/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:12:53 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/09/12 11:13:03 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	parse_words(char *str, char delimiter, int *i, int *count)
{
	if (str[*i] == '\'')
	{
		(*i)++;
		while (str[*i] && str[*i] != '\'')
			(*i)++;
		(*count)++;
	}
	else if (str[*i] == '\"')
	{
		(*i)++;
		while (str[*i] && str[*i] != '\"')
			(*i)++;
		(*count)++;
	}
	else
	{
		while (str[*i] && str[*i] != delimiter)
			(*i)++;
		(*count)++;
	}
}

static int	word_count(char *str, char delimiter)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != delimiter)
			parse_words(str, delimiter, &i, &count);
		if (str[i] == '\0')
			break ;
		i++;
	}
	return (count);
}

static char	*get_words(char *str, char delimiter, int *i)
{
	char	*result;
	char	quote_type;

	result = ft_calloc(1, sizeof(char));
	if (str[*i] == '\"' || str[*i] == '\'')
	{
		quote_type = str[*i];
		while (str[++(*i)] && str[*i] != quote_type)
		{
			if (str[*i] == '\\' && str[*i + 1] == quote_type)
				(*i)++;
			result = ft_strjoin(result, ft_substr(str, *i, 1));
		}
	}
	else
	{
		while (str[*i] && str[*i] != delimiter)
		{
			result = ft_strjoin(result, ft_substr(str, *i, 1));
			(*i)++;
		}
	}
	return (result);
}

char	**split_with_quotes(char *str, char delimiter)
{
	char	**result;
	int		i;
	int		j;

	result = ft_calloc((word_count(str, delimiter) + 1),
			sizeof(char *));
	if (!result)
		return (NULL);
	i = -1;
	j = 0;
	while (str[++i])
	{
		if (str[i] && str[i] != delimiter)
		{
			result[j] = get_words(str, delimiter, &i);
			j++;
		}
		if (str[i] == '\0')
			break ;
	}
	return (result);
}
