/************************************************************************
 * Copyright(c) 2010, One Unified. All rights reserved.                 *
 *                                                                      *
 * This file is provided as is WITHOUT ANY WARRANTY                     *
 *  without even the implied warranty of                                *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                *
 *                                                                      *
 * This software may not be used nor distributed without proper license *
 * agreement.                                                           *
 *                                                                      *
 * See the file LICENSE.txt for redistribution information.             *
 ************************************************************************/

#include "StdAfx.h"
#include "TradingDb.h"

#include <stdexcept>
#include <cassert>

namespace ou { // One Unified
namespace tf { // TradeFrame

CTradingDb::CTradingDb( const char* szDbFileName ) 
//: CDbSession( szDbFileName )
{
}

CTradingDb::~CTradingDb(void) {
}

} // namespace tf
} // namespace ou
