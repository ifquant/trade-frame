/************************************************************************
 * Copyright(c) 2009, One Unified. All rights reserved.                 *
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

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;
using namespace boost::gregorian;

#include "TradingEnumerations.h"
#include "KeyTypes.h"

namespace ou { // One Unified
namespace tf { // TradeFrame

class CExecution {
public:

  typedef keytypes::idExecution_t idExecution_t;  // used for database updates, need to persist like orderid
  typedef keytypes::idOrder_t idOrder_t;
  typedef boost::shared_ptr<CExecution> pExecution_t;
  typedef const pExecution_t& pExecution_ref;

  struct TableRowDef {
    template<class A>
    void Fields( A& a ) {
      ou::db::Field( a, "executionid", idExecution );
      ou::db::Field( a, "orderid", idOrder );
      ou::db::Field( a, "quantity", nQuantity );
      ou::db::Field( a, "price", dblPrice );
      ou::db::Field( a, "orderside", eOrderSide );
      ou::db::Field( a, "executiontimestamp", dtExecutionTimeStamp );
      ou::db::Field( a, "exchange", sExchange );
      ou::db::Field( a, "exchangeexecutionid", sExchangeExecutionId );

      ou::db::Key( a, "executionid" );
      ou::db::Constraint( a, "orderid", "orders", "orderid" );
    }
    //  "create index if not exists idx_executions_orderid on executions( orderid );",

  idExecution_t idExecution;
  idOrder_t idOrder;
  unsigned long nQuantity;  // quantity executed
  double dblPrice;  // execution price
  OrderSide::enumOrderSide eOrderSide;
  ptime dtExecutionTimeStamp;
  std::string sExchange;
  std::string sExchangeExecutionId;  // unique execution id supplied by provider

  TableRowDef( idExecution_t idExecution_, idOrder_t idOrder_, 
    unsigned long nQuantity_, double dblPrice_, OrderSide::enumOrderSide eOrderSide_,
    std::string sExchange_, std::string sExchangeExecutionId_ )
    : idExecution( idExecution_ ), idOrder( idOrder_ ), nQuantity( nQuantity_ ), 
      dblPrice( dblPrice_ ), eOrderSide( eOrderSide_ ), 
      sExchange( sExchange_ ), sExchangeExecutionId( sExchangeExecutionId_ ) {};
  };

  const static std::string m_sTableName;

  CExecution( const TableRowDef& row ): m_row( row ) {};
  CExecution( 
    idExecution_t idExecution, idOrder_t nOrderId,
    double dblPrice, unsigned long nQuantity, OrderSide::enumOrderSide eOrderSide,
    const std::string& sExchange, const std::string& sExchangeExecutionId );
  ~CExecution(void);

  double GetPrice( void ) const { return m_row.dblPrice; };
  unsigned long GetSize( void ) const { return m_row.nQuantity; };
  OrderSide::enumOrderSide GetOrderSide( void ) const { return m_row.eOrderSide; };
  const std::string& GetExchange( void ) const { return m_row.sExchange; };
  const std::string& GetExchangeExecutionId( void ) const { return m_row.sExchangeExecutionId; };
  ptime GetTimeStamp( void ) const { return m_row.dtExecutionTimeStamp; };

protected:

private:

  TableRowDef m_row;

};

} // namespace tf
} // namespace ou