#pragma once
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// draw the sprite here, coresspondence with the variable name
extern unordered_map<string, vector<wstring>> BUFFER_MAP = {
	{"title",
	 vector<wstring>{
		 L".'''.  l'''.  .'''.  .'''   .'''   'l'  l.  l  .''''",
		 L"l      l'''.  l   l   '''.   '''.   l   l '.l  l ''l",
		 L"'...'  l   l  '...'  '...'  '...'  .l.  l   l  '.l l",
		 L" ",
		 L"             l'''.  .'''.  .'''.  l'''.",
		 L"             l'''.  l   l  l.l '  l   l",
		 L"             l   l  '...'  l   l  l...'",
	 }},
	{"start",
	 vector<wstring>{
		 L"  PLAY  "}},
	{"load",
	 vector<wstring>{
		 L"  LOAD  "}},
	{"settings",
	 vector<wstring>{
		 L"SETTINGS"}},
	{"exit",
	 vector<wstring>{
		 L"  EXIT  "}},
	{"frame",
	 vector<wstring>{
		 L"╔═╗",
		 L"║ ║",
		 L"╚═╝"}},

	// player & enemies
	{
		"player",
		vector<wstring>{
			L"ll"}},
	{"enemy1", // blue
	 vector<wstring>{
		 L"l...  'l..",
		 L" '.'l'''l'",
		 L" .'.'...' ",
		 L".l' ' l '."}},
	{"enemy2", // yellow
	 vector<wstring>{
		 L" .l'l. ",
		 L".l '.l.",
		 L".l.' l.",
		 L"  l'l  "}},
	{"enemy3", // green
	 vector<wstring>{
		 L"  ....  ",
		 L".'l..l'.",
		 L"'.l''l.'",
		 L" .'  '. "}},
	{"enemy4", // red
	 vector<wstring>{
		 L" .l.   ..",
		 L"'   l.' l",
		 L".  .''. l",
		 L"l.'    'l"}},
	{"enemy5", // purple
	 vector<wstring>{
		 L"  .l.. l. ",
		 L".l  '.'  l",
		 L"'l..l l..l",
		 L" '..' '..'"}},
	{"enemy6", // orange
	 vector<wstring>{
		 L" l .. l .",
		 L"l'l''l'l'",
		 L"l.l..l.l.",
		 L" l '' l '"}},
	{"enemy7", // brown
	 vector<wstring>{
		 L" l ..l.. ",
		 L"l..'''.'l",
		 L"l''...'.l",
		 L" l ''l'' "}},
	{"enemy8", // pink
	 vector<wstring>{
		 L" .l.  .  ",
		 L".ll ''.''",
		 L"l' l..'..",
		 L"l. 'l  l "}},

	// select mode screen
	{
		"chooseTitle",
		vector<wstring>{
			L"▌│█║▌║▌║ CHOOSE MODE ║▌║▌║█│▌"}},
	{"levelMode",
	 vector<wstring>{
		 L"  TIMED MODE  "}},
	{"infinityMode",
	 vector<wstring>{
		 L" ENDLESS MODE "}},

	// score & level
	{
		"score",
		vector<wstring>{
			L"SCORE "}},
	{"level",
	 vector<wstring>{
		 L"LEVEL "}},

	// settings screen
	{
		"settingsTitle",
		vector<wstring>{
			L"▌│█║▌║▌║ SETTINGS ║▌║▌║█│▌"}},
	{"sound",
	 vector<wstring>{
		 L"SOUND:"}},
	{"on",
	 vector<wstring>{
		 L"  ON"}},
	{"off",
	 vector<wstring>{
		 L" OFF"}},
	{"back",
	 vector<wstring>{
		 L"   BACK   "}},
	// Clear level screen
	{
		"clearLevelTitle",
		vector<wstring>{
			L"▌│█║▌║▌║ LEVEL PASSED ! ║▌║▌║█│▌" // 9 each side incl spaces
		}},
	{"continueButton",
	 vector<wstring>{
		 L"  CONTINUE  "}},

	// pause screen
	{
		"pauseTitle",
		vector<wstring>{
			L"▌│█║▌║▌║ PAUSE ║▌║▌║█│▌" // 9 each side incl spaces
		}},
	{"resumeButton",
	 vector<wstring>{
		 L"  RESUME  "}},
	{"restartButton",
	 vector<wstring>{
		 L"  RESTART "}},
	{"saveButton",
	 vector<wstring>{
		 L"   SAVE   "}},

	// exit & game over screen
	{
		"exitTitle",
		vector<wstring>{
			L"YOU SHOULD SPENT MUCH MORE TIME TO PLAY INSTEAD OF STUDY :vVv"}},
	{
		"gameoverTitle",
		vector<wstring>{
			L"███████████████████████████████████████████████████──",
			L"█████████████──────────█████████─────────█████████───",
			L"████████████████████████████████████████████████─────",
			L"███████████████████████████████████████████████──────",
			L"──────────────────────────────────▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄──",
			L"────██───█──█───██─██──████──██──█─────────────────█─",
			L"────█─█──█──█──█───────█--───-█──█─█─────────────█─█─",
			L"────█───██──█───█████──█--█──██──█─────────────────█─",
			L"─────────────────────────────────▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──",
			L"█████████████████████████████████████████████████████",
			L"█████████████████████████████████████████████████████",
			L"████████────────████████████████──────────────███████",
			L"████████──██─█──████████████████────██─██─────███████",
			L"████████──█─██──████████████████────███─█─────███████",
			L"████████────────████████████████──────────────███████",
			L"█████████████████████████████████████████████████████",
			L"████████████───────────────────────────────██████████",
			L"█████████────██████████████████████████────██████████",
			L"████████████─────────────────────────────████████████",
		} // 53x19
	},
	{
		"inspirationalText",
		vector<wstring>{
			L"-- HahA Try AgalN, WhY NoT, ------  "} // 29x1
	},
	{
		"easterEgg1",
		vector<wstring>{
			L"           The game takes no time to load, but we made u wait cuz we hate u.",
			L"The name of the loading bar component in our code is litera''y \"Loading bar, cuz why not\""} // 90x2
	},
	{
		"easterEgg2",
		vector<wstring>{
			L"??????????????????????????????"} // 29x1
	},
	{"slots",
	 vector<wstring>{
		 L"Available slots"}},
	{"saveTitle",
	 vector<wstring>{
		 L"▌│█║▌║▌║ SAVE GAME ║▌║▌║█│▌"}},
	{"loadTitle",
	 vector<wstring>{
		 L"▌│█║▌║▌║ LOAD GAME ║▌║▌║█│▌"}},
	{"lightLine",
	 vector<wstring>{
		 L"- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"}}};

// Any buffer that requires padding to block
extern vector<string> CHAR_TO_BLOCK = {
	"player",
	"enemy1",
	"enemy2",
	"enemy3",
	"enemy4",
	"enemy5",
	"enemy6",
	"enemy7",
	"enemy8",
	"title",
	// "Some string here"
};

// Any buffer that requires a clear buffer (movable GameObject)
extern vector<string> CLEAR_BUFFER = {
	"player",
	"enemy1",
	"enemy2",
	"enemy3",
	"enemy4",
	"enemy5",
	"enemy6",
	"enemy7",
	"enemy8"};