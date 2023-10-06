/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:39:34 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/06 13:03:52 by johnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static char	*ft_strjoin_wfree(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (str == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	free(s1);
	free(s2);
	return (str);
}

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
			result = ft_strjoin_wfree(result, ft_substr(str, *i, 1));
		}
	}
	else
	{
		while (str[*i] && str[*i] != delimiter)
		{
			result = ft_strjoin_wfree(result, ft_substr(str, *i, 1));
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
