//       _________ __                 __
//      /   _____//  |_____________ _/  |______     ____  __ __  ______
//      \_____  \\   __\_  __ \__  \\   __\__  \   / ___\|  |  \/  ___/
//      /        \|  |  |  | \// __ \|  |  / __ \_/ /_/  >  |  /\___ |
//     /_______  /|__|  |__|  (____  /__| (____  /\___  /|____//____  >
//             \/                  \/          \//_____/            \/
//  ______________________                           ______________________
//                        T H E   W A R   B E G I N S
//         Stratagus - A free fantasy real time strategy game engine
//
/**@name animation_spawnmissile.cpp - The animation SpawnMissile. */
//
//      (c) Copyright 2012 by Joris Dauphin
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; only version 2 of the License.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//      02111-1307, USA.
//

//@{

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "stratagus.h"

#include "animation/animation_spawnmissile.h"

#include "actions.h"
#include "animation.h"
#include "map.h"
#include "missile.h"
#include "unit.h"

//SpawnMissile flags
#define ANIM_SM_DAMAGE 1
#define ANIM_SM_TOTARGET 2
#define ANIM_SM_PIXEL 4
#define ANIM_SM_RELTARGET 8
#define ANIM_SM_RANGED 16

/**
**  Parse flags list in animation frame.
**
**  @param unit       Unit of the animation.
**  @param parseflag  Flag list to parse.
**
**  @return The parsed value.
*/
static int ParseAnimFlags(CUnit &unit, const char *parseflag)
{
	char s[100];
	int flags = 0;

	strcpy(s, parseflag);
	char* cur = s;
	char* next = s;
	while (next){
		next = strchr(cur, '.');
		if (next){
			*next = '\0';
			++next;
		}
		if (unit.Anim.Anim->Type == AnimationSpawnMissile) {
			if (!strcmp(cur, "damage")) {
				flags |= ANIM_SM_DAMAGE;
			} else if (!strcmp(cur, "totarget")) {
				flags |= ANIM_SM_TOTARGET;
			} else if (!strcmp(cur, "pixel")) {
				flags |= ANIM_SM_PIXEL;
			} else if (!strcmp(cur, "reltarget")) {
				flags |= ANIM_SM_RELTARGET;
			} else if (!strcmp(cur, "ranged")) {
				flags |= ANIM_SM_RANGED;
			}
		}
		cur = next;
	}
	return flags;
}


/* virtual */ void CAnimation_SpawnMissile::Action(CUnit& unit, int &/*move*/, int /*scale*/) const
{
	Assert(unit.Anim.Anim == this);

	const int startx = ParseAnimInt(&unit, this->startX.c_str());
	const int starty = ParseAnimInt(&unit, this->startY.c_str());
	const int destx = ParseAnimInt(&unit, this->destX.c_str());
	const int desty = ParseAnimInt(&unit, this->destY.c_str());
	const int flags = ParseAnimFlags(unit, this->flags.c_str());
	CUnit *goal;
	PixelPos start;
	PixelPos dest;

	if ((flags & ANIM_SM_RELTARGET)) {
		goal = unit.CurrentOrder()->GetGoal();
	} else {
		goal = &unit;
	}
	if (!goal || goal->Destroyed || goal->Removed) {
		return;
	}
	if ((flags & ANIM_SM_PIXEL)) {
		start.x = goal->tilePos.x * PixelTileSize.x + goal->IX + startx;
		start.y = goal->tilePos.y * PixelTileSize.y + goal->IY + starty;
	} else {
		start.x = (goal->tilePos.x + startx) * PixelTileSize.x + PixelTileSize.x / 2;
		start.y = (goal->tilePos.y + starty) * PixelTileSize.y + PixelTileSize.y / 2;
	}
	if ((flags & ANIM_SM_TOTARGET)) {
		CUnit *target = goal->CurrentOrder()->GetGoal();
		Assert(goal->CurrentAction() == UnitActionAttack);
		if (!target  || target->Destroyed || target->Removed) {
			return;
		}
		if (flags & ANIM_SM_PIXEL) {
			dest.x = target->tilePos.x * PixelTileSize.x + target->IX + destx;
			dest.y = target->tilePos.y * PixelTileSize.y + target->IY + desty;
		} else {
			dest.x = (target->tilePos.x + destx) * PixelTileSize.x + target->Type->TileWidth * PixelTileSize.x / 2;
			dest.y = (target->tilePos.y + desty) * PixelTileSize.y + target->Type->TileHeight * PixelTileSize.y / 2;
		}
	} else {
		if ((flags & ANIM_SM_PIXEL)) {
			dest.x = goal->tilePos.x * PixelTileSize.x + goal->IX + destx;
			dest.y = goal->tilePos.y * PixelTileSize.y + goal->IY + desty;
		} else {
			dest.x = (goal->tilePos.x + destx) * PixelTileSize.x + goal->Type->TileWidth * PixelTileSize.x / 2;
			dest.y = (goal->tilePos.y + desty) * PixelTileSize.y + goal->Type->TileHeight * PixelTileSize.y / 2;
		}
	}
	const int dist = goal->MapDistanceTo(dest.x, dest.y);
	if ((flags & ANIM_SM_RANGED) && !(flags & ANIM_SM_PIXEL)
		&& dist > goal->Stats->Variables[ATTACKRANGE_INDEX].Max
		&& dist < goal->Type->MinAttackRange) {
	} else {
		Missile *missile = MakeMissile(*MissileTypeByIdent(this->missile.c_str()), start, dest);
		if (flags & ANIM_SM_DAMAGE) {
			missile->SourceUnit = &unit;
			unit.RefsIncrease();
		}
		if (flags & ANIM_SM_TOTARGET) {
			missile->TargetUnit = goal->CurrentOrder()->GetGoal();
			goal->CurrentOrder()->GetGoal()->RefsIncrease();
		}
	}
}

/* virtual */ void CAnimation_SpawnMissile::Init(const char* s)
{
	// FIXME : Bad const_cast
	char* op2 = const_cast<char*>(s);

	char *next = strchr(op2, ' ');
	if (next) {
		while (*next == ' ') {
			*next++ = '\0';
		}
	}
	this->missile = op2;
	op2 = next;
	next = strchr(op2, ' ');
	if (next) {
		while (*next == ' ') {
			*next++ = '\0';
		}
		this->startX = op2;
	}
	op2 = next;
	next = strchr(op2, ' ');
	if (next) {
		while (*next == ' ') {
			*next++ = '\0';
		}
		this->startY = op2;
	}
	op2 = next;
	next = strchr(op2, ' ');
	if (next) {
		while (*next == ' ') {
			*next++ = '\0';
		}
		this->destX = op2;
	}
	op2 = next;
	next = strchr(op2, ' ');
	if (next) {
		while (*next == ' ') {
			*next++ = '\0';
		}
		this->destY = op2;
	}
	op2 = next;
	if (next) {
		while (*op2 == ' ') {
			++op2;
		}
		this->flags = op2;
	}
}

//@}