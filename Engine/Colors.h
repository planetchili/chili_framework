/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Colors.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

class Color
{
public:
	unsigned int dword;
public:
	constexpr Color() : dword()
	{
	}

	constexpr Color(const Color& col)
		:
		dword(col.dword)
	{
	}

	constexpr Color(unsigned int dw)
		:
		dword(dw)
	{
	}

	constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b)
		:
		dword((x << 24u) | (r << 16u) | (g << 8u) | b)
	{
	}

	constexpr Color(unsigned char r, unsigned char g, unsigned char b)
		:
		dword((r << 16u) | (g << 8u) | b)
	{
	}

	constexpr Color(Color col, unsigned char x)
		:
		Color((x << 24u) | col.dword)
	{
	}

	Color& operator =(Color color)
	{
		dword = color.dword;
		return *this;
	}

	constexpr unsigned char GetX() const
	{
		return dword >> 24u;
	}

	constexpr unsigned char GetA() const
	{
		return GetX();
	}

	constexpr unsigned char GetR() const
	{
		return (dword >> 16u) & 0xFFu;
	}

	constexpr unsigned char GetG() const
	{
		return (dword >> 8u) & 0xFFu;
	}

	constexpr unsigned char GetB() const
	{
		return dword & 0xFFu;
	}

	void SetX(unsigned char x)
	{
		dword = (dword & 0xFFFFFFu) | (x << 24u);
	}

	void SetA(unsigned char a)
	{
		SetX(a);
	}

	void SetR(unsigned char r)
	{
		dword = (dword & 0xFF00FFFFu) | (r << 16u);
	}

	void SetG(unsigned char g)
	{
		dword = (dword & 0xFFFF00FFu) | (g << 8u);
	}

	void SetB(unsigned char b)
	{
		dword = (dword & 0xFFFFFF00u) | b;
	}
};

namespace Colors
{
	static constexpr Color MakeRGB(unsigned char r,
	                               unsigned char g,
	                               unsigned char b)
	{
		return (r << 16) | (g << 8) | b;
	}

	namespace Black
	{
		static constexpr Color Black = MakeRGB(0u, 0u, 0u);

		namespace Variations
		{
			static constexpr Color Xiketic = MakeRGB(12u, 2u, 15u);
			static constexpr Color MidnightBlue = MakeRGB(25u, 25u, 112u);
			static constexpr Color DimGray = MakeRGB(105u, 105u, 105u);
			static constexpr Color Ebony = MakeRGB(85u, 93u, 80u);
			static constexpr Color Taupe = MakeRGB(73u, 59u, 59u);
			static constexpr Color GrayofDavy = MakeRGB(1u, 85u, 85u);
			static constexpr Color Charcoal = MakeRGB(54u, 69u, 79u);
			static constexpr Color OuterSpace = MakeRGB(65u, 74u, 76u);
			static constexpr Color CafeNoir = MakeRGB(75u, 54u, 33u);
			static constexpr Color BlackBean = MakeRGB(61u, 12u, 2u);
			static constexpr Color BlackOlive = MakeRGB(59u, 60u, 54u);
			static constexpr Color Onyx = MakeRGB(53u, 56u, 57u);
			static constexpr Color Jet = MakeRGB(52u, 52u, 52u);
			static constexpr Color LeatherJacket = MakeRGB(37u, 53u, 41u);
			static constexpr Color Raisin = MakeRGB(36u, 33u, 36u);
			static constexpr Color CharlestonGreen = MakeRGB(35u, 43u, 43u);
			static constexpr Color Eerie = MakeRGB(27u, 27u, 27u);
			static constexpr Color Licorice = MakeRGB(26u, 17u, 16u);
		} // namespace Variations
	} // namespace Black

	namespace Blue
	{
		static constexpr Color Blue = MakeRGB(0u, 0u, 255u);

		namespace Tints
		{
			static constexpr Color Baby = MakeRGB(137u, 207u, 240u);
			static constexpr Color Light = MakeRGB(173u, 216u, 230u);
			static constexpr Color Periwinkle = MakeRGB(204u, 204u, 255u);
			static constexpr Color Powder = MakeRGB(176u, 224u, 230u);
			static constexpr Color Ice = MakeRGB(153u, 255u, 255u);
			static constexpr Color Morning = MakeRGB(141u, 163u, 153u);
		} // namespace Tints
		namespace Culture
		{
			static constexpr Color Argentinian = MakeRGB(108u, 180u, 238u);
			static constexpr Color Spanish = MakeRGB(0u, 112u, 184u);
			static constexpr Color Egyptian = MakeRGB(16u, 52u, 166u);
			static constexpr Color Polynesian = MakeRGB(34u, 76u, 152u);
		} // namespace Culture
		namespace Shades
		{
			static constexpr Color Uranian = MakeRGB(175u, 216u, 245u);
			static constexpr Color Medium = MakeRGB(0u, 0u, 205u);
			static constexpr Color Ruddy = MakeRGB(118u, 171u, 223u);
			static constexpr Color Liberty = MakeRGB(84u, 90u, 167u);
			static constexpr Color Ultramarine = MakeRGB(64u, 0u, 255u);
			static constexpr Color Neron = MakeRGB(77u, 77u, 255u);
			static constexpr Color Delft = MakeRGB(31u, 48u, 94u);
			static constexpr Color Duck = MakeRGB(0u, 119u, 145u);
			static constexpr Color Dark = MakeRGB(0u, 0u, 139u);
			static constexpr Color Resolution = MakeRGB(0u, 35u, 185u);
			static constexpr Color Navy = MakeRGB(0u, 0u, 128u);
			static constexpr Color Sapphire = MakeRGB(8u, 37u, 103u);
			static constexpr Color Picotee = MakeRGB(46u, 39u, 135u);
			static constexpr Color Independence = MakeRGB(76u, 81u, 109u);
			static constexpr Color SpaceCadet = MakeRGB(29u, 41u, 81u);
			static constexpr Color Fluorescent = MakeRGB(21u, 244u, 238u);
			static constexpr Color Teal = MakeRGB(54u, 117u, 136u);
		} // namespace Shades
	} // namespace Blue

	namespace Brown
	{
		static constexpr Color Brown = MakeRGB(150u, 75u, 0u);

		namespace Web
		{
			static constexpr Color Brown = MakeRGB(165u, 42u, 42u);
		} // namespace Web

		namespace Variations
		{
			static constexpr Color Beaver = MakeRGB(159u, 129u, 112u);
			static constexpr Color Beige = MakeRGB(245u, 245u, 220u);
			static constexpr Color Buff = MakeRGB(240u, 220u, 130u);
			static constexpr Color BurntUmber = MakeRGB(138u, 51u, 36u);
			static constexpr Color Chestnut = MakeRGB(149u, 69u, 53u);
			static constexpr Color Chocolate = MakeRGB(123u, 63u, 0u);
			static constexpr Color Cocoa = MakeRGB(210u, 105u, 30u);
			static constexpr Color DesertSand = MakeRGB(237u, 201u, 175u);
			static constexpr Color Khaki = MakeRGB(195u, 176u, 145u);
			static constexpr Color Kobicha = MakeRGB(107u, 68u, 35u);
			static constexpr Color Peru = MakeRGB(205u, 133u, 63u);
			static constexpr Color RawUmber = MakeRGB(130u, 102u, 68u);
			static constexpr Color Rosy = MakeRGB(188u, 143u, 143u);
			static constexpr Color Russet = MakeRGB(128u, 70u, 27u);
			static constexpr Color Sandy = MakeRGB(244u, 164u, 96u);
			static constexpr Color SmokeyTopaz = MakeRGB(131u, 42u, 34u);
			static constexpr Color Tan = MakeRGB(210u, 180u, 140u);
			static constexpr Color Taupe = MakeRGB(72u, 60u, 50u);
			static constexpr Color Walnut = MakeRGB(92u, 82u, 72u);
			static constexpr Color Wenge = MakeRGB(100u, 84u, 82u);
			static constexpr Color Wood = MakeRGB(193u, 154u, 107u);
		} // namespace Variations
	} // namespace Brown

	namespace Cyan
	{
		static constexpr Color Cyan = MakeRGB(0u, 255u, 255u);

		namespace Printing
		{
			static constexpr Color Cyan = MakeRGB(0u, 183u, 235u);
		}

		namespace Miscellaneous
		{
			static constexpr Color Aqua = MakeRGB(0u, 255u, 255u);
			static constexpr Color AeroBlue = MakeRGB(201u, 255u, 229u);
			static constexpr Color Aquamarine = MakeRGB(127u, 255u, 212u);
			static constexpr Color BlueGreen = MakeRGB(13u, 152u, 186u);
			static constexpr Color CaribbeanCurrent = MakeRGB(0u, 109u, 111u);
			static constexpr Color Celeste = MakeRGB(178u, 255u, 255u);
			static constexpr Color Cerulean = MakeRGB(0u, 123u, 167u);
			static constexpr Color CharlestonGreen = MakeRGB(35u, 43u, 43u);
			static constexpr Color DarkCyan = MakeRGB(0u, 139u, 139u);
			static constexpr Color ElectricBlue = MakeRGB(125u, 249u, 255u);
			static constexpr Color GreenBlue = MakeRGB(17u, 100u, 180u);
			static constexpr Color Keppel = MakeRGB(58u, 176u, 158u);
			static constexpr Color LightCyan = MakeRGB(224u, 255u, 255u);
			static constexpr Color LightSeaGreen = MakeRGB(32u, 178u, 170u);
			static constexpr Color MidnightGreen = MakeRGB(0u, 76u, 84u);
			static constexpr Color PaleAqua = MakeRGB(190u, 211u, 229u);
			static constexpr Color PaleCyan = MakeRGB(135u, 211u, 248u);
			static constexpr Color RobinEggBlue = MakeRGB(0u, 204u, 204u);
			static constexpr Color SkyBlue = MakeRGB(128u, 218u, 235u);
			static constexpr Color Teal = MakeRGB(0u, 128u, 128u);
			static constexpr Color TiffanyBlue = MakeRGB(129u, 216u, 208u);
			static constexpr Color Turquoise = MakeRGB(64u, 224u, 208u);
			static constexpr Color Verdigris = MakeRGB(67u, 179u, 174u);
			static constexpr Color Viridian = MakeRGB(64u, 130u, 109u);
		} // namespace Miscellaneous
	} // namespace Cyan
	namespace Green
	{
		namespace Biological
		{
			static constexpr Color Artichoke = MakeRGB(143u, 151u, 121u);
			static constexpr Color ArtichokePantone = MakeRGB(75u, 111u, 0u);
			static constexpr Color Asparagus = MakeRGB(135u, 169u, 107u);
			static constexpr Color Avocado = MakeRGB(86u, 130u, 3u);
			static constexpr Color Fern = MakeRGB(79u, 121u, 66u);
			static constexpr Color Forest = MakeRGB(34u, 139u, 34u);
			static constexpr Color Jungle = MakeRGB(41u, 171u, 135u);
			static constexpr Color Laurel = MakeRGB(169u, 186u, 157u);
			static constexpr Color Lime = MakeRGB(192u, 255u, 0u);
			static constexpr Color Mantis = MakeRGB(116u, 195u, 101u);
			static constexpr Color Mint = MakeRGB(62u, 180u, 137u);
			static constexpr Color Moss = MakeRGB(139u, 154u, 91u);
			static constexpr Color DarkMoss = MakeRGB(74u, 93u, 35u);
			static constexpr Color Myrtle = MakeRGB(49u, 120u, 115u);
			static constexpr Color Pine = MakeRGB(1u, 121u, 111u);
			static constexpr Color Reseda = MakeRGB(108u, 124u, 89u);
			static constexpr Color Sap = MakeRGB(80u, 125u, 42u);
			static constexpr Color Irish = MakeRGB(0u, 158u, 96u);
			static constexpr Color Tea = MakeRGB(208u, 240u, 192u);
			static constexpr Color Olive = MakeRGB(128u, 128u, 0u);
		} // namespace Biological

		namespace NonBiological
		{
			static constexpr Color Emerald = MakeRGB(80u, 200u, 120u);
			static constexpr Color GreenEarth = MakeRGB(218u, 221u, 152u);
			static constexpr Color Hooker = MakeRGB(73u, 121u, 107u);
			static constexpr Color Jade = MakeRGB(0u, 168u, 107u);
			static constexpr Color Malachite = MakeRGB(11u, 218u, 81u);
			static constexpr Color Sea = MakeRGB(46u, 139u, 87u);
		} // namespace NonBiological

		namespace AdditionalDefinitions
		{
			static constexpr Color Pigment = MakeRGB(0u, 165u, 80u);
			static constexpr Color Natural = MakeRGB(0u, 159u, 107u);
			static constexpr Color Munsell = MakeRGB(0u, 168u, 119u);
			static constexpr Color Pantone = MakeRGB(0u, 173u, 131u);
			static constexpr Color Crayola = MakeRGB(28u, 172u, 120u);
			static constexpr Color Bright = MakeRGB(102u, 255u, 0u);
			static constexpr Color BrightMint = MakeRGB(79u, 255u, 176u);
			static constexpr Color Dark = MakeRGB(1u, 50u, 32u);
			static constexpr Color DarkPastel = MakeRGB(3u, 192u, 60u);
			static constexpr Color GreenYellow = MakeRGB(173u, 255u, 47u);
			static constexpr Color Harlequin = MakeRGB(63u, 255u, 0u);
			static constexpr Color Neon = MakeRGB(57u, 255u, 20u);
			static constexpr Color Screamin = MakeRGB(118u, 255u, 152u);
		} // namespace AdditionalDefinitions

		namespace NotableShades
		{
			static constexpr Color Army = MakeRGB(75u, 83u, 32u);
			static constexpr Color Bottle = MakeRGB(0u, 106u, 78u);
			static constexpr Color Brunswick = MakeRGB(27u, 77u, 62u);
			static constexpr Color Castleton = MakeRGB(0u, 86u, 59u);
			static constexpr Color Celadon = MakeRGB(172u, 225u, 175u);
			static constexpr Color Dartmouth = MakeRGB(0u, 165u, 80u);
			static constexpr Color Feldgrau = MakeRGB(77u, 93u, 83u);
			static constexpr Color GO = MakeRGB(0u, 171u, 102u);
			static constexpr Color Gotham = MakeRGB(0u, 87u, 63u);
			static constexpr Color Hunter = MakeRGB(53u, 94u, 59u);
			static constexpr Color Indian = MakeRGB(19u, 136u, 8u);
			static constexpr Color Islamic = MakeRGB(0u, 144u, 0u);
			static constexpr Color Kelly = MakeRGB(76u, 187u, 23u);
			static constexpr Color Midnight = MakeRGB(0u, 73u, 83u);
			static constexpr Color MichiganState = MakeRGB(24u, 69u, 59u);
			static constexpr Color Pakistani = MakeRGB(0u, 102u, 0u);
			static constexpr Color Paris = MakeRGB(80u, 200u, 120u);
			static constexpr Color Persian = MakeRGB(0u, 166u, 149u);
			static constexpr Color Rifle = MakeRGB(66u, 76u, 56u);
			static constexpr Color Russian = MakeRGB(103u, 146u, 103u);
			static constexpr Color SacramentoState = MakeRGB(0u, 78u, 56u);
			static constexpr Color SingaporeBus = MakeRGB(85u, 221u, 51u);
			static constexpr Color Spanish = MakeRGB(0u, 145u, 80u);
			static constexpr Color UniversityofNorthTexas = MakeRGB(5u, 144u, 51u);
			static constexpr Color PhilippinesForest = MakeRGB(1u, 68u, 33u);
			static constexpr Color Pigment = MakeRGB(0u, 165u, 80u);
		} // namespace NotableShades
	} // namespace Green

	namespace Gray
	{
		namespace Achromatic
		{
			static constexpr Color Gainsboro = MakeRGB(220u, 220u, 220u);
			static constexpr Color Light = MakeRGB(211u, 211u, 211u);
			static constexpr Color Silver = MakeRGB(192u, 192u, 192u);
			static constexpr Color Medium = MakeRGB(190u, 190u, 190u);
			static constexpr Color Spanish = MakeRGB(152u, 152u, 152u);
			static constexpr Color Dim = MakeRGB(105u, 105u, 105u);
			static constexpr Color Davy = MakeRGB(85u, 85u, 85u);
			static constexpr Color Jet = MakeRGB(52u, 52u, 52u);

			namespace Middle
			{
				static constexpr Color Geomean60to1 = MakeRGB(101u, 101u, 101u);
				static constexpr Color Lab = MakeRGB(119u, 119u, 119u);
				static constexpr Color EighteenPercent = MakeRGB(124u, 124u, 124u);
				static constexpr Color sRGB = MakeRGB(128u, 128u, 128u);
				static constexpr Color Macintosh = MakeRGB(146u, 146u, 146u);
				static constexpr Color AbsoluteWhiteness = MakeRGB(188u, 188u, 188u);
			} // namespace Middle
		} // namespace Achromatic

		namespace OffGrays
		{
			namespace Neutral
			{
				static constexpr Color Xanadu = MakeRGB(115u, 134u, 120u);
				static constexpr Color Platinum = MakeRGB(229u, 228u, 226u);
				static constexpr Color Ash = MakeRGB(178u, 190u, 181u);
				static constexpr Color Battleship = MakeRGB(132u, 132u, 130u);
				static constexpr Color Gunmetal = MakeRGB(42u, 52u, 57u);
				static constexpr Color Nickel = MakeRGB(114u, 116u, 114u);
				static constexpr Color Charcoal = MakeRGB(54u, 69u, 79u);
				static constexpr Color Stone = MakeRGB(146u, 142u, 133u);
			} // namespace Neutral
			namespace Cool
			{
				static constexpr Color Cool = MakeRGB(144u, 144u, 192u);
				static constexpr Color Cadet = MakeRGB(145u, 163u, 176u);
				static constexpr Color BlueGray = MakeRGB(102u, 153u, 204u);
				static constexpr Color Glaucous = MakeRGB(96u, 130u, 182u);
				static constexpr Color Slate = MakeRGB(112u, 128u, 144u);
				static constexpr Color GrayGreen = MakeRGB(94u, 113u, 106u);
				static constexpr Color Marengo = MakeRGB(76u, 88u, 102u);
			} // namespace Cool
			namespace Warm
			{
				static constexpr Color Cinerous = MakeRGB(152u, 129u, 123u);
				static constexpr Color Puce = MakeRGB(204u, 136u, 153u);
				static constexpr Color RoseQuartz = MakeRGB(170u, 152u, 169u);
				static constexpr Color RocketMetallic = MakeRGB(138u, 129u, 141u);
				static constexpr Color Taupe = MakeRGB(72u, 60u, 50u);
			} // namespace Warm
		} // namespace OffGrays
	} // namespace Gray

	namespace Magenta
	{
		static constexpr Color Magenta = MakeRGB(255u, 0u, 255u);

		namespace Historical
		{
			static constexpr Color MagentaDye = MakeRGB(202u, 31u, 123u);
			static constexpr Color Process = MakeRGB(255u, 0u, 144u);
		} // namespace Historical
		namespace Variations
		{
			static constexpr Color Amaranth = MakeRGB(171u, 39u, 79u);
			static constexpr Color Crayola = MakeRGB(246u, 83u, 166u);
			static constexpr Color RazzleDazzleRose = MakeRGB(255u, 51u, 204u);
			static constexpr Color Hot = MakeRGB(255u, 29u, 206u);
			static constexpr Color PurplePizzazz = MakeRGB(254u, 78u, 218u);
			static constexpr Color Orchid = MakeRGB(218u, 112u, 214u);
			static constexpr Color Pantone = MakeRGB(208u, 65u, 126u);
			static constexpr Color Telemagenta = MakeRGB(207u, 52u, 118u);
			static constexpr Color Sky = MakeRGB(207u, 113u, 175u);
			static constexpr Color SteelPink = MakeRGB(204u, 51u, 204u);
			static constexpr Color Haze = MakeRGB(159u, 69u, 118u);
			static constexpr Color Quinacridone = MakeRGB(142u, 59u, 89u);
			static constexpr Color Plum = MakeRGB(142u, 69u, 133u);
			static constexpr Color Dark = MakeRGB(139u, 0u, 139u);
			static constexpr Color Garnet = MakeRGB(115u, 54u, 214u);

			namespace ShockingPink
			{
				static constexpr Color Crayola = MakeRGB(255u, 111u, 255u);
				static constexpr Color Original = MakeRGB(252u, 15u, 192u);
			} // namespace ShockingPink
		} // namespace Variations
	} // namespace Magenta

	namespace Orange
	{
		static constexpr Color Orange = MakeRGB(255u, 127u, 0u);

		namespace Web
		{
			static constexpr Color Orange = MakeRGB(255u, 165u, 0u);
			static constexpr Color Dark = MakeRGB(255u, 140u, 0u);
			static constexpr Color Coral = MakeRGB(255u, 127u, 80u);
		} // namespace Web
		namespace Aerospace
		{
			static constexpr Color Aerospace = MakeRGB(255u, 79u, 0u);
			static constexpr Color Safety = MakeRGB(255u, 121u, 0u);
		} // namespace Aerospace
		namespace Safety
		{
			static constexpr Color Pantone = MakeRGB(255u, 88u, 0u);
			static constexpr Color Crayola = MakeRGB(255u, 117u, 56u);
		} // namespace Safety
		namespace Miscellaneous
		{
			static constexpr Color PapayaWhip = MakeRGB(255u, 239u, 213u);
			static constexpr Color Peach = MakeRGB(255u, 229u, 180u);
			static constexpr Color Light = MakeRGB(254u, 216u, 177u);
			static constexpr Color Apricot = MakeRGB(251u, 206u, 177u);
			static constexpr Color Melon = MakeRGB(253u, 188u, 180u);
			static constexpr Color AtomicTangerine = MakeRGB(253u, 153u, 102u);
			static constexpr Color TeaRose = MakeRGB(248u, 131u, 121u);
			static constexpr Color Carrot = MakeRGB(237u, 145u, 33u);
			static constexpr Color Peel = MakeRGB(255u, 159u, 0u);
			static constexpr Color Princeton = MakeRGB(245u, 128u, 37u);
			static constexpr Color Texas = MakeRGB(255u, 130u, 0u);
			static constexpr Color Spanish = MakeRGB(232u, 97u, 0u);
			static constexpr Color Tangerine = MakeRGB(242u, 133u, 0u);
			static constexpr Color Pumpkin = MakeRGB(255u, 117u, 24u);
			static constexpr Color Saffron = MakeRGB(244u, 196u, 48u);
			static constexpr Color Giants = MakeRGB(254u, 90u, 29u);
			static constexpr Color Vermilion = MakeRGB(227u, 66u, 52u);
			static constexpr Color Tomato = MakeRGB(255u, 99u, 71u);
			static constexpr Color Syracuse = MakeRGB(212u, 69u, 0u);
			static constexpr Color Bittersweet = MakeRGB(254u, 111u, 94u);
			static constexpr Color Persimmon = MakeRGB(236u, 88u, 0u);
			static constexpr Color Butterscotch = MakeRGB(224u, 149u, 64u);
			static constexpr Color PersianOrage = MakeRGB(217u, 144u, 88u);
			static constexpr Color Alloy = MakeRGB(196u, 98u, 16u);
			static constexpr Color Burned = MakeRGB(191u, 87u, 0u);
			static constexpr Color Burnt = MakeRGB(191u, 87u, 0u);
			static constexpr Color BittersweetShimmer = MakeRGB(191u, 79u, 81u);
			static constexpr Color Jasper = MakeRGB(208u, 83u, 64u);
			static constexpr Color EyeoftheTiger = MakeRGB(181u, 105u, 23u);
			static constexpr Color LightCoral = MakeRGB(240u, 128u, 128u);
			static constexpr Color OrangeRed = MakeRGB(255u, 128u, 114u);
		} // namespace AdditionalDefinitions
	} // namespace Orange

	namespace Pink
	{
		static constexpr Color Pink = MakeRGB(255u, 192u, 203u);

		namespace Web
		{
			static constexpr Color Light = MakeRGB(255u, 182u, 193u);
			static constexpr Color Hot = MakeRGB(255u, 105u, 18u);
			static constexpr Color Deep = MakeRGB(255u, 20u, 147u);
		} // namespace Web

		namespace Miscellaneous
		{
			static constexpr Color Champagne = MakeRGB(241u, 221u, 207u);
			static constexpr Color PinkLace = MakeRGB(225u, 221u, 244u);
			static constexpr Color Piggy = MakeRGB(253u, 221u, 230u);
			static constexpr Color Pale = MakeRGB(249u, 204u, 202u);
			static constexpr Color Baby = MakeRGB(244u, 194u, 194u);
			static constexpr Color Spanish = MakeRGB(247u, 191u, 190u);
			static constexpr Color Cameo = MakeRGB(239u, 187u, 204u);
			static constexpr Color Orchid = MakeRGB(242u, 189u, 205u);
			static constexpr Color FairyTale = MakeRGB(242u, 193u, 209u);
			static constexpr Color Sakura = MakeRGB(255u, 183u, 197u);
			static constexpr Color LightHot = MakeRGB(255u, 179u, 222u);
			static constexpr Color Lavender = MakeRGB(252u, 180u, 213u);
			static constexpr Color CottonCandy = MakeRGB(255u, 188u, 217u);
			static constexpr Color CarnationPink = MakeRGB(255u, 166u, 201u);
			static constexpr Color BakerMiller = MakeRGB(255u, 145u, 175u);
			static constexpr Color TickleMe = MakeRGB(252u, 137u, 172u);
			static constexpr Color Amaranth = MakeRGB(241u, 156u, 187u);
			static constexpr Color Charm = MakeRGB(230u, 143u, 172u);
			static constexpr Color Chinese = MakeRGB(222u, 111u, 161u);
			static constexpr Color Mimi = MakeRGB(255u, 218u, 233u);
			static constexpr Color Tango = MakeRGB(228u, 113u, 122u);
			static constexpr Color Congolese = MakeRGB(248u, 131u, 121u);
			static constexpr Color Pastel = MakeRGB(222u, 165u, 164u);
			static constexpr Color NewYork = MakeRGB(215u, 131u, 127u);
			static constexpr Color Solid = MakeRGB(137u, 56u, 67u);
			static constexpr Color Silver = MakeRGB(196u, 174u, 173u);
			static constexpr Color Queen = MakeRGB(232u, 204u, 215u);
			static constexpr Color PinkLavender = MakeRGB(219u, 178u, 209u);
			static constexpr Color Mountbatten = MakeRGB(153u, 122u, 141u);
			static constexpr Color Chilean = MakeRGB(232u, 195u, 186u);
			static constexpr Color PaleDogwood = MakeRGB(237u, 205u, 194u);
			static constexpr Color Pantone = MakeRGB(215u, 72u, 154u);
			static constexpr Color Mexican = MakeRGB(228u, 0u, 124u);
			static constexpr Color Barbie = MakeRGB(218u, 24u, 132u);
			static constexpr Color Fandango = MakeRGB(222u, 82u, 133u);
			static constexpr Color Paradise = MakeRGB(230u, 62u, 98u);
			static constexpr Color Brink = MakeRGB(251u, 96u, 127u);
			static constexpr Color French = MakeRGB(253u, 108u, 158u);
			static constexpr Color Bright = MakeRGB(255u, 0u, 127u);
			static constexpr Color Persian = MakeRGB(247u, 127u, 190u);
			static constexpr Color Rose = MakeRGB(255u, 102u, 204u);
			static constexpr Color LightDeep = MakeRGB(255u, 92u, 205u);
			static constexpr Color Ultra = MakeRGB(255u, 111u, 255u);
			static constexpr Color Shocking = MakeRGB(252u, 15u, 192u);
			static constexpr Color Super = MakeRGB(207u, 107u, 169u);
			static constexpr Color RosePompadour = MakeRGB(237u, 122u, 155u);
			static constexpr Color Steel = MakeRGB(204u, 51u, 204);
		} // namespace Miscellaneous
	} // namespace Pink

	namespace Red
	{
		static constexpr Color Red = MakeRGB(255u, 255u, 255u);

		namespace Tints
		{
			static constexpr Color Salmon = MakeRGB(250u, 128u, 114u);
			static constexpr Color LightRed = MakeRGB(255u, 0u, 0u);
			static constexpr Color SalmonPink = MakeRGB(255u, 145u, 164u);
			static constexpr Color CoralPink = MakeRGB(248u, 131u, 121u);
		} // namespace Tints

		namespace Definitions
		{
			static constexpr Color Pigment = MakeRGB(237u, 28u, 36u);
			static constexpr Color Natural = MakeRGB(196u, 2u, 51u);
			static constexpr Color Munsell = MakeRGB(242u, 0u, 60u);
			static constexpr Color Pantone = MakeRGB(237u, 41u, 57u);
			static constexpr Color Crayola = MakeRGB(238u, 32u, 77u);
		} // namespace Definitions

		namespace Shades
		{
			static constexpr Color Scarlet = MakeRGB(255u, 36u, 0u);
			static constexpr Color Imperial = MakeRGB(237u, 41u, 57u);
			static constexpr Color Indian = MakeRGB(205u, 92u, 92u);
			static constexpr Color Spanish = MakeRGB(230u, 0u, 38u);
			static constexpr Color Carmine = MakeRGB(150u, 0u, 24u);
			static constexpr Color Ruby = MakeRGB(224u, 17u, 95u);
			static constexpr Color Crimson = MakeRGB(220u, 20u, 60u);
			static constexpr Color AlabamaCrimson = MakeRGB(158u, 27u, 50u);
			static constexpr Color Rusty = MakeRGB(218u, 44u, 67u);
			static constexpr Color FireEngine = MakeRGB(206u, 32u, 41u);
			static constexpr Color Cardinal = MakeRGB(196u, 30u, 58u);
			static constexpr Color Chili = MakeRGB(226u, 61u, 40u);
			static constexpr Color Cornell = MakeRGB(179u, 27u, 27u);
			static constexpr Color Madder = MakeRGB(255u, 0u, 0u);
			static constexpr Color Redwood = MakeRGB(164u, 90u, 82u);
			static constexpr Color OklahomaCrimson = MakeRGB(132u, 22u, 23u);
			static constexpr Color ChocolateCosmos = MakeRGB(88u, 17u, 26u);
			static constexpr Color Dark = MakeRGB(139u, 0u, 0u);
			static constexpr Color WebMaroon = MakeRGB(128u, 0u, 0u);
			static constexpr Color Barn = MakeRGB(124u, 10u, 2u);
			static constexpr Color Turkey = MakeRGB(169u, 17u, 1u);
			static constexpr Color Cinnabar = MakeRGB(228u, 77u, 48u);
			static constexpr Color Blood = MakeRGB(102u, 0u, 0u);
			static constexpr Color Lust = MakeRGB(230u, 32u, 32u);
		} // namespace Shades
	} // namespace Red

	namespace Violet
	{
		namespace Purple
		{
			namespace Historical
			{
				static constexpr Color Tyrian = MakeRGB(127u, 0u, 255u);
				static constexpr Color Han = MakeRGB(127u, 0u, 255u);
				static constexpr Color Royal = MakeRGB(127u, 0u, 255u);
				static constexpr Color Mauveine = MakeRGB(127u, 0u, 255u);
				static constexpr Color RedViolet = MakeRGB(127u, 0u, 255u);
				static constexpr Color ElectricPurple = MakeRGB(127u, 0u, 255u);
			} // namespace Historical
			namespace Web
			{
				static constexpr Color Purple = MakeRGB(128u, 0u, 128u);
				static constexpr Color X11 = MakeRGB(160u, 32u, 240u);
				static constexpr Color Medium = MakeRGB(147u, 112u, 219u);
			} // namespace Web
			namespace Munsell
			{
				static constexpr Color Purple = MakeRGB(159u, 0u, 197u);
			}

			namespace Miscellaneous
			{
				static constexpr Color Pale = MakeRGB(250u, 230u, 250u);
				static constexpr Color Thistle = MakeRGB(216u, 191u, 216u);
				static constexpr Color Heliotrope = MakeRGB(223u, 115u, 255u);
				static constexpr Color Phlox = MakeRGB(223u, 0u, 255u);
				static constexpr Color Liseran = MakeRGB(223u, 111u, 161u);
				static constexpr Color Mulberry = MakeRGB(197u, 75u, 140u);
				static constexpr Color Purpura = MakeRGB(154u, 78u, 174u);
				static constexpr Color NorthwesternPurple = MakeRGB(78u, 48u, 132u);
				static constexpr Color KansasState = MakeRGB(81u, 40u, 136u);
				static constexpr Color PompAndPower = MakeRGB(134u, 96u, 142u);
				static constexpr Color MardiGras = MakeRGB(136u, 0u, 133u);
				static constexpr Color Eminence = MakeRGB(108u, 48u, 130u);
				static constexpr Color Byzantium = MakeRGB(112u, 41u, 99u);
				static constexpr Color PansyPurple = MakeRGB(120u, 24u, 74u);
				static constexpr Color Palatinate = MakeRGB(114u, 36u, 108u);
				static constexpr Color Dark = MakeRGB(48u, 25u, 52u);
			} // namespace Miscellaneous
		} // namespace Purple
		namespace Violet
		{
			namespace Spectral
			{
				static constexpr Color ColorWheel = MakeRGB(127u, 0u, 255u);
				static constexpr Color Electric = MakeRGB(143u, 0u, 255u);
				static constexpr Color Vivid = MakeRGB(159u, 54u, 255u);
			} // namespace Spectral
			namespace Web
			{
				static constexpr Color Violet = MakeRGB(238u, 130u, 238u);
				static constexpr Color Dark = MakeRGB(148u, 0u, 211u);
			} // namespace Web
			namespace Miscellaneous
			{
				static constexpr Color Mauve = MakeRGB(224u, 176u, 255u);
				static constexpr Color Lavender = MakeRGB(181u, 126u, 220u);
				static constexpr Color Wisteria = MakeRGB(201u, 160u, 220u);
				static constexpr Color French = MakeRGB(136u, 6u, 206u);
				static constexpr Color African = MakeRGB(178u, 132u, 190u);
				static constexpr Color Ultra = MakeRGB(100u, 83u, 148u);
				static constexpr Color Chinese = MakeRGB(133u, 96u, 136u);
				static constexpr Color Grape = MakeRGB(111u, 45u, 168u);
				static constexpr Color Spanish = MakeRGB(76u, 40u, 130u);
				static constexpr Color Japanese = MakeRGB(91u, 50u, 86u);
				static constexpr Color English = MakeRGB(86u, 60u, 92u);
				static constexpr Color Russian = MakeRGB(50u, 23u, 77u);
			} // namespace Miscellaneous
		} // namespace Violet
	} // namespace Violet

	namespace White
	{
		static constexpr Color White = MakeRGB(255u, 255u, 255u);

		namespace Pastel
		{
			static constexpr Color AliceBlue = MakeRGB(240u, 248u, 255u);

			namespace Antique
			{
				static constexpr Color Version1 = MakeRGB(250u, 235u, 215u);
				static constexpr Color Version2 = MakeRGB(238u, 223u, 204u);
				static constexpr Color Version3 = MakeRGB(205u, 192u, 176u);
				static constexpr Color Version4 = MakeRGB(139u, 131u, 120u);
			} // namespace Antique
			static constexpr Color Azure = MakeRGB(240u, 255u, 255u);

			namespace Bisque
			{
				static constexpr Color Version1 = MakeRGB(255u, 228u, 196u);
				static constexpr Color Version2 = MakeRGB(238u, 213u, 183u);
				static constexpr Color Version3 = MakeRGB(255u, 250u, 250u);
				static constexpr Color Version4 = MakeRGB(255u, 250u, 250u);
			} // namespace Bisque
			static constexpr Color BlanchedAlmond = MakeRGB(255u, 250u, 250u);

			namespace Cornsilk
			{
				static constexpr Color Version1 = MakeRGB(255u, 250u, 250u);
				static constexpr Color Version2 = MakeRGB(255u, 250u, 250u);
				static constexpr Color Version3 = MakeRGB(255u, 250u, 250u);
				static constexpr Color Version4 = MakeRGB(255u, 250u, 250u);
			} // namespace Cornsilk
			static constexpr Color FloralWhite = MakeRGB(255u, 250u, 240u);
			static constexpr Color Gainsboro = MakeRGB(220u, 220u, 220u);
			static constexpr Color GhostWhite = MakeRGB(248u, 248u, 255u);

			namespace Honeydrew
			{
				static constexpr Color Version1 = MakeRGB(240u, 255u, 240u);
				static constexpr Color Version2 = MakeRGB(244u, 238u, 224u);
				static constexpr Color Version3 = MakeRGB(193u, 205u, 193u);
				static constexpr Color Version4 = MakeRGB(131u, 139u, 131u);
			} // namespace Honeydrew
			namespace Ivory
			{
				static constexpr Color Version1 = MakeRGB(255u, 255u, 240u);
				static constexpr Color Version2 = MakeRGB(238u, 238u, 224u);
				static constexpr Color Version3 = MakeRGB(205u, 205u, 193u);
				static constexpr Color Version4 = MakeRGB(139u, 139u, 131u);
			} // namespace Ivory
			static constexpr Color Lavender = MakeRGB(230u, 230u, 250u);
			static constexpr Color LavenderBlush = MakeRGB(255u, 240u, 245u);
			static constexpr Color LavenderClinton = MakeRGB(255u, 250u, 205u);
			static constexpr Color Linen = MakeRGB(240u, 240u, 230u);
			static constexpr Color MintCream = MakeRGB(245u, 255u, 250u);
			static constexpr Color MistyRose = MakeRGB(255u, 228u, 225u);
			static constexpr Color Moccasin = MakeRGB(255u, 228u, 181u);
			static constexpr Color NavajoWhite = MakeRGB(255u, 222u, 173u);
			static constexpr Color OldLace = MakeRGB(253u, 245u, 230u);
			static constexpr Color PapayaWhip = MakeRGB(255u, 239u, 213u);

			namespace PeachPuff
			{
				static constexpr Color Version1 = MakeRGB(255u, 218u, 185u);
				static constexpr Color Version2 = MakeRGB(238u, 203u, 173u);
				static constexpr Color Version3 = MakeRGB(205u, 175u, 149u);
				static constexpr Color Version4 = MakeRGB(139u, 119u, 101u);
			} // namespace PeachPuff
			namespace Seashell
			{
				static constexpr Color Version1 = MakeRGB(255u, 245u, 238u);
				static constexpr Color Version2 = MakeRGB(238u, 229u, 222u);
				static constexpr Color Version3 = MakeRGB(205u, 197u, 191u);
				static constexpr Color Version4 = MakeRGB(139u, 134u, 130u);
			} // namespace Seashell
			namespace Snow
			{
				static constexpr Color Version1 = MakeRGB(255u, 250u, 250u);
				static constexpr Color Version2 = MakeRGB(238u, 233u, 233u);
				static constexpr Color Version3 = MakeRGB(205u, 201u, 201u);
				static constexpr Color Version4 = MakeRGB(139u, 137u, 137u);
			} // namespace Snow
			static constexpr Color WhiteSmoke = MakeRGB(245u, 245u, 245u);
		} // namespace Pastel
	} // namespace White

	namespace Yellow
	{
		static constexpr Color Yellow = MakeRGB(255, 255, 0);

		namespace Tints
		{
			static constexpr Color Cream = MakeRGB(255u, 255u, 204u);
			static constexpr Color Light = MakeRGB(255u, 255u, 224u);
			static constexpr Color LemonChiffon = MakeRGB(255u, 250u, 205u);
		} // namespace Tints
		namespace Miscellaneous
		{
			static constexpr Color Bright = MakeRGB(252u, 232u, 131u);
			static constexpr Color Cyber = MakeRGB(255u, 211u, 0u);
			static constexpr Color DarkGoldenrod = MakeRGB(218u, 165u, 32u);
			static constexpr Color Gold = MakeRGB(255u, 215u, 0u);
			static constexpr Color Goldenrod = MakeRGB(218u, 165u, 32u);
			static constexpr Color Greenish = MakeRGB(238u, 234u, 98u);
			static constexpr Color Hunyadi = MakeRGB(231u, 172u, 65u);
			static constexpr Color Lemon = MakeRGB(253u, 255u, 0u);
			static constexpr Color Khaki = MakeRGB(195u, 176u, 145u);
			static constexpr Color LightGoldenrod = MakeRGB(250u, 250u, 210u);
			static constexpr Color Maximum = MakeRGB(250u, 250u, 255u);
			static constexpr Color Mellow = MakeRGB(248u, 222u, 126u);
			static constexpr Color Munsell = MakeRGB(239u, 204u, 0u);

			namespace OldGold
			{
				static constexpr Color Light = MakeRGB(207u, 181u, 59u);
				static constexpr Color Dark = MakeRGB(211u, 188u, 141u);
			} // namespace OldGold
			static constexpr Color PaleGoldenrod = MakeRGB(238u, 232u, 170u);
			static constexpr Color Royal = MakeRGB(250u, 218u, 94u);
			static constexpr Color Safety = MakeRGB(238u, 210u, 2u);
			static constexpr Color Unmellow = MakeRGB(255u, 255u, 102u);
			static constexpr Color Xanathic = MakeRGB(238u, 237u, 9u);
		} // namespace Miscellaneous
	} // namespace Yellow
}
