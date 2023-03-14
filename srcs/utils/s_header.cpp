/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_header.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 20:50:29 by stissera          #+#    #+#             */
/*   Updated: 2023/03/14 10:36:54 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.h"

s_header::s_header()
{
	this->clear();
}

void	s_header::clear()
{
	Methode.clear();
	Dir.clear();
	Host.clear();
	User_Agent.clear();
	Content_Length = 0;
	Content_Type.clear();
	Content_Encoding.clear();
	Transfer_Encoding.clear();
	Accept.clear();
	Cookie.clear(); // change to a ref of cookie in instance
	time_out = 0;
	Connexion.clear();
	// A voir MISS some directives...
	file.first.clear();
	file.second.clear();
	other.clear(); // Need change to a map<string, string>
}