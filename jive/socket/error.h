/**
  * @file error.h
  * 
  * @brief Represents errors from socket operations.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 05 Jan 2022
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once


#include "jive/create_exception.h"

namespace jive
{


CREATE_SYSTEM_ERROR(SocketError, std::system_error);
CREATE_EXCEPTION(SocketDisconnected, std::runtime_error);


} // end namespace jive
