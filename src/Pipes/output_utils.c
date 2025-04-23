/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:51:50 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/23 10:55:47 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_file_directors(int *output_fd, int *buff_fd)
{
	if (*output_fd != 1)
		close(*output_fd);
	if (*buff_fd != 0)
		close(*buff_fd);
}
