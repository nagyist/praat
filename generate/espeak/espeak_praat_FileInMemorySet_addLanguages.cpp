/* espeak_praat_FileInMemorySet_addLanguages.cpp
 *
 * This file was automatically created from files in the folder `generate/espeak/data`
 * by the script `generate/espeak/GENERATE.praat` in the Praat source distribution.
 *
 * eSpeak NG version: 1.52-dev, downloaded 2024-08-24T19:38Z from https://github.com/espeak-ng/espeak-ng
 * File creation date: Sat Mar 28 17:00:44 2026
 *
 * Copyright (C) 2005-2014 Jonathan Duddington (for eSpeak)
 * Copyright (C) 2015-2023 Reese Dunn (for eSpeak-NG)
 * Copyright (C) 2012-2024 David Weenink, 2024,2026 Paul Boersma (for Praat)
 *
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see: <http://www.gnu.org/licenses/>.
 */

#include "espeak_praat.h"
#include "FileInMemory.h"

void espeak_praat_FileInMemorySet_addLanguages (FileInMemorySet me) {
	try {
		static unsigned char fim1_data[112] = 
"name Vietnamese (Northern)\nlanguage vi\n\nwords 1 2\npitch 95 175\n\n\ntone 100 225 800 100 2000 50 5400 7"
"5 8000 200\n"
		;
		autoFileInMemory fim1 = FileInMemory_createWithData (111, fim1_data, true, U"./data/lang/aav/vi");
		my addItem_move (fim1.move());

		static unsigned char fim2_data[144] = 
"name Vietnamese (Central)\nlanguage vi-vn-x-central\nphonemes vi-hue\ndictrules 1\n\nwords 1\npitch 82 118"
"   //80 118\n voicing 90  //18\n flutter  20\n"
		;
		autoFileInMemory fim2 = FileInMemory_createWithData (143, fim2_data, true, U"./data/lang/aav/vi-VN-x-central");
		my addItem_move (fim2.move());

		static unsigned char fim3_data[143] = 
"name Vietnamese (Southern)\nlanguage vi-vn-x-south\nphonemes vi-sgn\ndictrules 2\n\nwords 1\npitch 82 118 "
"  //80 118\n voicing 90  //18\n flutter  20\n"
		;
		autoFileInMemory fim3 = FileInMemory_createWithData (142, fim3_data, true, U"./data/lang/aav/vi-VN-x-south");
		my addItem_move (fim3.move());

		static unsigned char fim4_data[42] = 
"name Esperanto\nlanguage eo\n\napostrophe 2\n"
		;
		autoFileInMemory fim4 = FileInMemory_createWithData (41, fim4_data, true, U"./data/lang/art/eo");
		my addItem_move (fim4.move());

		static unsigned char fim5_data[30] = 
"name Interlingua\nlanguage ia\n"
		;
		autoFileInMemory fim5 = FileInMemory_createWithData (29, fim5_data, true, U"./data/lang/art/ia");
		my addItem_move (fim5.move());

		static unsigned char fim6_data[51] = 
"name Ido\nlanguage io\nphonemes eo\nstatus testing\n \n"
		;
		autoFileInMemory fim6 = FileInMemory_createWithData (50, fim6_data, true, U"./data/lang/art/io");
		my addItem_move (fim6.move());

		static unsigned char fim7_data[70] = 
"name Lojban\nlanguage jbo\n\nspeed 80   // speed adjustment, percentage\n"
		;
		autoFileInMemory fim7 = FileInMemory_createWithData (69, fim7_data, true, U"./data/lang/art/jbo");
		my addItem_move (fim7.move());

		static unsigned char fim8_data[136] = 
"name Lingua Franca Nova\r\nlanguage lfn\r\n\r\nphonemes base2\r\nl_unpronouncable 0\r\nnumbers 2 3\r\n\r\nstressLe"
"ngth  150 140 180 180 0 0 200 200\r\n"
		;
		autoFileInMemory fim8 = FileInMemory_createWithData (135, fim8_data, true, U"./data/lang/art/lfn");
		my addItem_move (fim8.move());

		static unsigned char fim9_data[57] = 
"name Klingon\nlanguage piqd\nstatus testing\nstressRule 3\n\n"
		;
		autoFileInMemory fim9 = FileInMemory_createWithData (56, fim9_data, true, U"./data/lang/art/piqd");
		my addItem_move (fim9.move());

		static unsigned char fim10_data[141] = 
"name Pyash\nlanguage py\nmaintainer Logan Streondj <logan@liberit.ca>\nstatus testing\n\nspeed 80   // sp"
"eed adjustment, percentage\nstressRule 0\n"
		;
		autoFileInMemory fim10 = FileInMemory_createWithData (140, fim10_data, true, U"./data/lang/art/py");
		my addItem_move (fim10.move());

		static unsigned char fim11_data[58] = 
"name Lang Belta\nlanguage qdb\n\nnumbers 4 3\n\nreplace 1 t ?\n"
		;
		autoFileInMemory fim11 = FileInMemory_createWithData (57, fim11_data, true, U"./data/lang/art/qdb");
		my addItem_move (fim11.move());

		static unsigned char fim12_data[174] = 
"name Quenya\nlanguage qya\nstressRule 2\n// rule=penultimate, with qya_rules for light penultimate syll"
"ables to move primary stress to the preceding (antepenultimate) syllable\n"
		;
		autoFileInMemory fim12 = FileInMemory_createWithData (173, fim12_data, true, U"./data/lang/art/qya");
		my addItem_move (fim12.move());

		static unsigned char fim13_data[176] = 
"name Sindarin\nlanguage sjn\nstressRule 2\n// rule=penultimate, with sjn_rules for light penultimate sy"
"llables to move primary stress to the preceding (antepenultimate) syllable\n"
		;
		autoFileInMemory fim13 = FileInMemory_createWithData (175, fim13_data, true, U"./data/lang/art/sjn");
		my addItem_move (fim13.move());

		static unsigned char fim14_data[104] = 
"name xextan-test\nlanguage xex\n\nphonemes pt-br\nphonemes pt\n\npitch 80 130\n\ndictrules 1\ntunes s7 c7 q7 "
"e7\n"
		;
		autoFileInMemory fim14 = FileInMemory_createWithData (103, fim14_data, true, U"./data/lang/art/xex");
		my addItem_move (fim14.move());

		static unsigned char fim15_data[115] = 
"name Nahuatl (Classical)\nlanguage nci\n\nintonation 3\nstressRule 2\nstressLength  190  190  200  200  0"
"  0  220  240\n"
		;
		autoFileInMemory fim15 = FileInMemory_createWithData (114, fim15_data, true, U"./data/lang/azc/nci");
		my addItem_move (fim15.move());

		static unsigned char fim16_data[29] = 
"name Lithuanian\nlanguage lt\n"
		;
		autoFileInMemory fim16 = FileInMemory_createWithData (28, fim16_data, true, U"./data/lang/bat/lt");
		my addItem_move (fim16.move());

		static unsigned char fim17_data[313] = 
"name Latgalian\nlanguage ltg\nmaintainer Valdis Vitolins <valdis.vitolins@odo.lv>\nstatus testing\nphone"
"mes lv\ndictionary lv\ndictrules 2   // Setting for Latgalian pronunciation\nwords 0 2\npitch 64 118\nton"
"e 60 150 204 100 400 255 700 10 3000 255\nstressAmp 12 10 8 8 0 0 15 16\nstressLength 160 140 200 140 "
"0 0 240 160\n"
		;
		autoFileInMemory fim17 = FileInMemory_createWithData (312, fim17_data, true, U"./data/lang/bat/ltg");
		my addItem_move (fim17.move());

		static unsigned char fim18_data[230] = 
"name Latvian\nlanguage lv\nmaintainer Valdis Vitolins <valdis.vitolins@odo.lv>\nstatus mature\nwords 0 2"
"\npitch 67 123\ntone 60 150 204 100 400 255 700 10 3000 255\nstressAmp 11 8 11 9 0 0 14 12\nstressLength"
" 160 120 200 130 0 0 230 180\n"
		;
		autoFileInMemory fim18 = FileInMemory_createWithData (229, fim18_data, true, U"./data/lang/bat/lv");
		my addItem_move (fim18.move());

		static unsigned char fim19_data[42] = 
"name Swahili\nlanguage sw\n\nstatus testing\n"
		;
		autoFileInMemory fim19 = FileInMemory_createWithData (41, fim19_data, true, U"./data/lang/bnt/sw");
		my addItem_move (fim19.move());

		static unsigned char fim20_data[43] = 
"name Setswana\nlanguage tn\n\nstatus testing\n"
		;
		autoFileInMemory fim20 = FileInMemory_createWithData (42, fim20_data, true, U"./data/lang/bnt/tn");
		my addItem_move (fim20.move());

		static unsigned char fim21_data[125] = 
"name Georgian\nlanguage ka\nlowercaseSentence\t// A period followed by a lowercase letter is considered"
" a sentence (mkhedruli)\n"
		;
		autoFileInMemory fim21 = FileInMemory_createWithData (124, fim21_data, true, U"./data/lang/ccs/ka");
		my addItem_move (fim21.move());

		static unsigned char fim22_data[38] = 
"name Welsh\nlanguage cy\n\nintonation 4\n"
		;
		autoFileInMemory fim22 = FileInMemory_createWithData (37, fim22_data, true, U"./data/lang/cel/cy");
		my addItem_move (fim22.move());

		static unsigned char fim23_data[67] = 
"name Gaelic (Irish)\nlanguage ga\n\ndictrules 1  // fix for eclipsis\n"
		;
		autoFileInMemory fim23 = FileInMemory_createWithData (66, fim23_data, true, U"./data/lang/cel/ga");
		my addItem_move (fim23.move());

		static unsigned char fim24_data[52] = 
"name Gaelic (Scottish)\nlanguage gd\n\nstatus testing\n"
		;
		autoFileInMemory fim24 = FileInMemory_createWithData (51, fim24_data, true, U"./data/lang/cel/gd");
		my addItem_move (fim24.move());

		static unsigned char fim25_data[40] = 
"name Oromo\nlanguage om\n\nstatus testing\n"
		;
		autoFileInMemory fim25 = FileInMemory_createWithData (39, fim25_data, true, U"./data/lang/cus/om");
		my addItem_move (fim25.move());

		static unsigned char fim26_data[56] = 
"name Kannada\nlanguage kn\n\nintonation 2\n//consonants 80\n"
		;
		autoFileInMemory fim26 = FileInMemory_createWithData (55, fim26_data, true, U"./data/lang/dra/kn");
		my addItem_move (fim26.move());

		static unsigned char fim27_data[58] = 
"name Malayalam\nlanguage ml\n\nintonation 2\n//consonants 80\n"
		;
		autoFileInMemory fim27 = FileInMemory_createWithData (57, fim27_data, true, U"./data/lang/dra/ml");
		my addItem_move (fim27.move());

		static unsigned char fim28_data[52] = 
"name Tamil\nlanguage ta\n\nintonation 2\nconsonants 80\n"
		;
		autoFileInMemory fim28 = FileInMemory_createWithData (51, fim28_data, true, U"./data/lang/dra/ta");
		my addItem_move (fim28.move());

		static unsigned char fim29_data[71] = 
"name Telugu\nlanguage te\n\nstatus testing\n\nintonation 2\n//consonants 80\n"
		;
		autoFileInMemory fim29 = FileInMemory_createWithData (70, fim29_data, true, U"./data/lang/dra/te");
		my addItem_move (fim29.move());

		static unsigned char fim30_data[31] = 
"name Greenlandic\nlanguage kl\n\n"
		;
		autoFileInMemory fim30 = FileInMemory_createWithData (30, fim30_data, true, U"./data/lang/esx/kl");
		my addItem_move (fim30.move());

		static unsigned char fim31_data[55] = 
"name Basque\nlanguage eu\n\nstatus testing\nstressRule 15\n"
		;
		autoFileInMemory fim31 = FileInMemory_createWithData (54, fim31_data, true, U"./data/lang/eu");
		my addItem_move (fim31.move());

		static unsigned char fim32_data[44] = 
"name Danish\nlanguage da\n\ntunes s2 c2 q2 e2\n"
		;
		autoFileInMemory fim32 = FileInMemory_createWithData (43, fim32_data, true, U"./data/lang/gmq/da");
		my addItem_move (fim32.move());

		static unsigned char fim33_data[105] = 
"name Faroese\nlanguage fo\nmaintainer iSolveIT ApS (Andras Eliassen) <andras@isolveit.net>\nstatus test"
"ing\n"
		;
		autoFileInMemory fim33 = FileInMemory_createWithData (104, fim33_data, true, U"./data/lang/gmq/fo");
		my addItem_move (fim33.move());

		static unsigned char fim34_data[28] = 
"name Icelandic\nlanguage is\n"
		;
		autoFileInMemory fim34 = FileInMemory_createWithData (27, fim34_data, true, U"./data/lang/gmq/is");
		my addItem_move (fim34.move());

		static unsigned char fim35_data[88] = 
"name Norwegian Bokm\303\245l\nlanguage nb\nlanguage no\nphonemes no\ndictionary no\n\nintonation 4\n"
		;
		autoFileInMemory fim35 = FileInMemory_createWithData (87, fim35_data, true, U"./data/lang/gmq/nb");
		my addItem_move (fim35.move());

		static unsigned char fim36_data[26] = 
"name Swedish\nlanguage sv\n"
		;
		autoFileInMemory fim36 = FileInMemory_createWithData (25, fim36_data, true, U"./data/lang/gmq/sv");
		my addItem_move (fim36.move());

		static unsigned char fim37_data[124] = 
"name Afrikaans\nlanguage af\n\nmaintainer Christo de Klerk <christodeklerk@gmail.com>\nstatus mature\n\nro"
"ughness 0\npitch 63 120\n"
		;
		autoFileInMemory fim37 = FileInMemory_createWithData (123, fim37_data, true, U"./data/lang/gmw/af");
		my addItem_move (fim37.move());

		static unsigned char fim38_data[43] = 
"name German\nlanguage de\ntunes s4 c1 q4 e1\n"
		;
		autoFileInMemory fim38 = FileInMemory_createWithData (42, fim38_data, true, U"./data/lang/gmw/de");
		my addItem_move (fim38.move());

		static unsigned char fim39_data[141] = 
"name English (Great Britain)\nlanguage en-gb  2\nlanguage en 2\n\nmaintainer Reece H. Dunn <msclrhd@gmai"
"l.com>\nstatus mature\n\ntunes s1 c1 q1 e1\n"
		;
		autoFileInMemory fim39 = FileInMemory_createWithData (140, fim39_data, true, U"./data/lang/gmw/en");
		my addItem_move (fim39.move());

		static unsigned char fim40_data[336] = 
"name English (Caribbean)\nlanguage en-029\nlanguage en 10\n\nmaintainer Reece H. Dunn <msclrhd@gmail.com"
">\nstatus mature\n\nphonemes en-wi\ndictrules 8\nstressLength 175 175  175 175  220 220  250 290\n\nreplace"
" 00 D d\nreplace 00 T t[\nreplace 00 U@ o@\nreplace 03 @ a#\nreplace 03 3 a#\nreplace 03 N n\n\nformant 1  "
"98  100 100\nformant 2  98  100 100\n"
		;
		autoFileInMemory fim40 = FileInMemory_createWithData (335, fim40_data, true, U"./data/lang/gmw/en-029");
		my addItem_move (fim40.move());

		static unsigned char fim41_data[296] = 
"name English (Scotland)\nlanguage en-gb-scotland\nlanguage en 4\n\nmaintainer Reece H. Dunn <msclrhd@gma"
"il.com>\nstatus mature\n\nphonemes en-sc\ndictrules 2 5 6 7\nstressLength  180 130 200 200 0 0 250 270\n\nr"
"eplace 03 @ V\nreplace 03 I i\nreplace 03 I2 i\nreplace 01 aI aI2\nreplace 02 a a/\nreplace 02 u: U\n"
		;
		autoFileInMemory fim41 = FileInMemory_createWithData (295, fim41_data, true, U"./data/lang/gmw/en-GB-scotland");
		my addItem_move (fim41.move());

		static unsigned char fim42_data[239] = 
"name English (Lancaster)\nlanguage en-gb-x-gbclan\nlanguage en-gb  3\nlanguage en 5\n\nmaintainer Reece H"
". Dunn <msclrhd@gmail.com>\nstatus mature\n\nphonemes en-n\n\nstressLength 160 150  180 180  220 220  290"
" 290\n\nreplace 03 N n\nreplace 03 i  I2\n"
		;
		autoFileInMemory fim42 = FileInMemory_createWithData (238, fim42_data, true, U"./data/lang/gmw/en-GB-x-gbclan");
		my addItem_move (fim42.move());

		static unsigned char fim43_data[189] = 
"name English (West Midlands)\nlanguage en-gb-x-gbcwmd\nlanguage en-gb 9\nlanguage en 9\n\nphonemes en-wm\n"
"\nreplace 00 h NULL\nreplace 00 o@ O@\ndictrules 6\nintonation 4\nstressAdd 0 0 0 0 0 0 0 20\n"
		;
		autoFileInMemory fim43 = FileInMemory_createWithData (188, fim43_data, true, U"./data/lang/gmw/en-GB-x-gbcwmd");
		my addItem_move (fim43.move());

		static unsigned char fim44_data[250] = 
"name English (Received Pronunciation)\nlanguage en-gb-x-rp\nlanguage en-gb  4\nlanguage en 5\n\nmaintaine"
"r Reece H. Dunn <msclrhd@gmail.com>\nstatus mature\n\nphonemes en-rp\n\nreplace 00  o@  O@\nreplace 03 I i"
"\nreplace 03 I2 i\nreplace 03 @ a#\nreplace 03 3 a#\n"
		;
		autoFileInMemory fim44 = FileInMemory_createWithData (249, fim44_data, true, U"./data/lang/gmw/en-GB-x-rp");
		my addItem_move (fim44.move());

		static unsigned char fim45_data[258] = 
"name English (America)\nlanguage en-us 2\nlanguage en 3\n\nmaintainer Reece H. Dunn <msclrhd@gmail.com>\n"
"status mature\n\nphonemes en-us\ndictrules 3 6\n\nstressLength 140 120 190 170 0 0 255 300\nstressAmp  17 "
"16  19 19  19 19  21 19\n\nreplace 03 I  i\nreplace 03 I2 i\n"
		;
		autoFileInMemory fim45 = FileInMemory_createWithData (257, fim45_data, true, U"./data/lang/gmw/en-US");
		my addItem_move (fim45.move());

		static unsigned char fim46_data[272] = 
"name English (America, New York City)\nlanguage en-us-nyc\n\nmaintainer Richard Calvi <richard.calvi@gm"
"ail.com>\nstatus testing\n\nphonemes en-us-nyc\ndictrules 3 6\n\nstressLength 140 120 190 170 0 0 255 300\n"
"stressAmp  17 16  19 19  19 19  21 19\n\nreplace 03 I  i\nreplace 03 I2 i\n"
		;
		autoFileInMemory fim46 = FileInMemory_createWithData (271, fim46_data, true, U"./data/lang/gmw/en-US-nyc");
		my addItem_move (fim46.move());

		static unsigned char fim47_data[32] = 
"name Luxembourgish\nlanguage lb\n"
		;
		autoFileInMemory fim47 = FileInMemory_createWithData (31, fim47_data, true, U"./data/lang/gmw/lb");
		my addItem_move (fim47.move());

		static unsigned char fim48_data[24] = 
"name Dutch\nlanguage nl\n"
		;
		autoFileInMemory fim48 = FileInMemory_createWithData (23, fim48_data, true, U"./data/lang/gmw/nl");
		my addItem_move (fim48.move());

		static unsigned char fim49_data[24] = 
"name Greek\nlanguage el\n"
		;
		autoFileInMemory fim49 = FileInMemory_createWithData (23, fim49_data, true, U"./data/lang/grk/el");
		my addItem_move (fim49.move());

		static unsigned char fim50_data[100] = 
"name Greek (Ancient)\nlanguage grc\n\nstressLength 170 170  190 190  0 0  230 240\ndictrules 1\nwords 3\n"
		;
		autoFileInMemory fim50 = FileInMemory_createWithData (99, fim50_data, true, U"./data/lang/grk/grc");
		my addItem_move (fim50.move());

		static unsigned char fim51_data[43] = 
"name Assamese\nlanguage as\n\nstatus testing\n"
		;
		autoFileInMemory fim51 = FileInMemory_createWithData (42, fim51_data, true, U"./data/lang/inc/as");
		my addItem_move (fim51.move());

		static unsigned char fim52_data[26] = 
"name Bengali\nlanguage bn\n"
		;
		autoFileInMemory fim52 = FileInMemory_createWithData (25, fim52_data, true, U"./data/lang/inc/bn");
		my addItem_move (fim52.move());

		static unsigned char fim53_data[40] = 
"name Bishnupriya Manipuri\nlanguage bpy\n"
		;
		autoFileInMemory fim53 = FileInMemory_createWithData (39, fim53_data, true, U"./data/lang/inc/bpy");
		my addItem_move (fim53.move());

		static unsigned char fim54_data[43] = 
"name Gujarati\nlanguage gu\n\nstatus testing\n"
		;
		autoFileInMemory fim54 = FileInMemory_createWithData (42, fim54_data, true, U"./data/lang/inc/gu");
		my addItem_move (fim54.move());

		static unsigned char fim55_data[24] = 
"name Hindi\nlanguage hi\n"
		;
		autoFileInMemory fim55 = FileInMemory_createWithData (23, fim55_data, true, U"./data/lang/inc/hi");
		my addItem_move (fim55.move());

		static unsigned char fim56_data[27] = 
"name Konkani\nlanguage kok\n"
		;
		autoFileInMemory fim56 = FileInMemory_createWithData (26, fim56_data, true, U"./data/lang/inc/kok");
		my addItem_move (fim56.move());

		static unsigned char fim57_data[42] = 
"name Marathi\nlanguage mr\n\nstatus testing\n"
		;
		autoFileInMemory fim57 = FileInMemory_createWithData (41, fim57_data, true, U"./data/lang/inc/mr");
		my addItem_move (fim57.move());

		static unsigned char fim58_data[38] = 
"name Nepali\nlanguage ne\n\ndictrules 1\n"
		;
		autoFileInMemory fim58 = FileInMemory_createWithData (37, fim58_data, true, U"./data/lang/inc/ne");
		my addItem_move (fim58.move());

		static unsigned char fim59_data[40] = 
"name Oriya\nlanguage or\n\nstatus testing\n"
		;
		autoFileInMemory fim59 = FileInMemory_createWithData (39, fim59_data, true, U"./data/lang/inc/or");
		my addItem_move (fim59.move());

		static unsigned char fim60_data[26] = 
"name Punjabi\nlanguage pa\n"
		;
		autoFileInMemory fim60 = FileInMemory_createWithData (25, fim60_data, true, U"./data/lang/inc/pa");
		my addItem_move (fim60.move());

		static unsigned char fim61_data[67] = 
"name Sindhi\nlanguage sd\nmaintainer Ejaz Shah <eashah67@gmail.com>\n"
		;
		autoFileInMemory fim61 = FileInMemory_createWithData (66, fim61_data, true, U"./data/lang/inc/sd");
		my addItem_move (fim61.move());

		static unsigned char fim62_data[56] = 
"name Sinhala\nlanguage si\n\nstatus testing\n\nintonation 2\n"
		;
		autoFileInMemory fim62 = FileInMemory_createWithData (55, fim62_data, true, U"./data/lang/inc/si");
		my addItem_move (fim62.move());

		static unsigned char fim63_data[95] = 
"name Urdu\nlanguage ur\nmaintainer Ejaz Shah <eashah67@gmail.com>\nstatus testing\n\nstressRule 6\n\n"
		;
		autoFileInMemory fim63 = FileInMemory_createWithData (94, fim63_data, true, U"./data/lang/inc/ur");
		my addItem_move (fim63.move());

		static unsigned char fim64_data[62] = 
"name Armenian (East Armenia)\nlanguage hy\nlanguage hy-arevela\n"
		;
		autoFileInMemory fim64 = FileInMemory_createWithData (61, fim64_data, true, U"./data/lang/ine/hy");
		my addItem_move (fim64.move());

		static unsigned char fim65_data[366] = 
"name Armenian (West Armenia)\nlanguage hyw\nlanguage hy-arevmda\nlanguage hy  8\n\ndictionary hy\ndictrule"
"s 1\n\nphonemes hy\n\n// change consonants for West Armenian pronunciation\nreplace 00 b  p#\nreplace 00 d"
"  t#\nreplace 00 dz ts#\nreplace 00 dZ tS#\nreplace 00 g  k#\n\nreplace 00 p  b\nreplace 00 t  d\nreplace 0"
"0 ts dz\nreplace 00 tS dZ\nreplace 00 k  g\n\nreplace 00 R2 R  // ??\n"
		;
		autoFileInMemory fim65 = FileInMemory_createWithData (365, fim65_data, true, U"./data/lang/ine/hyw");
		my addItem_move (fim65.move());

		static unsigned char fim66_data[104] = 
"name Albanian\nlanguage sq\n\n// add this line to remove '\303\253' at the end of words\n// replace 00  @/  NU"
"LL\n"
		;
		autoFileInMemory fim66 = FileInMemory_createWithData (103, fim66_data, true, U"./data/lang/ine/sq");
		my addItem_move (fim66.move());

		static unsigned char fim67_data[91] = 
"name Persian\nlanguage fa\nmaintainer Shadyar Khodayari <shadyar81@gmail.com>\nstatus mature\n"
		;
		autoFileInMemory fim67 = FileInMemory_createWithData (90, fim67_data, true, U"./data/lang/ira/fa");
		my addItem_move (fim67.move());

		static unsigned char fim68_data[270] = 
"name Persian (Pinglish)\n// Sometimes, Farsi speakers write Farsi words using English characters, par"
"ticularly in Chat and SMS (texte messages).), called Pinglish\nlanguage fa-latn\nmaintainer Shadyar Kh"
"odayari <shadyar81@gmail.com>\nstatus mature\ndictrules 1\nphonemes fa\n\n"
		;
		autoFileInMemory fim68 = FileInMemory_createWithData (269, fim68_data, true, U"./data/lang/ira/fa-Latn");
		my addItem_move (fim68.move());

		static unsigned char fim69_data[41] = 
"name Kurdish\nlanguage ku\n\n//words 1 48\n\n"
		;
		autoFileInMemory fim69 = FileInMemory_createWithData (40, fim69_data, true, U"./data/lang/ira/ku");
		my addItem_move (fim69.move());

		static unsigned char fim70_data[570] = 
"name Cherokee //https://github.com/espeak-ng/espeak-ng/blob/master/docs/voices.md\nlanguage chr-US-Qa"
"aa-x-west 5\n\nmaintainer Michael Conrad <m.conrad.202@gmail.com>\nstatus testing\n\npitch 90 160\n\nvoicin"
"g 100\n\nconsonants 100 100\n\nspeed 100\n\nwords 2 1\n\nphonemes chr\n\n//stress on all syllables to simulate"
" stress on no syllables\nstressRule 9 \nstressLength 175 175 175 175 175 175 175 175 //all vowels the "
"same length regardless of stress\nstressAmp 10 10 10 10 10 10 10 10  //all vowels the same strength r"
"egardless of marked stress\n\nintonation 1\n\ntunes\tchrs chrc chrq chre\n\n"
		;
		autoFileInMemory fim70 = FileInMemory_createWithData (569, fim70_data, true, U"./data/lang/iro/chr");
		my addItem_move (fim70.move());

		static unsigned char fim71_data[298] = 
"name Latin\nlanguage la\nstressRule 2 0 2\n// rule=penultimate\n// unstressed_wd1=0\n// unstressed_wd2=2\n"
"stressOpt 0 5 // flags=0100001 (no automatic secondary stress + don't stres monosyllables)\n\n// short"
" gap between words\nwords 2\n\n// Note: The Latin voice needs long vowels to be marked with macrons\n"
		;
		autoFileInMemory fim71 = FileInMemory_createWithData (297, fim71_data, true, U"./data/lang/itc/la");
		my addItem_move (fim71.move());

		static unsigned char fim72_data[53] = 
"name Japanese\nlanguage ja\nphonemes ja\n\nintonation 4\n"
		;
		autoFileInMemory fim72 = FileInMemory_createWithData (52, fim72_data, true, U"./data/lang/jpx/ja");
		my addItem_move (fim72.move());

		static unsigned char fim73_data[52] = 
"name Korean\nlanguage ko\npitch 80 118\nintonation 2\n\n"
		;
		autoFileInMemory fim73 = FileInMemory_createWithData (51, fim73_data, true, U"./data/lang/ko");
		my addItem_move (fim73.move());

		static unsigned char fim74_data[43] = 
"name Hawaiian\nlanguage haw\nstatus testing\n"
		;
		autoFileInMemory fim74 = FileInMemory_createWithData (42, fim74_data, true, U"./data/lang/map/haw");
		my addItem_move (fim74.move());

		static unsigned char fim75_data[184] = 
"name Totontepec Mixe\nlanguage mto\n\nmaintainer Bill Dengler <codeofdusk@gmail.com> and Elizabeth Rese"
"ndiz <e.r.resendiz7@gmail.com>\nstatus testing\n\nlowercaseSentence\ntunes s6 c6 q6 e6\n"
		;
		autoFileInMemory fim75 = FileInMemory_createWithData (183, fim75_data, true, U"./data/lang/miz/mto");
		my addItem_move (fim75.move());

		static unsigned char fim76_data[211] = 
"name K'iche'\nlanguage quc\nstatus testing\nstressRule 3 // stress on final syllable\nstressAmp 8 8 20 1"
"5 0 0 25 25  // reduce unstressed vowels\nstressLength 120 120 200 150 0 0 250 250 // reduce unstress"
"ed vowels\n"
		;
		autoFileInMemory fim76 = FileInMemory_createWithData (210, fim76_data, true, U"./data/lang/myn/quc");
		my addItem_move (fim76.move());

		static unsigned char fim77_data[135] = 
"name Indonesian\nlanguage id\n\nstressLength 160 200  180 180  0 0  220 240\nstressAmp    16  18   18  1"
"8   0 0  22  21\n\nconsonants 80 80\n"
		;
		autoFileInMemory fim77 = FileInMemory_createWithData (134, fim77_data, true, U"./data/lang/poz/id");
		my addItem_move (fim77.move());

		static unsigned char fim78_data[368] = 
"name M\304\201ori\nlanguage mi\nstatus testing\n\n// https://github.com/espeak-ng/espeak-ng/blob/master/docs/v"
"oices.md#words\nwords 1 2\n\n// taken from Jacky\npitch  115 130\n\nformant 0 150 155 100\nformant 1 90 155"
" 70\nformant 2 95 70 64\nformant 3 15 20 30\nformant 4 20 30 40\nformant 5 65 20 65\nformant 6 70 80 100\n"
"formant 7 20 80 100\nformant 8 100 95 80\nvoicing 135\nconsonants 110\n"
		;
		autoFileInMemory fim78 = FileInMemory_createWithData (367, fim78_data, true, U"./data/lang/poz/mi");
		my addItem_move (fim78.move());

		static unsigned char fim79_data[431] = 
"// Last updated: 14 October 2010, Jason Ong (jason@portalgroove.com)\nname Malay\nlanguage ms\nphonemes"
" id\n\nstressLength 160 200  180 180  0 0  220 240\nstressAmp    16  18   18  18   0 0  22  21\nintonati"
"on\t3\t// Less intonation, and comma does not raise the pitch.\n\n// Nuance - Peninsula Malaysia\n// repl"
"ace\t3 a\t@\t// change 'saya' to 'saye'\n\t\t\t\t// (only the last phoneme of a word, only in unstressed syl"
"lables)\n\t\t\t\t\nconsonants 80 80\n"
		;
		autoFileInMemory fim79 = FileInMemory_createWithData (430, fim79_data, true, U"./data/lang/poz/ms");
		my addItem_move (fim79.move());

		static unsigned char fim80_data[89] = 
"name Quechua\nlanguage qu\nstressRule 2 // stress on penultimate syllable\nstatus testing\n\n"
		;
		autoFileInMemory fim80 = FileInMemory_createWithData (88, fim80_data, true, U"./data/lang/qu");
		my addItem_move (fim80.move());

		static unsigned char fim81_data[28] = 
"name Aragonese\nlanguage an\n"
		;
		autoFileInMemory fim81 = FileInMemory_createWithData (27, fim81_data, true, U"./data/lang/roa/an");
		my addItem_move (fim81.move());

		static unsigned char fim82_data[26] = 
"name Catalan\nlanguage ca\n"
		;
		autoFileInMemory fim82 = FileInMemory_createWithData (25, fim82_data, true, U"./data/lang/roa/ca");
		my addItem_move (fim82.move());

		static unsigned char fim83_data[64] = 
"name Spanish (Spain)\nlanguage es\ndictrules 1\ntunes s6 c6 q6 e6\n"
		;
		autoFileInMemory fim83 = FileInMemory_createWithData (63, fim83_data, true, U"./data/lang/roa/es");
		my addItem_move (fim83.move());

		static unsigned char fim84_data[168] = 
"name Spanish (Latin America)\nlanguage es-419\nlanguage es-mx 6\n\nphonemes es-la\ndictrules 2\nintonation"
" 2\nstressLength 170 200  230 180  0 0  250 280\n\ntunes s6 c6 q6 e6\n\n"
		;
		autoFileInMemory fim84 = FileInMemory_createWithData (167, fim84_data, true, U"./data/lang/roa/es-419");
		my addItem_move (fim84.move());

		static unsigned char fim85_data[80] = 
"name French (France)\nlanguage fr-fr\nlanguage fr\n\ndictrules 1\ntunes s3 c3 q3 e3\n"
		;
		autoFileInMemory fim85 = FileInMemory_createWithData (79, fim85_data, true, U"./data/lang/roa/fr");
		my addItem_move (fim85.move());

		static unsigned char fim86_data[85] = 
"name French (Belgium)\nlanguage fr-be\nlanguage fr 8\n\ndictrules 2\ntunes s3 c3 q3 e3\n\n\n"
		;
		autoFileInMemory fim86 = FileInMemory_createWithData (84, fim86_data, true, U"./data/lang/roa/fr-BE");
		my addItem_move (fim86.move());

		static unsigned char fim87_data[87] = 
"name French (Switzerland)\nlanguage fr-ch\nlanguage fr 8\n\ndictrules 3\ntunes s3 c3 q3 e3\n"
		;
		autoFileInMemory fim87 = FileInMemory_createWithData (86, fim87_data, true, U"./data/lang/roa/fr-CH");
		my addItem_move (fim87.move());

		static unsigned char fim88_data[141] = 
"name Haitian Creole\nlanguage ht\nstatus testing\nmaintainer  // TODO somebody should take responsibili"
"ty for this\n\nphonemes ht\ndictionary ht\n\n"
		;
		autoFileInMemory fim88 = FileInMemory_createWithData (140, fim88_data, true, U"./data/lang/roa/ht");
		my addItem_move (fim88.move());

		static unsigned char fim89_data[110] = 
"name Italian\nlanguage it\n\nmaintainer Christian Leo M <llajta2012@gmail.com>\nstatus mature\n\ntunes s4 "
"c4 q4 e4\n"
		;
		autoFileInMemory fim89 = FileInMemory_createWithData (109, fim89_data, true, U"./data/lang/roa/it");
		my addItem_move (fim89.move());

		static unsigned char fim90_data[63] = 
"name Papiamento\nlanguage pap\n\nstatus testing\n\nphonemes base2\n\n"
		;
		autoFileInMemory fim90 = FileInMemory_createWithData (62, fim90_data, true, U"./data/lang/roa/pap");
		my addItem_move (fim90.move());

		static unsigned char fim91_data[96] = 
"name Portuguese (Portugal)\nlanguage pt\nlanguage pt-pt\nphonemes pt-pt\n\ndictrules 1\nintonation 2\n"
		;
		autoFileInMemory fim91 = FileInMemory_createWithData (95, fim91_data, true, U"./data/lang/roa/pt");
		my addItem_move (fim91.move());

		static unsigned char fim92_data[110] = 
"name Portuguese (Brazil)\nlanguage pt-br\nlanguage pt 6\n\ndictrules 2\nstressLength 200 115 230 230 0 0 "
"250 270\n\n"
		;
		autoFileInMemory fim92 = FileInMemory_createWithData (109, fim92_data, true, U"./data/lang/roa/pt-BR");
		my addItem_move (fim92.move());

		static unsigned char fim93_data[27] = 
"name Romanian\nlanguage ro\n"
		;
		autoFileInMemory fim93 = FileInMemory_createWithData (26, fim93_data, true, U"./data/lang/roa/ro");
		my addItem_move (fim93.move());

		static unsigned char fim94_data[48] = 
"name Guarani\nlanguage gn\ndictrules 1\nwords 0 1\n"
		;
		autoFileInMemory fim94 = FileInMemory_createWithData (47, fim94_data, true, U"./data/lang/sai/gn");
		my addItem_move (fim94.move());

		static unsigned char fim95_data[42] = 
"name Amharic\nlanguage am\n\nstatus testing\n"
		;
		autoFileInMemory fim95 = FileInMemory_createWithData (41, fim95_data, true, U"./data/lang/sem/am");
		my addItem_move (fim95.move());

		static unsigned char fim96_data[51] = 
"name Arabic\nlanguage ar\nphonemes ar\n\nstressRule 4\n"
		;
		autoFileInMemory fim96 = FileInMemory_createWithData (50, fim96_data, true, U"./data/lang/sem/ar");
		my addItem_move (fim96.move());

		static unsigned char fim97_data[41] = 
"name Hebrew\nlanguage he\n\nstatus testing\n"
		;
		autoFileInMemory fim97 = FileInMemory_createWithData (40, fim97_data, true, U"./data/lang/sem/he");
		my addItem_move (fim97.move());

		static unsigned char fim98_data[42] = 
"name Maltese\nlanguage mt\n\nstatus testing\n"
		;
		autoFileInMemory fim98 = FileInMemory_createWithData (41, fim98_data, true, U"./data/lang/sem/mt");
		my addItem_move (fim98.move());

		static unsigned char fim99_data[94] = 
"name Tigrinya\nlanguage ti\n\nmaintainer Biniam Gebremichael <biniamg@gmail.com>\nstatus testing\n"
		;
		autoFileInMemory fim99 = FileInMemory_createWithData (93, fim99_data, true, U"./data/lang/sem/ti");
		my addItem_move (fim99.move());

		static unsigned char fim100_data[687] = 
"name Chinese (Mandarin, latin as English)\nlanguage cmn\nlanguage zh-cmn\nlanguage zh\n\nphonemes cmn\ndic"
"tionary cmn\nwords 1\npitch 80 118\n\ndict_min 100000\n\n//for some dialects\n\n//[en]: replace ng with n\n//"
"[zh]: \357\277\275\336\272\357\277\275\357\277\275\357\277\275\357\277\275\357\277\275\357\277\275\357\277\275ng\357\277\275\357\277\275\357\277\275n\n//replace 0 N n\n\n//[en]: replace rfx consonants\n//[zh]:"
" \357\277\275\336\276\357\277\275\357\277\275\357\277\275\357\277\275\357\277\275\357\277\275\357\277\275r\357\277\275\357\277\275\357\277\275l\357\277\275\357\277\275z\357\277\275\357\277\275er\357\277\275\357\277\275\357\277\275e\n//replace 0 ts.h tsh\n//replace 0 ts."
" ts\n//replace 0 s. s\n//replace 0 i. i[\n//replace 0 z. l\n//replace 0 z. z\n//replace 0 @r @\n\n//[en]: r"
"eplace beginning n or l\n//[zh]: \357\277\275\357\277\275\357\277\275\357\277\275nl\357\277\275\357\277\275n\357\277\275\357\277\275\357\277\275l\357\277\275\357\277\275l\357\277\275\357\277\275\357\277\275n\n//replace 2 n l\n//r"
"eplace 2 l n\n\n//[en]: replace beginning w with v\n//[zh]: w\357\277\275\357\277\275\357\277\275v\n//replace 0 w  v\n"
		;
		autoFileInMemory fim100 = FileInMemory_createWithData (686, fim100_data, true, U"./data/lang/sit/cmn");
		my addItem_move (fim100.move());

		static unsigned char fim101_data[162] = 
"name Chinese (Mandarin, latin as Pinyin)\nlanguage cmn-latn-pinyin\nlanguage zh-cmn\nlanguage zh\n\nphone"
"mes cmn\ndictionary cmn\nwords 1\npitch 80 118\n\ndict_min 100000\n"
		;
		autoFileInMemory fim101 = FileInMemory_createWithData (161, fim101_data, true, U"./data/lang/sit/cmn-Latn-pinyin");
		my addItem_move (fim101.move());

		static unsigned char fim102_data[129] = 
"name Hakka Chinese\nlanguage hak\nmaintainer Chen Chien-ting <yoxem.tem98@nctu.edu.tw>\nstatus testing\n"
"phonemes hak\ndictionary hak\n"
		;
		autoFileInMemory fim102 = FileInMemory_createWithData (128, fim102_data, true, U"./data/lang/sit/hak");
		my addItem_move (fim102.move());

		static unsigned char fim103_data[57] = 
"name Myanmar (Burmese)\nmaintainer Min Maung\nlanguage my\n"
		;
		autoFileInMemory fim103 = FileInMemory_createWithData (56, fim103_data, true, U"./data/lang/sit/my");
		my addItem_move (fim103.move());

		static unsigned char fim104_data[195] = 
"name Chinese (Cantonese)\nlanguage yue\nlanguage zh-yue\nlanguage zh 8\n\nphonemes yue\ndictionary yue\n\n//"
" interpret English letters as 1=English words, 2=jyutping\ndictrules 1\n\nwords 1\ndict_min 10000\n"
		;
		autoFileInMemory fim104 = FileInMemory_createWithData (194, fim104_data, true, U"./data/lang/sit/yue");
		my addItem_move (fim104.move());

		static unsigned char fim105_data[214] = 
"name Chinese (Cantonese, latin as Jyutping)\nlanguage yue\nlanguage zh-yue\nlanguage zh 8\n\nphonemes yue"
"\ndictionary yue\n\n// interpret English letters as 1=English words, 2=jyutping\ndictrules 2\n\nwords 1\ndi"
"ct_min 10000\n"
		;
		autoFileInMemory fim105 = FileInMemory_createWithData (213, fim105_data, true, U"./data/lang/sit/yue-Latn-jyutping");
		my addItem_move (fim105.move());

		static unsigned char fim106_data[93] = 
"name Shan (Tai Yai)\nlanguage shn\nmaintainer ronaldaug <contact@ronaldaug.ml>\nstatus testing\n"
		;
		autoFileInMemory fim106 = FileInMemory_createWithData (92, fim106_data, true, U"./data/lang/tai/shn");
		my addItem_move (fim106.move());

		static unsigned char fim107_data[38] = 
"name Thai\nlanguage th\nstatus testing\n"
		;
		autoFileInMemory fim107 = FileInMemory_createWithData (37, fim107_data, true, U"./data/lang/tai/th");
		my addItem_move (fim107.move());

		static unsigned char fim108_data[46] = 
"name Azerbaijani\nlanguage az\n\nstatus testing\n"
		;
		autoFileInMemory fim108 = FileInMemory_createWithData (45, fim108_data, true, U"./data/lang/trk/az");
		my addItem_move (fim108.move());

		static unsigned char fim109_data[26] = 
"name Bashkir\nlanguage ba\n"
		;
		autoFileInMemory fim109 = FileInMemory_createWithData (25, fim109_data, true, U"./data/lang/trk/ba");
		my addItem_move (fim109.move());

		static unsigned char fim110_data[41] = 
"name Chuvash\nlanguage cv\nstatus testing\n"
		;
		autoFileInMemory fim110 = FileInMemory_createWithData (40, fim110_data, true, U"./data/lang/trk/cv");
		my addItem_move (fim110.move());

		static unsigned char fim111_data[29] = 
"name Karakalpak\nlanguage kaa"
		;
		autoFileInMemory fim111 = FileInMemory_createWithData (28, fim111_data, true, U"./data/lang/trk/kaa");
		my addItem_move (fim111.move());

		static unsigned char fim112_data[41] = 
"name Kazakh\nlanguage kk\nstatus testing\n\n"
		;
		autoFileInMemory fim112 = FileInMemory_createWithData (40, fim112_data, true, U"./data/lang/trk/kk");
		my addItem_move (fim112.move());

		static unsigned char fim113_data[44] = 
"name Kyrgyz\nlanguage ky\n\ntunes s3 c3 q3 e3\n"
		;
		autoFileInMemory fim113 = FileInMemory_createWithData (43, fim113_data, true, U"./data/lang/trk/ky");
		my addItem_move (fim113.move());

		static unsigned char fim114_data[40] = 
"name Nogai\nlanguage nog\nstatus testing\n"
		;
		autoFileInMemory fim114 = FileInMemory_createWithData (39, fim114_data, true, U"./data/lang/trk/nog");
		my addItem_move (fim114.move());

		static unsigned char fim115_data[26] = 
"name Turkmen\nlanguage tk\n"
		;
		autoFileInMemory fim115 = FileInMemory_createWithData (25, fim115_data, true, U"./data/lang/trk/tk");
		my addItem_move (fim115.move());

		static unsigned char fim116_data[26] = 
"name Turkish\nlanguage tr\n"
		;
		autoFileInMemory fim116 = FileInMemory_createWithData (25, fim116_data, true, U"./data/lang/trk/tr");
		my addItem_move (fim116.move());

		static unsigned char fim117_data[24] = 
"name Tatar\nlanguage tt\n"
		;
		autoFileInMemory fim117 = FileInMemory_createWithData (23, fim117_data, true, U"./data/lang/trk/tt");
		my addItem_move (fim117.move());

		static unsigned char fim118_data[25] = 
"name Uyghur\nlanguage ug\n"
		;
		autoFileInMemory fim118 = FileInMemory_createWithData (24, fim118_data, true, U"./data/lang/trk/ug");
		my addItem_move (fim118.move());

		static unsigned char fim119_data[40] = 
"name Uzbek\nlanguage uz\n\nstatus testing\n"
		;
		autoFileInMemory fim119 = FileInMemory_createWithData (39, fim119_data, true, U"./data/lang/trk/uz");
		my addItem_move (fim119.move());

		static unsigned char fim120_data[238] = 
"name Estonian\nlanguage et\n\nstressAmp 18 16 22 22 20 22 22 22\nstressLength 150 180 200 200 0 0 210 25"
"0\nstressOpt 1 2 4 6 // (S_NO_DIM + S_FINAL_DIM = S_FINAL_DIM_ONLY), S_FINAL_NO_2, S_2_TO_HEAVY\nstres"
"sRule 0\n\nintonation 3\nspellingStress\n"
		;
		autoFileInMemory fim120 = FileInMemory_createWithData (237, fim120_data, true, U"./data/lang/urj/et");
		my addItem_move (fim120.move());

		static unsigned char fim121_data[238] = 
"name Finnish\nlanguage fi\n\n\nstressAmp 18 16 22 22 20 22 22 22\nstressLength 150 180 200 200 0 0 210 25"
"0\nstressOpt 1 2 4 6 // (S_NO_DIM + S_FINAL_DIM = S_FINAL_DIM_ONLY), S_FINAL_NO_2, S_2_TO_HEAVY\nstres"
"sRule 0\n\nintonation 3\nspellingStress\n"
		;
		autoFileInMemory fim121 = FileInMemory_createWithData (237, fim121_data, true, U"./data/lang/urj/fi");
		my addItem_move (fim121.move());

		static unsigned char fim122_data[74] = 
"name Hungarian\nlanguage hu\nbrackets 0\nbracketsAnnounced 0\npitch 81 117\n\n\n"
		;
		autoFileInMemory fim122 = FileInMemory_createWithData (73, fim122_data, true, U"./data/lang/urj/hu");
		my addItem_move (fim122.move());

		static unsigned char fim123_data[46] = 
"name Lule Saami\nlanguage smj\n\nstatus testing\n"
		;
		autoFileInMemory fim123 = FileInMemory_createWithData (45, fim123_data, true, U"./data/lang/urj/smj");
		my addItem_move (fim123.move());

		static unsigned char fim124_data[53] = 
"name Belarusian\nlanguage be\ndict_min  2000\nspeed 95\n"
		;
		autoFileInMemory fim124 = FileInMemory_createWithData (52, fim124_data, true, U"./data/lang/zle/be");
		my addItem_move (fim124.move());

		static unsigned char fim125_data[58] = 
"name Russian\nlanguage ru\nreplace 03 a a#\ndict_min  20000\n"
		;
		autoFileInMemory fim125 = FileInMemory_createWithData (57, fim125_data, true, U"./data/lang/zle/ru");
		my addItem_move (fim125.move());

		static unsigned char fim126_data[281] = 
"name Russian (Latvia)\nlanguage ru-lv 2\n\nmaintainer Valdis Vitolins <valdis.vitolins@odo.lv>\nstatus t"
"esting\n\nphonemes ru-lv\ndictrules 2\ndict_min  20000\nspeed 95\n\nwords 0 2\ntone 150 220 450 255 750 20 3"
"500 255\nstressAmp 12 10 8 8 0 0 16 17\nstressLength 160 140 200 140 0 0 240 160\n\n"
		;
		autoFileInMemory fim126 = FileInMemory_createWithData (280, fim126_data, true, U"./data/lang/zle/ru-LV");
		my addItem_move (fim126.move());

		static unsigned char fim127_data[92] = 
"name Russian (Classic)\nlanguage ru-cl\nreplace 03 a a#\ndict_min  20000\nspeed 95\ndictrules 3\n"
		;
		autoFileInMemory fim127 = FileInMemory_createWithData (91, fim127_data, true, U"./data/lang/zle/ru-cl");
		my addItem_move (fim127.move());

		static unsigned char fim128_data[98] = 
"name Ukrainian\nlanguage uk\n\nmaintainer Andrij Mizyk <andm1zyk@proton.me>\nstatus testing\n\nspeed 80"
		;
		autoFileInMemory fim128 = FileInMemory_createWithData (97, fim128_data, true, U"./data/lang/zle/uk");
		my addItem_move (fim128.move());

		static unsigned char fim129_data[112] = 
"name Bulgarian\nlanguage bg\n\nstressAmp 13 12 17 17 20 22 22 21 \nstressLength 180 170  200 200  200 20"
"0  210 220\n"
		;
		autoFileInMemory fim129 = FileInMemory_createWithData (111, fim129_data, true, U"./data/lang/zls/bg");
		my addItem_move (fim129.move());

		static unsigned char fim130_data[231] = 
"name Bosnian\nlanguage bs\nphonemes hr\n\npitch 81 120\nformant 0 100 100 100\nformant 1  97  97 100\nforma"
"nt 2  97  97 100\nformant 3  97 102 100\nformant 4  97 102 100\nformant 5  97 102 100\n\nstressAdd 10 10 "
"0 0 0 0 -30 -30\ndictrules 3 4\n"
		;
		autoFileInMemory fim130 = FileInMemory_createWithData (230, fim130_data, true, U"./data/lang/zls/bs");
		my addItem_move (fim130.move());

		static unsigned char fim131_data[263] = 
"name Croatian\nlanguage hr\nlanguage hbs\n\n// attributes towards !variant3\npitch 81 120\nformant 0 100 1"
"00 100\nformant 1  97  97 100\nformant 2  97  97 100\nformant 3  97 102 100\nformant 4  97 102 100\nforma"
"nt 5  97 102 100\n\nstressAdd 10 10 0 0 0 0 -30 -30\ndictrules 1\n"
		;
		autoFileInMemory fim131 = FileInMemory_createWithData (262, fim131_data, true, U"./data/lang/zls/hr");
		my addItem_move (fim131.move());

		static unsigned char fim132_data[29] = 
"name Macedonian\nlanguage mk\n"
		;
		autoFileInMemory fim132 = FileInMemory_createWithData (28, fim132_data, true, U"./data/lang/zls/mk");
		my addItem_move (fim132.move());

		static unsigned char fim133_data[44] = 
"name Slovenian\nlanguage sl\n\nstatus testing\n"
		;
		autoFileInMemory fim133 = FileInMemory_createWithData (43, fim133_data, true, U"./data/lang/zls/sl");
		my addItem_move (fim133.move());

		static unsigned char fim134_data[251] = 
"name Serbian\nlanguage sr\n\n// attributes towards !variant3 pitch 80 120\nformant 0 100 100 100\nformant"
" 1  97  97 100\nformant 2  97  97 100\nformant 3  97 102 100\nformant 4  97 102 100\nformant 5  97 102 1"
"00\n\nstressAdd 10 10 0 0 0 0 -30 -30\ndictrules 2 4\n"
		;
		autoFileInMemory fim134 = FileInMemory_createWithData (250, fim134_data, true, U"./data/lang/zls/sr");
		my addItem_move (fim134.move());

		static unsigned char fim135_data[24] = 
"name Czech\nlanguage cs\n"
		;
		autoFileInMemory fim135 = FileInMemory_createWithData (23, fim135_data, true, U"./data/lang/zlw/cs");
		my addItem_move (fim135.move());

		static unsigned char fim136_data[39] = 
"name Polish\nlanguage pl\n\nintonation 2\n"
		;
		autoFileInMemory fim136 = FileInMemory_createWithData (38, fim136_data, true, U"./data/lang/zlw/pl");
		my addItem_move (fim136.move());

		static unsigned char fim137_data[25] = 
"name Slovak\nlanguage sk\n"
		;
		autoFileInMemory fim137 = FileInMemory_createWithData (24, fim137_data, true, U"./data/lang/zlw/sk");
		my addItem_move (fim137.move());

	} catch (MelderError) {
		Melder_throw (U"Not everything was added to the FileInMemorySet.");
	}
}

/* End of file espeak_praat_FileInMemorySet_addLanguages.cpp */
