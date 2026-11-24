// =============================================================================
//  HOTKEY (formerly "Encoder") — plugin header
//  Shared declarations every module file includes.
// =============================================================================
#pragma once
#include <rack.hpp>


using namespace rack;

// The plugin itself — defined once in plugin.cpp.
extern Plugin* pluginInstance;

// ---- One line per module ----------------------------------------------------
extern Model* modelEncoder;

// ---- Plugin-wide settings ---------------------------------------------------
// These are about YOUR hardware and taste, not about a particular patch, so
// they're saved once in Rack's settings (not in each patch) and apply to
// every Encoder module. Defined and saved in plugin.cpp.
extern bool settingReverseDial;   // flip which way the dial/wheel turns knobs
extern bool settingShowValue;     // show the knob's value while it's being turned
extern bool settingDoubleTapReset; // double-tap dial push on an unlinked knob = double-click it (reset)
extern bool settingWheelClickPush; // mouse wheel click acts as the dial push
extern bool settingMultiHold;      // several numbers held = the dial turns all those knobs together
extern bool settingShowGroupModules; // outline + slot tag on every module in the current group
extern bool settingPageFullBlock; // page readout: false = real range ("11-14"), true = full block ("11-20")

// ---- Remaps: your own numbering for a type of module ------------------------
// Normally knobs/buttons are numbered by panel position. A remap replaces
// that order for EVERY module of one type (e.g. every VCV VCO), in every
// patch. Stored as the param numbers in the order you want them.
struct Remap {
	std::vector<int> knobs;     // param IDs: knob 1 first
	std::vector<int> buttons;   // param IDs: button Q first
	// PUSH LINKS: which control the dial's push presses for a knob.
	// knob param ID -> the param ID to press (the knob itself, for a push
	// encoder like Uno's, or a separate button).
	std::map<int, int> pushLinks;
	// PER-KNOB SENSITIVITY: knob param ID -> its own sensitivity multiplier
	// (set by holding its number + numpad + / -). Knobs not listed use the
	// overall sensitivity.
	std::map<int, float> knobSens;
	// CUSTOM KEYS (custom mode): control param ID -> its own key. Extra to
	// the normal numbers / letters; a knob = hold its key + turn, a button
	// = tap its key. Works on any page.
	std::map<int, int> customKeys;
	// WRAP ROUND: knob param ID -> true = wraps (past the end comes round to
	// the start, like an endless encoder), false = stops at its ends. Only
	// knobs you've changed are listed; the rest follow the automatic guess
	// (SELECTED MODULE > Knobs that wrap round).
	std::map<int, bool> wrapKnobs;

	bool empty() const {
		return knobs.empty() && buttons.empty() && pushLinks.empty() && knobSens.empty() && customKeys.empty() &&
			wrapKnobs.empty();
	}
	bool operator==(const Remap& o) const {
		return knobs == o.knobs && buttons == o.buttons && pushLinks == o.pushLinks &&
			knobSens == o.knobSens && customKeys == o.customKeys && wrapKnobs == o.wrapKnobs;
	}
};

// ---- Keys (learnable, saved for all patches) --------------------------------
// Every key the Encoder listens for. Right-click > Keys lets you change any
// of them by pressing the key you want ("Learn"); "Reset keys to default"
// puts them back. Saved once in Rack's settings, so they follow you to
// every patch — they're about your keyboard and hands, not a patch.
struct KeyBinding {
	int key = -1;    // GLFW key code (-1 = none)
	int mods = 0;    // Shift / Ctrl / Alt that must be held with it
	KeyBinding() {}
	KeyBinding(int key, int mods) : key(key), mods(mods) {}
	bool operator==(const KeyBinding& o) const { return key == o.key && mods == o.mods; }
};

static const int KEY_ROW = 10;   // 10 knob keys, 10 button keys, 10 slot keys

struct KeyMap {
	KeyBinding push;                 // the dial's push (any extra Ctrl etc. is ignored)
	KeyBinding turnUp, turnDown;     // turn the held knob without a dial
	KeyBinding sensUp, sensDown;     // sensitivity + / -
	KeyBinding groupPrev, groupNext, groupNew, groupClear;
	KeyBinding pageNext;   // one tap = the selected module's next page (wraps)
	// Optional separate page key for BUTTONS (modules with more than 10).
	// Set: pageNext flips only knobs, this flips only buttons. Not set (-1):
	// buttons follow the knob pages. (Either way, 10 or fewer buttons are
	// playable on every page.)
	KeyBinding buttonPageNext;
	// NUMPAD MODES (used by the MIDI-friendly layout): the numpad digits do
	// one job at a time — KNOBS, BUTTONS or SLOTS — and the mode key cycles
	// between them. In SLOTS mode, hold the assign key + a digit to assign.
	bool padModes = false;
	KeyBinding modeKey, padAssignKey;
	KeyBinding padSelectKey;   // held + a digit = select that slot, from ANY mode
	KeyBinding lockKey;        // lock / unlock the Encoder's keys (always works)
	int knobs[KEY_ROW];      // hold one to pick a knob on the current page
	int buttons[KEY_ROW];    // tap one to press a button on the current page
	int slots[KEY_ROW];      // with Shift = select slot, with Alt = assign slot
	// The modifiers held with a slot key: select (default Shift) and assign
	// (default Alt). Changeable from a short list in the Keys menu — learning
	// a modifier on its own doesn't work (it looks like the start of a combo).
	// Not used by the Numpad layout (its slots use Enter / * + a digit).
	int slotSelectMods = GLFW_MOD_SHIFT;
	int slotAssignMods = GLFW_MOD_ALT;
	// The MOUSE gestures' modifiers (held while clicking a control of the
	// selected module). All three must be different. Picked from a list in
	// Key mapping > MOUSE, like the slot modifiers.
	int remapMods = GLFW_MOD_ALT;                          // hold number + click = renumber
	int pushLinkMods = GLFW_MOD_ALT | RACK_MOD_CTRL;     // hold number + click = set / clear knob push
	int customMods = GLFW_MOD_ALT | GLFW_MOD_SHIFT;         // click = give it its own key

	KeyMap() { setDefaults(); }

	void setDefaults() {
		push = {GLFW_KEY_END, 0};
		// Turn up / down without a dial: = and - , right beside the number
		// row (one hand: hold a number, tap = or -). Not the arrow keys —
		// Rack grabs those for scrolling. Rack only uses - and = WITH Ctrl
		// (zoom); the group keys use them WITH Shift.
		turnUp = {GLFW_KEY_EQUAL, 0};
		turnDown = {GLFW_KEY_MINUS, 0};
		sensUp = {GLFW_KEY_KP_ADD, 0};
		sensDown = {GLFW_KEY_KP_SUBTRACT, 0};
		groupPrev = {GLFW_KEY_MINUS, GLFW_MOD_SHIFT};
		groupNext = {GLFW_KEY_EQUAL, GLFW_MOD_SHIFT};
		groupNew = {GLFW_KEY_EQUAL, GLFW_MOD_ALT};
		groupClear = {GLFW_KEY_DELETE, GLFW_MOD_ALT};
		// Next page: numpad / — one tap (same feel as the MIDI
		// layout's numpad / mode key, instead of Shift + slot key again).
		pageNext = {GLFW_KEY_KP_DIVIDE, 0};
		buttonPageNext = {GLFW_KEY_KP_MULTIPLY, 0};   // right beside numpad /
		padModes = false;
		modeKey = KeyBinding();
		padAssignKey = KeyBinding();
		padSelectKey = KeyBinding();
		lockKey = {GLFW_KEY_SCROLL_LOCK, 0};   // fittingly
		slotSelectMods = GLFW_MOD_SHIFT;
		slotAssignMods = GLFW_MOD_ALT;
		remapMods = GLFW_MOD_ALT;
		pushLinkMods = GLFW_MOD_ALT | RACK_MOD_CTRL;
		customMods = GLFW_MOD_ALT | GLFW_MOD_SHIFT;
		const int numberRow[KEY_ROW] = {GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5,
			GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9, GLFW_KEY_0};
		const int letterRow[KEY_ROW] = {GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_T,
			GLFW_KEY_Y, GLFW_KEY_U, GLFW_KEY_I, GLFW_KEY_O, GLFW_KEY_P};
		for (int i = 0; i < KEY_ROW; i++) {
			knobs[i] = numberRow[i];
			buttons[i] = letterRow[i];
			slots[i] = numberRow[i];
		}
	}

	// ---- Ready-made layouts (right-click > Keys > Layout) -------------------
	//   STANDARD: the defaults above.
	//   LAPTOP:   no numpad — sensitivity on [ and ], push on \ , next page `.
	//   MIDI:     for Rack's "QWERTY keyboard" MIDI, which plays notes on
	//             ` 1 2 3 5 6 7 9 0 = [ ]  Q-P  Z-M , . ; / L S D G H J and
	//             IGNORES Shift/Ctrl/Alt — so EVERYTHING moves to the numpad
	//             block and the keys above it, which it never touches:
	//               numpad /      : mode  KNOBS -> BUTTONS -> SLOTS
	//               numpad 1-0    : hold = knob / tap = button / tap = slot
	//               numpad * held : + a digit = assign slot (any mode)
	//               numpad Enter held : + a digit = select slot (any mode)
	//               numpad + / -  : sensitivity
	//               numpad .      : next page
	//               PgUp / PgDn   : turn (while holding a knob digit)
	//               Shift+PgUp/Dn : group next / previous; Alt+PgUp: new group
	//               End           : push
	//             (Not F-keys: on some PCs, Windows / the keyboard took
	//              F1, F3, F4 and F11 before Rack ever saw them. Not the
	//              scattered free letters either — too awkward to play.)
	enum Layout { LAYOUT_STANDARD, LAYOUT_LAPTOP, LAYOUT_MIDI, NUM_LAYOUTS };
	// Named by HOW they play (the numpad one suits everyone, not just
	// keyboard-MIDI users — it's simply also safe with keyboard MIDI).
	static const char* layoutName(int l) {
		static const char* names[NUM_LAYOUTS] = {
			"Standard - two hands, no modes",
			"Laptop - no numpad",
			"Numpad - one hand, modes (safe with keyboard MIDI)"};
		return (l >= 0 && l < NUM_LAYOUTS) ? names[l] : "Custom";
	}
	static const char* layoutShortName(int l) {
		static const char* names[NUM_LAYOUTS] = {"Standard", "Laptop", "Numpad"};
		return (l >= 0 && l < NUM_LAYOUTS) ? names[l] : "Custom";
	}

	void setLayout(int layout) {
		setDefaults();
		if (layout == LAYOUT_LAPTOP) {
			sensUp = {GLFW_KEY_RIGHT_BRACKET, 0};
			sensDown = {GLFW_KEY_LEFT_BRACKET, 0};
			push = {GLFW_KEY_BACKSLASH, 0};
			pageNext = {GLFW_KEY_GRAVE_ACCENT, 0};   // ` (left of 1): no numpad /
			buttonPageNext = KeyBinding();           // none: buttons follow knob pages
		}
		else if (layout == LAYOUT_MIDI) {
			for (int i = 0; i < KEY_ROW; i++) {
				knobs[i] = -1;      // all on the numpad digits, by mode
				buttons[i] = -1;
				slots[i] = -1;
			}
			padModes = true;
			pageNext = {GLFW_KEY_KP_DECIMAL, 0};   // numpad / is the mode key here
			buttonPageNext = KeyBinding();         // numpad * is the assign key here
			modeKey = {GLFW_KEY_KP_DIVIDE, 0};
			padAssignKey = {GLFW_KEY_KP_MULTIPLY, 0};
			padSelectKey = {GLFW_KEY_KP_ENTER, 0};
			// - and = are taken here (a button key, and a MIDI note), so
			// turning uses PgUp / PgDn on their own; groups use them + Shift.
			turnUp = {GLFW_KEY_PAGE_UP, 0};
			turnDown = {GLFW_KEY_PAGE_DOWN, 0};
			groupPrev = {GLFW_KEY_PAGE_DOWN, GLFW_MOD_SHIFT};
			groupNext = {GLFW_KEY_PAGE_UP, GLFW_MOD_SHIFT};
			groupNew = {GLFW_KEY_PAGE_UP, GLFW_MOD_ALT};   // (Insert is a slot key here)
		}
	}

	bool operator==(const KeyMap& o) const {
		if (!(push == o.push && turnUp == o.turnUp && turnDown == o.turnDown && sensUp == o.sensUp &&
				sensDown == o.sensDown && groupPrev == o.groupPrev && groupNext == o.groupNext &&
				groupNew == o.groupNew && groupClear == o.groupClear && pageNext == o.pageNext && buttonPageNext == o.buttonPageNext &&
				padModes == o.padModes && modeKey == o.modeKey && padAssignKey == o.padAssignKey &&
				padSelectKey == o.padSelectKey && lockKey == o.lockKey &&
				slotSelectMods == o.slotSelectMods && slotAssignMods == o.slotAssignMods &&
				remapMods == o.remapMods && pushLinkMods == o.pushLinkMods && customMods == o.customMods))
			return false;
		for (int i = 0; i < KEY_ROW; i++)
			if (knobs[i] != o.knobs[i] || buttons[i] != o.buttons[i] || slots[i] != o.slots[i]) return false;
		return true;
	}

	// Which ready-made layout these keys exactly match, or -1 (= custom).
	int matchingLayout() const {
		for (int l = 0; l < NUM_LAYOUTS; l++) {
			KeyMap m;
			m.setLayout(l);
			if (*this == m) return l;
		}
		return -1;
	}

	// Can knob badges show real numbers 1-40 (the key is the last digit)?
	// Yes while the knob keys are the number row, or numpad only.
	bool knobsShowNumbers() const {
		KeyMap d;
		bool numberRow = true, numpadOnly = true;
		for (int i = 0; i < KEY_ROW; i++) {
			if (knobs[i] != d.knobs[i]) numberRow = false;
			if (knobs[i] != -1) numpadOnly = false;
		}
		return numberRow || numpadOnly;
	}
};
extern KeyMap settingKeys;

// ---- Extra dials (keyboards / macropads with more than one knob) -----------
// The MAIN dial (scroll or the turn keys, set up by "Set up my dial...")
// works as always: hold a number + turn. Up to 4 EXTRA dials can be learned
// on top of it. The computer can't tell knobs apart, so each extra dial is
// known by the keys it sends: one key for right, one for left, and an
// optional push key (set your knobs to send their own keys, e.g. F13-F24,
// in VIA / QMK or the keyboard's software, then "Learn"). By default extra
// dial N turns knob N of the selected module directly (no number to hold),
// on the current page. Saved for all patches, like the keys. Nothing is
// learned by default, so nothing changes until you set one up.
static const int NUM_DIALS = 4;
// What an extra dial turns: 0-9 = knob 1-10 of the selected module, or:
static const int DIAL_HELD = KEY_ROW;       // the held knob (like the main dial)
static const int DIAL_NAV = KEY_ROW + 1;    // the navigation knob (pick module; push = next page)
static const int NUM_DIAL_TARGETS = KEY_ROW + 2;
static const int NUM_DIAL_SENS = 5;
static const float DIAL_SENS[NUM_DIAL_SENS] = {0.25f, 0.5f, 1.f, 2.f, 4.f};
struct DialSlot {
	KeyBinding up, down;   // the keys it sends when turned right / left
	KeyBinding push;       // its push (optional, -1 = none)
	int target = 0;        // see above
	float sens = 1.f;      // on top of the overall / per-knob sensitivity
	bool invert = false;
	bool learned() const { return up.key >= 0 && down.key >= 0; }
};
extern DialSlot settingDials[NUM_DIALS];

// A short readable name for a key, e.g. "Q", "End", "Num+", "Shift+-".
std::string keyName(int key, int mods = 0);

// ---- Slot backups -----------------------------------------------------------
// The slots live in the Encoder module (saved with the patch). If that
// Encoder is deleted and a fresh one added, its slots would be lost — so the
// last few slot lists are also kept here. A fresh, empty Encoder restores
// the backup whose modules are actually in the current patch. (Every Rack
// module has a unique ID, so a backup only ever matches its own patch.)
// Each backup = every group's 10 slots, one after another.
static const size_t MAX_SLOT_BACKUPS = 10;
extern std::vector<std::vector<int64_t>> settingSlotBackups;   // newest first

// ---- Sensitivity --------------------------------------------------------------
// How far the dial moves a knob, as a multiplier of the basic step
// (1 click = 1% of a smooth knob's range). x1 = normal, x0.1 = fine, x8 = fast.
extern float settingSensitivity;
static const float SENS_MIN = 0.1f;
static const float SENS_MAX = 8.f;
static const float SENS_STEP = 1.5f;   // numpad + / - multiplies / divides by this
extern bool settingAcceleration;       // turn the dial faster -> bigger steps

// ---- Highlights (right-click > Highlights) ----------------------------------
// What's drawn on the rack, and in which colour. All saved once for every
// patch — it's about your eyes and screen, not a patch.
//
// The colour choices: the current amber / teal defaults, the Okabe-Ito set
// (designed to stay distinguishable for colour-blind viewers), and white.
struct HighlightColor {
	const char* name;
	unsigned char r, g, b;
};
static const HighlightColor HIGHLIGHT_COLORS[] = {
	{"Amber", 0xff, 0xb0, 0x30},
	{"Teal", 0x40, 0xd0, 0xc0},
	{"Orange (colour-blind safe)", 0xe6, 0x9f, 0x00},
	{"Sky blue (colour-blind safe)", 0x56, 0xb4, 0xe9},
	{"Bluish green (colour-blind safe)", 0x00, 0x9e, 0x73},
	{"Yellow (colour-blind safe)", 0xf0, 0xe4, 0x42},
	{"Blue (colour-blind safe)", 0x00, 0x72, 0xb2},
	{"Vermillion (colour-blind safe)", 0xd5, 0x5e, 0x00},
	{"Reddish purple (colour-blind safe)", 0xcc, 0x79, 0xa7},
	{"White", 0xf0, 0xf0, 0xf0},
};
static const int NUM_HIGHLIGHT_COLORS = sizeof(HIGHLIGHT_COLORS) / sizeof(HIGHLIGHT_COLORS[0]);
static const int COLOR_AMBER = 0, COLOR_TEAL = 1;

extern bool settingShowPlayedOutline;   // outline around the module being played
extern bool settingShowKnobBadges;      // number badges on its knobs
extern bool settingShowButtonBadges;    // letter badges on its buttons
extern bool settingShowSlotNumbers;     // tiny slot-key labels beside the panel's slot lights
// Colours are stored as 0xRRGGBB (any colour — picked with a colour slider
// or typed as a code like #40D0C0 in the menu).
extern int settingColorPlayed;
extern int settingColorGroup;          // group 1's colour (and any group left on "default")
extern int settingColorKnobBadges;
extern int settingColorButtonBadges;
extern int settingColorMapping;        // lit boxes / keys in the Key and MIDI mapping views
static inline int packRGB(const HighlightColor& c) { return (c.r << 16) | (c.g << 8) | c.b; }
static const int RGB_AMBER = 0xffb030, RGB_TEAL = 0x40d0c0;
extern float settingHighlightBrightness; // 0.2 - 1
extern bool settingFollowRoomBrightness; // dim with Rack's room brightness
extern bool settingFollowSelected;       // scroll the rack to a selected module that's off screen

// ---- Game controller (read directly — no MIDI set-up) ------------------------
// What each gamepad control does. The controls use the standard gamepad
// layout (Xbox names; PlayStation shown alongside): button 0-14 = A, B, X, Y,
// LB, RB, View, Menu, Guide, L-stick click, R-stick click, D-pad up / right /
// down / left; 100 + axis = sticks / triggers (0 L-stick across, 1 L-stick
// up-down, 2 R-stick across, 3 R-stick up-down, 4 LT, 5 RT). -1 = none.
enum PadAction {
	PA_MODULE_PREV, PA_MODULE_NEXT,   // previous / next module in the group
	PA_GROUP_PREV, PA_GROUP_NEXT,     // previous / next group
	PA_KNOB_A, PA_KNOB_B,             // sticks: knobs J1 / J2 (tilt = speed)
	PA_PRESS_A, PA_PRESS_B,           // ... and pressing them (like the dial push)
	PA_BTN1, PA_BTN2, PA_BTN3, PA_BTN4,   // the four gamepad buttons
	PA_BANK_PREV, PA_BANK_NEXT,       // move J1 / J2 to knobs 3-4, 5-6 ... and the four buttons to 5-8 ...
	PA_PAGE_PREV, PA_PAGE_NEXT,
	PA_UNDO, PA_HOME,                 // Rack's undo; home = group 1, first module, page 1
	NUM_PAD_ACTIONS
};
static const int PAD_MAP_VERSION = 2;   // (saved maps from an older layout are ignored)
static const int PAD_AXIS = 100;   // control ids 100+ are axes
struct PadMap {
	int control[NUM_PAD_ACTIONS];
	PadMap() { setDefaults(); }
	void setDefaults() {
		// Default layout: triggers = groups, bumpers = modules, D-pad up /
		// down = pages, D-pad left / right = which knobs / buttons the pad plays.
		static const int defaults[NUM_PAD_ACTIONS] = {
			4, 5,                            // LB / RB        module
			PAD_AXIS + 4, PAD_AXIS + 5,      // LT / RT        group
			PAD_AXIS + 1, PAD_AXIS + 3,      // left / right stick, up-down
			9, 10,                           // stick clicks
			0, 1, 2, 3,                      // A B X Y
			14, 12,                          // D-pad left / right   J1 J2 + buttons along
			13, 11,                          // D-pad down / up      page
			6, 7};                           // View / Menu    undo / home
		for (int i = 0; i < NUM_PAD_ACTIONS; i++) control[i] = defaults[i];
	}
	bool operator==(const PadMap& o) const {
		for (int i = 0; i < NUM_PAD_ACTIONS; i++) if (control[i] != o.control[i]) return false;
		return true;
	}
};
extern bool settingPadOn;    // use a game controller when one is plugged in (default on)
extern PadMap settingPadMap;

// ---- Dial calibration (set by the "Set up my dial" wizard) -------------------
// How many of Rack's scroll units one click of YOUR dial / wheel sends. A
// normal mouse wheel is 50; smooth-scrolling devices and some dials differ.
extern float settingScrollUnit;
extern bool settingDialSetupDone;      // has the wizard been run (or skipped)?

// ---- MIDI controller ("follow the selected module") --------------------------
// A hardware controller's knobs / buttons are linked to ROLES — "knob 3",
// "button 5", "slot 2", "next page" ... — not to fixed parameters. So the
// same physical knob plays knob 3 of WHATEVER module is selected.
// Saved once for all patches: it's your controller template.
// (PITCHBEND: the pitch bend wheel, used as a smooth high-resolution knob.
//  PROGRAM: a program change — some controllers' pads / bank buttons send
//  these; it acts as one press.)
enum MidiType { MIDI_NONE, MIDI_CC, MIDI_NOTE, MIDI_PITCHBEND, MIDI_PROGRAM };
// How a CC knob sends its turns. AUTO = decided while learning (see
// `detected`). Absolute = normal knob (0-127). The relative ones are endless
// encoders, which send "up a bit / down a bit" in one of three common codes.
enum MidiKnobMode { MODE_AUTO, MODE_ABSOLUTE, MODE_REL_64, MODE_REL_TWOS, MODE_REL_SIGNED, NUM_MIDI_MODES };
struct MidiMapping {
	int type = MIDI_NONE;
	int number = -1;                // CC number or note number, 0-127
	int mode = MODE_AUTO;           // what you chose (Auto by default)
	int detected = MODE_ABSOLUTE;   // what Auto found while learning
	bool invert = false;            // turn the direction round (a fader mounted upside
	                                //  down, an encoder sending the opposite code)
	bool toggle = false;            // the controller BUTTON is in toggle mode (sends "on"
	                                //  one press, "off" the next): every press = one tap
	int port = 0;                   // which MIDI input it came from: 0 = input 1, 1 = input 2
	int effectiveMode() const { return mode == MODE_AUTO ? detected : mode; }
	bool operator==(const MidiMapping& o) const {
		return type == o.type && number == o.number && mode == o.mode && detected == o.detected
			&& invert == o.invert && toggle == o.toggle && port == o.port;
	}
};
// Role name ("knob1" ... "knob10", "button1" ..., "slot1" ..., "pageNext",
// "pagePrev", "groupNext", "groupPrev", "navTurn", "navPush") -> mapping.
extern std::map<std::string, MidiMapping> settingMidiMap;
// How a normal (absolute) controller knob — a pot or fader with end stops —
// moves a module knob whose value doesn't match the pot's position:
//  CATCHUP  turning moves the knob straight away in the same direction, at a
//           speed that makes knob and pot meet at the end of travel; from then
//           on they move together. Never jumps, never dead. (Default.)
//  PICKUP   the knob ignores the pot until the pot reaches its value.
//  RELATIVE every pot movement is added to the knob, like an endless encoder.
// (Endless encoders don't need any of this — they only ever send "up / down".)
enum MidiPotMode { POT_CATCHUP, POT_PICKUP, POT_RELATIVE, NUM_POT_MODES };
extern int settingMidiPotMode;
extern json_t* settingMidiPortJ;    // the chosen MIDI input (driver / device / channel), as saved
extern midi::Port* activeMidiPort;  // the active Encoder's MIDI input (for saving)
extern json_t* settingMidiPortJ2;   // ... and the second MIDI input (a second controller)
extern midi::Port* activeMidiPort2;
json_t* midiMapToJson(const std::map<std::string, MidiMapping>& map);
std::map<std::string, MidiMapping> midiMapFromJson(json_t* mapJ);

// ---- Folded menu sections ---------------------------------------------------
// Click a section heading in the right-click menu to fold / unfold it. Which
// sections are folded is remembered (section name -> folded).
extern std::map<std::string, bool> settingMenuFolded;

// ---- Panel navigation encoder ----------------------------------------------
// Hold this modifier while turning the keyboard dial (or pressing its push)
// and it works the panel's navigation encoder instead. 0 = off.
extern int settingNavModifier;
// MODULE mode: true = turning selects modules directly; false = turning
// moves a highlight and PUSH selects (the default).
// Key = "PluginSlug/ModuleSlug", e.g. "Fundamental/VCO".
extern std::map<std::string, Remap> settingRemaps;
