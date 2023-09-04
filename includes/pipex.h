/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 11:24:25 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/09/04 12:18:29 by sebasnadu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/includes/libft.h"

# define HERE_DOC_PATH ".pipex_here_doc"
# define INVALID_INPUT_PATH ".pipex_error"
# define URANDOM_PATH ".pipex_urandom"

enum e_pipex_err
{
	END = 1,
	CMD_NOT_FOUND = 0,
	NO_FILE = -1,
	NO_AUTH = -2,
	INV_ARGS = -3,
	NO_MEMORY = -4,
	PIPE_ERR = -5,
	DUP_ERR = -6,
	FORK_ERR = -7,
	NO_PATH = -8,
	CMD_FAIL = -9,
};

typedef enum e_bool
{
	false,
	true
}				t_bool;

typedef struct s_pipex
{
	int		fd_in;
	int		fd_out;
	t_bool	here_doc;
	t_bool	is_urandom;
	t_bool	is_invalid_input;
	char	**cmd_paths;
	char	***cmd_args;
	int		cmd_count;
}				t_pipex;

#endif
