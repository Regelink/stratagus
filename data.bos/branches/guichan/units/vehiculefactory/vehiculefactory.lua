--            ____            
--           / __ )____  _____
--          / __  / __ \/ ___/
--         / /_/ / /_/ (__  ) 
--        /_____/\____/____/  
--
--  Invasion - Battle of Survival                  
--   A GPL'd futuristic RTS game
--
--	vehiculefactory.lua	-	Define the vehiculefactory
--
--	(c) Copyright 2001 - 2005 by Fran�ois Beerten, Lutz Sammer and Crestez Leonard
--
--      This program is free software; you can redistribute it and/or modify
--      it under the terms of the GNU General Public License as published by
--      the Free Software Foundation; either version 2 of the License, or
--      (at your option) any later version.
--  
--      This program is distributed in the hope that it will be useful,
--      but WITHOUT ANY WARRANTY; without even the implied warranty of
--      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--      GNU General Public License for more details.
--  
--      You should have received a copy of the GNU General Public License
--      along with this program; if not, write to the Free Software
--      Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
--
--	$Id$

DefineIcon({
	Name = "icon-vfac",
	Size = {46, 38},
	Frame = 0,
	File = GetCurrentLuaPath().."/vehicle_factory_i.png"})

DefineConstruction("construction-vfac", {
	Constructions = {
		{Percent = 0, File = "main", Frame = 1},
		{Percent = 10, File = "main", Frame = 2},
		{Percent = 20, File = "main", Frame = 3},
		{Percent = 30, File = "main", Frame = 4},
		{Percent = 40, File = "main", Frame = 5},
		{Percent = 50, File = "main", Frame = 6},
		{Percent = 60, File = "main", Frame = 7},
		{Percent = 70, File = "main", Frame = 8},
		{Percent = 80, File = "main", Frame = 9},
		{Percent = 90, File = "main", Frame = 9},
	}
    })

DefineAnimations("animations-vfac", {
    Still = {"frame 15", "wait 5", "frame 16", "wait 5", "frame 17", "wait 5", "frame 18", "wait 5", "frame 19", 
        "wait 100", "frame 18", "wait 5", "frame 17", "wait 5", "frame 16", "wait 5", "frame 15", "wait 300", },
    Train = {"frame 10", "wait 5", "frame 11", "wait 5", "frame 12", "wait 5", "frame 13", "wait 5", "frame 14", 
        "wait 15", },
    })

MakeSound("vfac-selected", GetCurrentLuaPath().."/sfx_vfac.select.wav")
MakeSound("vfac-ready", GetCurrentLuaPath().."/vehicle.factory.completed.wav")
MakeSound("vfac-help", GetCurrentLuaPath().."/vehicle.factory.underattack.wav")
MakeSound("vfac-dead", GetCurrentLuaPath().."/sfx_vfac.die.wav")

DefineUnitType("unit-vfac", {
	Name = "Vehicle Factory",
	Image = {"file", GetCurrentLuaPath().."/vehicle_factory.png", "size", {256, 256}},
	Shadow = {"file", GetCurrentLuaPath().."/vehicle_factory_s.png", "size", {256, 256}},
	Animations = "animations-vfac", Icon = "icon-vfac",
	Costs = {"time", 200, "titanium", 750, "crystal", 100},
	RepairHp = 2, RepairCosts = {"titanium", 2}, Construction = "construction-vfac",
	Speed = 0, HitPoints = 550, DrawLevel = 25, TileSize = {7, 5},
	BoxSize = {220, 156}, SightRange = 2, Armor = 30, BasicDamage = 0,
	PiercingDamage = 0, Missile = "missile-none", Priority = 35,
	AnnoyComputerFactor = 45, Demand = 400, Points = 200,
	ExplodeWhenKilled = "missile-160x128-explosion", Corpse = {"build-dead-body6", 0},
	Type = "land",  Building = true, BuilderOutside = true, VisibleUnderFog = true,
	Sounds = {
		"selected", "vfac-selected",
		"ready", "vfac-ready",
		"help", "vfac-help",
		"dead", "vfac-dead"}
	})

DefineAnimations("animations-elitebuild6", {
    Death = {"unbreakable begin", "wait 1", "frame 30", "wait 2000", 
        "frame 30", "wait 200", "frame 30", "wait 200", "frame 31", "wait 200",
        "frame 31", "wait 200", "frame 31", "wait 1", "unbreakable end", "wait 1", },
    })

DefineUnitType("build-dead-body6", {
	Name = "FactoryCrater",
	Image = {"file", GetCurrentLuaPath().."/vehicle_factory.png", "size", {256, 256}},
	Animations = "animations-elitebuild6", Icon = "icon-cancel",
	Speed = 0, HitPoints = 999, DrawLevel = 10, TileSize = {7, 5},
	BoxSize = {220, 156}, SightRange = 1, BasicDamage = 0,
	PiercingDamage = 0, Missile = "missile-none",
	Priority = 0, Type = "land", Building = true, Vanishes = true
	})

DefineAllow("unit-vfac", "AAAAAAAAAAAAAAAA")
