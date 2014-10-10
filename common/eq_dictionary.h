/*
EQEMu:  Everquest Server Emulator

Copyright (C) 2001-2014 EQEMu Development Team (http://eqemulator.net)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY except by those people which sell it, which
are required to give you total support for your newly bought product;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef EQ_DICTIONARY_H
#define EQ_DICTIONARY_H

#include "types.h"
#include "eq_constants.h"
#include "clientversions.h"
#include <string>
#include "../common/patches/client62_constants.h"
#include "../common/patches/titanium_constants.h"
#include "../common/patches/sof_constants.h"
#include "../common/patches/sod_constants.h"
#include "../common/patches/underfoot_constants.h"
#include "../common/patches/rof_constants.h"
#include "../common/patches/rof2_constants.h"

// *** DO NOT CHANGE without a full understanding of the consequences..the server is set up to use these settings explicitly!! ***
// *** You will cause compilation failures and corrupt your database if partial or incorrect attempts to change them are made!! ***

// Hard-coded values usually indicate that further research is needed and the values given are from the old (known) system

using namespace RoF2::maps;		// server inventory maps enumeration (code and database sync'd to reference)
using namespace RoF2::slots;	// server possessions slots enumeration (code and database sync'd to reference)

class EmuConstants {
	// an immutable value is required to initialize arrays, etc... use this class as a repository for those
public:
	// database
	static const EQClientVersion CHARACTER_CREATION_CLIENT = EQClientRoF; // adjust according to starting item placement and target client

	// inventory
	static uint16 InventoryMapSize(int16 map);
	//static std::string InventoryLocationName(Location_Struct location);
	static std::string InventoryMapName(int16 map);
	static std::string InventoryMainName(int16 main);
	static std::string InventorySubName(int16 sub);
	static std::string InventoryAugName(int16 aug);

	// these are currently hard-coded for existing inventory system..do not use in place of special client version handlers until ready
	static const uint16	MAP_POSSESSIONS_SIZE = RoF2::consts::MAP_POSSESSIONS_SIZE;
	static const uint16 MAP_BANK_SIZE = RoF2::consts::MAP_BANK_SIZE;
	static const uint16 MAP_SHARED_BANK_SIZE = RoF2::consts::MAP_SHARED_BANK_SIZE;
	static const uint16 MAP_TRADE_SIZE = RoF2::consts::MAP_TRADE_SIZE;
	static const uint16 MAP_WORLD_SIZE = RoF2::consts::MAP_WORLD_SIZE;
	static const uint16 MAP_LIMBO_SIZE = RoF2::consts::MAP_LIMBO_SIZE;
	static const uint16 MAP_TRIBUTE_SIZE = Titanium::consts::MAP_TRIBUTE_SIZE; // server is setup for 'presumed' Titanium value of 5..if Titanium::consts is changed, hard-code this to '5' until server/db is updated
	static const uint16 MAP_TROPHY_TRIBUTE_SIZE = 0;
	static const uint16 MAP_GUILD_TRIBUTE_SIZE = 0;
	static const uint16 MAP_MERCHANT_SIZE = 0;
	static const uint16 MAP_DELETED_SIZE = 0;
	static const uint16 MAP_CORPSE_SIZE = RoF2::consts::MAP_CORPSE_SIZE; // no bitmask use..limits to size of client corpse window (see EQLimits::InventoryMapSize(MapCorpse, <EQClientVersion))
	static const uint16 MAP_BAZAAR_SIZE = Titanium::consts::MAP_BAZAAR_SIZE;
	static const uint16 MAP_INSPECT_SIZE = RoF2::consts::MAP_INSPECT_SIZE;
	static const uint16 MAP_REAL_ESTATE_SIZE = 0;
	static const uint16 MAP_VIEW_MOD_PC_SIZE = NOT_USED;
	static const uint16 MAP_VIEW_MOD_BANK_SIZE = NOT_USED;
	static const uint16 MAP_VIEW_MOD_SHARED_BANK_SIZE = NOT_USED;
	static const uint16 MAP_VIEW_MOD_LIMBO_SIZE = NOT_USED;
	static const uint16 MAP_ALT_STORAGE_SIZE = 0;
	static const uint16 MAP_ARCHIVED_SIZE = 0;
	static const uint16 MAP_MAIL_SIZE = 0;
	static const uint16 MAP_GUILD_TROPHY_TRIBUTE_SIZE = 0;
	static const uint16 MAP_KRONO_SIZE = RoF::consts::MAP_KRONO_SIZE;
	static const uint16 MAP_OTHER_SIZE = 0;

	// most of these definitions will go away with the structure-based system..this maintains compatibility for now
	// (these are mainly to assign specific values to constants used in conversions and to identify per-client ranges/offsets)
	static const int16 EQUIPMENT_BEGIN = RoF2::consts::EQUIPMENT_BEGIN;
	static const int16 EQUIPMENT_END = RoF2::consts::EQUIPMENT_END;
	static const uint16 EQUIPMENT_SIZE = RoF2::consts::EQUIPMENT_SIZE;

	static const int16 GENERAL_BEGIN = RoF2::consts::GENERAL_BEGIN;
	static const int16 GENERAL_END = RoF2::consts::GENERAL_END;
	static const uint16 GENERAL_SIZE = RoF2::consts::GENERAL_SIZE;
	static const int16 GENERAL_BAGS_BEGIN = RoF2::consts::GENERAL_BAGS_BEGIN;
	static const int16 GENERAL_BAGS_END_OFFSET = RoF2::consts::GENERAL_BAGS_END_OFFSET;
	static const int16 GENERAL_BAGS_END = RoF2::consts::GENERAL_BAGS_END;

	static const int16 CURSOR_BAG_BEGIN = RoF2::consts::CURSOR_BAG_BEGIN;
	static const int16 CURSOR_BAG_END_OFFSET = RoF2::consts::CURSOR_BAG_END_OFFSET;
	static const int16 CURSOR_BAG_END = RoF2::consts::CURSOR_BAG_END;

	static const int16 BANK_BEGIN = RoF2::consts::BANK_BEGIN;
	static const int16 BANK_END = RoF2::consts::BANK_END;
	static const int16 BANK_BAGS_BEGIN = RoF2::consts::BANK_BAGS_BEGIN;
	static const int16 BANK_BAGS_END_OFFSET = RoF2::consts::BANK_BAGS_END_OFFSET;
	static const int16 BANK_BAGS_END = RoF2::consts::BANK_BAGS_END;

	static const int16 SHARED_BANK_BEGIN = RoF2::consts::SHARED_BANK_BEGIN;
	static const int16 SHARED_BANK_END = RoF2::consts::SHARED_BANK_END;
	static const int16 SHARED_BANK_BAGS_BEGIN = RoF2::consts::SHARED_BANK_BAGS_BEGIN;
	static const int16 SHARED_BANK_BAGS_END_OFFSET = RoF2::consts::SHARED_BANK_BAGS_END_OFFSET;
	static const int16 SHARED_BANK_BAGS_END = RoF2::consts::SHARED_BANK_BAGS_END;

	static const int16 TRADE_BEGIN = RoF2::consts::TRADE_BEGIN;
	static const int16 TRADE_END = RoF2::consts::TRADE_END;
	static const int16 TRADE_NPC_END = RoF2::consts::TRADE_NPC_END;
	static const int16 TRADE_NPC_SIZE = RoF2::consts::TRADE_NPC_SIZE;
	static const int16 TRADE_BAGS_BEGIN = RoF2::consts::TRADE_BAGS_BEGIN;
	static const int16 TRADE_BAGS_END_OFFSET = RoF2::consts::TRADE_BAGS_END_OFFSET;
	static const int16 TRADE_BAGS_END = RoF2::consts::TRADE_BAGS_END;

	static const int16 WORLD_BEGIN = RoF2::consts::WORLD_BEGIN;
	static const int16 WORLD_END = RoF2::consts::WORLD_END;
	static const int16 WORLD_SIZE = MAP_WORLD_SIZE;

	static const int16 TRIBUTE_BEGIN = RoF2::consts::TRIBUTE_BEGIN;
	static const int16 TRIBUTE_END = RoF2::consts::TRIBUTE_END;
	static const int16 TRIBUTE_SIZE = MAP_TRIBUTE_SIZE;

	static const int16 CORPSE_BEGIN = RoF2::consts::CORPSE_BEGIN;
	static const int16 CORPSE_END = RoF2::consts::CORPSE_END;

	static const int16 MATERIAL_BEGIN = Underfoot::consts::MATERIAL_BEGIN;
	static const int16 MATERIAL_END = Underfoot::consts::MATERIAL_END;
	static const int16 MATERIAL_TINT_END = Underfoot::consts::MATERIAL_TINT_END;
	static const int16 MATERIAL_SIZE = Underfoot::consts::MATERIAL_SIZE;

	// items
	// common and container sizes will not increase until the new 'location' struct is implemented
	static const uint16 ITEM_COMMON_SIZE = Underfoot::consts::ITEM_COMMON_SIZE;
	static const uint16 ITEM_CONTAINER_SIZE = Underfoot::consts::ITEM_CONTAINER_SIZE;

	// player profile
	//static const uint32 CLASS_BITMASK = 0;	// needs value
	//static const uint32 RACE_BITMASK = 0;	// needs value

	// BANDOLIERS_COUNT sets maximum limit..active limit will need to be handled by the appropriate AA
	static const uint32 BANDOLIERS_COUNT = Titanium::consts::BANDOLIERS_COUNT;	// count = number of bandolier instances
	static const uint32 BANDOLIER_SIZE = Titanium::consts::BANDOLIER_SIZE;		// size = number of equipment slots in bandolier instance
	static const uint32 POTION_BELT_SIZE = Titanium::consts::POTION_BELT_SIZE;

	// legacy-related functions
	//static int ServerToPerlSlot(int slot);	// encode
	//static int PerlToServerSlot(int slot);	// decode
};

class EQLimits {
	// values should default to a non-beneficial value..unless value conflicts with intended operation
	//
	// EmuConstants may be used as references..but, not every reference needs to be in EmuConstants (i.e., AllowsEmptyBagInBag(), CoinHasWeight(), etc...)
public:
	// client version validation (checks to avoid crashing zone server when accessing reference arrays)
	// use this inside of class Client (limits to actual clients)
	static bool				IsValidClientVersion(uint32 version);
	static uint32			ValidateClientVersion(uint32 version);
	static EQClientVersion	ValidateClientVersion(EQClientVersion version);

	// basically..any non-client classes - do not when setting a valid client
	static bool				IsValidNPCVersion(uint32 version);
	static uint32			ValidateNPCVersion(uint32 version);
	static EQClientVersion	ValidateNPCVersion(EQClientVersion version);

	// these are 'universal' - do not when setting a valid client
	static bool				IsValidMobVersion(uint32 version);
	static uint32			ValidateMobVersion(uint32 version);
	static EQClientVersion	ValidateMobVersion(EQClientVersion version);

	// inventory
	static uint16	InventoryMapSize(int16 map, uint32 version);
	static uint64	PossessionsBitmask(uint32 version);
	static uint64	EquipmentBitmask(uint32 version);
	static uint64	GeneralBitmask(uint32 version);
	static uint64	CursorBitmask(uint32 version);

	static bool	AllowsEmptyBagInBag(uint32 version);
	static bool AllowsClickCastFromBag(uint32 version);

	// items
	static uint16	ItemCommonSize(uint32 version);
	static uint16	ItemContainerSize(uint32 version);

	// player profile
	static bool	CoinHasWeight(uint32 version);

	static uint32	BandoliersCount(uint32 version);
	static uint32	BandolierSize(uint32 version);

	static uint32	PotionBeltSize(uint32 version);
};

#endif /* EQ_LIMITS_H */

/*
Working Notes:
--------------

- full review of client_packet.cpp and client translators needed


*/
