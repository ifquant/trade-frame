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

#include <string>
#include <iostream>

namespace ou { // One Unified
namespace tf { // TradeFrame

#include <LIbCommon/FastDelegate.h>
using namespace fastdelegate;

#include "HDF5DataManager.h"

// called from IterateCallback (which is called as HDF5 iterates the directory
// this class is called recursively as the group hierarchy is traversed
// class T needs to supply method: Process( sObjectName, sObjectPath );
// example usage in SymbolSelectionFilter

/*
  string sBaseGroup = "/bar/86400/";
  HDF5IterateGroups<CSymbolSelectionFilter> control;
  int result = control.Start( sBaseGroup, this );
  */

class HDF5IterateGroups {
public:

  typedef FastDelegate2<const std::string&,const std::string&> OnObjectHandler_t;  // objectpath, objectname

  HDF5IterateGroups( void ) {};

  void SetOnHandleObject( OnObjectHandler_t handler ) {
    HandleObject = handler;;
  }
  void SetOnHandleGroup( OnObjectHandler_t handler ) {
    HandleGroup = handler;
  }

  int Start( const std::string &sBaseGroup ) {
    CHDF5DataManager dm;
    m_sBaseGroup = sBaseGroup;
    int idx = 0;  // starting location for interrupted queries
    int result = dm.GetH5File()->iterateElems( sBaseGroup, &idx, &HDF5IterateCallback, this );  
    return result;
  }

  void Process( const std::string &sObjectName ) {
    CHDF5DataManager dm;
    std::string sObjectPath;
    if ( '/' == m_sBaseGroup[ m_sBaseGroup.size() - 1 ] ) {
      sObjectPath = m_sBaseGroup + sObjectName;
    }
    else {
      sObjectPath = m_sBaseGroup + '/' + sObjectName;
    }
    H5G_stat_t stats;
    try {
      dm.GetH5File()->getObjinfo( sObjectPath, stats );
      switch ( stats.type ) {
        case H5G_DATASET: 
          try {
            if ( NULL != HandleObject ) HandleObject( sObjectPath, sObjectName );
          }
          catch ( std::exception e ) {
            std::cout << "CFilterSelectionIteratorControl::Process Object " << sObjectName << " problem: " << e.what() << std::endl;
          }
          catch (...) {
            std::cout << "CFilterSelectionIteratorControl::Process Object " << sObjectName << " unknown problems" << std::endl;
          }
          break;
        case H5G_GROUP: {
            int idx = 0;  // starting location for interrupted queries
            sObjectPath.append( "/" );
            if ( NULL != HandleGroup ) HandleGroup( sObjectPath, sObjectName );
            HDF5IterateGroups control;  // recursive call
            control.SetOnHandleObject( HandleObject );
            control.SetOnHandleGroup( HandleGroup );
            int result = control.Start( sObjectPath );
          }
          break;
        default:
          break;
      }
    }
    catch ( H5::Exception e ) {
      std::cout << "CFilterSelectionIteratorControl::Process H5::Exception " << e.getDetailMsg() << std::endl;
      e.walkErrorStack( H5E_WALK_DOWNWARD, (H5E_walk2_t) &CHDF5DataManager::PrintH5ErrorStackItem, 0 );
    }
  }
protected:
  std::string m_sBaseGroup;
private:
  OnObjectHandler_t HandleObject;
  OnObjectHandler_t HandleGroup;

  static herr_t HDF5IterateCallback( hid_t group, const char *name, void *op_data ) {
    HDF5IterateGroups *pControl = ( HDF5IterateGroups *) op_data;
    pControl->Process( name );
  return 0;  
  }
};

} // namespace tf
} // namespace ou
