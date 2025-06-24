/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aychikhi <aychikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 21:42:59 by ayaarab           #+#    #+#             */
/*   Updated: 2025/06/24 16:38:33 by aychikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*join_paths(char *dir, char *cmd)
{
	char	*tmp;
	char	*path;

	tmp = ft_strjoin(dir, "/");
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}

static char	*check_current_dir(char *cmd)
{
	char	*full;

	full = join_paths(".", cmd);
	if (access(full, X_OK) == 0)
		return (full);
	free(full);
	return (NULL);
}

static char	*search_in_path(char *cmd, t_env *env)
{
	char	**path;
	char	*path_env;
	int		i;
	char	*full;

	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	i = 0;
	path = ft_split(path_env, ':');
	if (!path)
		return (NULL);
	while (path[i])
	{
		full = join_paths(path[i], cmd);
		if (access(full, X_OK) == 0)
		{
			free_2d_arr(path);
			return (full);
		}
		free(full);
		i++;
	}
	free_2d_arr(path);
	return (NULL);
}

char	*get_cmd_path(char *cmd, t_env *env)
{
	char	*path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = search_in_path(cmd, env);
	if (path)
		return (path);
	return (check_current_dir(cmd));
}

void	exec_externals_in_child(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	**envp;

	path = get_cmd_path(cmd->cmd, env);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found ", 2);
		ft_putstr_fd("\n", 2);
		exit(127);
	}
	envp = list_to_env(env);
	execute_child_process(cmd, path, envp);
	free(path);
	free_2d_arr(envp);
}
