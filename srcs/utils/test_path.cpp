/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_path.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 23:59:34 by stissera          #+#    #+#             */
/*   Updated: 2023/03/25 01:03:38 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

std::ifstream	*ft::test_path(std::string &path)
{
	//Voir si une autre methode fonctionne pour verifier que le fichier existe.
	std::ifstream	*file = new std::ifstream(path, std::ios::binary);
	//file.open(path);
	if (file->is_open())
		return (file);
	return (nullptr);
}