#pragma once

#include "Graphics.h"
#include "Vec2.h"

class SpriteCodex
{
public:
	// width and height of all tiles
	static constexpr int tileSize = 16;
	// base color for all tiles
	static constexpr Color baseColor = { 192,192,192 };
	// 16x16 tile sprites assume (192,192,192) background and top left origin
	static void DrawTile0(const Vei2& pos, Graphics& gfx);
	static void DrawTile1(const Vei2& pos, Graphics& gfx);
	static void DrawTile2(const Vei2& pos, Graphics& gfx);
	static void DrawTile3(const Vei2& pos, Graphics& gfx);
	static void DrawTile4(const Vei2& pos, Graphics& gfx);
	static void DrawTile5(const Vei2& pos, Graphics& gfx);
	static void DrawTile6(const Vei2& pos, Graphics& gfx);
	static void DrawTile7(const Vei2& pos, Graphics& gfx);
	static void DrawTile8(const Vei2& pos, Graphics& gfx);
	static void DrawTileButton(const Vei2& pos, Graphics& gfx);
	static void DrawTileCross(const Vei2& pos, Graphics& gfx);
	static void DrawTileFlag(const Vei2& pos, Graphics& gfx);
	static void DrawTileBomb(const Vei2& pos, Graphics& gfx);
	static void DrawTileBombRed(const Vei2& pos, Graphics& gfx);
	// Tile selector function valid input 0-8
	static void DrawTileNumber(const Vei2& pos, int n, Graphics& gfx);
	// Win Screen 254x192 center origin
	static void DrawWin(const Vei2& pos, Graphics& gfx);

	// Text for size selection (center origin)
	//
	static constexpr int sizeselWidth = 240;
	static constexpr int sizeselHeight = 56;
	static void DrawSmall(const Vei2& pos, Graphics& gfx);
	static void DrawMedium(const Vei2& pos, Graphics& gfx);
	static void DrawLarge(const Vei2& pos, Graphics& gfx);
};