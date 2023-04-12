/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_clt_data.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 00:41:52 by stissera          #+#    #+#             */
/*   Updated: 2023/04/12 10:09:22 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/common.h"

s_clt_data::s_clt_data()
{
	file = 0;
	minus_header = 0;
	header.clear();
	data_size = 0;
	data_sended = 1;
	wsatus = 0;
	wpid = -1;
}