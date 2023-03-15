/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_path.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 23:59:34 by stissera          #+#    #+#             */
/*   Updated: 2023/03/15 14:11:57 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

bool	ft::test_path(std::string &path)
{
	//Voir si une autre methode fonctionne pour verifier que le fichier existe.
	std::ifstream	file;
	file.open(path);
	if (file.is_open())
	{
		file.close();
		return (true);
	}
	return (false);
}