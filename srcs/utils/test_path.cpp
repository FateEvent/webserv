/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_path.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 23:59:34 by stissera          #+#    #+#             */
/*   Updated: 2023/03/31 17:43:47 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

std::ifstream	*ft::test_path(std::string &path)
{
	//Voir si une autre method fonctionne pour verifier que le fichier existe.
	std::ifstream	*file = new std::ifstream(path, std::ios::binary);
	//file.open(path);
	if (file->is_open())
		return (file);
	return (nullptr);
}