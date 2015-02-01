/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _DAMAGE_TYPES_HPP_
#define _DAMAGE_TYPES_HPP_

#include "../Util/EnumIdMap.hpp"
#include "Hitpoints.hpp"
#include <limits>

enum EDamageType {
  DMG_NONE	= 0,
  DMG_WORLD	= 1,
  DMG_SWORD   = 2,
  DMG_ARROW   = 4,
  DMG_HAMMER  = 8,
  DMG_BOMB    = 16,
  DMG_FIRE    = 32,

  DMG_RUN     = 64,

  DMG_FLYING_OBJECT = 128,            //!< Bush, stone, ... that has been thrown

  DMG_ALL     = std::numeric_limits<int32_t>::max(),                  //!< Flag for take/block all damage types
};

typedef int32_t DamageTypeMask;

class CDamageTypeIdMap : public CStringListEnumIdMap<CDamageTypeIdMap, int32_t> {
public:
  void init();
};

struct SDamageData {
  Hitpoints defaultDamage;
};

class CDamageDataMap : public CEnumIdMap<CDamageDataMap, EDamageType, SDamageData> {
public:
  void init();
};

#endif // _DAMAGE_TYPES_HPP_
