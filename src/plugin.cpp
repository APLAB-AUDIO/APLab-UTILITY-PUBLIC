// =============================================================================
//  HOTKEY (formerly "Encoder") — plugin registration
//  Rack calls init() once when it loads the plugin. All we do here is tell
//  Rack which modules exist so they appear in the module browser.
//  (The C++ names — Encoder, modelEncoder, Encoder.cpp — are kept on purpose;
//   only what Rack and the user see was renamed, on 2026-11-02.)
// =============================================================================
#include "plugin.hpp"


Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;

	p->addModel(modelEncoder);

	// ---- Settings from before the rename (2026-11-02) ----------------------
	// Rack keeps each plugin's settings under its slug. This plugin used to be
	// "Encoder"; now it's "APLab-UTILITY". Rack only hands us settings saved
	// under the NEW slug (right after init), so if there are none yet, read
	// the old ones here. From the next save on they live under the new slug.
	// (Rack keeps the old entry too, so going back to an old build still works.)
	json_t* allJ = settings::pluginSettingsJ;
	if (allJ && !json_object_get(allJ, "APLab-UTILITY")) {
		if (json_t* oldJ = json_object_get(allJ, "Encoder")) settingsFromJson(oldJ);
	}
}


// =============================================================================
//  PLUGIN-WIDE SETTINGS
//  Rack calls these to save/load our settings along with its own settings
//  file, so they survive restarts and apply to every patch.
// =============================================================================
bool settingReverseDial = false;
bool settingShowValue = true;
bool settingPageFullBlock = false;
bool settingDoubleTapReset = true;
bool settingWheelClickPush = true;
bool settingMultiHold = true;
bool settingShowGroupModules = true;
float settingSensitivity = 1.f;
bool settingAcceleration = false;
float settingScrollUnit = 50.f;
bool settingShowPlayedOutline = true;
bool settingShowKnobBadges = true;
bool settingShowButtonBadges = true;
bool settingShowSlotNumbers = true;
int settingColorPlayed = RGB_AMBER;
int settingColorGroup = RGB_TEAL;
int settingColorKnobBadges = RGB_AMBER;
int settingColorButtonBadges = RGB_AMBER;
int settingColorMapping = RGB_TEAL;
float settingHighlightBrightness = 1.f;
bool settingFollowRoomBrightness = false;
bool settingFollowSelected = true;   // on: it only moves when the module is off screen
bool settingPadOn = true;
PadMap settingPadMap;
bool settingDialSetupDone = false;
// Alt, not Shift: Rack uses Shift + scroll itself (sideways scrolling, fast
// knob turning), and nothing of Rack's uses Alt + scroll.
int settingNavModifier = GLFW_MOD_ALT;
std::map<std::string, MidiMapping> settingMidiMap;
int settingMidiPotMode = POT_CATCHUP;
std::map<std::string, bool> settingMenuFolded;
json_t* settingMidiPortJ = nullptr;
json_t* settingMidiPortJ2 = nullptr;
midi::Port* activeMidiPort2 = nullptr;

// ---- MIDI mappings <-> JSON (settings, and "Save / Load mapping file") ------
// Each role = [type, number, mode, detected, invert, toggle, port]. Older
// saves are shorter: the missing ones take their defaults.
json_t* midiMapToJson(const std::map<std::string, MidiMapping>& map) {
	json_t* mapJ = json_object();
	for (const auto& kv : map) {
		const MidiMapping& m = kv.second;
		json_t* a = json_array();
		json_array_append_new(a, json_integer(m.type));
		json_array_append_new(a, json_integer(m.number));
		json_array_append_new(a, json_integer(m.mode));
		json_array_append_new(a, json_integer(m.detected));
		json_array_append_new(a, json_integer(m.invert ? 1 : 0));
		json_array_append_new(a, json_integer(m.toggle ? 1 : 0));
		json_array_append_new(a, json_integer(m.port));
		json_object_set_new(mapJ, kv.first.c_str(), a);
	}
	return mapJ;
}
std::map<std::string, MidiMapping> midiMapFromJson(json_t* mapJ) {
	std::map<std::string, MidiMapping> map;
	const char* role;
	json_t* a;
	json_object_foreach(mapJ, role, a) {
		MidiMapping m;
		auto get = [&](size_t i, int fallback) {
			json_t* x = json_array_get(a, i);
			return x ? (int) json_integer_value(x) : fallback;
		};
		m.type = get(0, MIDI_NONE);
		m.number = get(1, -1);
		m.mode = clamp(get(2, MODE_AUTO), 0, NUM_MIDI_MODES - 1);
		m.detected = clamp(get(3, MODE_ABSOLUTE), 0, NUM_MIDI_MODES - 1);
		m.invert = get(4, 0) != 0;
		m.toggle = get(5, 0) != 0;
		m.port = clamp(get(6, 0), 0, 1);
		if (m.type > MIDI_NONE && m.type <= MIDI_PROGRAM) map[role] = m;
	}
	return map;
}
// The active Encoder's MIDI input, so its choice can be saved (set by the
// panel; null when there's no Encoder).
midi::Port* activeMidiPort = nullptr;
std::vector<std::vector<int64_t>> settingSlotBackups;
KeyMap settingKeys;
DialSlot settingDials[NUM_DIALS] = {};
// (Each extra dial turns its own knob number by default: dial 1 = knob 1 ...)
static struct DialDefaults { DialDefaults() { for (int i = 0; i < NUM_DIALS; i++) settingDials[i].target = i; } } dialDefaults;

// A short readable name for a key, e.g. "Q", "End", "Num+", "Shift+-".
std::string keyName(int key, int mods) {
	std::string s;
	if (mods & RACK_MOD_CTRL) s += "Ctrl+";
	if (mods & GLFW_MOD_SHIFT) s += "Shift+";
	if (mods & GLFW_MOD_ALT) s += "Alt+";
	if (key < 0) return s + "none";

	// Keys with no printable letter (or where the letter would be unclear).
	if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_9) return s + string::f("Num%d", key - GLFW_KEY_KP_0);
	if (key >= GLFW_KEY_F1 && key <= GLFW_KEY_F25) return s + string::f("F%d", key - GLFW_KEY_F1 + 1);
	switch (key) {
		case GLFW_KEY_KP_ADD: return s + "Num+";
		case GLFW_KEY_KP_SUBTRACT: return s + "Num-";
		case GLFW_KEY_KP_MULTIPLY: return s + "Num*";
		case GLFW_KEY_KP_DIVIDE: return s + "Num/";
		case GLFW_KEY_KP_DECIMAL: return s + "Num.";
		case GLFW_KEY_KP_ENTER: return s + "NumEnter";
		case GLFW_KEY_END: return s + "End";
		case GLFW_KEY_HOME: return s + "Home";
		case GLFW_KEY_INSERT: return s + "Insert";
		case GLFW_KEY_DELETE: return s + "Delete";
		case GLFW_KEY_PAGE_UP: return s + "PgUp";
		case GLFW_KEY_PAGE_DOWN: return s + "PgDn";
		case GLFW_KEY_UP: return s + "Up";
		case GLFW_KEY_DOWN: return s + "Down";
		case GLFW_KEY_LEFT: return s + "Left";
		case GLFW_KEY_RIGHT: return s + "Right";
		case GLFW_KEY_ENTER: return s + "Enter";
		case GLFW_KEY_TAB: return s + "Tab";
		case GLFW_KEY_SPACE: return s + "Space";
		case GLFW_KEY_BACKSPACE: return s + "Backspace";
		case GLFW_KEY_PAUSE: return s + "Pause";
		case GLFW_KEY_SCROLL_LOCK: return s + "ScrollLock";
		case GLFW_KEY_CAPS_LOCK: return s + "CapsLock";
		case GLFW_KEY_MENU: return s + "Menu";
		default: break;
	}
	// Letters, numbers and symbols: what your keyboard layout prints.
	const char* n = glfwGetKeyName(key, 0);
	if (n && n[0]) return s + string::uppercase(n);
	return s + string::f("Key%d", key);
}

static json_t* bindingToJson(const KeyBinding& b) {
	json_t* a = json_array();
	json_array_append_new(a, json_integer(b.key));
	json_array_append_new(a, json_integer(b.mods));
	return a;
}
static void bindingFromJson(json_t* a, KeyBinding& b) {
	if (!a) return;
	json_t* k = json_array_get(a, 0);
	json_t* m = json_array_get(a, 1);
	if (k) b.key = (int) json_integer_value(k);
	if (m) b.mods = (int) json_integer_value(m);
}
static json_t* rowToJson(const int* row) {
	json_t* a = json_array();
	for (int i = 0; i < KEY_ROW; i++) json_array_append_new(a, json_integer(row[i]));
	return a;
}
static void rowFromJson(json_t* a, int* row) {
	if (!a) return;
	for (int i = 0; i < KEY_ROW; i++) {
		json_t* k = json_array_get(a, i);
		if (k) row[i] = (int) json_integer_value(k);
	}
}
std::map<std::string, Remap> settingRemaps;

// A list of numbers <-> a JSON array.
static json_t* intsToJson(const std::vector<int>& v) {
	json_t* a = json_array();
	for (int x : v) json_array_append_new(a, json_integer(x));
	return a;
}
static std::vector<int> intsFromJson(json_t* a) {
	std::vector<int> v;
	size_t i;
	json_t* x;
	if (a) json_array_foreach(a, i, x) v.push_back((int) json_integer_value(x));
	return v;
}

// ---- Remaps saved under old plugin / module slugs ---------------------------
// Remaps are stored as "PluginSlug/ModuleSlug". The slugs were renamed
// (2026-11-01 Orbital -> APLab-ORBITAL with new module slugs; 2026-11-02 this
// plugin, Encoder -> APLab-UTILITY / HOTKEY), so remaps saved before that are
// moved to the new names once, when settings load. If one already exists
// under the new name, that one wins and the old one is dropped.
static void renameOldRemapKeys() {
	static const char* const RENAMES[][2] = {
		{"Encoder/Encoder", "APLab-UTILITY/HOTKEY"},
		{"Orbital/Uno", "APLab-ORBITAL/UNO"},
		{"Orbital/MAS", "APLab-ORBITAL/MAS"},
		{"Orbital/Pulse", "APLab-ORBITAL/PULSE"},
		{"Orbital/Wave", "APLab-ORBITAL/WAVE"},
		{"Orbital/Wave2", "APLab-ORBITAL/WAVE2DEV"},
		{"Orbital/Random", "APLab-ORBITAL/GENESIS"},
		{"Orbital/Envelope", "APLab-ORBITAL/ENVELOPE"},
		{"Orbital/Horizon", "APLab-ORBITAL/ATMOSPHERE"},
	};
	for (const auto& r : RENAMES) {
		auto it = settingRemaps.find(r[0]);
		if (it == settingRemaps.end()) continue;
		if (!settingRemaps.count(r[1])) settingRemaps[r[1]] = it->second;
		settingRemaps.erase(it);
	}
}

json_t* settingsToJson() {
	json_t* rootJ = json_object();
	json_object_set_new(rootJ, "reverseDial", json_boolean(settingReverseDial));
	json_object_set_new(rootJ, "showValue", json_boolean(settingShowValue));
	json_object_set_new(rootJ, "pageFullBlock", json_boolean(settingPageFullBlock));
	json_object_set_new(rootJ, "doubleTapReset", json_boolean(settingDoubleTapReset));
	json_object_set_new(rootJ, "wheelClickPush", json_boolean(settingWheelClickPush));
	json_object_set_new(rootJ, "multiHold", json_boolean(settingMultiHold));
	json_object_set_new(rootJ, "showGroupModules", json_boolean(settingShowGroupModules));
	json_object_set_new(rootJ, "sensitivity", json_real(settingSensitivity));
	json_object_set_new(rootJ, "acceleration", json_boolean(settingAcceleration));
	json_object_set_new(rootJ, "scrollUnit", json_real(settingScrollUnit));
	json_object_set_new(rootJ, "followSelected", json_boolean(settingFollowSelected));
	json_t* padJ = json_object();
	json_object_set_new(padJ, "on", json_boolean(settingPadOn));
	json_t* padMapJ = json_array();
	for (int i = 0; i < NUM_PAD_ACTIONS; i++) json_array_append_new(padMapJ, json_integer(settingPadMap.control[i]));
	json_object_set_new(padJ, "map", padMapJ);
	json_object_set_new(padJ, "version", json_integer(PAD_MAP_VERSION));
	json_object_set_new(rootJ, "gamepad", padJ);
	json_t* hlJ = json_object();
	json_object_set_new(hlJ, "played", json_boolean(settingShowPlayedOutline));
	json_object_set_new(hlJ, "knobBadges", json_boolean(settingShowKnobBadges));
	json_object_set_new(hlJ, "buttonBadges", json_boolean(settingShowButtonBadges));
	json_object_set_new(hlJ, "slotNumbers", json_boolean(settingShowSlotNumbers));
	json_object_set_new(hlJ, "rgbPlayed", json_integer(settingColorPlayed));
	json_object_set_new(hlJ, "rgbGroup", json_integer(settingColorGroup));
	json_object_set_new(hlJ, "rgbKnobBadges", json_integer(settingColorKnobBadges));
	json_object_set_new(hlJ, "rgbButtonBadges", json_integer(settingColorButtonBadges));
	json_object_set_new(hlJ, "rgbMapping", json_integer(settingColorMapping));
	json_object_set_new(hlJ, "brightness", json_real(settingHighlightBrightness));
	json_object_set_new(hlJ, "followRoom", json_boolean(settingFollowRoomBrightness));
	json_object_set_new(rootJ, "highlights", hlJ);
	json_object_set_new(rootJ, "dialSetupDone", json_boolean(settingDialSetupDone));
	json_object_set_new(rootJ, "navMod", json_integer(settingNavModifier));

	json_t* midiJ = json_object();
	json_object_set_new(midiJ, "potMode", json_integer(settingMidiPotMode));
	// The MIDI inputs: the live ports if there are, else what was loaded.
	if (activeMidiPort) json_object_set_new(midiJ, "port", activeMidiPort->toJson());
	else if (settingMidiPortJ) json_object_set(midiJ, "port", settingMidiPortJ);
	if (activeMidiPort2) json_object_set_new(midiJ, "port2", activeMidiPort2->toJson());
	else if (settingMidiPortJ2) json_object_set(midiJ, "port2", settingMidiPortJ2);
	json_object_set_new(midiJ, "map", midiMapToJson(settingMidiMap));
	json_object_set_new(rootJ, "midi", midiJ);

	// (Both folded AND unfolded are remembered: some sections start folded.)
	json_t* foldJ = json_object();
	for (const auto& kv : settingMenuFolded)
		json_object_set_new(foldJ, kv.first.c_str(), json_boolean(kv.second));
	json_object_set_new(rootJ, "menuFolded", foldJ);

	json_t* remapsJ = json_object();
	for (const auto& kv : settingRemaps) {
		json_t* r = json_object();
		json_object_set_new(r, "knobs", intsToJson(kv.second.knobs));
		json_object_set_new(r, "buttons", intsToJson(kv.second.buttons));
		json_t* pushJ = json_object();
		for (const auto& link : kv.second.pushLinks)
			json_object_set_new(pushJ, std::to_string(link.first).c_str(), json_integer(link.second));
		json_object_set_new(r, "push", pushJ);
		json_t* sensJ = json_object();
		for (const auto& s : kv.second.knobSens)
			json_object_set_new(sensJ, std::to_string(s.first).c_str(), json_real(s.second));
		json_object_set_new(r, "sens", sensJ);
		json_t* customJ = json_object();
		for (const auto& ck : kv.second.customKeys)
			json_object_set_new(customJ, std::to_string(ck.first).c_str(), json_integer(ck.second));
		json_object_set_new(r, "custom", customJ);
		if (!kv.second.wrapKnobs.empty()) {
			json_t* wrapJ = json_object();
			for (const auto& wk : kv.second.wrapKnobs)
				json_object_set_new(wrapJ, std::to_string(wk.first).c_str(), json_boolean(wk.second));
			json_object_set_new(r, "wrap", wrapJ);
		}
		json_object_set_new(remapsJ, kv.first.c_str(), r);
	}
	json_object_set_new(rootJ, "remaps", remapsJ);

	json_t* backupsJ = json_array();
	for (const auto& b : settingSlotBackups) {
		json_t* bJ = json_array();
		for (int64_t id : b) json_array_append_new(bJ, json_integer(id));
		json_array_append_new(backupsJ, bJ);
	}
	json_object_set_new(rootJ, "slotBackups", backupsJ);

	json_t* keysJ = json_object();
	const KeyMap& k = settingKeys;
	json_object_set_new(keysJ, "push", bindingToJson(k.push));
	json_object_set_new(keysJ, "turnUp", bindingToJson(k.turnUp));
	json_object_set_new(keysJ, "turnDown", bindingToJson(k.turnDown));
	json_object_set_new(keysJ, "sensUp", bindingToJson(k.sensUp));
	json_object_set_new(keysJ, "sensDown", bindingToJson(k.sensDown));
	json_object_set_new(keysJ, "groupPrev", bindingToJson(k.groupPrev));
	json_object_set_new(keysJ, "groupNext", bindingToJson(k.groupNext));
	json_object_set_new(keysJ, "groupNew", bindingToJson(k.groupNew));
	json_object_set_new(keysJ, "groupClear", bindingToJson(k.groupClear));
	json_object_set_new(keysJ, "pageNext", bindingToJson(k.pageNext));
	json_object_set_new(keysJ, "buttonPageNext", bindingToJson(k.buttonPageNext));
	json_object_set_new(keysJ, "padModes", json_boolean(k.padModes));
	json_object_set_new(keysJ, "modeKey", bindingToJson(k.modeKey));
	json_object_set_new(keysJ, "padAssignKey", bindingToJson(k.padAssignKey));
	json_object_set_new(keysJ, "padSelectKey", bindingToJson(k.padSelectKey));
	json_object_set_new(keysJ, "lockKey", bindingToJson(k.lockKey));
	json_object_set_new(keysJ, "knobs", rowToJson(k.knobs));
	json_object_set_new(keysJ, "buttons", rowToJson(k.buttons));
	json_object_set_new(keysJ, "slots", rowToJson(k.slots));
	json_object_set_new(keysJ, "slotSelectMods", json_integer(k.slotSelectMods));
	json_object_set_new(keysJ, "slotAssignMods", json_integer(k.slotAssignMods));
	json_object_set_new(keysJ, "remapMods", json_integer(k.remapMods));
	json_object_set_new(keysJ, "pushLinkMods", json_integer(k.pushLinkMods));
	json_object_set_new(keysJ, "customMods", json_integer(k.customMods));
	json_object_set_new(rootJ, "keys", keysJ);
	// Extra dials (new 2026-11-14; older builds simply ignore this entry).
	json_t* dialsJ = json_array();
	for (int i = 0; i < NUM_DIALS; i++) {
		const DialSlot& d = settingDials[i];
		json_t* dJ = json_object();
		json_object_set_new(dJ, "up", bindingToJson(d.up));
		json_object_set_new(dJ, "down", bindingToJson(d.down));
		json_object_set_new(dJ, "push", bindingToJson(d.push));
		json_object_set_new(dJ, "target", json_integer(d.target));
		json_object_set_new(dJ, "sens", json_real(d.sens));
		json_object_set_new(dJ, "invert", json_boolean(d.invert));
		json_array_append_new(dialsJ, dJ);
	}
	json_object_set_new(rootJ, "dials", dialsJ);
	return rootJ;
}

void settingsFromJson(json_t* rootJ) {
	json_t* j;
	if ((j = json_object_get(rootJ, "reverseDial"))) settingReverseDial = json_boolean_value(j);
	if ((j = json_object_get(rootJ, "showValue"))) settingShowValue = json_boolean_value(j);
	// (Page readout is always the real range now — the option was removed
	//  from the menu, so an old "full block" choice isn't loaded.)
	settingPageFullBlock = false;
	if ((j = json_object_get(rootJ, "doubleTapReset"))) settingDoubleTapReset = json_boolean_value(j);
	if ((j = json_object_get(rootJ, "wheelClickPush"))) settingWheelClickPush = json_boolean_value(j);
	if ((j = json_object_get(rootJ, "multiHold"))) settingMultiHold = json_boolean_value(j);
	if ((j = json_object_get(rootJ, "showGroupModules"))) settingShowGroupModules = json_boolean_value(j);
	if ((j = json_object_get(rootJ, "sensitivity"))) settingSensitivity = clamp((float) json_number_value(j), SENS_MIN, SENS_MAX);
	if ((j = json_object_get(rootJ, "acceleration"))) settingAcceleration = json_boolean_value(j);
	if ((j = json_object_get(rootJ, "scrollUnit"))) settingScrollUnit = clamp((float) json_number_value(j), 1.f, 1000.f);
	if ((j = json_object_get(rootJ, "followSelected"))) settingFollowSelected = json_boolean_value(j);
	if (json_t* padJ = json_object_get(rootJ, "gamepad")) {
		if (json_t* onJ = json_object_get(padJ, "on")) settingPadOn = json_boolean_value(onJ);
		json_t* verJ = json_object_get(padJ, "version");
		bool current = verJ && json_integer_value(verJ) == PAD_MAP_VERSION;
		json_t* mapJ = current ? json_object_get(padJ, "map") : nullptr;   // older layout: defaults
		if (mapJ) {
			for (int i = 0; i < NUM_PAD_ACTIONS; i++)
				if (json_t* c = json_array_get(mapJ, i)) settingPadMap.control[i] = (int) json_integer_value(c);
		}
	}
	if (json_t* hlJ = json_object_get(rootJ, "highlights")) {
		auto getBool = [&](const char* k, bool& v) { if (json_t* x = json_object_get(hlJ, k)) v = json_boolean_value(x); };
		// Colours: 0xRRGGBB ("rgb..."). Older settings stored a number from the
		// colour list ("color...") — converted once.
		auto getColor = [&](const char* rgbKey, const char* oldKey, int& v) {
			if (json_t* x = json_object_get(hlJ, rgbKey)) v = (int) json_integer_value(x) & 0xffffff;
			else if (json_t* o = json_object_get(hlJ, oldKey))
				v = packRGB(HIGHLIGHT_COLORS[clamp((int) json_integer_value(o), 0, NUM_HIGHLIGHT_COLORS - 1)]);
		};
		getBool("played", settingShowPlayedOutline);
		getBool("knobBadges", settingShowKnobBadges);
		getBool("buttonBadges", settingShowButtonBadges);
		getBool("slotNumbers", settingShowSlotNumbers);
		getColor("rgbPlayed", "colorPlayed", settingColorPlayed);
		getColor("rgbGroup", "colorGroup", settingColorGroup);
		getColor("rgbKnobBadges", "colorKnobBadges", settingColorKnobBadges);
		getColor("rgbButtonBadges", "colorButtonBadges", settingColorButtonBadges);
		if (json_t* x = json_object_get(hlJ, "rgbMapping")) settingColorMapping = (int) json_integer_value(x) & 0xffffff;
		if (json_t* x = json_object_get(hlJ, "brightness")) settingHighlightBrightness = clamp((float) json_number_value(x), 0.2f, 1.f);
		getBool("followRoom", settingFollowRoomBrightness);
	}
	if ((j = json_object_get(rootJ, "dialSetupDone"))) settingDialSetupDone = json_boolean_value(j);
	// Extra dials (settings from before they existed have none: all empty).
	if (json_t* dialsJ = json_object_get(rootJ, "dials")) {
		for (int i = 0; i < NUM_DIALS; i++) {
			json_t* dJ = json_array_get(dialsJ, i);
			if (!dJ) continue;
			DialSlot d;
			d.target = i;
			bindingFromJson(json_object_get(dJ, "up"), d.up);
			bindingFromJson(json_object_get(dJ, "down"), d.down);
			bindingFromJson(json_object_get(dJ, "push"), d.push);
			if (json_t* x = json_object_get(dJ, "target")) d.target = clamp((int) json_integer_value(x), 0, NUM_DIAL_TARGETS - 1);
			if (json_t* x = json_object_get(dJ, "sens")) d.sens = clamp((float) json_number_value(x), DIAL_SENS[0], DIAL_SENS[NUM_DIAL_SENS - 1]);
			if (json_t* x = json_object_get(dJ, "invert")) d.invert = json_boolean_value(x);
			settingDials[i] = d;
		}
	}
	// (Saved as "navMod": the earlier test build saved Shift as "navModifier",
	//  which is deliberately ignored so everyone starts on the new Alt default.)
	if ((j = json_object_get(rootJ, "navMod"))) settingNavModifier = (int) json_integer_value(j);
	// ("navDirect" from older saves is ignored: turning always selects now.)

	settingMenuFolded.clear();
	if (json_t* foldJ = json_object_get(rootJ, "menuFolded")) {
		const char* name;
		json_t* v;
		json_object_foreach(foldJ, name, v) settingMenuFolded[name] = json_boolean_value(v);
	}

	settingMidiMap.clear();
	if (json_t* midiJ = json_object_get(rootJ, "midi")) {
		// (The old on/off "pickup" setting is ignored on purpose: it was only
		//  ever the default, so everyone moves to the new default, catch-up.)
		if (json_t* p = json_object_get(midiJ, "potMode")) {
			int m = (int) json_integer_value(p);
			if (m >= 0 && m < NUM_POT_MODES) settingMidiPotMode = m;
		}
		if (json_t* portJ = json_object_get(midiJ, "port")) {
			if (settingMidiPortJ) json_decref(settingMidiPortJ);
			settingMidiPortJ = json_deep_copy(portJ);
		}
		if (json_t* portJ = json_object_get(midiJ, "port2")) {
			if (settingMidiPortJ2) json_decref(settingMidiPortJ2);
			settingMidiPortJ2 = json_deep_copy(portJ);
		}
		if (json_t* mapJ = json_object_get(midiJ, "map")) settingMidiMap = midiMapFromJson(mapJ);
	}

	settingKeys.setDefaults();
	if (json_t* keysJ = json_object_get(rootJ, "keys")) {
		KeyMap& k = settingKeys;
		bindingFromJson(json_object_get(keysJ, "push"), k.push);
		bindingFromJson(json_object_get(keysJ, "turnUp"), k.turnUp);
		bindingFromJson(json_object_get(keysJ, "turnDown"), k.turnDown);
		bindingFromJson(json_object_get(keysJ, "sensUp"), k.sensUp);
		bindingFromJson(json_object_get(keysJ, "sensDown"), k.sensDown);
		bindingFromJson(json_object_get(keysJ, "groupPrev"), k.groupPrev);
		bindingFromJson(json_object_get(keysJ, "groupNext"), k.groupNext);
		bindingFromJson(json_object_get(keysJ, "groupNew"), k.groupNew);
		bindingFromJson(json_object_get(keysJ, "groupClear"), k.groupClear);
		bindingFromJson(json_object_get(keysJ, "pageNext"), k.pageNext);
		// (Older saved keys have no pageNext: it keeps its default — but if
		//  they're the numpad MIDI layout, use that layout's page key.)
		if (!json_object_get(keysJ, "pageNext") && k.padModes) k.pageNext = {GLFW_KEY_KP_DECIMAL, 0};
		bindingFromJson(json_object_get(keysJ, "buttonPageNext"), k.buttonPageNext);
		// Older saved keys: only Standard-like keys get the numpad * default.
		if (!json_object_get(keysJ, "buttonPageNext") && (k.padModes || k.pageNext.key != GLFW_KEY_KP_DIVIDE))
			k.buttonPageNext = KeyBinding();
		if (json_t* pm = json_object_get(keysJ, "padModes")) k.padModes = json_boolean_value(pm);
		bindingFromJson(json_object_get(keysJ, "modeKey"), k.modeKey);
		bindingFromJson(json_object_get(keysJ, "padAssignKey"), k.padAssignKey);
		bindingFromJson(json_object_get(keysJ, "padSelectKey"), k.padSelectKey);
		bindingFromJson(json_object_get(keysJ, "lockKey"), k.lockKey);
		// Older saved numpad layouts didn't have the select key yet.
		if (!json_object_get(keysJ, "padSelectKey") && k.padModes) k.padSelectKey = {GLFW_KEY_KP_ENTER, 0};
		rowFromJson(json_object_get(keysJ, "knobs"), k.knobs);
		rowFromJson(json_object_get(keysJ, "buttons"), k.buttons);
		rowFromJson(json_object_get(keysJ, "slots"), k.slots);
		// (Older saves have neither: they keep the defaults, Shift / Alt.)
		if (json_t* m = json_object_get(keysJ, "slotSelectMods")) k.slotSelectMods = (int) json_integer_value(m);
		if (json_t* m = json_object_get(keysJ, "slotAssignMods")) k.slotAssignMods = (int) json_integer_value(m);
		if (k.slotSelectMods == 0 || k.slotAssignMods == 0 || k.slotSelectMods == k.slotAssignMods) {
			k.slotSelectMods = GLFW_MOD_SHIFT;   // never allowed: a slot key on its own, or both the same
			k.slotAssignMods = GLFW_MOD_ALT;
		}
		// Mouse gestures (older saves: the defaults). All three must be set
		// and different, or they'd fight over the same click.
		if (json_t* m = json_object_get(keysJ, "remapMods")) k.remapMods = (int) json_integer_value(m);
		if (json_t* m = json_object_get(keysJ, "pushLinkMods")) k.pushLinkMods = (int) json_integer_value(m);
		if (json_t* m = json_object_get(keysJ, "customMods")) k.customMods = (int) json_integer_value(m);
		if (!k.remapMods || !k.pushLinkMods || !k.customMods || k.remapMods == k.pushLinkMods ||
				k.remapMods == k.customMods || k.pushLinkMods == k.customMods) {
			k.remapMods = GLFW_MOD_ALT;
			k.pushLinkMods = GLFW_MOD_ALT | RACK_MOD_CTRL;
			k.customMods = GLFW_MOD_ALT | GLFW_MOD_SHIFT;
		}

		// Settings saved before 2026-10-11 still have the OLD turn keys (the
		// arrows, which Rack grabs). If they're untouched, move them to the
		// new defaults so nobody is left with keys that don't work.
		if (k.turnUp == KeyBinding(GLFW_KEY_UP, 0) && k.turnDown == KeyBinding(GLFW_KEY_DOWN, 0)) {
			KeyMap d;
			k.turnUp = d.turnUp;
			k.turnDown = d.turnDown;
		}
	}

	settingSlotBackups.clear();
	if ((j = json_object_get(rootJ, "slotBackups"))) {
		size_t i;
		json_t* bJ;
		json_array_foreach(j, i, bJ) {
			// Every group's slots, one after another (10 per group).
			std::vector<int64_t> b;
			size_t k;
			json_t* idJ;
			json_array_foreach(bJ, k, idJ) b.push_back(json_integer_value(idJ));
			settingSlotBackups.push_back(b);
		}
	}

	settingRemaps.clear();
	if ((j = json_object_get(rootJ, "remaps"))) {
		const char* key;
		json_t* r;
		json_object_foreach(j, key, r) {
			Remap remap;
			remap.knobs = intsFromJson(json_object_get(r, "knobs"));
			remap.buttons = intsFromJson(json_object_get(r, "buttons"));
			json_t* pushJ = json_object_get(r, "push");
			if (pushJ) {
				const char* knobId;
				json_t* targetJ;
				json_object_foreach(pushJ, knobId, targetJ)
					remap.pushLinks[std::atoi(knobId)] = (int) json_integer_value(targetJ);
			}
			json_t* sensJ = json_object_get(r, "sens");
			if (sensJ) {
				const char* knobId;
				json_t* valueJ;
				json_object_foreach(sensJ, knobId, valueJ)
					remap.knobSens[std::atoi(knobId)] = clamp((float) json_number_value(valueJ), SENS_MIN, SENS_MAX);
			}
			if (json_t* customJ = json_object_get(r, "custom")) {
				const char* paramId;
				json_t* keyJ;
				json_object_foreach(customJ, paramId, keyJ)
					remap.customKeys[std::atoi(paramId)] = (int) json_integer_value(keyJ);
			}
			if (json_t* wrapJ = json_object_get(r, "wrap")) {
				const char* paramId;
				json_t* onJ;
				json_object_foreach(wrapJ, paramId, onJ)
					remap.wrapKnobs[std::atoi(paramId)] = json_boolean_value(onJ);
			}
			settingRemaps[key] = remap;
		}
	}
	renameOldRemapKeys();
}
