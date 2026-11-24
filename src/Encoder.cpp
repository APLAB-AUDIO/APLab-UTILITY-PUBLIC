// =============================================================================
//  HOTKEY (formerly "Encoder") — the module
//
//  WHAT IT WILL DO (built one step at a time):
//    Lets the push-dial on a computer keyboard act like a real hardware
//    encoder for ANY module in the patch — fully hands-on, no mouse, like
//    playing a hardware modular without a MIDI controller.
//      - A hotkey picks a module (one of 10 slots).
//      - Hold a number 1-9 to pick one of its knobs.
//      - Turn or push the dial; it only acts while a number is held.
//    (Pointing at a knob and turning is already built into Rack:
//     View menu > "Control knobs with mouse wheel" — so we don't redo it.)
//
//  HOW THE DIAL REACHES US:
//    - Turning the dial = vertical scroll (set in iCUE). One click = 50 in
//      Rack's scroll units; fast turns can arrive as 100-200 in one go.
//    - Pushing the dial = a key; End for now (a Corsair K70's dial sends Ctrl+End).
//      The K70 dial only ever sends an instant tap, never a held push.
//
//  STEP 3: the module slots (10 since 2026-10-11).
//      Put a module in a slot (setup, before playing) — either:
//        - point at it and press Alt + slot key, or
//        - right-click Encoder > Slots > Slot N > Assign, then click it.
//      Alt + slot key over empty space clears that slot.
//      (Alt, because Ctrl+Shift+0 / Ctrl+Shift+= get swallowed by Windows
//       before Rack sees them, Ctrl+0/-/= are Rack's zoom keys, and Shift
//       is already used for selecting.)
//      Select a slot while playing:  Shift + slot key.
//      Slot keys: 1 2 3 4 5 6 7 8 9 0   (slots 1 to 10; was 12 with - =)
//
//  GROUPS: banks of 10 slots (up to 99). The slot keys act on the current
//      group; the slot lights show it; "GROUP n/m" above them.
//        Shift + -  / Shift + =  : previous / next group (wraps round)
//        Alt + =                 : new group (jump to it, then fill it)
//        Alt + Delete            : clear THIS group (undo: Ctrl+Z)
//        Right-click             : clear ALL groups (undoable too)
//      A group exists only while it has modules — an empty one vanishes as
//      soon as you leave it. The module you're playing stays selected while
//      you flip through groups. A module can be in several groups (once
//      per group), e.g. a mixer you want in every group.
//      The selected module gets a coloured outline; the 10 lights on the
//      panel show which slots are filled and which one is selected.
//    Slots (and which one is selected) are saved with the patch.
//
//  STEP 4a (this version): play the selected module's knobs.
//      Hold a number 1-9 and turn the dial -> that knob moves.
//      Works with anything that scrolls (keyboard dial OR mouse wheel),
//      and with the = / - keys for keyboards without a dial (was the
//      arrow keys, but Rack grabs those for scrolling).
//      While a number is held, scroll/arrows are kept for the knob, so
//      Rack doesn't scroll. Let go and everything is back to normal.
//      Knobs are numbered by where they sit on the panel: top row left to
//      right, then the next row down. Small number badges show on the
//      selected module; the held one lights up.
//      One dial click = 1% of the knob's range (stepped knobs: one step).
//      Sensitivity / acceleration come later. Push does nothing yet.
//
//  STEP 4b: play the selected module's buttons.
//      Letter row Q W E R T Y U I O = buttons/switches 1-9 (panel order),
//      tapped directly — no number or dial needed. Square letter badges.
//        - Push buttons: key down = pressed, key up = released, so a
//          module's own tap / hold / double-tap (e.g. Uno's) just works.
//        - Toggles / multi-way switches: each tap = next position, wraps.
//      Known clash for later: Rack's computer-keyboard MIDI uses letters.
//
//
//  STEP 5a: your own numbering (remap).
//      Hold a number, then Alt-click a control on the selected module:
//        knob   -> becomes that number;
//        button -> becomes the letter under that number (1=Q, 2=W ...).
//      It swaps places with whatever had that number. The remap applies to
//      EVERY module of that type, in every patch (saved in Rack settings).
//      Right-click > "Reset numbering to panel order" undoes it.
//      Control finding now searches the whole module (sub-panels too) and
//      counts custom control types as knobs unless they're plain on/off.
//
//
//  STEP 5b: push links.
//      Hold a knob's number, then Ctrl+Alt-click:
//        the knob itself -> its push is linked (push encoders, e.g. Uno's);
//        a button        -> that button becomes the knob's push.
//      Ctrl+Alt-click the same one again to unlink. A small dot under a
//      knob's badge = linked. Saved per module type, like remaps.
//      Then: hold the number + push the dial -> the module gets exactly the
//      press a mouse click would give it, and decides what it means itself
//      (so Uno's own tap / double-tap / hold work). Each press lasts at least
//      40 ms, because the K70 dial lets go instantly.
//      Right-click > "Reset push links to automatic" removes YOUR links
//      (and "no link"s) for that module type, back to the automatic ones.
//
//  STEP 6: automatic push links — most push encoders link themselves:
//        - known one-piece push encoders, recognised by type (Orbital's
//          placement encoders on Uno and Pulse) link to themselves;
//        - a button sitting right on top of a knob (the usual two-part
//          push encoder) becomes that knob's push.
//      Your own Ctrl+Alt-click always wins; Ctrl+Alt-clicking what a knob
//      is already linked to switches it off (even an automatic one).
//
//  KEYS (step 11a): every key above can be changed. Right-click > Keys lists
//      each action with its key; click one and press the new key ("Learn",
//      Esc cancels). Knob / button / slot keys each have their own list of
//      10. Clashes with the Encoder's other keys, or with Rack shortcuts we
//      know of, are warned about on the panel readout. "Reset keys to
//      default" puts everything back. Saved once for all patches.
//      Keys > Layout sets every key at once: Standard, Laptop (no numpad:
//      sensitivity [ ], push \), or MIDI-friendly — everything on the
//      numpad block, which Rack's QWERTY-keyboard MIDI never touches:
//      numpad / switches the digits between KNOBS, BUTTONS and SLOTS
//      (shown as "NUMPAD: KNOBS" above the group line; the badges of the kind
//      not being played are dimmed); in ANY mode, hold numpad * + digit to
//      assign a slot, hold numpad Enter + digit to select one; PgUp/PgDn turn; Shift+PgUp/PgDn groups; Alt+PgUp new.
//      "Layout: Custom" once you've learned your own keys.
//      (Numpad 1-0 always picks knobs too.) Keys Rack or Windows take first
//      (e.g. Ctrl+S) never arrive, so can't be learned; after 3 s of waiting
//      the readout hints "taken? Esc".
//
//  CURRENT GROUP ON MODULES (menu toggle, on by default): every module in
//      the current group gets a thin dim outline and a tag with its slot
//      key at the top, so you can see what the slot keys hit right now.
//
//  MIDI CONTROLLER MODE: right-click > MIDI input (any Rack MIDI driver,
//      incl. Loopback) and MIDI mappings. A controller knob / button is
//      linked to a ROLE (knob 1-10, button 1-10, slot 1-10, next / previous
//      page and group, navigation encoder) and plays it on whatever module
//      is selected. Learn (knob type auto-detected) or set CC / note and
//      type by hand. Pots / faders "catch up" by default (move straight
//      away, never jump; pickup and relative are menu choices). All patches.
//
//  PANEL CONTROLS (placeholder positions): the big NAVIGATION ENCODER
//      (endless; drag or MIDI-MAP it): turn = previous / next module in the
//      group, click = next page. No modes . < > beside slot 0 =
//      previous / next group. LOCK button. SENS knob = sensitivity.
//      Keyboard: hold Alt (menu: Shift / Ctrl / off) + turn the dial = turn
//      the navigation encoder; Alt + dial push = its push. (Alt, because Rack
//      itself uses Shift + scroll for sideways scrolling.)
//      LOCK (button or Scroll Lock, learnable): the Encoder ignores all its
//      keys, dial and clicks until unlocked; the panel says LOCKED. A MIDI
//      controller still works while locked (Lock is for typing).
//
//  CUSTOM MODE: Shift+Alt-click any control on the selected module and
//      press a key: that control gets its OWN key (Backspace removes it).
//      Knob = hold the key + turn; button = tap it. Works on every page,
//      alongside the normal numbers / letters; its badge shows the key.
//      Saved per module type.
//
//  UNDO: turns made with the dial / wheel / turn keys go into Rack's undo
//      list (Ctrl+Z) — one step per gesture (until the number is let go, a
//      different knob is turned, or a 1 s pause). Toggle / multi-way
//      switches pressed with keys are undoable too.
//
//  ONLY ONE ENCODER PER PATCH: the first is active; extra copies go
//      inactive (display: "ONLY ONE ENCODER inactive") and take over if the
//      active one is deleted. Slots are also backed up in Rack's settings,
//      so a fresh Encoder (newly added, or taking over) restores this
//      patch's slots instead of starting empty.
//
//  STEP 7: sensitivity and acceleration.
//      Right-click: Sensitivity slider (x0.1 - x8, x1 = 1% per click) and
//      "Acceleration" (turn faster = bigger steps, up to x6).
//      Numpad + / - while playing: alone = overall sensitivity; while
//      holding a number = THAT knob only, remembered per module type.
//      The panel readout's third line shows it ("x1.0", "KNOB x0.4" while
//      holding a knob with its own; "A" = acceleration on), and lights up
//      briefly when it changes. Right-click > "Reset knob sensitivities".
//      Knob WITHOUT a push link: double-tap the dial push = the same as a
//      mouse double-click on it (usually reset to default). Single tap does
//      nothing. Menu: "Double-tap push resets knob" (on by default).
//      Mouse wheel CLICK also works as the push (menu: "Mouse wheel click =
//      push", on by default) — for people using a mouse wheel as the dial.
//      It lights the blue push light, and mouse buttons report a real hold.
//
//  BUTTON PAGES (2026-10-11): a module with 10 or fewer buttons has them on
//      EVERY page. With more than 10, buttons page on their own with the
//      "Next button page" key (default numpad *; numpad / then flips only
//      knobs) — or, with no such key set, they follow the knob pages.
//      Readout shows "K2/2 B1/3" when the two differ.
//
//  PAGES: 0 counts as 10, so each page has 10 knobs (1-0) and 10 buttons
//      (Q-P). A module with more gets more pages (at most 4 = 40 each);
//      only the pages it needs exist. Press the selected module's slot key
//      (Shift + slot key) AGAIN to flip to the next page; it wraps round.
//      Knobs and buttons flip together. EVERY control shows a badge: the
//      current page bright, other pages dimmed. Knobs show their real
//      number (key = last digit: 15 = hold 5 on page 2); buttons show the
//      letter plus page number from page 2 ("W2").
//      The panel readout shows e.g. "PAGE 2/2  11-14" (right-click >
//      Page readout: real range, or full block "11-20").
//
//      Number row AND number pad both work. Right-click menu:
//        "Invert scroll / turn" and "Show value while turning"
//      (both saved once for all patches, not per patch).
//
//  WHERE THE WORK HAPPENS:
//    Everything here is keyboard/screen work, so it all lives on the UI
//    side (the panel / ModuleWidget). process() is empty, so the module
//    costs nothing on the audio thread.
// =============================================================================
#include "plugin.hpp"
#include <osdialog.h>   // Rack's file dialog (MIDI mapping files)
#include <keyboard.hpp>   // Rack's computer-keyboard MIDI (fed by the MIDI grid while learning)


// 10 slots, on the number row 1-0 — the same 10 as knobs and buttons per
// page. (Was 12 with - and =; cut to 10 on 2026-10-11 so - and = are free
// for switching groups later.)
static const int NUM_SLOTS = 10;

// THE KEYS themselves (which key is slot 3, knob 5, button W ...) live in
// settingKeys (plugin.hpp) so they can be learned / changed. Defaults:
//   slots + knobs: number row 1-0   buttons: Q-P   push: End
// The lookups below always read the CURRENT keys.

// Which slot a key belongs to, or -1 if it isn't a slot key.
static int slotForKey(int key) {
	for (int i = 0; i < NUM_SLOTS; i++)
		if (settingKeys.slots[i] == key) return i;
	return -1;
}

// The knob keys: 10 per page.
static const int NUM_KNOB_KEYS = KEY_ROW;
// The number pad also picks knobs 1-10 (always, as a second set of keys).
static const int KNOB_PAD_KEYS[NUM_KNOB_KEYS] = {
	GLFW_KEY_KP_1, GLFW_KEY_KP_2, GLFW_KEY_KP_3, GLFW_KEY_KP_4, GLFW_KEY_KP_5,
	GLFW_KEY_KP_6, GLFW_KEY_KP_7, GLFW_KEY_KP_8, GLFW_KEY_KP_9, GLFW_KEY_KP_0,
};

// NUMPAD MODES (layouts with padModes on, e.g. MIDI-friendly): the numpad
// digits do one job at a time, switched by the mode key.
enum PadMode { PAD_KNOBS, PAD_BUTTONS, PAD_SLOTS, NUM_PAD_MODES };
static const char* PAD_MODE_NAMES[NUM_PAD_MODES] = {"KNOBS", "BUTTONS", "SLOTS"};
static int padMode = PAD_KNOBS;

// Which numpad digit a key is (0 = numpad 1 ... 9 = numpad 0), or -1.
static int padDigit(int key) {
	for (int i = 0; i < NUM_KNOB_KEYS; i++)
		if (KNOB_PAD_KEYS[i] == key) return i;
	return -1;
}
// The label for numpad digit i: "1" ... "9", "0".
static std::string padLabel(int i) {
	return (i == 9) ? "0" : string::f("%d", i + 1);
}

// Which knob a key picks on the current page (0 = first), or -1.
// (The numpad picks knobs too — unless numpad modes are on and the numpad
//  is currently in BUTTONS or SLOTS mode.)
static int knobForKey(int key) {
	for (int i = 0; i < NUM_KNOB_KEYS; i++) {
		if (settingKeys.knobs[i] == key) return i;
		if (KNOB_PAD_KEYS[i] == key && (!settingKeys.padModes || padMode == PAD_KNOBS)) return i;
	}
	return -1;
}

// The button keys: 10 per page.
static const int NUM_BUTTON_KEYS = KEY_ROW;

// Does a key event match a learned key + its Shift/Ctrl/Alt exactly?
static bool keyMatches(const KeyBinding& b, int key, int mods) {
	return b.key >= 0 && key == b.key && mods == b.mods;
}

// PAGES: a module with more than 10 knobs (or buttons) gets more pages of
// 10. Knobs and buttons flip pages together. At most 4 pages = 40 each.
static const int PER_PAGE = 10;
static const int MAX_PAGES = 4;

// Which button a key presses on the current page (0 = first), or -1.
// (In numpad BUTTONS mode, the numpad digits press buttons.)
static int buttonForKey(int key) {
	for (int i = 0; i < NUM_BUTTON_KEYS; i++)
		if (settingKeys.buttons[i] == key) return i;
	if (settingKeys.padModes && padMode == PAD_BUTTONS) return padDigit(key);
	return -1;
}

// How long a button's value stays on screen after a tap (seconds).
static const double BUTTON_VALUE_SHOW_TIME = 1.2;

// How far one dial click moves a smooth knob, as a fraction of its range.
static const float CLICK_FRACTION = 0.01f;
// Rack reports one scroll-wheel/dial click as 50 scroll units.
// (Now measured per dial by the setup wizard: settingScrollUnit, default 50.)

// Colour of the selected-module outline and the slot lights.
static const NVGcolor SLOT_COLOR = nvgRGB(0xff, 0xb0, 0x30);   // amber
// Colour of the current group's outlines and tags — different from the
// played module's amber so the two are easy to tell apart. (Both become
// choosable in the Highlights step.)
static const NVGcolor GROUP_COLOR = nvgRGB(0x40, 0xd0, 0xc0);  // teal
// (Those two are the PANEL's colours. The highlights drawn on the rack use
//  the colours picked in right-click > Highlights — see hlColor below.)

// How bright the rack highlights are: the Highlights brightness slider,
// and (if "Follow room brightness" is on) Rack's room brightness too, so
// they dim along with the room instead of glaring.
static float highlightAlpha() {
	float a = settingHighlightBrightness;
	if (settingFollowRoomBrightness) a *= clamp(settings::rackBrightness, 0.15f, 1.f);
	return a;
}
// A colour stored as 0xRRGGBB -> a drawing colour.
static NVGcolor rgbColor(int rgb) {
	return nvgRGB((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff);
}
// A highlight colour (0xRRGGBB), at the highlight brightness.
static NVGcolor hlColor(int rgb, float extraAlpha = 1.f) {
	return nvgTransRGBAf(rgbColor(rgb), highlightAlpha() * extraAlpha);
}


// GROUPS: banks of 10 slots. There's always at least one. A group only
// exists while it has modules in it (except the one you're on, so you can
// fill a new one). At most 99, just so the readout stays readable.
static const int MAX_GROUPS = 99;


// =============================================================================
//  THE ENGINE SIDE
//  Makes no sound. It only HOLDS the groups of slots, so they live with the
//  module and are saved with the patch. Only the UI side touches them.
// =============================================================================
// ---- The SENS knob on the panel ----------------------------------------------
// Its position 0-1 maps to sensitivity x0.1 ... x8 on a log scale (like the
// menu slider), with x1 at its default. Hover shows "x1.0".
static float sensToKnob(float s) { return std::log(s / SENS_MIN) / std::log(SENS_MAX / SENS_MIN); }
static float sensFromKnob(float v) { return SENS_MIN * std::pow(SENS_MAX / SENS_MIN, clamp(v, 0.f, 1.f)); }
struct SensParamQuantity : ParamQuantity {
	std::string getDisplayValueString() override { return string::f("x%.2f", sensFromKnob(getValue())); }
	std::string getUnit() override { return ""; }
};

struct Encoder : Module {
	// The panel's own controls (placeholder positions until the real panel).
	// They're real Rack params, so VCV's MIDI-MAP can map a hardware knob /
	// buttons to them. They're only READ on the UI side — process() stays
	// empty, so the module still costs nothing on the audio thread.
	enum ParamId {
		NAV_PARAM,          // the navigation encoder (endless; 24 steps a turn): turn = module, push = next page
		MODE_PARAM,         // UNUSED since 2.0 (the MODE button is gone). Kept so
		                    // the ids after it don't shift in saved patches.
		LOCK_PARAM,         // lock: the Encoder ignores its keys until pressed again
		SENS_PARAM,         // sensitivity knob (kept in step with numpad + / - and the menu)
		GROUP_PREV_PARAM,   // the small buttons beside the 0 slot light
		GROUP_NEXT_PARAM,
		PARAMS_LEN
	};

	// Locked = ignore all of the Encoder's keys / dial (saved with the patch).
	bool locked = false;
	// AUTO-FILL (GROUPS & SLOTS > Fill slots automatically): every module in
	// the patch is put in a slot by itself, in rack order, 10 per group,
	// and kept up to date as modules come and go. Off = you fill slots by
	// hand (the default). Saved with the patch.
	bool autoFill = false;

	// Each group = 10 slots; each slot remembers a module by its Rack ID
	// number (-1 = empty). Always at least one group.
	std::vector<std::vector<int64_t>> groups;
	// The group the slot keys act on (the one the slot lights show).
	int currentGroup = 0;
	// The module being played: which group + slot. -1 = none. It stays
	// selected while you flip through other groups.
	int selectedGroup = -1;
	int selectedSlot = -1;

	// Each group's COLOUR (0xRRGGBB), shown on its outlines / tags, the slot
	// lights and the group readout. -1 = AUTOMATIC: decided by the group's
	// NUMBER (group 2 is always purple, ...), so when a group disappears and
	// the ones after it move down, they take their new number's colour.
	// Anything else = a colour you picked in Colours. Same length as groups.
	std::vector<int> groupColors;

	// The automatic group colours, in order: group 1 the default (teal), then
	// the colour-blind-safe set, ordered so each next group is as DIFFERENT as
	// possible from the one before. Yellow and orange come LAST: they're the
	// closest to the played module's amber outline, so they only appear once
	// a patch has many groups. Order: teal, reddish purple, white, blue,
	// vermillion, sky blue, yellow, bluish green, orange.
	static const int* colorRotation(int& count) {
		static const int rotation[] = {-1, 8, 9, 6, 7, 3, 5, 4, 2};   // (indices into HIGHLIGHT_COLORS)
		count = (int) (sizeof(rotation) / sizeof(rotation[0]));
		return rotation;
	}
	// The colour group g's NUMBER gives it: group 1 (and every 9th after) =
	// the group colour from Colours, the rest from the rotation above.
	static int positionGroupColor(int g) {
		int n;
		const int* rotation = colorRotation(n);
		int r = rotation[std::max(g, 0) % n];
		return r < 0 ? settingColorGroup : packRGB(HIGHLIGHT_COLORS[r]);
	}

	// ---- Automatic colours that keep out of the way of picked ones ----------
	// A group's automatic colour starts from its number. But if that colour is
	// the same as (or very close to) a colour you PICKED for another group, or
	// to the group just before it, it moves on to the next colour in the
	// rotation that's free. With no picked colours, every group simply gets
	// its number's colour. Picked colours are never changed.
	// Worked out only when the colours change (outlines ask every frame).
	mutable std::vector<int> colorCacheKey;   // groupColors it was worked out for
	mutable int colorCacheGroup = -2;         // ... and group 1's colour
	mutable std::vector<int> autoColors;      // each group's automatic colour

	static bool colorsClose(int a, int b) {
		int dr = ((a >> 16) & 0xff) - ((b >> 16) & 0xff);
		int dg = ((a >> 8) & 0xff) - ((b >> 8) & 0xff);
		int db = (a & 0xff) - (b & 0xff);
		return dr * dr + dg * dg + db * db < 60 * 60;   // hard to tell apart as outlines
	}
	void resolveColors() const {
		if (colorCacheKey == groupColors && colorCacheGroup == settingColorGroup) return;
		colorCacheKey = groupColors;
		colorCacheGroup = settingColorGroup;
		int count = (int) groupColors.size(), n;
		const int* rotation = colorRotation(n);
		bool group1Picked = settingColorGroup != RGB_TEAL;   // you changed group 1's colour
		autoColors.assign(count, settingColorGroup);
		int previous = -1;   // the colour the group before actually shows
		for (int g = 0; g < count; g++) {
			int pick = positionGroupColor(g);
			if (g > 0) {
				for (int t = 0; t < n; t++) {
					int r = rotation[(g + t) % n];
					int c = r < 0 ? settingColorGroup : packRGB(HIGHLIGHT_COLORS[r]);
					bool clash = (previous >= 0 && colorsClose(c, previous)) ||
						(group1Picked && colorsClose(c, settingColorGroup));
					for (int o = 0; o < count && !clash; o++)
						if (o != g && groupColors[o] >= 0 && colorsClose(c, groupColors[o])) clash = true;
					if (!clash) { pick = c; break; }
				}
			}
			autoColors[g] = pick;
			previous = (groupColors[g] >= 0) ? groupColors[g] : pick;
		}
	}
	// Group g's automatic colour (what it shows unless you picked one).
	int autoGroupColor(int g) const {
		resolveColors();
		return (g >= 0 && g < (int) autoColors.size()) ? autoColors[g] : positionGroupColor(g);
	}
	// Keep groupColors the same length as groups (new ones are automatic).
	void syncColors() {
		while (groupColors.size() < groups.size()) groupColors.push_back(-1);
		groupColors.resize(groups.size());
	}
	// The colour actually used for group g.
	int groupColor(int g) const {
		if (g < 0 || g >= (int) groupColors.size() || groupColors[g] < 0) return autoGroupColor(g);
		return groupColors[g];
	}
	// Set group g's colour from Colours. Picking its automatic colour (or
	// double-click = default) makes it automatic again.
	void setGroupColor(int g, int c) {
		if (g < 0 || g >= (int) groupColors.size()) return;
		groupColors[g] = (c == autoGroupColor(g)) ? -1 : c;
	}

	static std::vector<int64_t> emptyGroup() { return std::vector<int64_t>(NUM_SLOTS, -1); }

	Encoder() {
		config(PARAMS_LEN, 0, 0, 0);   // panel controls only: no inputs, outputs or lights
		configParam(NAV_PARAM, 0.f, 1.f, 0.f, "Navigate (turn)");
		paramQuantities[NAV_PARAM]->randomizeEnabled = false;
		paramQuantities[NAV_PARAM]->resetEnabled = false;
		configButton(MODE_PARAM, "(unused)");   // no widget; see MODE_PARAM
		configButton(LOCK_PARAM, "Lock / unlock Hotkey's keys");
		configParam<SensParamQuantity>(SENS_PARAM, 0.f, 1.f, sensToKnob(1.f), "Sensitivity");
		configButton(GROUP_PREV_PARAM, "Previous group");
		configButton(GROUP_NEXT_PARAM, "Next group");
		groups.assign(1, emptyGroup());
		syncColors();
	}

	bool groupEmpty(int g) const {
		for (int64_t id : groups[g]) if (id >= 0) return false;
		return true;
	}

	// Every slot of every group, one after another (for backups / undo).
	std::vector<int64_t> flatten() const {
		std::vector<int64_t> flat;
		for (const auto& g : groups) flat.insert(flat.end(), g.begin(), g.end());
		return flat;
	}

	// Rebuild the groups from a flat list (10 per group). Keeps at least one
	// group and keeps the indices valid.
	void unflatten(const std::vector<int64_t>& flat) {
		groups.clear();
		groupColors.clear();   // back to default colours (undo puts its own back after)
		for (size_t i = 0; i < flat.size(); i += NUM_SLOTS) {
			std::vector<int64_t> g = emptyGroup();
			for (int s = 0; s < NUM_SLOTS && i + s < flat.size(); s++) g[s] = flat[i + s];
			groups.push_back(g);
		}
		if (groups.empty()) groups.assign(1, emptyGroup());
		fixIndices();
	}

	void fixIndices() {
		syncColors();
		int n = (int) groups.size();
		if (currentGroup < 0 || currentGroup >= n) currentGroup = 0;
		if (selectedGroup >= n || selectedSlot < 0 || selectedSlot >= NUM_SLOTS) {
			selectedGroup = -1;
			selectedSlot = -1;
		}
	}

	// ---- Saved with the patch -----------------------------------------------
	// Slots point at modules in THIS patch (by their ID number, which Rack
	// keeps the same when the patch is saved and reopened), so they're saved
	// in the patch file — not in Rack's settings like the remaps are.
	json_t* dataToJson() override {
		json_t* rootJ = json_object();
		json_t* groupsJ = json_array();
		for (const auto& g : groups) {
			json_t* gJ = json_array();
			for (int64_t id : g) json_array_append_new(gJ, json_integer(id));
			json_array_append_new(groupsJ, gJ);
		}
		json_object_set_new(rootJ, "groups", groupsJ);
		json_t* colorsJ = json_array();
		for (int c : groupColors) json_array_append_new(colorsJ, json_integer(c));
		json_object_set_new(rootJ, "groupRGB", colorsJ);   // 0xRRGGBB, -1 = default
		json_object_set_new(rootJ, "currentGroup", json_integer(currentGroup));
		json_object_set_new(rootJ, "selectedGroup", json_integer(selectedGroup));
		json_object_set_new(rootJ, "selectedSlot", json_integer(selectedSlot));
		json_object_set_new(rootJ, "locked", json_boolean(locked));
		json_object_set_new(rootJ, "autoFill", json_boolean(autoFill));
		return rootJ;
	}

	void dataFromJson(json_t* rootJ) override {
		groups.clear();
		json_t* groupsJ = json_object_get(rootJ, "groups");
		if (groupsJ) {
			size_t gi;
			json_t* gJ;
			json_array_foreach(groupsJ, gi, gJ) {
				std::vector<int64_t> g = emptyGroup();
				for (int s = 0; s < NUM_SLOTS; s++) {
					json_t* j = json_array_get(gJ, s);
					if (j) g[s] = json_integer_value(j);
				}
				groups.push_back(g);
			}
		}
		else if (json_t* slotsJ = json_object_get(rootJ, "slots")) {
			// Older patches (before groups): one list of slots = group 1.
			// (Even older ones had 12 slots; slots 11 and 12 are dropped.)
			std::vector<int64_t> g = emptyGroup();
			for (int s = 0; s < NUM_SLOTS; s++) {
				json_t* j = json_array_get(slotsJ, s);
				if (j) g[s] = json_integer_value(j);
			}
			groups.push_back(g);
		}
		if (groups.empty()) groups.assign(1, emptyGroup());

		groupColors.clear();
		if (json_t* colorsJ = json_object_get(rootJ, "groupRGB")) {
			size_t ci;
			json_t* cJ;
			// Older saves stored every group's starting colour. A colour that is
			// just what the rotation gave that position (old or new order)
			// becomes automatic, so it follows the group's number from now on.
			static const int oldRotation[] = {-1, 8, 9, 5, 6, 7, 4, 3, 2};
			json_array_foreach(colorsJ, ci, cJ) {
				int c = (int) json_integer_value(cJ);
				int g = (int) ci;
				if (c >= 0) {
					c &= 0xffffff;
					int o = oldRotation[g % 9];
					bool wasAuto = (c == positionGroupColor(g)) || (o >= 0 && c == packRGB(HIGHLIGHT_COLORS[o]));
					if (wasAuto) c = -1;
				}
				groupColors.push_back(c < 0 ? -1 : c);
			}
		}
		else if (json_t* oldJ = json_object_get(rootJ, "groupColors")) {
			// Older patches stored a number from the colour list: convert.
			size_t ci;
			json_t* cJ;
			json_array_foreach(oldJ, ci, cJ) {
				int c = (int) json_integer_value(cJ);
				groupColors.push_back(c < 0 ? -1 : packRGB(HIGHLIGHT_COLORS[clamp(c, 0, NUM_HIGHLIGHT_COLORS - 1)]));
			}
		}

		auto getInt = [&](const char* key, int fallback) {
			json_t* j = json_object_get(rootJ, key);
			return j ? (int) json_integer_value(j) : fallback;
		};
		currentGroup = getInt("currentGroup", 0);
		selectedSlot = getInt("selectedSlot", -1);
		selectedGroup = getInt("selectedGroup", selectedSlot >= 0 ? 0 : -1);
		json_t* lockJ = json_object_get(rootJ, "locked");
		locked = lockJ && json_boolean_value(lockJ);
		json_t* autoJ = json_object_get(rootJ, "autoFill");
		autoFill = autoJ && json_boolean_value(autoJ);
		fixIndices();
	}
};


// =============================================================================
//  INDICATOR LIGHT
//  A simple round light drawn directly on the panel. It is a screen-only
//  widget (not a Rack "module light"), because what it shows is keyboard/
//  screen activity, so the audio engine never needs to know about it.
// =============================================================================
struct IndicatorLight : Widget {
	NVGcolor color;
	float brightness = 0.f;   // 0 = off, 1 = fully lit

	// The dark "off" lens, always visible.
	void draw(const DrawArgs& args) override {
		float r = box.size.x / 2.f;
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, r, r, r);
		nvgFillColor(args.vg, nvgRGB(0x30, 0x30, 0x30));
		nvgFill(args.vg);
	}

	// The lit part. Layer 1 is Rack's "lights" layer, so it still glows
	// when the room brightness is turned down.
	void drawLayer(const DrawArgs& args, int layer) override {
		if (layer == 1 && brightness > 0.f) {
			float r = box.size.x / 2.f;
			nvgBeginPath(args.vg);
			nvgCircle(args.vg, r, r, r);
			nvgFillColor(args.vg, nvgTransRGBAf(color, brightness));
			nvgFill(args.vg);
		}
		Widget::drawLayer(args, layer);
	}
};


// =============================================================================
//  PAGE READOUT
//  A small dark window on the panel with two lines of text, e.g.
//      PAGE 2/2
//      11-14
//  The panel side fills in the text; this only draws it. The text glows on
//  Rack's lights layer, so it's readable when the room is dimmed.
// =============================================================================
struct PageDisplay : Widget {
	std::string line1, line2, line3;
	bool line3Bright = false;   // sensitivity just changed: show it brighter

	void draw(const DrawArgs& args) override {
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 2.f);
		nvgFillColor(args.vg, nvgRGB(0x0c, 0x0c, 0x0c));
		nvgFill(args.vg);
	}

	// One line of text, centred, made smaller if needed so it always fits
	// inside the window (never spills off the panel).
	void fitText(NVGcontext* vg, const std::string& text, float y, float size) {
		if (text.empty()) return;
		const float maxW = box.size.x - 3.f;
		nvgFontSize(vg, size);
		float bounds[4];
		float w = nvgTextBounds(vg, 0, 0, text.c_str(), NULL, bounds);
		if (w > maxW) nvgFontSize(vg, std::max(6.f, size * maxW / w));
		nvgText(vg, box.size.x / 2.f, y, text.c_str(), NULL);
	}

	void drawLayer(const DrawArgs& args, int layer) override {
		std::shared_ptr<window::Font> font = APP->window->uiFont;
		if (layer == 1 && font && font->handle >= 0) {
			nvgFontFaceId(args.vg, font->handle);
			nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFillColor(args.vg, SLOT_COLOR);
			fitText(args.vg, line1, box.size.y * 0.20f, 11.f);
			fitText(args.vg, line2, box.size.y * 0.48f, 13.f);
			// Third line: sensitivity, e.g. "x1.0" (or "KNOB x0.4").
			nvgFillColor(args.vg, line3Bright ? nvgRGB(0xff, 0xff, 0xff) : nvgTransRGBAf(SLOT_COLOR, 0.8f));
			fitText(args.vg, line3, box.size.y * 0.80f, 10.f);
		}
		Widget::drawLayer(args, layer);
	}
};


// =============================================================================
//  GROUP READOUT
//  One line above the slot lights, e.g. "GROUP 2/3". Glows on the lights
//  layer like the page readout.
// =============================================================================
struct GroupDisplay : Widget {
	std::string text;
	NVGcolor color = SLOT_COLOR;   // the group readout uses the group's colour

	void drawLayer(const DrawArgs& args, int layer) override {
		std::shared_ptr<window::Font> font = APP->window->uiFont;
		if (layer == 1 && font && font->handle >= 0 && !text.empty()) {
			nvgFontFaceId(args.vg, font->handle);
			// Shrink to fit if needed (e.g. "G12/14 BUTTONS").
			nvgFontSize(args.vg, 10.f);
			float bounds[4];
			float w = nvgTextBounds(args.vg, 0, 0, text.c_str(), NULL, bounds);
			if (w > box.size.x - 2.f) nvgFontSize(args.vg, std::max(6.f, 10.f * (box.size.x - 2.f) / w));
			nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFillColor(args.vg, color);
			nvgText(args.vg, box.size.x / 2.f, box.size.y / 2.f, text.c_str(), NULL);
		}
		Widget::drawLayer(args, layer);
	}
};


// =============================================================================
//  SLOT NUMBERS (Highlights: "Slot numbers on panel")
//  A tiny label just above each slot light: its slot key ("1" ... "0", or
//  whatever key you learned). Positions are filled in by the panel.
// =============================================================================
// The numbers sit INSIDE the slot lights: dark on a lit slot, the
// group colour on a dimly lit (filled) one, soft grey on an empty one — so
// they're readable whatever the light is doing. Long key names shrink to fit.
struct SlotNumbers : Widget {
	static const int N = 10;
	Vec at[N];                    // centre of each label (= the light's centre), in this widget
	float fit = 10.f;             // widest a label may be (the light's width), px
	float lit[N] = {};            // each light's brightness, set by the panel every frame
	std::string labels[N];
	int keys[N] = {-2, -2, -2, -2, -2, -2, -2, -2, -2, -2};   // keys the labels were built for
	NVGcolor color = SLOT_COLOR;

	void drawLayer(const DrawArgs& args, int layer) override {
		std::shared_ptr<window::Font> font = APP->window->uiFont;
		if (layer == 1 && font && font->handle >= 0) {
			nvgFontFaceId(args.vg, font->handle);
			nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			for (int i = 0; i < N; i++) {
				// Size: 8 px, smaller if the key's name is long ("Insert" ...).
				nvgFontSize(args.vg, 8.f);
				float w = nvgTextBounds(args.vg, 0.f, 0.f, labels[i].c_str(), NULL, NULL);
				if (w > fit && w > 0.f) nvgFontSize(args.vg, std::max(5.f, 8.f * fit / w));
				NVGcolor c = (lit[i] >= 0.5f) ? nvgRGB(0x10, 0x10, 0x10)          // on a bright (selected) light
					: (lit[i] > 0.f) ? nvgTransRGBAf(color, 0.95f)                 // a filled slot
					: nvgRGBA(0x90, 0x90, 0x90, 0x90);                              // empty
				nvgFillColor(args.vg, c);
				nvgText(args.vg, at[i].x, at[i].y + 0.5f, labels[i].c_str(), NULL);
			}
		}
		Widget::drawLayer(args, layer);
	}
};


// =============================================================================
//  SENSITIVITY SLIDER (right-click menu)
//  Slides the overall sensitivity from x0.1 to x8. The slider itself runs
//  0..1 on a log scale, so x1 sits in a sensible place and fine settings
//  are as easy to reach as fast ones. Double-click it to go back to x1.
// =============================================================================
struct SensitivityQuantity : Quantity {
	static float toSlider(float s) { return std::log(s / SENS_MIN) / std::log(SENS_MAX / SENS_MIN); }
	static float fromSlider(float v) { return SENS_MIN * std::pow(SENS_MAX / SENS_MIN, v); }

	float getValue() override { return toSlider(settingSensitivity); }
	void setValue(float v) override { settingSensitivity = fromSlider(clamp(v, 0.f, 1.f)); }
	float getMinValue() override { return 0.f; }
	float getMaxValue() override { return 1.f; }
	float getDefaultValue() override { return toSlider(1.f); }
	std::string getLabel() override { return "Sensitivity \"dial speed\""; }
	std::string getDisplayValueString() override { return string::f("x%.2f", settingSensitivity); }
};

struct SensitivitySlider : ui::Slider {
	SensitivitySlider() {
		quantity = new SensitivityQuantity;
		box.size.x = 200.f;
	}
	~SensitivitySlider() override {
		delete quantity;
	}
};


// =============================================================================
//  HIGHLIGHT BRIGHTNESS SLIDER (right-click > Highlights)
//  20% - 100%. Double-click it to go back to 100%.
// =============================================================================
struct BrightnessQuantity : Quantity {
	float getValue() override { return settingHighlightBrightness; }
	void setValue(float v) override { settingHighlightBrightness = clamp(v, 0.2f, 1.f); }
	float getMinValue() override { return 0.2f; }
	float getMaxValue() override { return 1.f; }
	float getDefaultValue() override { return 1.f; }
	std::string getLabel() override { return "Brightness"; }
	std::string getDisplayValueString() override { return string::f("%.0f%%", settingHighlightBrightness * 100.f); }
};

struct BrightnessSlider : ui::Slider {
	BrightnessSlider() {
		quantity = new BrightnessQuantity;
		box.size.x = 200.f;
	}
	~BrightnessSlider() override {
		delete quantity;
	}
};


// =============================================================================
//  COLOUR ROW (right-click > Highlights & colours)
//  One menu row per colour, so colours never need a deeper menu:
//      Label   [swatch]   [colour slider]   [#RRGGBB]
//  The slider sweeps round the colour wheel; the box takes any exact colour
//  code (e.g. #40D0C0 — whites, greys, pastels too). Double-click the
//  slider = back to the default colour.
// =============================================================================
static void rgbToHsv(int rgb, float& h, float& s, float& v) {
	float r = ((rgb >> 16) & 0xff) / 255.f, g = ((rgb >> 8) & 0xff) / 255.f, b = (rgb & 0xff) / 255.f;
	float mx = std::max(r, std::max(g, b)), mn = std::min(r, std::min(g, b)), d = mx - mn;
	v = mx;
	s = (mx > 0.f) ? d / mx : 0.f;
	if (d == 0.f) h = 0.f;
	else if (mx == r) h = 60.f * std::fmod((g - b) / d + 6.f, 6.f);
	else if (mx == g) h = 60.f * ((b - r) / d + 2.f);
	else h = 60.f * ((r - g) / d + 4.f);
}
static int hsvToRgb(float h, float s, float v) {
	float c = v * s, x = c * (1.f - std::fabs(std::fmod(h / 60.f, 2.f) - 1.f)), m = v - c;
	float r = 0, g = 0, b = 0;
	if (h < 60) { r = c; g = x; }
	else if (h < 120) { r = x; g = c; }
	else if (h < 180) { g = c; b = x; }
	else if (h < 240) { g = x; b = c; }
	else if (h < 300) { r = x; b = c; }
	else { r = c; b = x; }
	auto to8 = [](float f) { return clamp((int) std::round(f * 255.f), 0, 255); };
	return (to8(r + m) << 16) | (to8(g + m) << 8) | to8(b + m);
}
static std::string rgbHex(int rgb) { return string::f("#%06X", rgb & 0xffffff); }

struct HueQuantity : Quantity {
	std::function<int()> get;
	std::function<void(int)> set;
	int defaultRgb = 0;
	float getValue() override {
		float h, s, v;
		rgbToHsv(get(), h, s, v);
		return h;
	}
	void setValue(float hue) override {
		// Keep the colour's own strength, but never so pale or dark that
		// sliding the hue shows nothing.
		float h, s, v;
		rgbToHsv(get(), h, s, v);
		set(hsvToRgb(clamp(hue, 0.f, 359.9f), std::max(s, 0.6f), std::max(v, 0.85f)));
	}
	float getMinValue() override { return 0.f; }
	float getMaxValue() override { return 359.9f; }
	float getDefaultValue() override {
		float h, s, v;
		rgbToHsv(defaultRgb, h, s, v);
		return h;
	}
	void reset() override { set(defaultRgb); }   // double-click: the exact default colour
	std::string getDisplayValueString() override { return rgbHex(get()); }
	std::string getLabel() override { return ""; }
};

struct HexField : ui::TextField {
	std::function<void(int)> set;
	void onAction(const ActionEvent& e) override {
		std::string t = text;
		if (!t.empty() && t[0] == '#') t = t.substr(1);
		bool ok = t.size() == 6 && std::all_of(t.begin(), t.end(), ::isxdigit);
		if (ok && set) set((int) std::strtol(t.c_str(), nullptr, 16));
		e.consume(this);
	}
};

struct ColorRow : widget::OpaqueWidget {
	std::string label;
	std::function<int()> get;
	ui::Slider* slider;
	HexField* field;

	ColorRow(const std::string& label, std::function<int()> get, std::function<void(int)> set, int defaultRgb)
			: label(label), get(get) {
		box.size = Vec(330.f, BND_WIDGET_HEIGHT);
		HueQuantity* q = new HueQuantity;
		q->get = get;
		q->set = set;
		q->defaultRgb = defaultRgb;
		slider = new ui::Slider;
		slider->quantity = q;
		slider->box.pos = Vec(150.f, 0.f);
		slider->box.size = Vec(110.f, BND_WIDGET_HEIGHT);
		addChild(slider);
		field = new HexField;
		field->set = set;
		field->box.pos = Vec(264.f, 0.f);
		field->box.size = Vec(64.f, BND_WIDGET_HEIGHT);
		addChild(field);
	}
	~ColorRow() override {
		delete slider->quantity;
	}
	void step() override {
		// Show the current code, unless you're typing in the box.
		if (APP->event->selectedWidget != field) field->text = rgbHex(get());
		OpaqueWidget::step();
	}
	void draw(const DrawArgs& args) override {
		bndMenuLabel(args.vg, 0.f, 0.f, 124.f, box.size.y, -1, label.c_str());
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 126.f, 3.f, 20.f, box.size.y - 6.f, 3.f);
		nvgFillColor(args.vg, rgbColor(get()));
		nvgFill(args.vg);
		OpaqueWidget::draw(args);
	}
};


// =============================================================================
//  FOLDING MENU SECTION HEADING (right-click menu)
//  "▾ CONTROLS" — click to fold the section away ("▸ CONTROLS"), click
//  again to unfold it. The menu stays open; the choice is remembered.
// =============================================================================
struct SectionHeader : ui::MenuItem {
	std::string title;   // what it shows (may include live info, e.g. "group 2 of 3")
	std::string key;     // what it's remembered by (never changes)
	std::vector<widget::Widget*> items;   // the section's rows (owned by the menu)

	// Folded? Your own choice if you've ever clicked this heading; otherwise
	// the starting state: the rarely used sections start folded, so the menu
	// opens short and tidy.
	bool folded() {
		auto it = settingMenuFolded.find(key);
		if (it != settingMenuFolded.end()) return it->second;
		return key == "SELECTED MODULE" || key == "HIGHLIGHTS & COLOURS" || key == "MIDI CONTROLLER";
	}
	void apply() {
		for (widget::Widget* w : items) w->visible = !folded();
		text = std::string(folded() ? "\xe2\x96\xb8 " : "\xe2\x96\xbe ") + title;   // ▸ / ▾
		rightText = folded() ? "click to show" : "click to hide";
	}
	void onAction(const ActionEvent& e) override {
		settingMenuFolded[key] = !folded();
		apply();
		e.unconsume();   // keep the menu open
	}
};


// =============================================================================
//  SUBMENUS OPEN ON THE SIDE THAT HAS ROOM
//  Rack opens every submenu to the RIGHT of its menu. Near the right edge of
//  the screen there's no room, so Rack squeezes it back on screen — on top
//  of the menu you were using. This invisible helper sits in the Encoder's
//  right-click menu and, every frame, moves any submenu that wouldn't fit on
//  the right to the LEFT of its menu instead (all levels deep). Where there's
//  room on the right, nothing changes.
// =============================================================================
struct SubmenuSide : widget::Widget {
	SubmenuSide() { box.size = Vec(0.f, 0.f); }   // takes no space in the menu
	void step() override {
		ui::Menu* menu = dynamic_cast<ui::Menu*>(parent);
		widget::Widget* overlay = menu ? menu->parent : nullptr;
		if (!overlay) return;
		float screenW = overlay->box.size.x;
		for (ui::Menu* m = menu; m && m->childMenu; m = m->childMenu) {
			ui::Menu* child = m->childMenu;
			if (child->box.size.x <= 0.f) {
				// Just opened: its size isn't known until it lays itself out
				// (later this same frame). Keep it hidden for that one frame
				// (~1/60 s), so it never flashes on the wrong side first.
				child->visible = false;
				break;
			}
			float right = m->box.pos.x + m->box.size.x;
			if (right + child->box.size.x <= screenW) child->box.pos.x = right;   // room on the right: as normal
			else if (m->box.pos.x - child->box.size.x >= 0.f)
				child->box.pos.x = m->box.pos.x - child->box.size.x;               // no room: open to the left
			child->visible = true;   // in place: show it
		}
	}
};


// =============================================================================
//  MENU ITEMS THAT KEEP THE MENU OPEN
//  Rack closes the whole menu on any click. The rule here:
//  if a click CHANGES A SETTING (an on/off switch, a choice from a
//  list) the menu stays open, so you can see the tick move and carry on; if
//  it DOES something (learn, set up, clear, reset ...) it closes as usual.
//  These are drop-in versions of Rack's createBoolPtrMenuItem,
//  createCheckMenuItem and createIndexSubmenuItem that stay open.
// =============================================================================
struct StayCheckItem : ui::MenuItem {
	std::string note;                    // grey text on the right (before the tick)
	std::function<bool()> checked;
	std::function<void()> action;
	void step() override {
		bool on = checked && checked();
		rightText = note.empty() ? CHECKMARK(on) : (on ? note + "  " + CHECKMARK_STRING : note);
		MenuItem::step();
	}
	void onAction(const ActionEvent& e) override {
		if (action) action();
		e.unconsume();   // keep the menu open: it's a setting
	}
};
static StayCheckItem* stayCheck(const std::string& text, const std::string& note,
		std::function<bool()> checked, std::function<void()> action, bool disabled = false) {
	StayCheckItem* item = new StayCheckItem;
	item->text = text;
	item->note = note;
	item->checked = checked;
	item->action = action;
	item->disabled = disabled;
	return item;
}
// An on / off switch for a bool setting.
static StayCheckItem* stayBool(const std::string& text, const std::string& note, bool* setting) {
	return stayCheck(text, note, [=]() { return *setting; }, [=]() { *setting = !*setting; });
}
// A choice from a list, in a submenu; the row shows the current choice.
struct StayIndexSubmenu : ui::MenuItem {
	std::vector<std::string> labels;
	std::function<int()> getter;
	std::function<void(int)> setter;
	void step() override {
		int i = getter ? getter() : -1;
		rightText = ((i >= 0 && i < (int) labels.size()) ? labels[i] : std::string()) + "  " + RIGHT_ARROW;
		MenuItem::step();
	}
	ui::Menu* createChildMenu() override {
		ui::Menu* menu = new ui::Menu;
		for (int i = 0; i < (int) labels.size(); i++) {
			std::function<int()> get = getter;
			std::function<void(int)> set = setter;
			menu->addChild(stayCheck(labels[i], "", [=]() { return get() == i; }, [=]() { set(i); }));
		}
		return menu;
	}
};
// A one-off command inside the mapping views (Clear, Reset ...) that also
// keeps the menu open — the grid / list shows the result straight away, so
// you can carry on mapping.
struct StayActionItem : ui::MenuItem {
	std::function<void()> action;
	void onAction(const ActionEvent& e) override {
		if (action) action();
		e.unconsume();
	}
};
static StayActionItem* stayAction(const std::string& text, const std::string& rightText,
		std::function<void()> action, bool disabled = false) {
	StayActionItem* item = new StayActionItem;
	item->text = text;
	item->rightText = rightText;
	item->action = action;
	item->disabled = disabled;
	return item;
}
static StayIndexSubmenu* stayIndexSubmenu(const std::string& text, std::vector<std::string> labels,
		std::function<int()> getter, std::function<void(int)> setter) {
	StayIndexSubmenu* item = new StayIndexSubmenu;
	item->text = text;
	item->labels = labels;
	item->getter = getter;
	item->setter = setter;
	return item;
}


// =============================================================================
//  NAVIGATION ENCODER (the big knob at the top of the panel)
//  Endless: it spins round and round with no end stops. The panel side
//  watches its value and turns each 1/24 of a turn into one step — the same
//  way whether it's dragged with the mouse or moved by MIDI-MAP.
//  A click without dragging = its push. No double-click reset, and its
//  turns don't go into Undo (it's navigation, not a sound setting).
// =============================================================================
struct NavKnob : RoundBlackKnob {
	float dragDistance = 0.f;
	std::function<void()> onPush;   // set by the panel

	NavKnob() {
		// A full circle, so wrapping round from the end to the start looks
		// continuous instead of jumping.
		minAngle = 0.f;
		maxAngle = 2.f * float(M_PI);
	}
	void onDragStart(const DragStartEvent& e) override {
		dragDistance = 0.f;
		RoundBlackKnob::onDragStart(e);   // (Rack hides / locks the cursor as usual)
	}
	void onDragMove(const DragMoveEvent& e) override {
		if (e.button != GLFW_MOUSE_BUTTON_LEFT) return;
		ParamQuantity* pq = getParamQuantity();
		if (!pq) return;
		dragDistance += std::fabs(e.mouseDelta.x) + std::fabs(e.mouseDelta.y);
		// Up = clockwise. Wraps round instead of stopping at the end.
		pq->setImmediateValue(math::eucMod(pq->getValue() - e.mouseDelta.y * 0.004f, 1.f));
	}
	void onDragEnd(const DragEndEvent& e) override {
		// (Not Rack's knob drag-end: that would add the turn to Undo.)
		APP->window->cursorUnlock();
		if (e.button == GLFW_MOUSE_BUTTON_LEFT && dragDistance < 3.f && onPush) onPush();
	}
	void onDoubleClick(const DoubleClickEvent& e) override {}
};


struct EncoderWidget;   // declared below; the overlay reports back to it


// =============================================================================
//  OVERLAY
//  An invisible layer stretched over the WHOLE Rack window, on top of
//  everything. Normally Rack only sends scroll/keys/clicks to whatever is
//  under the mouse; sitting on top lets us see them wherever the mouse is.
//
//  It only "consumes" (keeps) the things that are ours — the slot hotkeys,
//  and the one click while assigning a slot. Everything else passes straight
//  through, so Rack behaves exactly as normal.
//
//  It also draws the outline around the selected module.
// =============================================================================
struct Overlay : Widget {
	EncoderWidget* owner = nullptr;

	// Badge labels for the current keys, worked out only when keys change.
	std::string knobLabels[KEY_ROW], buttonLabels[KEY_ROW];
	int labelsForKnobs[KEY_ROW] = {}, labelsForButtons[KEY_ROW] = {};
	bool labelsBuilt = false;

	void refreshKeyLabels() {
		if (labelsBuilt && std::memcmp(labelsForKnobs, settingKeys.knobs, sizeof(labelsForKnobs)) == 0 &&
				std::memcmp(labelsForButtons, settingKeys.buttons, sizeof(labelsForButtons)) == 0)
			return;
		std::memcpy(labelsForKnobs, settingKeys.knobs, sizeof(labelsForKnobs));
		std::memcpy(labelsForButtons, settingKeys.buttons, sizeof(labelsForButtons));
		for (int i = 0; i < KEY_ROW; i++) {
			// A key set to "none" means "the numpad digit" (numpad layouts).
			knobLabels[i] = settingKeys.knobs[i] >= 0 ? keyName(settingKeys.knobs[i]) : padLabel(i);
			buttonLabels[i] = settingKeys.buttons[i] >= 0 ? keyName(settingKeys.buttons[i]) : padLabel(i);
		}
		labelsBuilt = true;
	}

	void step() override;
	void draw(const DrawArgs& args) override;
	void drawHighlights(const DrawArgs& args);
	void drawBadge(NVGcontext* vg, std::shared_ptr<window::Font> font, Vec at,
		const std::string& label, NVGcolor base, bool square, bool active, bool dim);
	void drawValueBox(NVGcontext* vg, std::shared_ptr<window::Font> font, ParamWidget* pw);
	void onHoverScroll(const HoverScrollEvent& e) override;
	void onHoverKey(const HoverKeyEvent& e) override;
	void onButton(const ButtonEvent& e) override;
};


// =============================================================================
//  THE PANEL SIDE
// =============================================================================
// ---- Panel parts (AP LAB Hotkey panel, 2026-11-19) -------------------------
// Light / dark versions follow Rack's View > "Use dark panels".
struct HotkeyScrew : app::ThemedSvgScrew {
	HotkeyScrew() {
		setSvg(Svg::load(asset::plugin(pluginInstance, "res/ScrewLight.svg")),
		       Svg::load(asset::plugin(pluginInstance, "res/ScrewDark.svg")));
	}
};

// Mid-grey push buttons with an indented icon (frame 0 = up, 1 = pressed).
struct HotkeyPushButton : app::SvgSwitch {
	HotkeyPushButton(const char* name) {
		momentary = true;
		shadow->opacity = 0.f;   // the art draws its own bezel shadow
		addFrame(Svg::load(asset::plugin(pluginInstance, std::string("res/") + name + "_0.svg")));
		addFrame(Svg::load(asset::plugin(pluginInstance, std::string("res/") + name + "_1.svg")));
	}
};
struct HotkeyLockButton : HotkeyPushButton { HotkeyLockButton() : HotkeyPushButton("HotkeyLock") {} };
struct HotkeyPrevButton : HotkeyPushButton { HotkeyPrevButton() : HotkeyPushButton("HotkeyPrev") {} };
struct HotkeyNextButton : HotkeyPushButton { HotkeyNextButton() : HotkeyPushButton("HotkeyNext") {} };

// Amber padlock that glows over the LOCK button while Hotkey is locked.
// Drawn on the light layer, so it still shows with the room lights down.
struct LockGlow : widget::TransparentWidget {
	Module* module = nullptr;
	std::shared_ptr<window::Svg> svg;
	float level = 0.f;
	LockGlow() { svg = Svg::load(asset::plugin(pluginInstance, "res/HotkeyLock_lit.svg")); }
	bool isLocked();   // defined after Encoder is complete (below)
	void step() override {
		float target = isLocked() ? 1.f : 0.f;
		level += (target - level) * 0.35f;   // quick fade
		if (std::fabs(target - level) < 0.01f) level = target;
		TransparentWidget::step();
	}
	void drawLayer(const DrawArgs& args, int layer) override {
		if (layer == 1 && level > 0.f && svg && svg->handle) {
			nvgSave(args.vg);
			nvgGlobalAlpha(args.vg, level);
			window::svgDraw(args.vg, svg->handle);
			nvgRestore(args.vg);
		}
		TransparentWidget::drawLayer(args, layer);
	}
};
bool LockGlow::isLocked() {
	Encoder* e = dynamic_cast<Encoder*>(module);
	return e && e->locked;
}

struct EncoderWidget : ModuleWidget {
	IndicatorLight* upLight;
	IndicatorLight* downLight;
	IndicatorLight* pushLight;
	IndicatorLight* slotLights[NUM_SLOTS];
	PageDisplay* pageDisplay;
	GroupDisplay* groupDisplay;
	GroupDisplay* modeDisplay;   // "NUMPAD: KNOBS" (numpad layouts only)
	SlotNumbers* slotNumbers;    // tiny slot-key labels beside the slot lights

	// ---- Navigation encoder state ----------------------------------------
	// One fixed job each, no modes : turn = previous / next
	// module in the group, push = next page. Groups are the < > buttons.
	float navLast = -1.f;           // encoder value last frame (-1 = not read yet)
	float navAccum = 0.f;           // part-steps of encoder movement
	float navScrollAccum = 0.f;     // part-clicks of the keyboard dial while navigating
	bool lockWas = false, groupPrevWas = false, groupNextWas = false;   // buttons last frame
	float sensKnobWritten = -1.f;   // the SENS knob's value as we last saw / set it
	static constexpr float NAV_STEPS = 24.f;   // steps per full turn of the encoder
	int shownGroupKey[3] = {-1, -1, -2};   // what the group readout last showed
	double lastCleanupTime = -1.0;     // last check for deleted modules
	Overlay* overlay = nullptr;

	// When each dial event was last seen (seconds), for the flashes.
	double lastUpTime = -1.0;
	double lastDownTime = -1.0;
	// Push also flashes on each press, because the K70 dial presses AND
	// releases its key in the same instant — too fast to catch by checking
	// "is it held right now?" once per frame.
	double lastPushTime = -1.0;

	// Slot waiting for a click (right-click > Assign). -1 = not assigning.
	int learningSlot = -1;

	// The number key being held (0 = key 1), or -1 if none. If several are
	// held, the most recently pressed one wins, so you can roll from one
	// number to the next without letting go.
	int heldKnob = -1;
	// A knob held by its CUSTOM key (custom mode): its place in the knob
	// list (any page), and the key holding it. -1 = none.
	int heldCustom = -1;
	int heldCustomKey = -1;

	// The knob being held right now — by a custom key, or by a number key
	// on the current page — as its place in the knob list. -1 = none.
	int heldIndex() {
		if (heldCustom >= 0) return heldCustom;
		if (heldKnob >= 0) return onPage(heldKnob);
		return -1;
	}
	int heldKey = -1;   // the actual key held (number row or number pad)

	// The selected module's knobs in panel order (knob 1 first). Worked out
	// once when the selection changes — not every frame — because a
	// module's controls never move.
	std::vector<ParamWidget*> knobs;
	std::vector<ParamWidget*> buttons;   // same idea, for buttons/switches
	std::vector<ParamWidget*> autoPush;  // per knob: its automatic push target, or null
	int64_t knobsForModuleId = -1;

	// A push button pressed by a letter key, remembered so the matching
	// key-up releases it. Stored by module ID + param number (not a widget
	// pointer), so it's safe even if the selection changes mid-press.
	struct HeldButton {
		int64_t moduleId = -1;
		int paramId = -1;
		int key = -1;   // the key holding it (a letter, or a numpad digit)
	};
	HeldButton heldButtons[NUM_BUTTON_KEYS];

	// The page of the selected module being played (0 = page 1).
	int page = 0;
	// The button page, when buttons page on their own (see PAGES below).
	int buttonPage = 0;
	// What the page readout last showed (so it's only rewritten on change).
	static const int PAGE_KEY_LEN = 6;
	int shownPageKey[PAGE_KEY_LEN] = {-1, -1, -1, -1, -1, -1};

	// The button last tapped (its place in the whole list), and when — for
	// its value readout.
	int lastButton = -1;
	double lastButtonTime = -1.0;

	// Leftover part-clicks for stepped knobs (e.g. smooth-scroll mice send
	// fractions of a click; a stepped knob only moves on whole steps).
	float stepRemainder = 0.f;

	// The key the dial's push sends. a Corsair K70's dial sends Ctrl+End, so
	// we watch End (Ctrl is ignored). Fixed for now; becomes a
	// "Learn push key" option in a later step.
	// (The push key itself is settingKeys.push — learnable. Any Ctrl/Shift
	//  the dial adds is ignored, because a Corsair K70 sends Ctrl+End.)
	static int pushKey() { return settingKeys.push.key; }

	EncoderWidget(Encoder* module) {
		setModule(module);
		// Light / dark panel: follows Rack's View > "Use dark panels".
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Hotkey.svg"),
		                     asset::plugin(pluginInstance, "res/Hotkey-dark.svg")));

		// Two screws, top and bottom at x = 7.62 mm (same spots as Orbital MAS).
		addChild(createWidgetCentered<HotkeyScrew>(mm2px(Vec(7.62f, 2.54f))));
		addChild(createWidgetCentered<HotkeyScrew>(mm2px(Vec(7.62f, 125.96f))));

		// =====================================================================
		//  PANEL LAYOUT in mm (AP LAB panel 2026-11-19; positions match the
		//  components layer of the master SVGs). Top to bottom:
		//    navigation knob (turn = module, click = next page)  /  LOCK
		//    dial lights: up / push / down  (feedback for YOUR dial & wheel)
		//    numpad mode line (Numpad layout only)
		//    GROUP readout
		//    slot lights 1-9, then  < 0 >  (previous / next group either side),
		//      each slot's number centred UNDER its light (keeps it centred)
		//    page / sensitivity readout
		//    SENS knob
		// =====================================================================

		// ---- Navigation knob + LOCK -----------------------------------------
		NavKnob* nav = createParamCentered<NavKnob>(mm2px(Vec(10.16f, 21.f)), module, Encoder::NAV_PARAM);
		nav->onPush = [=]() { navPush(); };
		addParam(nav);

		ParamWidget* lockButton = createParamCentered<HotkeyLockButton>(mm2px(Vec(10.16f, 31.9f)), module, Encoder::LOCK_PARAM);
		addParam(lockButton);
		LockGlow* lockGlow = new LockGlow;
		lockGlow->module = module;
		lockGlow->box = lockButton->box;   // same size and spot as the button
		addChild(lockGlow);

		// ---- Dial lights, in a row: up (green) / push (blue) / down (red) ----
		upLight   = addLight(mm2px(Vec(5.16f, 40.2f)), 4.f, nvgRGB(0x40, 0xe0, 0x60));
		pushLight = addLight(mm2px(Vec(10.16f, 40.2f)), 4.f, nvgRGB(0x40, 0xa0, 0xff));
		downLight = addLight(mm2px(Vec(15.16f, 40.2f)), 4.f, nvgRGB(0xff, 0x50, 0x40));

		// ---- Numpad mode line (Numpad layout only) ----------------------------
		modeDisplay = new GroupDisplay;
		modeDisplay->box.pos = mm2px(Vec(1.6f, 44.2f));
		modeDisplay->box.size = mm2px(Vec(17.12f, 4.f));
		addChild(modeDisplay);

		// ---- Group readout, just above the slot lights -------------------------
		groupDisplay = new GroupDisplay;
		groupDisplay->box.pos = mm2px(Vec(1.6f, 48.4f));
		groupDisplay->box.size = mm2px(Vec(17.12f, 4.5f));
		addChild(groupDisplay);

		// ---- Slot lights: laid out like a phone keypad -----------------------
		//   1  2  3
		//   4  5  6
		//   7  8  9
		//   <  0  >     (slot 10 on the 0 key; previous / next group either side)
		// Lights 4 mm across, big enough to carry their number inside.
		const float slotTop = 58.2f, slotRow = 5.2f, slotSize = 4.f;
		for (int i = 0; i < NUM_SLOTS; i++) {
			int col = (i < 9) ? i % 3 : 1;
			int row = (i < 9) ? i / 3 : 3;
			float x = 5.16f + 5.f * col;
			float y = slotTop + slotRow * row;
			slotLights[i] = addLight(mm2px(Vec(x, y)), slotSize, SLOT_COLOR);
		}
		addParam(createParamCentered<HotkeyPrevButton>(mm2px(Vec(5.16f, slotTop + 3.f * slotRow)), module, Encoder::GROUP_PREV_PARAM));
		addParam(createParamCentered<HotkeyNextButton>(mm2px(Vec(15.16f, slotTop + 3.f * slotRow)), module, Encoder::GROUP_NEXT_PARAM));

		// ---- Slot numbers: INSIDE each slot light --------------------------------
		// Highlights > "Slot numbers on panel" turns them off.
		slotNumbers = new SlotNumbers;
		slotNumbers->box.size = box.size;
		slotNumbers->fit = mm2px(slotSize) - 2.f;
		for (int i = 0; i < NUM_SLOTS; i++) {
			int col = (i < 9) ? i % 3 : 1;
			int row = (i < 9) ? i / 3 : 3;
			slotNumbers->at[i] = mm2px(Vec(5.16f + 5.f * col, slotTop + slotRow * row));
		}
		addChild(slotNumbers);   // (added after the lights, so drawn on top of them)

		// ---- Page / sensitivity readout, under the slot lights ----------------
		pageDisplay = new PageDisplay;
		pageDisplay->box.pos = mm2px(Vec(2.6f, 79.4f));
		pageDisplay->box.size = mm2px(Vec(15.12f, 16.8f));
		addChild(pageDisplay);

		// ---- SENS knob, at the bottom, under its readout -----------------------
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(10.16f, 108.6f)), module, Encoder::SENS_PARAM));

		// (The overlay — the invisible layer that catches keys and the dial —
		//  is added in step(), once this module becomes the active Encoder.)
	}

	~EncoderWidget() override {
		// Take the overlay back off the Rack window when the module is removed,
		// and hand over to another Encoder (if there is one) next frame.
		if (overlay) {
			APP->scene->removeChild(overlay);
			delete overlay;
		}
		if (activeEncoder == this) activeEncoder = nullptr;
		// Keep this Encoder's MIDI input choice for the next one.
		if (activeMidiPort == &midiInput) {
			if (settingMidiPortJ) json_decref(settingMidiPortJ);
			settingMidiPortJ = midiInput.toJson();
			activeMidiPort = nullptr;
		}
		if (activeMidiPort2 == &midiInput2) {
			if (settingMidiPortJ2) json_decref(settingMidiPortJ2);
			settingMidiPortJ2 = midiInput2.toJson();
			activeMidiPort2 = nullptr;
		}
	}

	// =========================================================================
	//  ONLY ONE ACTIVE ENCODER
	//  Two Encoders would both react to every key and fight over the dial.
	//  Rack can't stop a second one being added, so the first one in the
	//  patch is ACTIVE and any other copy is INACTIVE: its display says so
	//  and it ignores everything. If the active one is deleted, another one
	//  takes over automatically.
	// =========================================================================
	static EncoderWidget* activeEncoder;

	bool isActive() { return activeEncoder == this; }

	// Become the active Encoder if there isn't one. Called every frame.
	void claimIfFree() {
		if (!module || activeEncoder) return;
		activeEncoder = this;
		restoreSlotsIfEmpty();   // a fresh Encoder picks up this patch's slots
		// MIDI input: the one you chose last time (saved with your settings).
		if (settingMidiPortJ) midiInput.fromJson(settingMidiPortJ);
		activeMidiPort = &midiInput;
		if (settingMidiPortJ2) midiInput2.fromJson(settingMidiPortJ2);
		activeMidiPort2 = &midiInput2;
		overlay = new Overlay;
		overlay->owner = this;
		APP->scene->addChild(overlay);   // added last = on top of everything

		// First time ever: point the way to the dial setup.
		if (!settingDialSetupDone)
			showKeyMessage("NEW?", "SET UP DIAL", "New to Hotkey? Right-click it > CONTROLS > \"Set up my dial...\" "
				"to set it up for your keyboard dial or mouse wheel.", 10.0);
	}

	// What an inactive copy shows. Only rewritten once.
	void showInactive() {
		if (pageDisplay->line1 == "ONLY ONE") return;
		pageDisplay->line1 = "ONLY ONE";
		pageDisplay->line2 = "HOTKEY";
		pageDisplay->line3 = "inactive";
		pageDisplay->line3Bright = false;
		for (int i = 0; i < NUM_SLOTS; i++) slotLights[i]->brightness = 0.f;
		upLight->brightness = downLight->brightness = pushLight->brightness = 0.f;
		groupDisplay->text = "";
		modeDisplay->text = "";
		slotNumbers->visible = false;
		shownGroupKey[0] = shownGroupKey[1] = -1;
		shownGroupKey[2] = -2;
		for (int i = 0; i < PAGE_KEY_LEN; i++) shownPageKey[i] = -1;   // redraw page text if it becomes active
	}

	IndicatorLight* addLight(Vec center, float sizeMm, NVGcolor color) {
		IndicatorLight* light = new IndicatorLight;
		light->box.size = mm2px(Vec(sizeMm, sizeMm));
		light->box.pos = center.minus(light->box.size.div(2.f));
		light->color = color;
		addChild(light);
		return light;
	}

	Encoder* enc() { return dynamic_cast<Encoder*>(module); }


	// =========================================================================
	//  SLOTS
	// =========================================================================

	// The module sitting in a slot of a group, or null if the slot is empty
	// (or its module has since been deleted from the patch).
	ModuleWidget* slotModuleIn(int group, int slot) {
		Encoder* e = enc();
		if (!e || group < 0 || group >= (int) e->groups.size() || slot < 0 || slot >= NUM_SLOTS) return nullptr;
		int64_t id = e->groups[group][slot];
		return id >= 0 ? APP->scene->rack->getModule(id) : nullptr;
	}

	// Same, in the CURRENT group (the one the slot keys act on).
	ModuleWidget* slotModule(int slot) {
		Encoder* e = enc();
		return e ? slotModuleIn(e->currentGroup, slot) : nullptr;
	}

	// The module under the mouse right now, or null over empty space.
	// (Rack tracks the exact widget under the mouse — maybe a knob — so
	//  climb up from it until we reach the module that contains it.)
	ModuleWidget* moduleUnderMouse() {
		for (Widget* w = APP->event->hoveredWidget; w; w = w->parent) {
			if (ModuleWidget* mw = dynamic_cast<ModuleWidget*>(w)) return mw;
		}
		return nullptr;
	}

	// Put a module in a slot of the CURRENT group (null = clear the slot).
	void assignSlot(int slot, ModuleWidget* mw) {
		assignSlotIn(enc() ? enc()->currentGroup : 0, slot, mw);
	}

	void assignSlotIn(int group, int slot, ModuleWidget* mw) {
		Encoder* e = enc();
		if (!e || group < 0 || group >= (int) e->groups.size()) return;
		if (mw == this) return;   // Encoder can't control itself
		leaveAutoFill();          // (arranging by hand: auto-fill steps aside)

		if (!mw) {
			int64_t removed = e->groups[group][slot];
			e->groups[group][slot] = -1;
			if (e->selectedGroup == group && e->selectedSlot == slot) {
				e->selectedGroup = -1;
				e->selectedSlot = -1;
			}
			backupSlots({removed});
			return;
		}

		// A module can be in ANY number of groups (e.g. a mixer you use all
		// the time, in every group), but only ONCE per group: if it's already
		// in another slot of THIS group, it moves (and if that was the selected
		// one, it stays selected in its new place). Other groups are left alone.
		int64_t id = mw->module->id;
		for (int s = 0; s < NUM_SLOTS; s++) {
			if (s != slot && e->groups[group][s] == id) {
				e->groups[group][s] = -1;
				if (e->selectedGroup == group && e->selectedSlot == s) e->selectedSlot = slot;
			}
		}
		e->groups[group][slot] = id;
		backupSlots();
	}


	// =========================================================================
	//  GROUPS
	//  Shift + -  / Shift + =  : previous / next group (wraps round)
	//  Alt + =                 : new group (jump to it, then fill it)
	//  Alt + Delete            : clear the current group (it disappears)
	//  A group with no modules vanishes as soon as you leave it.
	// =========================================================================

	// Remove a group, keeping the other indices pointing at the same groups.
	void eraseGroup(int g) {
		Encoder* e = enc();
		if (!e || (int) e->groups.size() <= 1) return;   // always keep one
		e->groups.erase(e->groups.begin() + g);
		if (g < (int) e->groupColors.size()) e->groupColors.erase(e->groupColors.begin() + g);   // its colour goes too
		if (e->selectedGroup == g) {
			e->selectedGroup = -1;
			e->selectedSlot = -1;
		}
		else if (e->selectedGroup > g) e->selectedGroup--;
		if (e->currentGroup > g) e->currentGroup--;
		e->fixIndices();
	}

	// Remove every empty group except the current one (e.g. after its last
	// module was deleted from the patch, or after leaving it).
	void dropEmptyGroups() {
		Encoder* e = enc();
		if (!e) return;
		for (int g = (int) e->groups.size() - 1; g >= 0; g--) {
			if (g != e->currentGroup && e->groupEmpty(g) && e->groups.size() > 1) eraseGroup(g);
		}
	}

	// Shift + - (dir = -1) / Shift + = (dir = +1).
	void switchGroup(int dir) {
		Encoder* e = enc();
		if (!e) return;
		int n = (int) e->groups.size();
		if (n <= 1) return;
		int target = (e->currentGroup + dir + n) % n;
		int leaving = e->currentGroup;
		e->currentGroup = target;
		if (e->groupEmpty(leaving)) eraseGroup(leaving);   // empty groups vanish
		learningSlot = -1;
	}

	// Alt + =: start a new, empty group and jump to it. (If you're already
	// on an empty group, that one is used — no piles of empty groups.)
	void newGroup() {
		Encoder* e = enc();
		if (!e || e->groupEmpty(e->currentGroup)) return;
		leaveAutoFill();
		if ((int) e->groups.size() >= MAX_GROUPS) return;
		e->groups.push_back(Encoder::emptyGroup());
		e->syncColors();   // the new group's colour is automatic: from its number
		e->currentGroup = (int) e->groups.size() - 1;
		learningSlot = -1;
	}

	// ---- AUTO-FILL ----------------------------------------------------------
	// Every module with knobs or buttons gets a slot, 10 per group, as many
	// groups as needed. Turning it on fills everything in rack order (row by
	// row, left to right). After that, slots STAY PUT so people can remember
	// them: a deleted module leaves its slot empty, a new module
	// goes into the first empty slot, and moving modules changes nothing.
	// "Re-sort slots in rack order" tidies up on request. Checked 4 times a
	// second, only from a cheap fingerprint of the module IDs.
	std::vector<double> autoFillPrint;   // last fingerprint (module IDs)

	std::vector<ModuleWidget*> autoFillModules() {
		std::vector<ModuleWidget*> mods;
		for (ModuleWidget* mw : APP->scene->rack->getModules()) {
			if (mw == this || !mw->module) continue;
			if (dynamic_cast<EncoderWidget*>(mw)) continue;   // never Encoder itself
			if (findControls(mw, false).empty() && findControls(mw, true).empty()) continue;   // nothing to play
			mods.push_back(mw);
		}
		std::sort(mods.begin(), mods.end(), [](ModuleWidget* a, ModuleWidget* b) {
			int ra = (int) std::round(a->box.pos.y / RACK_GRID_HEIGHT), rb = (int) std::round(b->box.pos.y / RACK_GRID_HEIGHT);
			return (ra != rb) ? ra < rb : a->box.pos.x < b->box.pos.x;
		});
		return mods;
	}
	// (IDs only, sorted: moving modules around doesn't count as a change.)
	std::vector<double> autoFillFingerprint() {
		std::vector<double> p;
		for (ModuleWidget* mw : APP->scene->rack->getModules()) {
			if (mw == this || !mw->module) continue;
			p.push_back((double) mw->module->id);
		}
		std::sort(p.begin(), p.end());
		return p;
	}
	// Keep the slots up to date WITHOUT moving anything: modules that are
	// gone leave an empty slot; new modules fill the first empty slots (in
	// rack order), adding groups at the end when every slot is full.
	void autoFillUpdate() {
		Encoder* e = enc();
		if (!e) return;
		std::vector<ModuleWidget*> mods = autoFillModules();
		std::vector<int64_t> present;
		for (ModuleWidget* mw : mods) present.push_back(mw->module->id);
		auto isPresent = [&](int64_t id) { return std::find(present.begin(), present.end(), id) != present.end(); };
		// Gone: empty its slot (everything else stays where it is).
		for (auto& g : e->groups)
			for (int64_t& id : g)
				if (id >= 0 && !isPresent(id)) id = -1;
		if (e->selectedGroup >= 0 && e->selectedSlot >= 0 && e->groups[e->selectedGroup][e->selectedSlot] < 0)
			e->selectedGroup = e->selectedSlot = -1;
		// New: into the first empty slot.
		for (ModuleWidget* mw : mods) {
			int64_t id = mw->module->id;
			bool inSlot = false;
			for (const auto& g : e->groups)
				if (std::find(g.begin(), g.end(), id) != g.end()) inSlot = true;
			if (inSlot) continue;
			bool placed = false;
			for (auto& g : e->groups) {
				for (int64_t& s : g)
					if (s < 0) { s = id; placed = true; break; }
				if (placed) break;
			}
			if (!placed && (int) e->groups.size() < MAX_GROUPS) {
				e->groups.push_back(Encoder::emptyGroup());
				e->groups.back()[0] = id;
			}
		}
		// Only EMPTY GROUPS AT THE END go (so no group number ever shifts).
		while (e->groups.size() > 1 && e->groupEmpty((int) e->groups.size() - 1)) {
			e->groups.pop_back();
			if (e->groupColors.size() > e->groups.size()) e->groupColors.resize(e->groups.size());
		}
		e->syncColors();
		if (e->currentGroup >= (int) e->groups.size()) e->currentGroup = (int) e->groups.size() - 1;
		autoFillPrint = autoFillFingerprint();
		backupSlots();
	}
	void autoFillNow() {
		Encoder* e = enc();
		if (!e) return;
		ModuleWidget* sel = selectedModule();
		int64_t selId = (sel && sel->module) ? sel->module->id : -1;
		std::vector<ModuleWidget*> mods = autoFillModules();
		int count = std::max(1, std::min((int) ((mods.size() + NUM_SLOTS - 1) / NUM_SLOTS), MAX_GROUPS));
		e->groups.assign(count, Encoder::emptyGroup());
		for (size_t i = 0; i < mods.size() && (int) (i / NUM_SLOTS) < count; i++)
			e->groups[i / NUM_SLOTS][i % NUM_SLOTS] = mods[i]->module->id;
		e->syncColors();
		// Keep playing the same module, wherever it landed.
		e->selectedGroup = e->selectedSlot = -1;
		for (int g = 0; g < count; g++)
			for (int s = 0; s < NUM_SLOTS; s++)
				if (selId >= 0 && e->groups[g][s] == selId) { e->selectedGroup = g; e->selectedSlot = s; }
		if (e->currentGroup >= count) e->currentGroup = count - 1;
		autoFillPrint = autoFillFingerprint();
		backupSlots();
	}
	// Turn auto-fill on / off from the menu (turning it on is one undo step:
	// Ctrl+Z brings your own slots back).
	void setAutoFill(bool on);
	void resortAutoFill();   // "Re-sort slots in rack order" (one undo step)
	// You arranged a slot by hand: auto-fill steps aside (it would undo you).
	void leaveAutoFill() {
		Encoder* e = enc();
		if (!e || !e->autoFill || autoFilling) return;
		e->autoFill = false;
		showKeyMessage("AUTO-FILL", "OFF", "Auto-fill is off: you're arranging slots by hand now");
	}
	bool autoFilling = false;   // (true while auto-fill itself is changing slots)

	// ---- FOLLOW: glide the rack to a selected module that's off screen -------
	// (GROUPS & SLOTS > Scroll to the selected module.) Only when it's NOT
	// already fully on screen — a module you can see never makes the rack
	// move. A short, smooth glide: each frame closes a fifth of the gap, for
	// at most 1.5 s (so it gives up gracefully at the edge of the rack).
	int64_t followLastId = -1;
	bool followActive = false;
	double followUntil = 0.0;
	int64_t followTargetId = -1;   // the module being glided to
	int followLastGroup = -1;      // (switching group: glide to it if none of it is on screen)
	// Set for one frame when a control ON THE PANEL (navigation knob, < / >
	// group buttons) picks a module or group: you're looking at the Encoder
	// while you use it, so the rack stays put (gliding away would take the
	// control out from under you). Hotkeys, the keyboard dial, MIDI and the
	// gamepad still glide.
	bool followSkip = false;

	// =========================================================================
	//  GAME CONTROLLER (read directly: plug it in and it works — no MIDI
	//  set-up). Uses the standard gamepad layout, so Xbox, PlayStation and
	//  most other pads behave the same. What each control does is in
	//  CONTROLS > Gamepad mapping (settingPadMap). Default layout:
	//    LB / RB        previous / next module
	//    LT / RT        previous / next group
	//    left / right stick   knob A / B of the bank (tilt = turn, further =
	//                   faster, let go = stop); click = press it (dial push)
	//    A B X Y        buttons 1-4 of the bank
	//    D-pad up/down  bank: sticks -> knobs 3-4, 5-6 ...; buttons -> 5-8 ...
	//    D-pad left/right  previous / next page
	//    View           undo          Menu    home (navigation press)
	//  Polled once per screen frame; works alongside keys, dial, mouse, MIDI.
	// =========================================================================
	int padBank = 0;                    // 0 = sticks on knobs 1-2, buttons 1-4
	static const int PAD_BANKS = 5;     // (10 knobs / 2 sticks)
	bool padWas[NUM_PAD_ACTIONS] = {};  // was each action's control down last frame?
	HeldButton padHeld[4];              // buttons held by A / B / X / Y
	std::string padName;                // the connected controller ("" = none)
	int padLearn = -1;                  // the action waiting for a gamepad control (-1 = none)
	double padLearnStart = 0.0;
	std::vector<int> padLearnBaseline;  // controls already down when learning began
	double padLastTime = -1.0;
	// Which you're using right now — the gamepad or the keyboard? The badges
	// on the module follow: J1 / J2 and A B X Y while it's the gamepad, the
	// keyboard keys once you touch a key again.
	double padLastUsed = -1.0, keysLastUsed = -1.0;
	bool padInUse() const { return !padName.empty() && padLastUsed > keysLastUsed; }
	bool padIsPlayStation() const {
		for (const char* s : {"PS3", "PS4", "PS5", "DualShock", "DualSense", "PlayStation", "Sony"})
			if (padName.find(s) != std::string::npos) return true;
		return false;
	}
	// The short badge label for gamepad button n (0-3 = Button 1-4): its
	// control's name — A B X Y, or the PlayStation symbols on a PlayStation pad.
	std::string padButtonLabel(int n) const {
		int c = settingPadMap.control[PA_BTN1 + n];
		static const char* xbox[15] = {"A", "B", "X", "Y", "LB", "RB", "View", "Menu", "Guide", "L3", "R3",
			"Up", "Right", "Down", "Left"};
		static const char* ps[4] = {"\xe2\x9c\x95", "\xe2\x97\x8b", "\xe2\x96\xa1", "\xe2\x96\xb3"};   // ✕ ○ □ △
		if (c >= 0 && c < 4 && padIsPlayStation()) return ps[c];
		if (c >= 0 && c < 15) return xbox[c];
		if (c == PAD_AXIS + 4) return "LT";
		if (c == PAD_AXIS + 5) return "RT";
		return "?";
	}

	// Is this control "pressed"? (A stick counts when pushed well over; a
	// trigger when pulled past halfway.)
	static bool padControlDown(const GLFWgamepadstate& st, int c) {
		if (c < 0) return false;
		if (c < PAD_AXIS) return c <= GLFW_GAMEPAD_BUTTON_LAST && st.buttons[c] == GLFW_PRESS;
		int ax = c - PAD_AXIS;
		if (ax > GLFW_GAMEPAD_AXIS_LAST) return false;
		if (ax >= 4) return st.axes[ax] > 0.f;   // triggers rest at -1
		return std::fabs(st.axes[ax]) > 0.6f;
	}
	// A stick as a knob: -1 ... +1, up / right = turn up.
	static float padStick(const GLFWgamepadstate& st, int c) {
		if (c < PAD_AXIS || c - PAD_AXIS > GLFW_GAMEPAD_AXIS_LAST) return 0.f;
		int ax = c - PAD_AXIS;
		float v = st.axes[ax];
		if (ax == 1 || ax == 3) v = -v;           // (pushing up reads as -1)
		if (ax >= 4) v = (v + 1.f) / 2.f;         // a trigger: 0 ... 1
		return v;
	}
	static std::string padControlName(int c) {
		static const char* buttons[15] = {"A (cross)", "B (circle)", "X (square)", "Y (triangle)", "LB (L1)", "RB (R1)",
			"View (Share)", "Menu (Options)", "Guide", "L-stick click (L3)", "R-stick click (R3)",
			"D-pad up", "D-pad right", "D-pad down", "D-pad left"};
		static const char* axes[6] = {"L-stick left-right", "L-stick up-down", "R-stick left-right", "R-stick up-down",
			"LT (L2)", "RT (R2)"};
		if (c < 0) return "not set";
		if (c < PAD_AXIS) return c < 15 ? buttons[c] : "?";
		return (c - PAD_AXIS < 6) ? axes[c - PAD_AXIS] : "?";
	}
	static const char* padActionName(int a) {
		static const char* names[NUM_PAD_ACTIONS] = {
			"Previous module", "Next module", "Previous group", "Next group",
			"Knob J1 \"stick\"", "Knob J2 \"stick\"", "Press J1", "Press J2",
			"Button 1", "Button 2", "Button 3", "Button 4",
			"J1 J2 + buttons back", "J1 J2 + buttons along \"knobs 3-4, buttons 5-8\"", "Previous page", "Next page",
			"Undo", "Home \"group 1, first module\""};
		return names[a];
	}
	static bool padActionIsStick(int a) { return a == PA_KNOB_A || a == PA_KNOB_B; }

	int findGamepad() {
		for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
			if (glfwJoystickIsGamepad(jid)) return jid;
		return -1;
	}
	// All controls that are down right now (for learning).
	static std::vector<int> padDownList(const GLFWgamepadstate& st) {
		std::vector<int> d;
		for (int b = 0; b <= GLFW_GAMEPAD_BUTTON_LAST; b++) if (padControlDown(st, b)) d.push_back(b);
		for (int ax = 0; ax <= GLFW_GAMEPAD_AXIS_LAST; ax++) if (padControlDown(st, PAD_AXIS + ax)) d.push_back(PAD_AXIS + ax);
		return d;
	}
	void startPadLearn(int action) {
		padLearn = action;
		padLearnStart = system::getTime();
		padLearnBaseline.clear();
		int jid = findGamepad();
		GLFWgamepadstate st;
		if (jid >= 0 && glfwGetGamepadState(jid, &st)) padLearnBaseline = padDownList(st);
	}

	void padFrame(double now) {
		double dt = (padLastTime < 0.0) ? 0.0 : std::min(now - padLastTime, 0.1);
		padLastTime = now;
		int jid = settingPadOn ? findGamepad() : -1;
		std::string name = (jid >= 0 && glfwGetGamepadName(jid)) ? glfwGetGamepadName(jid) : "";
		if (name != padName) {
			// Plugged in / unplugged: say so (once).
			if (!name.empty()) showKeyMessage("GAMEPAD", "CONNECTED", "Game controller connected: " + name +
				"  (CONTROLS > Gamepad mapping)");
			padName = name;
		}
		GLFWgamepadstate st;
		if (jid < 0 || !glfwGetGamepadState(jid, &st)) return;
		const PadMap& map = settingPadMap;

		// ---- Learning: the first control pressed (that wasn't already) -------
		if (padLearn >= 0) {
			if (now - padLearnStart > 8.0) { padLearn = -1; return; }
			for (int c : padDownList(st)) {
				if (std::find(padLearnBaseline.begin(), padLearnBaseline.end(), c) != padLearnBaseline.end()) continue;
				bool isStick = c >= PAD_AXIS && c - PAD_AXIS <= 3;
				if (padActionIsStick(padLearn) != isStick) continue;   // knobs take sticks; the rest take buttons / triggers
				// Swap with whatever had it, so nothing ends up doubled.
				int old = settingPadMap.control[padLearn];
				for (int a = 0; a < NUM_PAD_ACTIONS; a++)
					if (a != padLearn && settingPadMap.control[a] == c) settingPadMap.control[a] = old;
				settingPadMap.control[padLearn] = c;
				showKeyMessage("GAMEPAD", "SET", std::string(padActionName(padLearn)) + " = " + padControlName(c));
				padLearn = -1;
				for (int a = 0; a < NUM_PAD_ACTIONS; a++) padWas[a] = padControlDown(st, settingPadMap.control[a]);
				return;
			}
			return;
		}

		// ---- Playing ----------------------------------------------------------
		for (int a = 0; a < NUM_PAD_ACTIONS; a++) {
			if (padActionIsStick(a)) continue;
			bool d = padControlDown(st, map.control[a]);
			bool down = d && !padWas[a], up = !d && padWas[a];
			padWas[a] = d;
			if (!down && !up) continue;
			padLastUsed = now;   // (the badges switch to gamepad labels)
			switch (a) {
				case PA_MODULE_PREV: if (down) selectAdjacentModule(-1); break;
				case PA_MODULE_NEXT: if (down) selectAdjacentModule(+1); break;
				case PA_GROUP_PREV: if (down) switchGroup(-1); break;
				case PA_GROUP_NEXT: if (down) switchGroup(+1); break;
				case PA_PRESS_A: case PA_PRESS_B: {
					int knob = padBank * 2 + (a - PA_PRESS_A);
					if (down && knob < KEY_ROW) knobPressed(onPage(knob), false, true);   // held until let go
					else if (up && activePush.fromMidi) activePush.keyUp = true;
					break;
				}
				case PA_BTN1: case PA_BTN2: case PA_BTN3: case PA_BTN4: {
					int n = a - PA_BTN1;
					int button = padBank * 4 + n;
					if (down && button < KEY_ROW) pressButtonAt(onButtonPage(button), -1, padHeld[n]);
					else if (up) {
						if (padHeld[n].moduleId >= 0) releaseHeld(padHeld[n]);
						padHeld[n] = HeldButton();
					}
					break;
				}
				case PA_BANK_PREV: case PA_BANK_NEXT:
					// Move J1 / J2 (and the four buttons) along — like a page,
					// and shown the same way: the rest of the badges dim.
					if (down) {
						padBank = (padBank + (a == PA_BANK_NEXT ? 1 : PAD_BANKS - 1)) % PAD_BANKS;
						int b1 = padBank * 4 + 1, b2 = std::min(padBank * 4 + 4, KEY_ROW);
						std::string buttons = (b1 <= KEY_ROW) ? string::f("buttons %d-%d", b1, b2) : "no buttons";
						showKeyMessage(string::f("J1 J2 = %d %d", padBank * 2 + 1, padBank * 2 + 2), "",
							string::f("Gamepad: sticks J1 / J2 = knobs %d / %d, ", padBank * 2 + 1, padBank * 2 + 2) + buttons);
					}
					break;
				case PA_PAGE_PREV:
					if (down && selectedModule()) {
						if (heldKey >= 0) numberReleased(heldKey);
						int pc = pageCount();
						page = ((page - 1) % pc + pc) % pc;
					}
					break;
				case PA_PAGE_NEXT: if (down) nextPage(); break;
				case PA_UNDO: if (down) APP->history->undo(); break;
				case PA_HOME: if (down) goHome(); break;
			}
		}

		// ---- Sticks: tilt = turning speed (a small dead zone in the middle) ----
		for (int n = 0; n < 2 && dt > 0.0; n++) {
			float v = padStick(st, map.control[PA_KNOB_A + n]);
			float mag = (std::fabs(v) - 0.15f) / 0.85f;   // 15% dead zone
			if (mag <= 0.f) continue;
			int knob = padBank * 2 + n;
			if (knob >= KEY_ROW) continue;
			int index = onPage(knob);
			std::vector<ParamWidget*>& list = currentKnobs();
			if (index < 0 || index >= (int) list.size()) continue;
			ParamQuantity* pq = list[index]->getParamQuantity();
			// Full tilt: 40 clicks a second (stepped knobs: 8 steps a second).
			float rate = (pq && pq->snapEnabled) ? 8.f : 40.f;
			turnKnobAt(index, (v > 0.f ? 1.f : -1.f) * mag * mag * rate * (float) dt);
			padLastUsed = now;
			lastMidiKnob = index;   // (shows its value, like a MIDI knob)
			lastMidiKnobTime = now;
		}
	}

	static Rect moduleOnScreenRect(ModuleWidget* mw) {
		Vec a = mw->getRelativeOffset(Vec(0, 0), APP->scene);
		Vec b = mw->getRelativeOffset(mw->box.size, APP->scene);
		return Rect(a, b.minus(a));
	}
	static bool moduleOnScreen(ModuleWidget* mw) {
		RackScrollWidget* rs = APP->scene->rackScroll;
		if (!rs) return true;
		Rect r = moduleOnScreenRect(mw), v = rs->box;
		return r.pos.x >= v.pos.x && r.pos.y >= v.pos.y &&
			r.pos.x + r.size.x <= v.pos.x + v.size.x && r.pos.y + r.size.y <= v.pos.y + v.size.y;
	}
	void followStep(double now) {
		RackScrollWidget* rs = APP->scene->rackScroll;
		ModuleWidget* mw = (followTargetId >= 0) ? APP->scene->rack->getModule(followTargetId) : nullptr;
		if (!rs || !mw || now > followUntil) { followActive = false; return; }
		Rect r = moduleOnScreenRect(mw), v = rs->box;
		Vec d = r.getCenter().minus(v.getCenter());   // how far off-centre, in screen pixels
		// (A module taller / wider than the view: just get its top-left in.)
		if (r.size.y > v.size.y) d.y = r.pos.y - v.pos.y;
		if (r.size.x > v.size.x) d.x = r.pos.x - v.pos.x;
		if (std::fabs(d.x) < 3.f && std::fabs(d.y) < 3.f) { followActive = false; return; }
		float zoom = rs->getZoom();
		Vec grid = d.mult(0.2f).div(RACK_GRID_SIZE.mult(zoom));   // a fifth of the way, in grid units
		rs->setGridOffset(rs->getGridOffset().plus(grid));
	}

	// ---- Slot backup (see plugin.hpp) ---------------------------------------
	// Keep a copy of the current slots in Rack's settings. Any older backup
	// that shares a module with this one is from the same patch, so it's
	// replaced rather than kept twice. (removed = modules just taken out of
	// slots: they still mark the old backup as this patch's, so emptying
	// slots on purpose really empties the backup too.)
	void backupSlots(const std::vector<int64_t>& removed = {}) {
		Encoder* e = enc();
		if (!e) return;
		std::vector<int64_t> now = e->flatten();   // every group, one after another
		auto sharesModule = [&](const std::vector<int64_t>& b) {
			for (int64_t id : b) {
				if (id < 0) continue;
				if (std::find(removed.begin(), removed.end(), id) != removed.end()) return true;
				if (std::find(now.begin(), now.end(), id) != now.end()) return true;
			}
			return false;
		};
		settingSlotBackups.erase(std::remove_if(settingSlotBackups.begin(), settingSlotBackups.end(), sharesModule),
			settingSlotBackups.end());
		bool anyFilled = std::any_of(now.begin(), now.end(), [](int64_t id) { return id >= 0; });
		if (anyFilled) settingSlotBackups.insert(settingSlotBackups.begin(), now);
		if (settingSlotBackups.size() > MAX_SLOT_BACKUPS) settingSlotBackups.resize(MAX_SLOT_BACKUPS);
	}

	// A snapshot of all groups + where you are, for undo / redo.
	struct SlotSnapshot {
		std::vector<int64_t> flat;
		std::vector<int> colors;
		int currentGroup = 0, selectedGroup = -1, selectedSlot = -1;
		bool autoFill = false;   // (undo of "auto-fill on" turns it back off)
	};

	SlotSnapshot snapshot() {
		SlotSnapshot s;
		Encoder* e = enc();
		if (!e) return s;
		s.flat = e->flatten();
		s.colors = e->groupColors;
		s.currentGroup = e->currentGroup;
		s.selectedGroup = e->selectedGroup;
		s.selectedSlot = e->selectedSlot;
		s.autoFill = e->autoFill;
		return s;
	}

	void applySnapshot(const SlotSnapshot& s) {
		Encoder* e = enc();
		if (!e) return;
		std::vector<int64_t> before = e->flatten();
		e->currentGroup = s.currentGroup;
		e->selectedGroup = s.selectedGroup;
		e->selectedSlot = s.selectedSlot;
		e->autoFill = s.autoFill;
		if (e->autoFill) autoFillPrint = autoFillFingerprint();   // (don't rebuild straight over it)
		e->unflatten(s.flat);
		e->groupColors = s.colors;   // the groups' own colours come back too
		e->syncColors();
		backupSlots(before);
	}

	// ---- Deleted modules coming back (Ctrl+Z) --------------------------------
	// When a module is deleted from the patch, its slots are emptied (and a
	// group left with nothing vanishes). But we remember how everything looked
	// just before, so if Rack's Ctrl+Z brings the module back (same ID):
	//   - nothing changed in the slots since -> everything comes back EXACTLY
	//     (its slots, any group that vanished, colours, selection);
	//   - something changed since -> it goes back into its old slots where
	//     they're still free, and your newer changes are left alone.
	// Only while the patch is open (like Rack's own undo history).
	struct Grave {
		std::vector<int64_t> ids;     // the module(s) that went missing together
		SlotSnapshot before, after;   // the slots just before / just after
	};
	std::vector<Grave> graves;
	static const size_t MAX_GRAVES = 20;

	static bool sameSlots(const SlotSnapshot& a, const SlotSnapshot& b) {
		return a.flat == b.flat && a.colors == b.colors;
	}

	void restoreReturnedModules() {
		Encoder* e = enc();
		if (!e || graves.empty()) return;
		for (size_t i = graves.size(); i-- > 0;) {
			Grave& gr = graves[i];
			bool back = false;
			for (int64_t id : gr.ids) if (APP->engine->getModule(id)) back = true;
			if (!back) continue;
			Grave grave = gr;
			graves.erase(graves.begin() + i);
			if (sameSlots(snapshot(), grave.after)) {
				applySnapshot(grave.before);   // exactly as it was
			}
			else {
				// Put each returned module back into its old slots, where free.
				const std::vector<int64_t>& flat = grave.before.flat;
				for (size_t k = 0; k < flat.size(); k++) {
					int64_t id = flat[k];
					if (id < 0 || std::find(grave.ids.begin(), grave.ids.end(), id) == grave.ids.end()) continue;
					if (!APP->engine->getModule(id)) continue;
					int g = (int) (k / NUM_SLOTS), s = (int) (k % NUM_SLOTS);
					if (g < (int) e->groups.size() && e->groups[g][s] < 0) e->groups[g][s] = id;
				}
				backupSlots();
			}
		}
	}

	// Alt+Delete: clear the CURRENT group (which then disappears).
	// Menu: clear ALL groups. Both undoable (Ctrl+Z) — see ClearSlotsAction.
	void clearCurrentGroup();
	void clearAllGroups();

	// A fresh, EMPTY Encoder: bring back the backup whose modules are in
	// this patch (the one with the most of them). Slots whose module is no
	// longer in the patch stay empty.
	void restoreSlotsIfEmpty() {
		Encoder* e = enc();
		if (!e) return;
		for (int64_t id : e->flatten())
			if (id >= 0) return;   // has its own slots: leave it

		const std::vector<int64_t>* best = nullptr;
		int bestCount = 0;
		for (const auto& b : settingSlotBackups) {
			int count = 0;
			for (int64_t id : b)
				if (id >= 0 && APP->scene->rack->getModule(id)) count++;
			if (count > bestCount) {
				bestCount = count;
				best = &b;
			}
		}
		if (!best) return;
		// Bring back only modules that are really in this patch, then drop
		// any group that ended up empty.
		std::vector<int64_t> flat = *best;
		for (int64_t& id : flat)
			if (id >= 0 && !APP->scene->rack->getModule(id)) id = -1;
		e->currentGroup = 0;
		e->selectedGroup = -1;
		e->selectedSlot = -1;
		e->unflatten(flat);
		for (int g = (int) e->groups.size() - 1; g >= 0; g--) {
			if (e->groupEmpty(g) && e->groups.size() > 1) {
				e->groups.erase(e->groups.begin() + g);
				if (g < (int) e->groupColors.size()) e->groupColors.erase(e->groupColors.begin() + g);
			}
		}
		e->fixIndices();
	}

	// Select a slot (in the current group) for playing. Empty slots can't be
	// selected. Selecting the one that's ALREADY selected flips to its next
	// page (wrapping back to page 1 after the last one).
	void selectSlot(int slot) {
		Encoder* e = enc();
		if (!e || !slotModule(slot)) return;
		if (e->selectedGroup == e->currentGroup && e->selectedSlot == slot) {
			page = (page + 1) % pageCount();
		}
		else {
			e->selectedGroup = e->currentGroup;
			e->selectedSlot = slot;
			page = 0;
			buttonPage = 0;
		}
	}

	// The next-page key: flip the selected module to its next page (wraps).
	// Lets go of a held knob first, so it doesn't jump to another knob.
	void nextPage() {
		if (!selectedModule()) return;
		if (heldKey >= 0) numberReleased(heldKey);
		page = (page + 1) % pageCount();
	}

	// =========================================================================
	//  NAVIGATION ENCODER: one fixed job each, no modes 
	//    turn = select the group's previous / next module
	//    push = the selected module's next page (wraps)
	//  Groups are the < > buttons beside slot 0. Same with the keyboard dial
	//  while holding the nav modifier (default Alt), MIDI and the gamepad.
	// =========================================================================
	void navTurn(int dir) {
		if (!enc() || dir == 0) return;
		// (Turning selects directly — sleeker than browsing
		//  first and pushing to select.)
		selectAdjacentModule(dir);
	}

	// Select the next / previous FILLED slot in the current group, wrapping.
	// (The navigation knob, and the gamepad's bumpers.)
	void selectAdjacentModule(int dir) {
		Encoder* e = enc();
		if (!e) return;
		int s = (e->selectedGroup == e->currentGroup) ? e->selectedSlot : -1;
		if (s < 0 && dir < 0) s = 0;   // nothing selected yet: start from either end
		for (int n = 0; n < NUM_SLOTS; n++) {
			s = (s + dir + NUM_SLOTS) % NUM_SLOTS;
			if (slotModule(s)) break;
		}
		if (!slotModule(s)) return;   // nothing in this group
		if (!(e->selectedGroup == e->currentGroup && e->selectedSlot == s)) selectSlot(s);
	}
	// Home (gamepad): group 1, its first module, page 1.
	void goHome() {
		Encoder* e = enc();
		if (!e) return;
		if (e->currentGroup != 0) switchGroup(-e->currentGroup);
		for (int s = 0; s < NUM_SLOTS; s++)
			if (slotModule(s)) { selectSlot(s); break; }
		page = 0;
		buttonPage = 0;
	}

	void navPush() {
		if (!enc()) return;
		nextPage();
	}

	// LOCK: the Encoder ignores all its keys (and the dial) until unlocked —
	// handy while typing or setting up. The lock key / button always works.
	void toggleLock() {
		Encoder* e = enc();
		if (!e) return;
		e->locked = !e->locked;
		if (e->locked) {
			if (heldKey >= 0) numberReleased(heldKey);
			if (heldCustomKey >= 0) customReleased(heldCustomKey);
			for (int i = 0; i < NUM_BUTTON_KEYS; i++) releaseButton(i);
		}
		showKeyMessage(e->locked ? "LOCKED" : "UNLOCKED", keyName(settingKeys.lockKey.key, settingKeys.lockKey.mods),
			e->locked ? "Hotkey LOCKED - its keys and dial do nothing until you unlock (" +
				keyName(settingKeys.lockKey.key, settingKeys.lockKey.mods) + " or the LOCK button)"
			: std::string("Hotkey unlocked"));
	}
	bool isLocked() {
		Encoder* e = enc();
		return e && e->locked;
	}

	// The separate next-BUTTON-page key (default numpad *).
	void nextButtonPage() {
		if (!selectedModule() || buttonPageCount() <= 1) return;
		for (int i = 0; i < NUM_BUTTON_KEYS; i++) releaseButton(i);
		buttonPage = (buttonPage + 1) % buttonPageCount();
	}

	// ---- PAGES ------------------------------------------------------------
	// Knobs and buttons each need pages of 10 (at most 4).
	//   10 or fewer buttons: they're on EVERY page (never paged away).
	//   More than 10 buttons:
	//     - a separate button-page key is set -> buttons page on their own
	//       (numpad / = knob pages only, numpad * = button pages only);
	//     - otherwise -> buttons follow the knob pages.
	static int pagesFor(size_t count) {
		return clamp(((int) count + PER_PAGE - 1) / PER_PAGE, 1, MAX_PAGES);
	}
	int knobPageCount() { return pagesFor(currentKnobs().size()); }
	int buttonPageCount() { return pagesFor(currentButtons().size()); }
	bool buttonsPageSeparately() { return settingKeys.buttonPageNext.key >= 0; }

	// How many pages the next-page key cycles through.
	int pageCount() {
		return buttonsPageSeparately() ? knobPageCount() : std::max(knobPageCount(), buttonPageCount());
	}

	// The button page in use right now.
	int currentButtonPage() {
		int bp = buttonPageCount();
		if (bp <= 1) return 0;
		return (buttonsPageSeparately() ? buttonPage : page) % bp;
	}

	// A key's place on the page -> its place in the module's whole list.
	int onPage(int keyIndex) {
		return page * PER_PAGE + keyIndex;
	}
	int onButtonPage(int keyIndex) {
		return currentButtonPage() * PER_PAGE + keyIndex;
	}


	// =========================================================================
	//  KNOBS OF THE SELECTED MODULE
	// =========================================================================

	ModuleWidget* selectedModule() {
		Encoder* e = enc();
		return e ? slotModuleIn(e->selectedGroup, e->selectedSlot) : nullptr;
	}

	// The selected module's knobs in panel order. Only rebuilt when the
	// selected module changes.
	std::vector<ParamWidget*>& currentKnobs() {
		ModuleWidget* mw = selectedModule();
		int64_t id = mw ? mw->module->id : -1;
		if (id != knobsForModuleId) {
			knobsForModuleId = id;
			stepRemainder = 0.f;
			lastButton = -1;
			knobs = mw ? findControls(mw, false) : std::vector<ParamWidget*>();
			buttons = mw ? findControls(mw, true) : std::vector<ParamWidget*>();
			buildCustomKeys(mw);   // custom-mode keys for these controls
			if (heldCustom >= 0) { finishTurn(); heldCustom = -1; heldCustomKey = -1; }
			// Automatic push links (rules 2 and 3 in pushTargetFor), worked
			// out once here rather than every time they're needed.
			autoPush.assign(knobs.size(), nullptr);
			for (size_t i = 0; i < knobs.size(); i++) {
				if (isKnownPushEncoder(knobs[i])) autoPush[i] = knobs[i];
				else autoPush[i] = buttonOnKnob(mw, knobs[i], buttons);
			}
			// Keep the pages valid (e.g. a patch reloaded with fewer controls).
			int pages = std::max(pagesFor(knobs.size()), pagesFor(buttons.size()));
			if (page >= pages) page = 0;
			if (buttonPage >= pagesFor(buttons.size())) buttonPage = 0;
		}
		return knobs;
	}

	// The selected module's buttons/switches in panel order.
	std::vector<ParamWidget*>& currentButtons() {
		currentKnobs();   // rebuilds both lists if the selection changed
		return buttons;
	}

	// Every control on a module, however deep the developer tucked it.
	// (Rack's own getParams() only looks at the top level, so controls placed
	// inside a sub-panel widget would be missed — e.g. Submit's Drift.)
	static void collectParams(Widget* w, std::vector<ParamWidget*>& out) {
		for (Widget* child : w->children) {
			if (ParamWidget* pw = dynamic_cast<ParamWidget*>(child)) out.push_back(pw);
			else collectParams(child, out);
		}
	}

	// Button or knob? Rack's switch type = button. Rack's knob type = knob.
	// Developers sometimes build their own control types on neither; those
	// count as knobs, unless they behave exactly like an on/off button
	// (whole-number values 0 or 1 only).
	static bool isButtonControl(ParamWidget* pw) {
		if (dynamic_cast<Switch*>(pw)) return true;
		if (dynamic_cast<Knob*>(pw)) return false;
		ParamQuantity* pq = pw->getParamQuantity();
		return pq && pq->snapEnabled && pq->getMinValue() == 0.f && pq->getMaxValue() == 1.f;
	}

	// Find every knob (wantButtons = false) or every button/switch
	// (wantButtons = true) on a module, in reading order — then apply your
	// remap for that module type, if you've made one.
	static std::vector<ParamWidget*> findControls(ModuleWidget* mw, bool wantButtons) {
		std::vector<ParamWidget*> all;
		collectParams(mw, all);
		std::vector<ParamWidget*> found;
		for (ParamWidget* pw : all) {
			if (!pw->getParamQuantity()) continue;
			if (isButtonControl(pw) == wantButtons) found.push_back(pw);
		}

		// Sort top to bottom first... (positions measured on the module
		// itself, so tucked-away controls sort correctly too)
		auto centre = [mw](ParamWidget* pw) { return pw->getRelativeOffset(pw->box.size.div(2.f), mw); };
		std::sort(found.begin(), found.end(), [&](ParamWidget* a, ParamWidget* b) {
			return centre(a).y < centre(b).y;
		});
		// ...then group into rows (anything within 5 mm of the row's first
		// knob counts as the same row) and sort each row left to right.
		const float rowTolerance = mm2px(5.f);
		size_t rowStart = 0;
		for (size_t i = 1; i <= found.size(); i++) {
			bool newRow = (i == found.size()) ||
				(centre(found[i]).y - centre(found[rowStart]).y > rowTolerance);
			if (newRow) {
				std::sort(found.begin() + rowStart, found.begin() + i, [&](ParamWidget* a, ParamWidget* b) {
					return centre(a).x < centre(b).x;
				});
				rowStart = i;
			}
		}

		// ---- Your remap for this module type, if any ------------------------
		// The remap lists param numbers in your chosen order. Those come
		// first, in that order; anything not in it (e.g. a control added in
		// a newer version of the module) keeps its panel order after them.
		auto it = settingRemaps.find(remapKey(mw));
		if (it != settingRemaps.end()) {
			const std::vector<int>& order = wantButtons ? it->second.buttons : it->second.knobs;
			std::vector<ParamWidget*> sorted;
			for (int paramId : order) {
				for (ParamWidget* pw : found) {
					if (pw->getParamQuantity()->paramId == paramId) {
						sorted.push_back(pw);
						break;
					}
				}
			}
			for (ParamWidget* pw : found) {
				if (std::find(sorted.begin(), sorted.end(), pw) == sorted.end()) sorted.push_back(pw);
			}
			found = sorted;
		}
		return found;
	}

	// Which remap belongs to a module: "PluginSlug/ModuleSlug".
	static std::string remapKey(ModuleWidget* mw) {
		return mw->model->plugin->slug + "/" + mw->model->slug;
	}

	// ---- Remap gesture: hold a number + Alt-click a control -----------------
	// Knob   -> it becomes that number (1-0) on the CURRENT page.
	// Button -> it becomes the letter under that number (1 = Q, 2 = W ...).
	// It swaps places with whatever had that number, so nothing gets lost.
	// Applies to every module of this type, in every patch.
	// Returns true if the click was used.
	bool remapClicked(ParamWidget* clicked) {
		ModuleWidget* mw = selectedModule();
		if (!mw || heldKnob < 0 || !clicked || clicked->module != mw->module) return false;

		bool isButton = isButtonControl(clicked);
		std::vector<ParamWidget*> list = isButton ? currentButtons() : currentKnobs();
		int from = std::find(list.begin(), list.end(), clicked) - list.begin();
		int to = onPage(heldKnob);
		if (from >= (int) list.size() || to >= (int) list.size()) return false;

		std::swap(list[from], list[to]);

		// Store the whole new order (as param numbers) for this module type.
		std::vector<int> order;
		for (ParamWidget* pw : list) order.push_back(pw->getParamQuantity()->paramId);
		Remap& remap = settingRemaps[remapKey(mw)];
		(isButton ? remap.buttons : remap.knobs) = order;

		knobsForModuleId = -1;   // rebuild the lists with the new order
		return true;
	}

	// Forget the remap for the selected module's type: back to panel order.
	// (Push links are kept — they have their own "Reset push links to automatic".)
	void resetRemap() {
		ModuleWidget* mw = selectedModule();
		if (!mw) return;
		auto it = settingRemaps.find(remapKey(mw));
		if (it == settingRemaps.end()) return;
		it->second.knobs.clear();
		it->second.buttons.clear();
		if (it->second.empty()) settingRemaps.erase(it);
		knobsForModuleId = -1;
	}

	bool hasNumberingRemap(ModuleWidget* mw) {
		auto it = mw ? settingRemaps.find(remapKey(mw)) : settingRemaps.end();
		return it != settingRemaps.end() && (!it->second.knobs.empty() || !it->second.buttons.empty());
	}

	bool hasPushLinks(ModuleWidget* mw) {
		auto it = mw ? settingRemaps.find(remapKey(mw)) : settingRemaps.end();
		return it != settingRemaps.end() && !it->second.pushLinks.empty();
	}

	void clearPushLinks() {
		ModuleWidget* mw = selectedModule();
		if (!mw) return;
		auto it = settingRemaps.find(remapKey(mw));
		if (it == settingRemaps.end()) return;
		it->second.pushLinks.clear();
		if (it->second.empty()) settingRemaps.erase(it);
	}


	// =========================================================================
	//  SENSITIVITY (step 7)
	//  Overall sensitivity (menu slider, or numpad + / - on their own), and
	//  each knob's own (hold its number + numpad + / -), remembered per
	//  module type. A knob with its own setting ignores the overall one.
	// =========================================================================

	// The knob's own sensitivity if it has one, otherwise the overall one.
	float sensitivityFor(int index) {
		ModuleWidget* mw = selectedModule();
		std::vector<ParamWidget*>& list = currentKnobs();
		if (mw && index >= 0 && index < (int) list.size()) {
			auto it = settingRemaps.find(remapKey(mw));
			if (it != settingRemaps.end()) {
				auto s = it->second.knobSens.find(list[index]->getParamQuantity()->paramId);
				if (s != it->second.knobSens.end()) return s->second;
			}
		}
		return settingSensitivity;
	}

	bool knobHasOwnSensitivity(int index) {
		ModuleWidget* mw = selectedModule();
		std::vector<ParamWidget*>& list = currentKnobs();
		if (!mw || index < 0 || index >= (int) list.size()) return false;
		auto it = settingRemaps.find(remapKey(mw));
		return it != settingRemaps.end() &&
			it->second.knobSens.count(list[index]->getParamQuantity()->paramId);
	}

	// When sensitivity last changed — the readout highlights it briefly.
	double lastSensChangeTime = -1.0;

	// Numpad + (up = true) or - : faster / finer.
	//   With a number held -> only that knob (and it's remembered).
	//   Without            -> the overall sensitivity.
	void changeSensitivity(bool up) {
		float factor = up ? SENS_STEP : 1.f / SENS_STEP;
		ModuleWidget* mw = selectedModule();
		std::vector<ParamWidget*>& list = currentKnobs();
		int index = heldIndex();
		if (mw && index >= 0 && index < (int) list.size()) {
			float s = clamp(sensitivityFor(index) * factor, SENS_MIN, SENS_MAX);
			settingRemaps[remapKey(mw)].knobSens[list[index]->getParamQuantity()->paramId] = s;
		}
		else {
			settingSensitivity = clamp(settingSensitivity * factor, SENS_MIN, SENS_MAX);
		}
		lastSensChangeTime = system::getTime();
	}

	bool hasKnobSensitivities(ModuleWidget* mw) {
		auto it = mw ? settingRemaps.find(remapKey(mw)) : settingRemaps.end();
		return it != settingRemaps.end() && !it->second.knobSens.empty();
	}

	void resetKnobSensitivities() {
		ModuleWidget* mw = selectedModule();
		if (!mw) return;
		auto it = settingRemaps.find(remapKey(mw));
		if (it == settingRemaps.end()) return;
		it->second.knobSens.clear();
		if (it->second.empty()) settingRemaps.erase(it);
	}

	// ACCELERATION (menu): turning the dial quickly multiplies the step.
	// Measured from how fast scroll clicks arrive, smoothed so one quick
	// flick doesn't jump. Slow turns stay exactly x1.
	double lastScrollTime = -1.0;
	float clickRate = 0.f;   // dial clicks per second, smoothed

	float accelerationFor(float clicks) {
		return accelerationFor(clicks, lastScrollTime, clickRate);
	}
	// (The same, with its own memory — each extra dial keeps its own speed.)
	float accelerationFor(float clicks, double& lastScrollTime, float& clickRate) {
		double now = system::getTime();
		double dt = (lastScrollTime < 0) ? 1.0 : now - lastScrollTime;
		lastScrollTime = now;
		float rate = std::fabs(clicks) / (float) std::max(dt, 0.005);
		if (dt > 0.3) clickRate = 0.f;                 // paused: start again slow
		clickRate += (rate - clickRate) * 0.3f;        // smooth
		if (!settingAcceleration) return 1.f;
		// Below ~8 clicks per second: normal. Faster: up to x6.
		return clamp(1.f + (clickRate - 8.f) / 6.f, 1.f, 6.f);
	}


	// =========================================================================
	//  PUSH LINKS (step 5b)
	//  The dial's push does exactly what a mouse press on the linked control
	//  does. The module itself then decides what the press means — so a
	//  module with its own tap / double-tap / hold (like Uno) just works.
	// =========================================================================

	// The control the held knob's push is linked to, or null.
	ParamWidget* pushTargetForHeldKnob() {
		std::vector<ParamWidget*>& list = currentKnobs();
		int index = heldIndex();
		if (!selectedModule() || index < 0 || index >= (int) list.size()) return nullptr;
		return pushTargetFor(index);
	}

	// What a knob's push presses, decided in this order:
	//   1. YOUR link for it (Ctrl+Alt-click) — or your "no link", which
	//      switches off an automatic one. (Stored as -1.)
	//   2. Known push encoders — controls built as one widget that turns AND
	//      presses, recognised by type (Orbital's placement encoders, on Uno
	//      and Pulse). Linked to themselves.
	//   3. A button sitting right on top of the knob — the usual way
	//      developers build a push encoder from two parts.
	// (index = the knob's place in the selected module's knob list.)
	ParamWidget* pushTargetFor(int index) {
		ModuleWidget* mw = selectedModule();
		std::vector<ParamWidget*>& list = currentKnobs();
		if (!mw || index < 0 || index >= (int) list.size()) return nullptr;
		ParamWidget* knob = list[index];

		auto it = settingRemaps.find(remapKey(mw));
		if (it != settingRemaps.end()) {
			auto link = it->second.pushLinks.find(knob->getParamQuantity()->paramId);
			if (link != it->second.pushLinks.end())
				return link->second < 0 ? nullptr : findParamWidget(mw, link->second);
		}
		return autoPush[index];
	}

	// Rule 2: known one-piece push encoders, by the name of their type.
	// (Matching by type name, not by a module's internal param numbers,
	// keeps working even if that module's code is reorganised.)
	static bool isKnownPushEncoder(ParamWidget* pw) {
		std::string type = typeid(*pw).name();
		return type.find("PlacementEncoder") != std::string::npos;   // Orbital
	}

	// Rule 3: a button whose centre sits on the knob's centre (within a
	// quarter of the knob's size).
	static ParamWidget* buttonOnKnob(ModuleWidget* mw, ParamWidget* knob, const std::vector<ParamWidget*>& buttonList) {
		Vec kc = knob->getRelativeOffset(knob->box.size.div(2.f), mw);
		float tolerance = std::min(knob->box.size.x, knob->box.size.y) * 0.25f;
		for (ParamWidget* b : buttonList) {
			Vec bc = b->getRelativeOffset(b->box.size.div(2.f), mw);
			if (kc.minus(bc).norm() <= tolerance) return b;
		}
		return nullptr;
	}

	// The widget on a module for a given param number.
	static ParamWidget* findParamWidget(ModuleWidget* mw, int paramId) {
		std::vector<ParamWidget*> all;
		collectParams(mw, all);
		for (ParamWidget* pw : all) {
			ParamQuantity* pq = pw->getParamQuantity();
			if (pq && pq->paramId == paramId) return pw;
		}
		return nullptr;
	}

	// Press (down = true) or let go of a control, the way a mouse would.
	static void pressControl(ParamWidget* pw, bool down) {
		ParamQuantity* pq = pw->getParamQuantity();
		Switch* sw = dynamic_cast<Switch*>(pw);
		if (sw && pq) {
			// Rack's own buttons: set the value just as a click does.
			if (sw->momentary) pq->setValue(down ? pq->getMaxValue() : pq->getMinValue());
			else if (down) {
				float v = std::round(pq->getValue()) + 1.f;
				pq->setValue(v > pq->getMaxValue() ? pq->getMinValue() : v);
			}
			return;
		}
		// Anything else (e.g. Uno's push encoder): hand the control the same
		// "mouse button down / up" it gets from a real click.
		if (down) {
			Widget::DragStartEvent e;
			e.button = GLFW_MOUSE_BUTTON_LEFT;
			pw->onDragStart(e);
		}
		else {
			Widget::DragEndEvent e;
			e.button = GLFW_MOUSE_BUTTON_LEFT;
			pw->onDragEnd(e);
		}
	}

	// A press in progress, remembered by module ID + param number so it's
	// safe even if the selection changes before it's let go.
	struct ActivePush {
		int64_t moduleId = -1;
		int paramId = -1;
		double startTime = 0.0;
		bool keyUp = false;       // the push key / wheel button has been let go
		bool fromWheel = false;   // pushed with the mouse wheel button, not the key
		bool fromMidi = false;    // a MIDI "knob press": held until its release arrives
		int dialKey = -1;         // pushed with an EXTRA dial's push: that key (-1 = no)
	};
	ActivePush activePush;

	// Every press lasts at least this long. A Corsair K70's dial presses and
	// lets go in the same instant — too short for a module to ever notice.
	static constexpr double MIN_PUSH_TIME = 0.04;

	// For double-tap on an unlinked knob: when the last push was, and on
	// which knob (by module ID + param number).
	double lastTapTime = -1.0;
	int64_t lastTapModuleId = -1;
	int lastTapParamId = -1;
	// Two pushes closer together than this = a double-tap.
	static constexpr double DOUBLE_TAP_TIME = 0.4;

	// Dial push went down. Returns true if it did something.
	//   Linked knob   -> press the linked control (the module decides).
	//   Unlinked knob -> a double-tap acts like a mouse double-click on the
	//                    knob (normally: reset to default). Single tap: nothing.
	bool pushPressed(bool fromWheel = false) {
		return knobPressed(heldIndex(), fromWheel, false);
	}
	// Press knob `index` of the selected module (its place in the knob list):
	// from the dial push / wheel (the held knob) or a MIDI "knob press".
	// (dialKey = an extra dial's push key, so its own key-up lets go.)
	bool knobPressed(int index, bool fromWheel, bool fromMidi, int dialKey = -1) {
		std::vector<ParamWidget*>& list = currentKnobs();
		if (!selectedModule() || index < 0 || index >= (int) list.size()) return false;
		ParamWidget* target = pushTargetFor(index);
		if (!target) {
			// The panel readout says so, and how to fix it (no banner: this
			// can happen often, and a pop-up every time would be too much).
			showKeyMessage("PUSH", "NOT LINKED", "");
			return unlinkedPushAt(index);
		}
		endPush();   // finish any earlier press first
		pressControl(target, true);
		activePush.moduleId = target->module->id;
		activePush.paramId = target->getParamQuantity()->paramId;
		activePush.startTime = system::getTime();
		activePush.keyUp = false;
		activePush.fromWheel = fromWheel;
		activePush.fromMidi = fromMidi;
		activePush.dialKey = dialKey;
		lastPushParamId = activePush.paramId;
		lastPushModuleId = activePush.moduleId;
		lastPushStart = activePush.startTime;
		// Panel readout: what the push pressed — the knob itself (a one-piece
		// push encoder, e.g. Uno's) or the button's own name (e.g. "Freeze").
		bool itself = list[index] == target;
		std::string what = itself ? string::f("KNOB %d", index + 1) : target->getParamQuantity()->getLabel();
		showKeyMessage("PUSH", what, "");
		return true;
	}
	// The last control a push pressed (for the badge dot: solid while pushed).
	int64_t lastPushModuleId = -1;
	int lastPushParamId = -1;
	double lastPushStart = -1.0;
	// Is knob i's push being pressed right now (or was it just tapped)?
	bool pushShowing(ParamWidget* target) {
		if (!target || !target->getParamQuantity()) return false;
		bool same = target->module && target->module->id == lastPushModuleId &&
			target->getParamQuantity()->paramId == lastPushParamId;
		if (!same) return false;
		// Held: while the press lasts. A quick tap (e.g. the K70's instant
		// push) stays solid a moment, so you can see it happen.
		return activePush.moduleId >= 0 || system::getTime() - lastPushStart < 0.15;
	}

	// Is the push physically held right now (key or wheel button)?
	bool pushHeldNow() {
		bool key = pushKey() >= 0 && glfwGetKey(APP->window->win, pushKey()) == GLFW_PRESS;
		bool wheel = settingWheelClickPush &&
			glfwGetMouseButton(APP->window->win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
		return key || wheel;
	}

	// A push on a knob that has NO push link.
	bool unlinkedPushAt(int index) {
		if (!settingDoubleTapReset) return false;
		std::vector<ParamWidget*>& list = currentKnobs();
		if (!selectedModule() || index < 0 || index >= (int) list.size()) return false;
		ParamWidget* knob = list[index];
		int64_t moduleId = knob->module->id;
		int paramId = knob->getParamQuantity()->paramId;

		double now = system::getTime();
		bool sameKnob = (moduleId == lastTapModuleId && paramId == lastTapParamId);
		if (sameKnob && now - lastTapTime < DOUBLE_TAP_TIME) {
			// Second tap: give the knob the same double-click a mouse would.
			// Whatever that knob does on double-click happens — usually reset
			// to default (with Undo); knobs that switch double-click off
			// (like Uno's placement encoder) simply ignore it.
			Widget::DoubleClickEvent e;
			knob->onDoubleClick(e);
			showKeyMessage("PUSH x2", "RESET", "");   // (panel readout only)
			lastTapTime = -1.0;   // a third tap starts a new pair
		}
		else {
			lastTapTime = now;
			lastTapModuleId = moduleId;
			lastTapParamId = paramId;
		}
		return true;
	}

	// Let go of the current press (if any).
	void endPush() {
		if (activePush.moduleId < 0) return;
		ModuleWidget* mw = APP->scene->rack->getModule(activePush.moduleId);
		ParamWidget* pw = mw ? findParamWidget(mw, activePush.paramId) : nullptr;
		if (pw) pressControl(pw, false);
		activePush = ActivePush();
	}

	// Push links gesture: hold a number + Ctrl+Alt-click a control.
	//   Click the knob itself -> its push is linked (push encoders like Uno's).
	//   Click a button        -> that button is the knob's push.
	//   Click the linked one again -> unlinked.
	bool linkPushClicked(ParamWidget* clicked) {
		ModuleWidget* mw = selectedModule();
		std::vector<ParamWidget*>& list = currentKnobs();
		int index = heldIndex();
		if (!mw || index < 0 || index >= (int) list.size()) return false;
		if (!clicked || clicked->module != mw->module || !clicked->getParamQuantity()) return false;

		int knobId = list[index]->getParamQuantity()->paramId;
		int targetId = clicked->getParamQuantity()->paramId;
		std::map<int, int>& links = settingRemaps[remapKey(mw)].pushLinks;
		// Clicking what it's ALREADY linked to (yours or automatic) unlinks
		// it — stored as -1, so an automatic link stays off. Anything else
		// becomes the new link.
		ParamWidget* current = pushTargetFor(index);
		if (current == clicked) links[knobId] = -1;
		else links[knobId] = targetId;
		return true;
	}

	// Is a number held while a module is selected? Then the dial / arrows
	// belong to us, not to Rack.
	bool playing() {
		return heldIndex() >= 0 && selectedModule();
	}

	// =========================================================================
	//  UNDO for set-up changes (Ctrl+Z)
	//  Custom keys, remaps, push links, sensitivity resets (per module type)
	//  and learned keys / layouts each go into Rack's undo list as ONE step,
	//  so a wrong Alt-click or a wrong key can be taken back. (Defined after
	//  the undo-step types further down.)
	// =========================================================================
	void remapUndoable(ModuleWidget* mw, const std::string& name, std::function<void()> change);
	void keysUndoable(const std::string& name, std::function<void()> change);

	// =========================================================================
	//  UNDO for turns (Ctrl+Z)
	//  Like a mouse drag, a whole turning gesture is ONE undo step: from the
	//  first click on a knob until you let go of its number, move to another
	//  knob, or pause for a second. Then it's handed to Rack's undo list.
	// =========================================================================
	struct TurnGesture {
		int64_t moduleId = -1;
		int paramId = -1;
		float oldValue = 0.f;
		double lastTime = 0.0;
	};
	TurnGesture turnGesture;
	static constexpr double TURN_GESTURE_PAUSE = 1.0;

	// Called just before a knob is turned.
	void turnStarting(ParamQuantity* pq) {
		double now = system::getTime();
		if (turnGesture.moduleId == pq->module->id && turnGesture.paramId == pq->paramId) {
			turnGesture.lastTime = now;   // same gesture carries on
			return;
		}
		finishTurn();   // a different knob: close the old gesture first
		turnGesture.moduleId = pq->module->id;
		turnGesture.paramId = pq->paramId;
		turnGesture.oldValue = pq->getValue();
		turnGesture.lastTime = now;
	}

	// Close the current gesture: if the knob really changed, one undo step.
	void finishTurn() {
		finishMultiTurn();   // (several knobs at once: their own undo step)
		if (turnGesture.moduleId < 0) return;
		Module* m = APP->engine->getModule(turnGesture.moduleId);
		if (m && turnGesture.paramId < (int) m->paramQuantities.size()) {
			ParamQuantity* pq = m->paramQuantities[turnGesture.paramId];
			float newValue = pq->getValue();
			if (newValue != turnGesture.oldValue) {
				history::ParamChange* h = new history::ParamChange;
				h->name = "turn " + pq->getLabel();
				h->moduleId = turnGesture.moduleId;
				h->paramId = turnGesture.paramId;
				h->oldValue = turnGesture.oldValue;
				h->newValue = newValue;
				APP->history->push(h);
			}
		}
		turnGesture = TurnGesture();
	}

	// Same for a toggle / multi-way switch pressed with a key (Rack's own
	// switches are undoable too; push buttons aren't — nothing stays changed).
	void pushSwitchUndo(ParamQuantity* pq, float oldValue) {
		if (pq->getValue() == oldValue) return;
		history::ParamChange* h = new history::ParamChange;
		h->name = "switch " + pq->getLabel();
		h->moduleId = pq->module->id;
		h->paramId = pq->paramId;
		h->oldValue = oldValue;
		h->newValue = pq->getValue();
		APP->history->push(h);
	}

	// Turn the held knob by some number of dial clicks (+ = up, - = down).
	// (accel = the acceleration multiplier for this turn; 1 = none)
	void turnHeldKnob(float clicks, float accel = 1.f) {
		// Several numbers held (menu: on by default): turn them ALL together.
		// One number held: exactly as always.
		std::vector<int> held = heldKnobsAll();
		if (held.size() >= 2) {
			turnSeveral(held, clicks, accel);
			return;
		}
		turnKnobAt(heldIndex(), clicks, accel);
	}

	// =========================================================================
	//  SEVERAL KNOBS AT ONCE (2026-11-20)
	//  Hold 2, 3 ... number keys and turn: every held knob moves together.
	//  Each one moves by the same amount of ITS OWN range from where it is
	//  (with its own sensitivity), so their balance stays the same. The
	//  whole turn is ONE undo step. Push, value box, remap clicks etc. still
	//  use the last number pressed. How many keys a keyboard reports at
	//  once is up to the keyboard (often 3 to 6; some combos fewer).
	// =========================================================================
	// Every knob whose number is held right now, on the current page (the
	// last one pressed first). Fewer than 2 = the normal one-knob turn.
	std::vector<int> heldKnobsAll() {
		std::vector<int> held;
		if (!settingMultiHold || heldCustom >= 0 || heldKnob < 0 || !selectedModule()) return held;
		int n = (int) currentKnobs().size();
		held.push_back(onPage(heldKnob));
		bool padIsKnobs = !settingKeys.padModes || padMode == PAD_KNOBS;
		for (int i = 0; i < NUM_KNOB_KEYS; i++) {
			if (i == heldKnob) continue;
			int index = onPage(i);
			if (index >= n) continue;
			for (int k : {settingKeys.knobs[i], padIsKnobs ? KNOB_PAD_KEYS[i] : -1}) {
				if (k >= 0 && glfwGetKey(APP->window->win, k) == GLFW_PRESS) {
					held.push_back(index);
					break;
				}
			}
		}
		return held;
	}
	// Is knob `index` being held (for the badges)?
	bool knobHeld(int index) {
		if (index == heldIndex()) return true;
		if (!settingMultiHold || heldKnob < 0) return false;
		for (int i : heldKnobsAll()) if (i == index) return true;
		return false;
	}

	// One undo step for a multi-knob turn: every knob's value before.
	struct MultiGesture {
		std::vector<TurnGesture> knobs;
		std::map<int, float> remainder;   // part-steps per knob index (stepped knobs)
		double lastTime = 0.0;
	};
	MultiGesture multiGesture;

	void turnSeveral(const std::vector<int>& indices, float clicks, float accel) {
		std::vector<ParamWidget*>& list = currentKnobs();
		std::vector<TurnGesture> now;
		for (int index : indices) {
			ParamQuantity* pq = list[index]->getParamQuantity();
			if (!pq) continue;
			TurnGesture g;
			g.moduleId = pq->module->id;
			g.paramId = pq->paramId;
			g.oldValue = pq->getValue();
			now.push_back(g);
		}
		// Same knobs as the gesture running? Carry on. Otherwise close
		// whatever was going (one knob or another set) and start this one.
		bool same = now.size() == multiGesture.knobs.size();
		for (size_t i = 0; same && i < now.size(); i++) {
			bool found = false;
			for (const TurnGesture& g : multiGesture.knobs)
				if (g.moduleId == now[i].moduleId && g.paramId == now[i].paramId) found = true;
			same = found;
		}
		if (!same) {
			finishTurn();
			multiGesture.knobs = now;
		}
		multiGesture.lastTime = system::getTime();
		for (int index : indices) applyTurn(index, clicks, accel, multiGesture.remainder[index]);
	}

	// Close a multi-knob turn: the knobs that changed = one undo step.
	void finishMultiTurn() {
		if (multiGesture.knobs.empty()) return;
		history::ComplexAction* complex = new history::ComplexAction;
		int changed = 0;
		for (const TurnGesture& g : multiGesture.knobs) {
			Module* m = APP->engine->getModule(g.moduleId);
			if (!m || g.paramId >= (int) m->paramQuantities.size()) continue;
			float newValue = m->paramQuantities[g.paramId]->getValue();
			if (newValue == g.oldValue) continue;
			history::ParamChange* h = new history::ParamChange;
			h->name = "turn " + m->paramQuantities[g.paramId]->getLabel();
			h->moduleId = g.moduleId;
			h->paramId = g.paramId;
			h->oldValue = g.oldValue;
			h->newValue = newValue;
			complex->push(h);
			changed++;
		}
		if (changed > 0) {
			complex->name = string::f("turn %d knobs", changed);
			APP->history->push(complex);
		}
		else delete complex;
		multiGesture = MultiGesture();
	}

	// Turn the knob at a place in the selected module's knob list (used by
	// the held knob and by MIDI controller knobs).
	// ---- Knobs that WRAP ROUND ----------------------------------------------
	// Endless knobs (e.g. Pulse's rings) turn round and round with the mouse,
	// but many of them only have a plain 0-360 value — so anything else that
	// turns them (Encoder, MIDI-MAP ...) hits a stop at the end. For these
	// knobs the Encoder goes round to the other side itself instead.
	// Guessed automatically:
	//   - angle knobs: a 0-360 (or any 360-wide) range in degrees ("°")
	//   - full-circle knobs: the knob's graphic turns all the way round
	// ... and you can change any knob by hand (SELECTED MODULE > Knobs that
	// wrap round), remembered for every module of that type.
	static bool autoWraps(ParamWidget* pw) {
		ParamQuantity* pq = pw ? pw->getParamQuantity() : nullptr;
		if (!pq || pq->snapEnabled) return false;   // (stepped knobs: no)
		float range = pq->getMaxValue() - pq->getMinValue();
		if (pq->unit.find("\xc2\xb0") != std::string::npos && std::fabs(range - 360.f) < 0.01f) return true;   // "°"
		if (Knob* k = dynamic_cast<Knob*>(pw))
			if (k->maxAngle - k->minAngle >= 2.f * float(M_PI) - 0.01f) return true;
		return false;
	}
	bool knobWraps(ParamWidget* pw) {
		ModuleWidget* mw = selectedModule();
		ParamQuantity* pq = pw ? pw->getParamQuantity() : nullptr;
		if (!mw || !pq) return false;
		auto it = settingRemaps.find(remapKey(mw));
		if (it != settingRemaps.end()) {
			auto w = it->second.wrapKnobs.find(pq->paramId);
			if (w != it->second.wrapKnobs.end()) return w->second;   // your choice wins
		}
		return autoWraps(pw);
	}

	void turnKnobAt(int index, float clicks, float accel = 1.f) {
		std::vector<ParamWidget*>& list = currentKnobs();
		if (index < 0 || index >= (int) list.size()) return;   // no knob with that number
		ParamQuantity* pq = list[index]->getParamQuantity();
		if (!pq) return;

		turnStarting(pq);   // (for Undo)
		applyTurn(index, clicks, accel, stepRemainder);
	}

	// Move knob `index` by some dial clicks (the sums only, no Undo).
	// remainder = where its part-steps are kept (stepped knobs).
	void applyTurn(int index, float clicks, float accel, float& stepRemainder) {
		std::vector<ParamWidget*>& list = currentKnobs();
		if (index < 0 || index >= (int) list.size()) return;
		ParamQuantity* pq = list[index]->getParamQuantity();
		if (!pq) return;

		// Sensitivity: this knob's own, or the overall one — EXCEPT on stepped
		// controls with only a few positions (octave / waveform / range
		// selectors, e.g. VCV OCT): those always move exactly one step per
		// dial click, so sensitivity or acceleration never makes them skip.
		// (Stepped controls with many positions still use sensitivity.)
		bool fewSteps = pq->snapEnabled && (pq->getMaxValue() - pq->getMinValue()) <= 24.f;
		if (!fewSteps) clicks *= accel * sensitivityFor(index);

		if (pq->snapEnabled) {
			// Stepped knob: one click = one step. Keep part-clicks for later.
			stepRemainder += clicks;
			int steps = (int) stepRemainder;   // whole steps only
			stepRemainder -= steps;
			if (steps != 0)
				pq->setValue(pq->getValue() + steps);
		}
		else if (knobWraps(list[index])) {
			// A knob that wraps round: past the end comes round to the start
			// (and below the start, round to the end) — never a stop.
			pq->setScaledValue(math::eucMod(pq->getScaledValue() + clicks * CLICK_FRACTION, 1.f));
		}
		else {
			// Smooth knob: one click = a fixed slice of its whole range.
			pq->setScaledValue(pq->getScaledValue() + clicks * CLICK_FRACTION);
		}
		// NO clamping here, on purpose. We hand over the whole turn and let
		// the knob's own value type decide what happens at its ends — the
		// same way Rack's own knobs work. Normal knobs still stop at their
		// ends (Rack's standard value type clamps by itself); knobs that wrap
		// round, or edit several things at once (e.g. Orbital's shared
		// encoders), get the full turn and handle it themselves.
	}

	// ---- Buttons ------------------------------------------------------------
	// A letter key went down: press that button on the selected module.
	// Returns false if there's no button with that letter (so the key is
	// left alone for Rack).
	// (keyIndex = which letter, 0 = Q; the button is on the current BUTTON page.)
	bool buttonPressed(int keyIndex, int key) {
		if (keyIndex < 0) return false;
		return pressButtonAt(onButtonPage(keyIndex), key, heldButtons[keyIndex]);
	}

	// Press the button at a place in the button list. A push button is
	// remembered in `slot` until its key comes up.
	bool pressButtonAt(int index, int key, HeldButton& slot) {
		std::vector<ParamWidget*>& list = currentButtons();
		if (index < 0 || index >= (int) list.size()) return false;
		ParamQuantity* pq = list[index]->getParamQuantity();
		if (!pq) return false;
		// Rack's own buttons say whether they're push buttons. Custom on/off
		// controls (not Rack's type) are treated as toggles.
		Switch* sw = dynamic_cast<Switch*>(list[index]);

		if (sw && sw->momentary) {
			// Push button: held down for as long as the key is. The module
			// itself sees a normal press/release, so any tap / hold /
			// double-tap behaviour it has (like Uno's) just works.
			releaseHeld(slot);   // (in case a release was missed)
			pq->setValue(pq->getMaxValue());
			slot.moduleId = pq->module->id;
			slot.paramId = pq->paramId;
			slot.key = key;
		}
		else {
			// Toggle / multi-way switch: each tap moves to the next
			// position, and wraps round to the first after the last.
			float before = pq->getValue();
			float v = std::round(before) + 1.f;
			if (v > pq->getMaxValue()) v = pq->getMinValue();
			pq->setValue(v);
			pushSwitchUndo(pq, before);   // undoable, like clicking it
		}
		lastButton = index;
		lastButtonTime = system::getTime();
		return true;
	}

	// A letter key came up: let go of the push button it was holding.
	void releaseButton(int index) {
		releaseHeld(heldButtons[index]);
	}

	void releaseHeld(HeldButton& hb) {
		if (hb.moduleId < 0) return;
		Module* m = APP->engine->getModule(hb.moduleId);
		if (m && hb.paramId < (int) m->paramQuantities.size()) {
			ParamQuantity* pq = m->paramQuantities[hb.paramId];
			pq->setValue(pq->getMinValue());
		}
		hb = HeldButton();
	}

	// =========================================================================
	//  CUSTOM MODE: any control on its own key (per module type)
	//  Shift+Alt-click a control on the selected module, press the key you
	//  want (Backspace = remove its key, Esc = cancel). Then:
	//    knob   -> hold its key + turn (dial, wheel, turn keys); push works too
	//    button -> tap its key
	//  It works on ANY page, alongside the normal numbers / letters, and its
	//  badge shows the custom key. Saved per module type, like remaps.
	// =========================================================================

	// Push buttons held by a custom key (key -> what it's holding).
	std::map<int, HeldButton> customHeldButtons;

	// Which knob / button of the selected module has this custom key
	// (place in the list), or -1. Read from the per-index caches below.
	int customKnobFor(int key) {
		currentKnobs();
		for (size_t i = 0; i < knobCustomKey.size(); i++) if (knobCustomKey[i] == key) return (int) i;
		return -1;
	}
	int customButtonFor(int key) {
		currentKnobs();
		for (size_t i = 0; i < buttonCustomKey.size(); i++) if (buttonCustomKey[i] == key) return (int) i;
		return -1;
	}

	// The custom key of each knob / button of the selected module (-1 =
	// none). Rebuilt with the control lists, not every frame.
	std::vector<int> knobCustomKey, buttonCustomKey;
	std::vector<std::string> knobCustomLabel, buttonCustomLabel;   // badge text
	void buildCustomKeys(ModuleWidget* mw) {
		knobCustomKey.assign(knobs.size(), -1);
		buttonCustomKey.assign(buttons.size(), -1);
		knobCustomLabel.assign(knobs.size(), "");
		buttonCustomLabel.assign(buttons.size(), "");
		auto it = mw ? settingRemaps.find(remapKey(mw)) : settingRemaps.end();
		if (it == settingRemaps.end()) return;
		const std::map<int, int>& ck = it->second.customKeys;
		for (size_t i = 0; i < knobs.size(); i++) {
			auto c = ck.find(knobs[i]->getParamQuantity()->paramId);
			if (c != ck.end()) {
				knobCustomKey[i] = c->second;
				knobCustomLabel[i] = keyName(c->second);
			}
		}
		for (size_t i = 0; i < buttons.size(); i++) {
			auto c = ck.find(buttons[i]->getParamQuantity()->paramId);
			if (c != ck.end()) {
				buttonCustomKey[i] = c->second;
				buttonCustomLabel[i] = keyName(c->second);
			}
		}
	}

	// A key went down: is it a custom key? Returns true if it was used.
	bool customPressed(int key) {
		if (!selectedModule()) return false;
		int k = customKnobFor(key);
		if (k >= 0) {
			if (heldCustom != k) finishTurn();
			heldCustom = k;
			heldCustomKey = key;
			stepRemainder = 0.f;
			return true;
		}
		int b = customButtonFor(key);
		if (b >= 0) return pressButtonAt(b, key, customHeldButtons[key]);
		return false;
	}

	// A key came up: let go of whatever custom control it was holding.
	void customReleased(int key) {
		if (key == heldCustomKey) {
			finishTurn();
			heldCustom = -1;
			heldCustomKey = -1;
		}
		auto it = customHeldButtons.find(key);
		if (it != customHeldButtons.end()) {
			releaseHeld(it->second);
			customHeldButtons.erase(it);
		}
	}

	// The control whose custom key is being learned (its param ID), or -1.
	int customLearnParam = -1;
	std::string customLearnName;

	// Shift+Alt-click on a control of the selected module: learn its key.
	bool customClicked(ParamWidget* clicked) {
		ModuleWidget* mw = selectedModule();
		if (!mw || !clicked || clicked->module != mw->module || !clicked->getParamQuantity()) return false;
		customLearnParam = clicked->getParamQuantity()->paramId;
		customLearnName = clicked->getParamQuantity()->getLabel();
		startLearn(KA_CUSTOM);
		return true;
	}

	// The key pressed while learning a custom key (Backspace = remove it).
	void customLearned(int key, int mods) {
		ModuleWidget* mw = selectedModule();
		int paramId = customLearnParam;
		customLearnParam = -1;
		if (!mw) return;
		std::map<int, int>& ck = settingRemaps[remapKey(mw)].customKeys;

		if (key == GLFW_KEY_BACKSPACE) {
			ck.erase(paramId);
			knobsForModuleId = -1;   // rebuild the caches
			showKeyMessage("HOTKEY", "REMOVED", customLearnName + " no longer has its own hotkey");
			return;
		}

		// Clashes: another control of this module type with this key, or
		// one of the Encoder's own keys used on its own.
		std::string clash;
		for (const auto& c : ck)
			if (c.second == key && c.first != paramId) clash = "another control on this module";
		if (clash.empty()) {
			for (const UsedKey& u : usedKeys()) {
				if (u.b.key == key && u.b.mods == 0 && u.action >= 0) {
					clash = actionName(u.action);
					if (u.action >= KA_KNOB) clash += string::f(" %d", u.index + 1);
					break;
				}
			}
		}
		ck[paramId] = key;
		knobsForModuleId = -1;   // rebuild the caches

		std::string name = keyName(key);
		if (mods != 0) name += "  (Shift/Ctrl/Alt are ignored)";
		if (!clash.empty())
			showKeyMessage("CLASH", keyName(key), customLearnName + " = " + keyName(key) + ", but " + keyName(key) +
				" is also " + clash + " - its own hotkey wins on this module");
		else
			showKeyMessage("HOTKEY", keyName(key), customLearnName + " = " + name);
	}

	// =========================================================================
	//  MIDI CONTROLLER MODE ("follow the selected module")
	//  A hardware controller's knobs / buttons are linked to ROLES (knob 3,
	//  button 5, slot 2, next page ...), so the same physical knob plays
	//  knob 3 of WHATEVER module is selected, on the current page.
	//    Learn: right-click > MIDI mappings > click a box in the grid, then
	//           wiggle the knob (both ways, for an endless encoder) or press
	//           the button. The knob type (normal / endless, and which endless
	//           code) is detected automatically — or right-click the box to set
	//           it by hand, along with the CC / note number.
	//    Pots / faders (absolute) CATCH UP by default: they move the knob from
	//    where it is, never jumping (pickup / relative are menu choices).
	//  All read on the UI thread; nothing touches the audio thread.
	// =========================================================================
	midi::InputQueue midiInput;
	midi::InputQueue midiInput2;   // a second controller (optional)

	// The roles. (knobPressN = pressing knob N, like the dial push — for
	// push encoders on the controller.)
	static const std::vector<std::string>& midiRoles() {
		static std::vector<std::string> roles;
		if (roles.empty()) {
			for (const char* kind : {"knob", "knobPress", "button", "slot"})
				for (int i = 1; i <= KEY_ROW; i++) roles.push_back(string::f("%s%d", kind, i));
			for (const char* r : {"pageNext", "pagePrev", "groupNext", "groupPrev", "groupNew", "groupClear",
					"navTurn", "navPush", "sensUp", "sensDown", "buttonPageNext", "lock"})
				roles.push_back(r);
		}
		return roles;
	}
	static std::string midiRoleLabel(const std::string& role) {
		auto numbered = [&](const char* prefix, const char* label) -> std::string {
			size_t n = std::strlen(prefix);
			if (role.compare(0, n, prefix) == 0 && role.size() > n && std::isdigit((unsigned char) role[n]))
				return std::string(label) + " " + role.substr(n);
			return "";
		};
		for (auto p : std::vector<std::pair<const char*, const char*>>{
				{"knobPress", "Knob press"}, {"knob", "Knob"}, {"button", "Button"}, {"slot", "Slot"}}) {
			std::string s = numbered(p.first, p.second);
			if (!s.empty()) return s;
		}
		if (role == "pageNext") return "Next page";
		if (role == "pagePrev") return "Previous page";
		if (role == "groupNext") return "Next group";
		if (role == "groupPrev") return "Previous group";
		if (role == "groupNew") return "New group";
		if (role == "groupClear") return "Clear this group";
		if (role == "navTurn") return "Navigation knob (turn = module)";
		if (role == "navPush") return "Navigation knob (press = next page)";
		if (role == "sensUp") return "Sensitivity +";
		if (role == "sensDown") return "Sensitivity -";
		if (role == "buttonPageNext") return "Next button page";
		if (role == "lock") return "Lock on / off";
		return role;
	}
	static const char* midiModeName(int mode) {
		static const char* names[NUM_MIDI_MODES] = {"Auto", "Absolute (normal knob)",
			"Relative (64 = no change)", "Relative (two's complement)", "Relative (signed bit)"};
		return names[clamp(mode, 0, NUM_MIDI_MODES - 1)];
	}
	static std::string midiMappingText(const MidiMapping& m) {
		if (m.type == MIDI_NONE) return "not set";
		std::string input = (m.port == 1) ? ", input 2" : "";
		if (m.type == MIDI_PITCHBEND) return "Pitch bend" + std::string(m.invert ? " (inverted" + input + ")" : (input.empty() ? "" : " (input 2)"));
		if (m.type == MIDI_PROGRAM) return string::f("Program %d", m.number) + (input.empty() ? "" : " (input 2)");
		std::string s = string::f("%s %d", m.type == MIDI_CC ? "CC" : "Note", m.number);
		if (m.toggle) return s + " (toggle button" + input + ")";
		if (m.type == MIDI_NOTE) return s + (input.empty() ? "" : " (input 2)");
		if (m.type == MIDI_CC) {
			static const char* shortNames[NUM_MIDI_MODES] = {"auto", "absolute", "rel 64", "rel 2's", "rel signed"};
			s += std::string(" (") + shortNames[m.effectiveMode()] + (m.mode == MODE_AUTO ? ", auto" : "") +
				(m.invert ? ", inverted" : "") + input + ")";
		}
		return s;
	}

	// ---- Learning ---------------------------------------------------------
	std::string midiLearnRole;          // "" = not learning
	double midiLearnStart = -1.0, midiLearnFirst = -1.0;
	MidiMapping midiLearnMap;
	std::vector<int> midiLearnValues;   // CC values seen, for auto-detection
	static constexpr double MIDI_LEARN_TIMEOUT = 10.0;
	static constexpr double MIDI_LEARN_COLLECT = 1.2;   // keep listening this long after the first CC

	void startMidiLearn(const std::string& role) {
		learnAction = -1;
		midiLearnRole = role;
		midiLearnStart = system::getTime();
		midiLearnFirst = -1.0;
		midiLearnMap = MidiMapping();
		midiLearnValues.clear();
	}
	void cancelMidiLearn() {
		midiLearnRole = "";
		showKeyMessage("MIDI", "CANCELLED", "MIDI learn cancelled - nothing changed");
	}

	// AUTO-DETECT: what kind of knob sent these values?
	//   Endless encoders send small steps ("up 1, up 1, down 1") — the SAME
	//   value over and over — in one of three codes. A normal knob sweeps
	//   through changing values and rarely repeats one.
	static int detectMode(const std::vector<int>& v) {
		if (v.size() < 3) return MODE_ABSOLUTE;
		int repeats = 0;
		for (size_t i = 1; i < v.size(); i++) if (v[i] == v[i - 1]) repeats++;
		if (repeats * 3 < (int) v.size()) return MODE_ABSOLUTE;   // mostly changing = a normal knob
		auto all = [&](std::function<bool(int)> f) {
			for (int x : v) if (!f(x)) return false;
			return true;
		};
		bool twos = all([](int x) { return (x >= 1 && x <= 15) || (x >= 113 && x <= 127); });
		bool signedBit = all([](int x) { return (x >= 1 && x <= 15) || (x >= 65 && x <= 79); });
		bool offset64 = all([](int x) { return x >= 50 && x <= 78 && x != 64; });
		bool anyHigh2 = false, anyHighSigned = false;
		for (int x : v) {
			if (x >= 113) anyHigh2 = true;
			if (x >= 65 && x <= 79) anyHighSigned = true;
		}
		if (offset64 && !twos) return MODE_REL_64;
		if (twos && anyHigh2) return MODE_REL_TWOS;
		if (signedBit && anyHighSigned) return MODE_REL_SIGNED;
		if (twos) return MODE_REL_TWOS;   // only turned one way: the most common code
		return MODE_ABSOLUTE;
	}

	// Finish learning: store the mapping for the role.
	void finishMidiLearn() {
		if (midiLearnRole.empty()) return;
		MidiMapping m = midiLearnMap;
		if (m.type == MIDI_CC) m.detected = detectMode(midiLearnValues);
		// A role's old mode choice and invert survive re-learning; Auto stays Auto.
		auto old = settingMidiMap.find(midiLearnRole);
		m.mode = (old != settingMidiMap.end()) ? old->second.mode : MODE_AUTO;
		m.invert = (old != settingMidiMap.end()) && old->second.invert;
		m.toggle = (old != settingMidiMap.end()) && old->second.toggle;
		settingMidiMap[midiLearnRole] = m;
		showKeyMessage("MIDI", "LEARNED", midiRoleLabel(midiLearnRole) + " = " + midiMappingText(m), 6.0);
		midiLearnRole = "";
		absState.clear();
	}

	// ---- Playing ------------------------------------------------------------
	// What we remember per controller pot (per role), between messages.
	struct AbsState {
		float last = -1.f;        // last position the pot sent (0-1; -1 = none yet) — kept
		                          //  across module switches: it's about the pot
		bool picked = false;      // PICKUP: has the pot reached the knob yet?
		int64_t moduleId = -1;    // which knob it's playing right now
		int paramId = -1;
		// CATCHUP / RELATIVE: our own exact knob position. A stepped knob rounds
		// what we write, which would swallow small movements; so we keep the
		// unrounded value ourselves, and only re-read the knob if something
		// else (the mouse, another controller) has moved it since.
		float value = -1.f;       // our unrounded position (-1 = not yet)
		float written = -1.f;     // what the knob read just after we wrote it
	};
	std::map<std::string, AbsState> absState;
	int navAbsLast = -1;          // last absolute value for the nav encoder role
	float navMidiAccum = 0.f;
	HeldButton midiHeldButtons[KEY_ROW];
	// The knob a MIDI knob last moved, for the value readout.
	int lastMidiKnob = -1;
	double lastMidiKnobTime = -1.0;

	static int decodeRelative(int mode, int v) {
		switch (mode) {
			case MODE_REL_64: return v - 64;
			case MODE_REL_TWOS: return (v < 64) ? v : v - 128;
			case MODE_REL_SIGNED: return (v & 64) ? -(v & 63) : (v & 63);
		}
		return 0;
	}

	// A normal knob (pot / fader / pitch bend wheel, sends its POSITION, here
	// 0-1). The module knob usually isn't where the pot is, so
	// settingMidiPotMode decides how the two come together (see plugin.hpp).
	// Values here are 0-1 of the range.
	void absoluteKnob(const std::string& role, int index, float position) {
		std::vector<ParamWidget*>& list = currentKnobs();
		if (index < 0 || index >= (int) list.size()) return;
		ParamQuantity* pq = list[index]->getParamQuantity();
		if (!pq) return;
		AbsState& st = absState[role];
		if (st.moduleId != pq->module->id || st.paramId != pq->paramId) {
			// A different knob now (new module or page): start afresh with it,
			// but remember where the pot is.
			float last = st.last;
			st = AbsState();
			st.last = last;
			st.moduleId = pq->module->id;
			st.paramId = pq->paramId;
		}
		float pot = position;
		float before = st.last;   // pot's previous position (-1 = none yet)
		st.last = position;
		lastMidiKnob = index;
		lastMidiKnobTime = system::getTime();

		float knob = pq->getScaledValue();
		float target;
		bool wraps = knobWraps(list[index]);

		if (wraps) {
			// ---- A knob that WRAPS ROUND: the pot simply turns it the way you
			//      move it, round and round — no catching up, no stop. --------
			if (before < 0.f) return;
			float from = (st.value >= 0.f && std::fabs(knob - st.written) < 1e-6f) ? st.value : knob;
			target = math::eucMod(from + (pot - before), 1.f);
			st.value = target;
		}
		else if (settingMidiPotMode == POT_PICKUP) {
			// ---- PICKUP: wait until the pot reaches (or passes) the knob ----
			if (!st.picked) {
				if (std::fabs(pot - knob) < 0.02f) st.picked = true;
				else if (before >= 0.f && (before - knob) * (pot - knob) <= 0.f) st.picked = true;
			}
			if (!st.picked) return;
			target = pot;
		}
		else {
			// ---- CATCHUP / RELATIVE: move from where the knob IS ------------
			// The first message from a pot can't say which way it's turning
			// (we don't know where it was), so it only sets the starting point.
			if (before < 0.f) return;
			// Our unrounded position, unless the knob was moved by something else.
			float from = (st.value >= 0.f && std::fabs(knob - st.written) < 1e-6f) ? st.value : knob;
			float move = pot - before;
			if (settingMidiPotMode == POT_RELATIVE) {
				target = from + move;
			}
			else {
				// CATCHUP: share out the room left so knob and pot reach the end
				// together. Turning up: the knob covers its remaining room
				// (1 - from) in the time the pot covers ITS remaining room
				// (1 - before). Turning down: the same, towards 0. Once knob
				// and pot match, this is exactly "knob = pot".
				if (move > 0.f) target = (before < 1.f) ? from + move * (1.f - from) / (1.f - before) : 1.f;
				else            target = (before > 0.f) ? from + move * from / before : 0.f;
			}
			target = clamp(target, 0.f, 1.f);
			st.value = target;
		}
		turnStarting(pq);   // (for Undo, like any turn)
		pq->setScaledValue(target);
		st.written = pq->getScaledValue();
	}

	// (port = which MIDI input it arrived on: 0 = input 1, 1 = input 2.)
	void midiMessage(const midi::Message& msg, int port) {
		int status = msg.getStatus();
		bool isCC = (status == 0xb);
		bool noteOn = (status == 0x9 && msg.getValue() > 0);
		bool noteOff = (status == 0x8 || (status == 0x9 && msg.getValue() == 0));
		bool isBend = (status == 0xe);      // pitch bend wheel
		bool isProgram = (status == 0xc);   // program change (one data byte)
		if (!isCC && !noteOn && !noteOff && !isBend && !isProgram) return;
		int number = msg.getNote(), value = msg.getValue();
		// Pitch bend: one 14-bit position (fine resolution), 0-1.
		float bend = isBend ? ((msg.getValue() << 7) | msg.getNote()) / 16383.f : 0.f;
		if (isBend) number = 0;   // (there's only one pitch bend wheel)

		// ---- Learning: the first CC / note / bend / program decides; a CC
		//      then keeps listening a moment longer to see what kind of knob
		//      it is.
		if (!midiLearnRole.empty()) {
			if (midiLearnMap.type == MIDI_NONE) {
				midiLearnMap.port = port;
				if (isCC) {
					midiLearnMap.type = MIDI_CC;
					midiLearnMap.number = number;
					midiLearnFirst = system::getTime();
					midiLearnValues.push_back(value);
				}
				else if (noteOn || isProgram || isBend) {
					midiLearnMap.type = noteOn ? MIDI_NOTE : isProgram ? MIDI_PROGRAM : MIDI_PITCHBEND;
					midiLearnMap.number = number;
					finishMidiLearn();
				}
			}
			else if (isCC && midiLearnMap.type == MIDI_CC && number == midiLearnMap.number && port == midiLearnMap.port) {
				midiLearnValues.push_back(value);
			}
			return;
		}

		// (Lock does NOT stop MIDI: Lock is for typing without setting off the
		//  Encoder's keyboard keys. A MIDI controller keeps working while locked.)

		// ---- Playing: every role linked to this control --------------------
		for (const auto& kv : settingMidiMap) {
			const MidiMapping& m = kv.second;
			if (m.number != number || m.port != port) continue;
			// Program change: one press (a tap: pressed, let go a moment later).
			if (m.type == MIDI_PROGRAM && isProgram) {
				if (!isTurnRole(kv.first)) tapRole(kv.first, m);
				continue;
			}
			// Pitch bend wheel: a smooth knob (on a knob box); on a button box,
			// pushed past three-quarters = pressed.
			if (m.type == MIDI_PITCHBEND && isBend) {
				float pos = m.invert ? 1.f - bend : bend;
				int knob = -1;
				if (isTurnRole(kv.first) && kv.first.compare(0, 4, "knob") == 0)
					knob = std::atoi(kv.first.c_str() + 4) - 1;
				if (knob >= 0) absoluteKnob(kv.first, onPage(knob), pos);
				else if (kv.first == "navTurn") midiRole(kv.first, m, (int) std::round(bend * 127.f), false, true);   // (invert applied there)
				else midiRole(kv.first, m, (int) std::round(pos * 127.f), pos > 0.75f, false);
				continue;
			}
			bool match = (m.type == MIDI_CC && isCC) || (m.type == MIDI_NOTE && (noteOn || noteOff));
			if (!match) continue;
			// For buttons: a note-on, or a CC at 64 or more, is "pressed".
			bool down = (m.type == MIDI_NOTE) ? noteOn : value >= 64;
			// A controller button in TOGGLE mode: every press (whether it
			// sends "on" or "off") = one quick tap.
			if (m.toggle && !isTurnRole(kv.first)) {
				toggleTap(kv.first, m, down);
				continue;
			}
			midiRole(kv.first, m, value, down, m.type == MIDI_CC);
		}
	}

	// Things that TURN (knobs, the navigation knob) — everything else is a button.
	static bool isTurnRole(const std::string& role) {
		return (role.compare(0, 4, "knob") == 0 && role.compare(0, 9, "knobPress") != 0) || role == "navTurn";
	}

	// ---- Toggle-mode controller buttons --------------------------------------
	// The controller only changes between "on" and "off" on each press, so a
	// change either way = one press: we press, then let go a moment later
	// (long enough for the module to notice).
	std::map<std::string, bool> toggleState;   // the button's last state, per role
	struct PendingRelease {
		std::string role;
		MidiMapping m;
		double at;
	};
	std::vector<PendingRelease> pendingReleases;
	static constexpr double TAP_TIME = 0.06;

	void toggleTap(const std::string& role, const MidiMapping& m, bool down) {
		bool& last = toggleState[role];
		if (down == last) return;   // (a repeat of the same state: not a press)
		last = down;
		tapRole(role, m);
	}
	// One quick press of a button role: pressed now, let go a moment later
	// (toggle-mode buttons, program changes).
	void tapRole(const std::string& role, const MidiMapping& m) {
		ccButtonDown[role] = false;   // a fresh press, for the edge detector
		midiPressDown[role] = false;
		midiRole(role, m, 127, true, m.type == MIDI_CC);
		pendingReleases.push_back({role, m, system::getTime() + TAP_TIME});
	}
	void releaseDueTaps(double now) {
		for (size_t i = 0; i < pendingReleases.size();) {
			if (now >= pendingReleases[i].at) {
				PendingRelease p = pendingReleases[i];
				pendingReleases.erase(pendingReleases.begin() + i);
				midiRole(p.role, p.m, 0, false, p.m.type == MIDI_CC);
				ccButtonDown[p.role] = false;
			}
			else i++;
		}
	}

	// What one role does with a message.
	void midiRole(const std::string& role, const MidiMapping& m, int value, bool down, bool isCC) {
		auto numberAfter = [&](const char* prefix) -> int {
			size_t n = std::strlen(prefix);
			if (role.compare(0, n, prefix) != 0) return -1;
			return std::atoi(role.c_str() + n) - 1;   // 0-based
		};
		// Invert (per mapping, right-click its grid box): a pot's position is
		// read from the other end; an encoder's "up" and "down" swap.
		int mode = m.effectiveMode();
		if (isCC && m.invert && mode == MODE_ABSOLUTE) value = 127 - value;
		int sign = m.invert ? -1 : 1;

		int knob = numberAfter("knob");
		if (knob >= 0 && isCC) {
			int index = onPage(knob);
			if (mode == MODE_ABSOLUTE) absoluteKnob(role, index, value / 127.f);
			else {
				int delta = sign * decodeRelative(mode, value);
				turnKnobAt(index, (float) delta);
				lastMidiKnob = index;
				lastMidiKnobTime = system::getTime();
			}
			return;
		}
		// Knob press (push encoders on the controller): pressed / let go, like
		// holding the dial push. CC buttons count on the way past 64.
		int press = numberAfter("knobPress");
		if (press >= 0) {
			bool was = midiPressDown[role];
			midiPressDown[role] = down;
			if (down && !was) knobPressed(onPage(press), false, true);
			else if (!down && was && activePush.fromMidi) activePush.keyUp = true;
			return;
		}
		int button = numberAfter("button");
		if (button >= 0) {
			// Remember the press per button, so its release lets the same one go.
			bool wasDown = midiHeldButtons[button].moduleId != -1;   // (-2 = a toggle, still down)
			if (down && !wasDown) {
				std::vector<ParamWidget*>& list = currentButtons();
				int index = onButtonPage(button);
				Switch* sw = (index < (int) list.size()) ? dynamic_cast<Switch*>(list[index]) : nullptr;
				pressButtonAt(index, -1, midiHeldButtons[button]);
				// A toggle doesn't stay "held": mark it so a CC button isn't
				// pressed again until it's been let go.
				if (!(sw && sw->momentary)) midiHeldButtons[button].moduleId = -2;
			}
			else if (!down) {
				if (midiHeldButtons[button].moduleId >= 0) releaseHeld(midiHeldButtons[button]);
				midiHeldButtons[button] = HeldButton();
			}
			return;
		}
		int slot = numberAfter("slot");
		if (slot >= 0) {
			if (down) selectSlot(slot);
			return;
		}
		if (role == "navTurn" && isCC) {
			if (mode == MODE_ABSOLUTE) {
				// A normal knob: every 4 values = one step.
				if (navAbsLast >= 0) navMidiAccum += (value - navAbsLast) / 4.f;
				navAbsLast = value;
			}
			else navMidiAccum += sign * decodeRelative(mode, value);
			while (navMidiAccum >= 1.f) { navTurn(+1); navMidiAccum -= 1.f; }
			while (navMidiAccum <= -1.f) { navTurn(-1); navMidiAccum += 1.f; }
			return;
		}
		// The rest are buttons that act when pressed (CC buttons: on the way up past 64).
		if (!down) return;
		bool edge = !isCC || pressEdge(role, true);
		if (!edge) return;
		if (role == "pageNext") nextPage();
		else if (role == "pagePrev") {
			if (selectedModule()) {
				if (heldKey >= 0) numberReleased(heldKey);
				int pc = pageCount();
				page = ((page - 1) % pc + pc) % pc;
			}
		}
		else if (role == "groupNext") switchGroup(+1);
		else if (role == "groupPrev") switchGroup(-1);
		else if (role == "groupNew") newGroup();
		else if (role == "groupClear") clearCurrentGroup();
		else if (role == "sensUp") changeSensitivity(true);
		else if (role == "sensDown") changeSensitivity(false);
		else if (role == "buttonPageNext") nextButtonPage();
		else if (role == "lock") toggleLock();
		else if (role == "navPush") navPush();
	}
	std::map<std::string, bool> midiPressDown;   // knob presses: down right now?

	// CC "buttons" send 127 then 0; a few send 127 repeatedly. Only the
	// change from up to down counts as a press.
	std::map<std::string, bool> ccButtonDown;
	bool pressEdge(const std::string& role, bool down) {
		bool was = ccButtonDown[role];
		ccButtonDown[role] = down;
		return down && !was;
	}
	void midiReleaseEdges(const midi::Message& msg, int port) {
		// CC below 64 = released, for the edge detector above.
		if (msg.getStatus() != 0xb || msg.getValue() >= 64) return;
		for (const auto& kv : settingMidiMap)
			if (kv.second.type == MIDI_CC && kv.second.number == msg.getNote() && kv.second.port == port)
				ccButtonDown[kv.first] = false;
	}

	// Every frame (active Encoder only): read what's arrived, and finish a
	// CC learn once it's had a moment to see what kind of knob it is.
	void midiFrame(double now) {
		midi::Message msg;
		// Both MIDI inputs (the second is for a second controller).
		while (midiInput.tryPop(&msg, INT64_MAX)) {
			midiReleaseEdges(msg, 0);
			midiMessage(msg, 0);
		}
		while (midiInput2.tryPop(&msg, INT64_MAX)) {
			midiReleaseEdges(msg, 1);
			midiMessage(msg, 1);
		}
		releaseDueTaps(now);   // (toggle-mode controller buttons let go)
		if (!midiLearnRole.empty()) {
			if (midiLearnFirst >= 0 && now - midiLearnFirst > MIDI_LEARN_COLLECT) finishMidiLearn();
			else if (midiLearnFirst < 0 && now - midiLearnStart > MIDI_LEARN_TIMEOUT) {
				midiLearnRole = "";
				showKeyMessage("NO MIDI", "SEEN", "Nothing arrived from MIDI. Check right-click > MIDI input "
					"(driver, device, channel), then learn again.", 8.0);
			}
		}
	}

	// A number field inside a menu, for typing a CC / note number by hand.
	struct NumberField : ui::TextField {
		std::function<void(int)> onNumber;
		NumberField() {
			box.size.x = 100.f;
			placeholder = "0-127, Enter";
		}
		void onAction(const ActionEvent& e) override {
			bool digits = !text.empty() && std::all_of(text.begin(), text.end(), ::isdigit);
			int v = digits ? std::atoi(text.c_str()) : -1;
			if (v >= 0 && v <= 127 && onNumber) {
				onNumber(v);
				placeholder = string::f("set to %d", v);   // (the menu stays open; the grid shows it)
			}
			else placeholder = "0-127, Enter";
			text = "";
			e.consume(this);
		}
	};

	// =========================================================================
	//  MIDI MAPPINGS GRID (inside the right-click menu)
	//  Every role on one small screen instead of submenus four deep:
	//
	//      Knobs         [1][2][3][4][5][6][7][8][9][0]
	//      Knob press    [1][2][3][4][5][6][7][8][9][0]   (push encoders)
	//      Buttons       [Q][W][E][R][T][Y][U][I][O][P]
	//      Slots         [1][2][3][4][5][6][7][8][9][0]
	//      Page / group  [Pg+][Pg-][Gr+][Gr-][New][Clr]
	//      More          [Nav][Press][Sens+][Sens-][Btn pg][Lock]
	//      (line describing the box under the mouse)
	//  With these, everything can be played from a controller once set up.
	//
	//  A lit box is mapped. Click a box = learn it: the box blinks, you move or
	//  press the control on your controller, it lights up — and the menu STAYS
	//  OPEN, so you can map one box after another. Right-click a box = the
	//  rarely needed options: type a CC / note number, knob type, invert, clear.
	//  It draws straight from the settings, so it's always up to date.
	//
	//  Rack's computer-keyboard MIDI can't hear keys while a menu is open, so
	//  while the grid waits it passes your key presses on to it itself — so
	//  learning works with the computer keyboard as MIDI device too.
	// =========================================================================
	struct MidiGrid : widget::OpaqueWidget {
		EncoderWidget* owner = nullptr;
		std::set<int> keysToRack;   // keys we passed to the keyboard MIDI (their release must follow)

		~MidiGrid() override {
			for (int k : keysToRack) keyboard::release(k);   // menu closed with a key down: no stuck note
		}
		// While learning, the grid takes the keyboard (it's selected after the
		// click): Esc cancels, every other key goes on to Rack's keyboard MIDI.
		void onSelectKey(const SelectKeyEvent& e) override;   // (below EncoderWidget)
		static constexpr float LABEL_W = 84.f;   // row-name column
		static constexpr float CELL = 26.f;      // box pitch (knob / button / slot rows)
		static constexpr float WIDE = 43.f;      // box pitch (last row: wider labels)
		static constexpr float ROW = BND_WIDGET_HEIGHT + 2.f;
		static constexpr int ROWS = 6;
		Vec mouse = Vec(-1.f, -1.f);             // where the mouse is (-1 = not over us)

		MidiGrid() {
			box.size = Vec(LABEL_W + KEY_ROW * CELL + 4.f, ROWS * ROW + BND_WIDGET_HEIGHT + 4.f);
		}

		// The role in row r, box c ("" if there's no box there).
		static constexpr int WIDE_ROW = 4;       // rows from here on have 6 wide boxes
		static std::string roleAt(int r, int c) {
			static const char* pageRow[6] = {"pageNext", "pagePrev", "groupNext", "groupPrev", "groupNew", "groupClear"};
			static const char* moreRow[6] = {"navTurn", "navPush", "sensUp", "sensDown", "buttonPageNext", "lock"};
			if (r < 0 || r >= ROWS || c < 0) return "";
			if (r == 4) return c < 6 ? pageRow[c] : "";
			if (r == 5) return c < 6 ? moreRow[c] : "";
			if (c >= KEY_ROW) return "";
			static const char* kinds[4] = {"knob", "knobPress", "button", "slot"};
			return string::f("%s%d", kinds[r], c + 1);
		}
		// What's written on a box: the key that plays it, so it reads like the keyboard.
		static std::string boxLabel(int r, int c) {
			static const char* pageRow[6] = {"Pg+", "Pg-", "Gr+", "Gr-", "New", "Clr"};
			static const char* moreRow[6] = {"Nav", "Press", "Sens+", "Sens-", "Btn pg", "Lock"};
			if (r == 4) return pageRow[c];
			if (r == 5) return moreRow[c];
			// (Knob press boxes show the knob's key, like the Knobs row.)
			const int* keys = (r <= 1) ? settingKeys.knobs : (r == 2) ? settingKeys.buttons : settingKeys.slots;
			return keys[c] >= 0 ? keyName(keys[c]) : padLabel(c);
		}
		static Rect boxRect(int r, int c) {
			float pitch = (r >= WIDE_ROW) ? WIDE : CELL;
			return Rect(Vec(LABEL_W + c * pitch, r * ROW + 1.f), Vec(pitch - 3.f, BND_WIDGET_HEIGHT - 2.f));
		}
		// Which box is the mouse over? (row, column) or (-1, -1).
		void hovered(int& r, int& c) {
			r = c = -1;
			for (int rr = 0; rr < ROWS; rr++)
				for (int cc = 0; cc < KEY_ROW; cc++)
					if (!roleAt(rr, cc).empty() && boxRect(rr, cc).contains(mouse)) { r = rr; c = cc; return; }
		}
		static bool mapped(const std::string& role) {
			auto it = settingMidiMap.find(role);
			return it != settingMidiMap.end() && it->second.type != MIDI_NONE;
		}

		void onHover(const HoverEvent& e) override {
			mouse = e.pos;
			OpaqueWidget::onHover(e);
		}
		void onLeave(const LeaveEvent& e) override {
			mouse = Vec(-1.f, -1.f);
			OpaqueWidget::onLeave(e);
		}
		void onButton(const ButtonEvent& e) override;   // (below EncoderWidget: it calls into it)
		void draw(const DrawArgs& args) override;
	};

	// The rarely needed options for one role (right-click a grid box).
	void appendRoleOptions(Menu* menu, const std::string& role) {
		auto it = settingMidiMap.find(role);
		MidiMapping m = (it != settingMidiMap.end()) ? it->second : MidiMapping();
		menu->addChild(createMenuLabel(midiRoleLabel(role) + ": " + midiMappingText(m)));
		// (No "Learn" here: left-clicking the box does that.)
		menu->addChild(createSubmenuItem("Set CC number by hand", "", [=](Menu* menu) {
			NumberField* f = new NumberField;
			f->onNumber = [=](int v) {
				MidiMapping& mm = settingMidiMap[role];
				mm.type = MIDI_CC;
				mm.number = v;
				absState.clear();
			};
			menu->addChild(f);
		}));
		menu->addChild(createSubmenuItem("Set note number by hand", "", [=](Menu* menu) {
			NumberField* f = new NumberField;
			f->onNumber = [=](int v) {
				MidiMapping& mm = settingMidiMap[role];
				mm.type = MIDI_NOTE;
				mm.number = v;
			};
			menu->addChild(f);
		}));
		// Knob type and invert: for things that TURN only (knobs and the
		// navigation knob) — a button has no type or direction.
		bool turns = isTurnRole(role);
		if (!turns) {
			// Buttons: for controllers whose buttons are set to TOGGLE.
			menu->addChild(stayCheck("Controller button is a toggle", "each press = one tap",
				[=]() { auto f = settingMidiMap.find(role); return f != settingMidiMap.end() && f->second.toggle; },
				[=]() { settingMidiMap[role].toggle ^= true; toggleState.erase(role); }, m.type == MIDI_NONE));
		}
		if (turns) {
			std::vector<std::string> modes;
			for (int i = 0; i < NUM_MIDI_MODES; i++)
				modes.push_back(i == MODE_AUTO ? std::string("Auto (found: ") + midiModeName(m.detected) + ")" : midiModeName(i));
			menu->addChild(stayIndexSubmenu("Knob type", modes,
				[=]() { auto f = settingMidiMap.find(role); return f != settingMidiMap.end() ? f->second.mode : (int) MODE_AUTO; },
				[=](int i) { settingMidiMap[role].mode = i; absState.clear(); }));
			menu->addChild(stayCheck("Invert direction", "if it moves the wrong way",
				[=]() { auto f = settingMidiMap.find(role); return f != settingMidiMap.end() && f->second.invert; },
				[=]() { settingMidiMap[role].invert ^= true; absState.clear(); navAbsLast = -1; }, m.type == MIDI_NONE));
		}
		menu->addChild(stayAction("Clear", "", [=]() { settingMidiMap.erase(role); absState.erase(role); }, m.type == MIDI_NONE));
	}

	// ---- MIDI mapping as a FILE -----------------------------------------------
	// Save the whole controller set-up (every mapping + the pots & faders
	// choice) as a small .json file, or load one — replacing the current
	// mappings. Uses Rack's own file dialog.
	void saveMidiMappingFile() {
		osdialog_filters* filters = osdialog_filters_parse("Hotkey MIDI mapping (.json):json");
		char* path = osdialog_file(OSDIALOG_SAVE, asset::user("").c_str(), "MIDI mapping.json", filters);
		osdialog_filters_free(filters);
		if (!path) return;   // cancelled
		std::string file = path;
		std::free(path);
		if (system::getExtension(file) != ".json") file += ".json";
		json_t* rootJ = json_object();
		json_object_set_new(rootJ, "encoderMidiMapping", json_integer(1));   // (marks the file as ours; old name kept so files work both ways)
		json_object_set_new(rootJ, "potMode", json_integer(settingMidiPotMode));
		json_object_set_new(rootJ, "map", midiMapToJson(settingMidiMap));
		bool ok = json_dump_file(rootJ, file.c_str(), JSON_INDENT(2)) == 0;
		json_decref(rootJ);
		showKeyMessage("MIDI", ok ? "SAVED" : "NOT SAVED", ok ? "MIDI mapping saved to " + system::getFilename(file)
			: "Couldn't save the MIDI mapping file");
	}
	void loadMidiMappingFile() {
		osdialog_filters* filters = osdialog_filters_parse("Hotkey MIDI mapping (.json):json");
		char* path = osdialog_file(OSDIALOG_OPEN, asset::user("").c_str(), NULL, filters);
		osdialog_filters_free(filters);
		if (!path) return;
		std::string file = path;
		std::free(path);
		json_error_t error;
		json_t* rootJ = json_load_file(file.c_str(), 0, &error);
		json_t* mapJ = rootJ ? json_object_get(rootJ, "map") : nullptr;
		if (!rootJ || !json_object_get(rootJ, "encoderMidiMapping") || !mapJ) {
			if (rootJ) json_decref(rootJ);
			showKeyMessage("MIDI", "NOT LOADED", "That file isn't a Hotkey MIDI mapping - nothing changed");
			return;
		}
		settingMidiMap = midiMapFromJson(mapJ);
		if (json_t* p = json_object_get(rootJ, "potMode")) {
			int pm = (int) json_integer_value(p);
			if (pm >= 0 && pm < NUM_POT_MODES) settingMidiPotMode = pm;
		}
		json_decref(rootJ);
		absState.clear();
		toggleState.clear();
		showKeyMessage("MIDI", "LOADED", string::f("MIDI mapping loaded: %d controls from ", (int) settingMidiMap.size()) +
			system::getFilename(file));
	}

	// The MIDI mappings submenu: the grid, and "clear all".
	void appendMidiMappingsMenu(Menu* menu) {
		menu->addChild(createMenuLabel("Click a box, then move / press it on your controller."));
		menu->addChild(createMenuLabel("Lit = mapped. Right-click a box for more options."));
		MidiGrid* grid = new MidiGrid;
		grid->owner = this;
		menu->addChild(grid);
		menu->addChild(new MenuSeparator);
		menu->addChild(stayAction("Clear all MIDI mappings", "", [=]() {
			settingMidiMap.clear();
			absState.clear();
		}, settingMidiMap.empty()));
	}

	// A number key went down / up.
	void numberPressed(int key) {
		heldKnob = knobForKey(key);
		heldKey = key;
		stepRemainder = 0.f;
	}
	void numberReleased(int key) {
		if (key != heldKey) return;
		finishTurn();   // letting go of the number ends the turn (one undo step)
		// Fall back to another number that's still held, if any.
		heldKnob = -1;
		heldKey = -1;
		for (int i = 0; i < NUM_KNOB_KEYS; i++) {
			bool padIsKnobs = !settingKeys.padModes || padMode == PAD_KNOBS;
			for (int k : {settingKeys.knobs[i], padIsKnobs ? KNOB_PAD_KEYS[i] : -1}) {
				if (k < 0) continue;
				if (glfwGetKey(APP->window->win, k) == GLFW_PRESS) {
					heldKnob = i;
					heldKey = k;
				}
			}
		}
	}


	// =========================================================================
	//  DIAL
	// =========================================================================

	// Called by the overlay on every scroll.
	void scrollSeen(Vec delta) {
		// Vertical scroll decides the direction; fall back to horizontal.
		float d = (delta.y != 0.f) ? delta.y : delta.x;
		if (d > 0.f) lastUpTime = system::getTime();
		if (d < 0.f) lastDownTime = system::getTime();
	}

	// A flash: full brightness briefly after an event, then fades out.
	static float flash(double sinceEvent) {
		if (sinceEvent < 0.0) return 0.f;
		const double hold = 0.08, fade = 0.20;
		if (sinceEvent < hold) return 1.f;
		return clamp((float) (1.0 - (sinceEvent - hold) / fade), 0.f, 1.f);
	}


	// =========================================================================
	//  EXTRA DIALS (keyboards / macropads with 2, 3, 4 ... knobs)
	//  The main dial above is untouched. Each extra dial is learned by the
	//  keys it sends (right, left, push) and then turns ONE thing on its own:
	//  by default extra dial N = knob N of the selected module (current
	//  page), no number to hold. Or: the held knob, or the navigation knob.
	//  Push: a linked knob's push, or double-tap = reset, like the main dial.
	// =========================================================================
	// Learning a dial: -1 = not learning. Steps: turn right, turn left, push.
	int dialLearn = -1;
	enum { DL_RIGHT, DL_LEFT, DL_PUSH };
	int dialLearnStep = 0;
	double dialLearnStart = -1.0;
	DialSlot dialLearning;   // what's been learned so far
	static constexpr double DIAL_LEARN_TIMEOUT = 10.0;
	// Each extra dial's own speed (for acceleration).
	double dialLastTime[NUM_DIALS] = {-1.0, -1.0, -1.0, -1.0};
	float dialRate[NUM_DIALS] = {};

	static std::string dialTargetName(int t) {
		if (t == DIAL_HELD) return "the held knob";
		if (t == DIAL_NAV) return "navigation";
		return string::f("knob %d", t + 1);
	}
	// Short summary for the menu, e.g. "F13 / F14, push F15 -> knob 1".
	static std::string dialSummary(const DialSlot& d) {
		if (!d.learned()) return "not set up";
		std::string s = keyName(d.up.key, d.up.mods) + " / " + keyName(d.down.key, d.down.mods);
		if (d.push.key >= 0) s += ", push " + keyName(d.push.key, d.push.mods);
		return s + "  -> " + dialTargetName(d.target);
	}
	int dialsSetUp() {
		int n = 0;
		for (int i = 0; i < NUM_DIALS; i++) if (settingDials[i].learned()) n++;
		return n;
	}

	void startDialLearn(int dial) {
		if (wizStep >= 0) return;   // (the dial wizard is running)
		learnAction = -1;
		dialLearn = dial;
		dialLearnStep = DL_RIGHT;
		dialLearnStart = system::getTime();
		dialLearning = DialSlot();
		keyMessageTime = -1.0;
	}
	void cancelDialLearn(const std::string& why) {
		dialLearn = -1;
		showKeyMessage("CANCELLED", "", why);
	}

	// What else already uses this key + Shift/Ctrl/Alt ("" = nothing).
	// Checked against every Hotkey key and the other extra dials. An extra
	// dial's keys are its own (always kept from Rack), so a clash would
	// break the other key — those are refused, not just warned about.
	std::string dialClash(int dial, const KeyBinding& b) {
		if (b.key == pushKey()) return actionName(KA_PUSH);   // (the main push ignores Ctrl etc.)
		for (const UsedKey& u : usedKeys()) {
			if (u.b.key < 0 || !(u.b == b)) continue;
			std::string c = (u.action >= 0) ? actionName(u.action) : std::string("a numpad key");
			if (u.action >= KA_KNOB) c += string::f(" %d", u.index + 1);
			return c;
		}
		for (int i = 0; i < NUM_DIALS; i++) {
			if (i == dial) continue;
			const DialSlot& d = settingDials[i];
			if ((d.up.key >= 0 && d.up == b) || (d.down.key >= 0 && d.down == b) || (d.push.key >= 0 && d.push == b))
				return string::f("extra dial %d", i + 1);
		}
		return "";
	}

	// A key arrived while learning a dial (press only).
	void dialLearnKey(int key, int mods) {
		KeyBinding b(key, mods);
		int n = dialLearn + 1;
		if (dialLearnStep == DL_PUSH) {
			// Still turning? The dial's own turn keys are ignored here.
			if (b == dialLearning.up || b == dialLearning.down) return;
			if (key == GLFW_KEY_BACKSPACE && mods == 0) { finishDialLearn(); return; }   // no push
		}
		else if (dialLearnStep == DL_LEFT && b == dialLearning.up) return;   // still turning right
		std::string clash = dialClash(dialLearn, b);
		if (!clash.empty()) {
			dialLearn = -1;
			showKeyMessage("IN USE", keyName(key, mods), "Dial " + std::to_string(n) + ": " + keyName(key, mods) +
				" is already " + clash + ". Set this knob to send other keys (F13-F24 are free) in your keyboard's "
				"software, then Learn again. Nothing changed.", 10.0);
			return;
		}
		dialLearnStart = system::getTime();   // each step gets the full time
		if (dialLearnStep == DL_RIGHT) { dialLearning.up = b; dialLearnStep = DL_LEFT; }
		else if (dialLearnStep == DL_LEFT) { dialLearning.down = b; dialLearnStep = DL_PUSH; }
		else { dialLearning.push = b; finishDialLearn(); }
	}
	// Learned: keep its turn settings (target, sensitivity, invert), new keys.
	void finishDialLearn() {
		int i = dialLearn;
		dialLearn = -1;
		if (i < 0) return;
		DialSlot& d = settingDials[i];
		d.up = dialLearning.up;
		d.down = dialLearning.down;
		d.push = dialLearning.push;
		bool rack = isRackShortcut(d.up.key, d.up.mods) || isRackShortcut(d.down.key, d.down.mods) ||
			(d.push.key >= 0 && isRackShortcut(d.push.key, d.push.mods));
		showKeyMessage(string::f("DIAL %d", i + 1), "SET", string::f("Dial %d set up: ", i + 1) + dialSummary(d) +
			(rack ? "   (one of its keys is also a Rack shortcut - Hotkey keeps it while unlocked)" : ""), 8.0);
	}

	// Which extra dial (and what) a key is: 0 = none, +1 right, -1 left, 2 push.
	int dialForKey(int key, int mods, int& dial) {
		for (int i = 0; i < NUM_DIALS; i++) {
			const DialSlot& d = settingDials[i];
			if (!d.learned()) continue;
			dial = i;
			if (keyMatches(d.up, key, mods)) return +1;
			if (keyMatches(d.down, key, mods)) return -1;
			if (keyMatches(d.push, key, mods)) return 2;
		}
		dial = -1;
		return 0;
	}

	// An extra dial clicked once (dir = +1 right / -1 left).
	void dialTurned(int dial, int dir) {
		const DialSlot& d = settingDials[dial];
		if (d.invert) dir = -dir;
		double now = system::getTime();
		if (dir > 0) lastUpTime = now; else lastDownTime = now;   // the panel's dial lights
		if (d.target == DIAL_NAV) {
			navTurn(dir);
			return;
		}
		float clicks = (float) dir;
		float accel = accelerationFor(clicks, dialLastTime[dial], dialRate[dial]) * d.sens;
		if (!selectedModule()) {
			showKeyMessage(string::f("DIAL %d", dial + 1), "NO MODULE", "");   // (panel readout only)
			return;
		}
		if (d.target == DIAL_HELD) {
			if (playing()) turnHeldKnob(clicks, accel);
			return;
		}
		int index = onPage(d.target);
		if (index >= (int) currentKnobs().size()) {
			showKeyMessage(string::f("DIAL %d", dial + 1), string::f("NO KNOB %d", d.target + 1), "");
			return;
		}
		turnKnobAt(index, clicks, accel);
		lastMidiKnob = index;   // shows its value for a moment, like a MIDI knob
		lastMidiKnobTime = now;
	}

	// An extra dial's push went down.
	void dialPushed(int dial) {
		const DialSlot& d = settingDials[dial];
		lastPushTime = system::getTime();
		if (d.target == DIAL_NAV) { navPush(); return; }
		if (!selectedModule()) return;
		int index = (d.target == DIAL_HELD) ? heldIndex() : onPage(d.target);
		knobPressed(index, false, false, d.push.key);
	}

	// The submenu: CONTROLS > Extra dials.
	void appendExtraDialsMenu(Menu* menu) {
		menu->addChild(createMenuLabel("For keyboards and macropads with more knobs (up to 4 extra)."));
		menu->addChild(createMenuLabel("Set each knob to send its own keys (F13-F24 are ideal; volume keys never reach Rack),"));
		menu->addChild(createMenuLabel("then Learn: turn it right, then left, then press it (Backspace = no push)."));
		menu->addChild(createMenuLabel("Your main dial (Set up my dial...) keeps working as before."));
		for (int i = 0; i < NUM_DIALS; i++) {
			menu->addChild(new MenuSeparator);
			menu->addChild(createSubmenuItem(string::f("Dial %d", i + 1), dialSummary(settingDials[i]),
				[=](Menu* menu) { appendOneDialMenu(menu, i); }));
		}
	}
	void appendOneDialMenu(Menu* menu, int i) {
		DialSlot* d = &settingDials[i];
		menu->addChild(createMenuItem(d->learned() ? "Learn again..." : "Learn this dial...", "",
			[=]() { startDialLearn(i); }));
		StayIndexSubmenu* target = new StayIndexSubmenu;
		target->text = "Turns";
		for (int t = 0; t < NUM_DIAL_TARGETS; t++) {
			if (t == DIAL_HELD) target->labels.push_back("The held knob (hold a number, like the main dial)");
			else if (t == DIAL_NAV) target->labels.push_back("Navigation (pick module; push = next page)");
			else target->labels.push_back(string::f("Knob %d of the selected module", t + 1));
		}
		target->getter = [=]() { return d->target; };
		target->setter = [=](int t) { d->target = t; };
		menu->addChild(target);
		StayIndexSubmenu* sens = new StayIndexSubmenu;
		sens->text = "Speed";
		for (int k = 0; k < NUM_DIAL_SENS; k++) sens->labels.push_back(string::f("x%g", DIAL_SENS[k]));
		sens->getter = [=]() {
			for (int k = 0; k < NUM_DIAL_SENS; k++) if (std::fabs(d->sens - DIAL_SENS[k]) < 1e-4f) return k;
			return -1;
		};
		sens->setter = [=](int k) { d->sens = DIAL_SENS[k]; };
		menu->addChild(sens);
		menu->addChild(stayBool("Invert", "if it turns the wrong way", &d->invert));
		menu->addChild(createMenuLabel("Overall sensitivity, acceleration and double-tap reset apply too."));
		menu->addChild(new MenuSeparator);
		menu->addChild(createMenuItem("Forget this dial", "", [=]() {
			*d = DialSlot();
			d->target = i;
			showKeyMessage(string::f("DIAL %d", i + 1), "CLEARED", string::f("Dial %d forgotten", i + 1));
		}, !d->learned()));
	}


	// =========================================================================
	//  LEARNING KEYS (right-click > Keys)
	//  Click an action, press the key you want. Only keys that actually
	//  reach Rack can be learned — if Windows or another program grabs a key
	//  first, it simply never arrives. Clashes with the Encoder's other keys
	//  or Rack's own shortcuts are warned about on the panel readout.
	// =========================================================================
	enum KeyAction {
		KA_PUSH, KA_TURN_UP, KA_TURN_DOWN, KA_SENS_UP, KA_SENS_DOWN,
		KA_GROUP_PREV, KA_GROUP_NEXT, KA_GROUP_NEW, KA_GROUP_CLEAR, KA_PAGE_NEXT, KA_BUTTON_PAGE, KA_LOCK,
		KA_PAD_MODE, KA_PAD_SELECT, KA_PAD_ASSIGN,   // (Numpad layout only)
		KA_KNOB, KA_BUTTON, KA_SLOT, KA_CUSTOM,
	};
	int learnAction = -1;   // which action is waiting for a key (-1 = none)
	int learnIndex = 0;     // for knob / button / slot keys: which one (0-9)

	// A message on the readout for a moment after learning (e.g. a clash).
	std::string keyMessage1, keyMessage2;
	double keyMessageTime = -1.0;

	double learnStartTime = -1.0;

	void startLearn(int action, int index = 0) {
		dialLearn = -1;   // (one thing learning at a time)
		learnAction = action;
		learnIndex = index;
		learnStartTime = system::getTime();
		keyMessageTime = -1.0;
	}
	// Learning gives up after this long with no key — nothing changes.
	static constexpr double LEARN_TIMEOUT = 8.0;

	// The full sentence for the banner at the top of the Rack window (the
	// panel only has room for a short version).
	std::string keyBanner;

	void cancelLearn() {
		learnAction = -1;
		showKeyMessage("CANCELLED", "", "Key learning cancelled - nothing changed");
	}
	// How long the banner stays (seconds). Most messages 4 s; the dial
	// wizard's summary longer, as there's more to read.
	double keyBannerSecs = 4.0;

	void showKeyMessage(const std::string& a, const std::string& b, const std::string& banner,
			double bannerSecs = 4.0) {
		keyMessage1 = a;
		keyMessage2 = b;
		keyBanner = banner;
		keyBannerSecs = bannerSecs;
		keyMessageTime = system::getTime();
	}

	// What the banner says right now ("" = no banner).
	std::string currentBanner(double now) {
		if (wizStep >= 0) return wizardBanner(now);
		if (dialLearn >= 0) {
			int left = std::max(0, (int) std::ceil(DIAL_LEARN_TIMEOUT - (now - dialLearnStart)));
			std::string step = (dialLearnStep == DL_RIGHT) ? "turn the knob RIGHT one click"
				: (dialLearnStep == DL_LEFT) ? "now turn it LEFT one click"
				: "now press the knob's push (Backspace = it has none)";
			std::string s = string::f("Learn extra dial %d: ", dialLearn + 1) + step + string::f("   (Esc cancels, %ds)", left);
			if (now - dialLearnStart > 4.0)
				s += "   Nothing happening? That knob sends something Rack can't see (e.g. volume) - give it other keys.";
			return s;
		}
		if (!midiLearnRole.empty()) {
			int left = std::max(0, (int) std::ceil(MIDI_LEARN_TIMEOUT - (now - midiLearnStart)));
			return "MIDI learn - " + midiRoleLabel(midiLearnRole) + ": move the controller knob a little BOTH ways, "
				"or press the button" + string::f("   (Esc cancels, %ds)", left);
		}
		if (learnAction >= 0) {
			std::string what = actionName(learnAction);
			if (learnAction >= KA_KNOB && learnAction <= KA_SLOT) what += string::f(" %d", learnIndex + 1);
			if (learnAction == KA_CUSTOM) what = customLearnName + " (Backspace removes its key)";
			int left = (int) std::ceil(LEARN_TIMEOUT - (now - learnStartTime));
			std::string s = "Press the new key for " + what + "   (Esc cancels, " + string::f("%ds)", std::max(left, 0));
			if (now - learnStartTime > 3.0) s += "   Nothing happening? That key is taken by Rack or Windows.";
			return s;
		}
		// The banner stays a little longer than the panel text (4 s vs 2.5 s)
		// — it's the full sentence, so it needs a moment more to read.
		if (keyMessageTime >= 0 && now - keyMessageTime < keyBannerSecs) return keyBanner;
		return "";
	}


	// =========================================================================
	//  DIAL SETUP WIZARD (right-click > "Set up my dial...")
	//  Four short steps that set the Encoder up for ANY keyboard dial, mouse
	//  wheel or encoder — whatever it sends:
	//    1. turn RIGHT one click -> measures one click, works out direction
	//    2. turn LEFT one click  -> confirms direction
	//    3. press the push       -> that becomes the push key (or wheel click)
	//    4. HOLD the push        -> checks whether the push can hold
	//  A dial that sends KEYS instead of scroll gets those as the turn keys.
	//  Nothing arriving for 12 s = Rack can't see the dial (it's probably
	//  sending volume keys) — the wizard says so and stops. Esc cancels.
	// =========================================================================
	enum WizStep { WIZ_RIGHT, WIZ_LEFT, WIZ_PUSH, WIZ_HOLD, WIZ_LAYOUT };
	bool wizCanHold = false;       // result of the HOLD step, for the summary
	int wizStep = -1;               // -1 = wizard not running
	double wizStepTime = 0.0;       // when the current step started
	double wizFirstTime = -1.0;     // first scroll of this step (-1 = none yet)
	float wizSum = 0.f;             // scroll collected in this step
	float wizMin = 0.f;             // the smallest single scroll seen this step
	int wizCount = 0;               // how many scrolls arrived this step
	double wizLastTime = -1.0;      // the last scroll of this step
	float wizRightSum = 0.f;        // the right turn's total (for direction)
	float wizRightMin = 0.f;        // the right turn's smallest step
	bool wizKeysTurn = false;       // the dial sends keys, not scroll
	bool wizWheelPush = false;      // the push turned out to be the wheel button
	double wizHoldDown = -1.0;      // when the push went down in the HOLD step
	static constexpr double WIZ_TIMEOUT = 12.0;
	// A turn is over once no scroll has arrived for this long — so it
	// doesn't matter how many clicks you turn.
	static constexpr double WIZ_TURN_DONE = 0.6;
	// Smallest step below this (Rack units; a normal wheel click is 50) =
	// a SMOOTH dial / wheel / touchpad with no clicks.
	static constexpr float WIZ_SMOOTH_BELOW = 15.f;
	bool wizSmooth = false;

	void startWizard() {
		learnAction = -1;
		dialLearn = -1;
		wizStep = WIZ_RIGHT;
		wizKeysTurn = false;
		wizWheelPush = false;
		wizSmooth = false;
		wizBeginStep(WIZ_RIGHT);
	}
	void wizBeginStep(int step) {
		wizStep = step;
		wizStepTime = system::getTime();
		wizFirstTime = -1.0;
		wizLastTime = -1.0;
		wizSum = 0.f;
		wizMin = 0.f;
		wizCount = 0;
		wizHoldDown = -1.0;
	}
	void wizCancel() {
		wizStep = -1;
		showKeyMessage("SETUP", "CANCELLED", "Dial setup cancelled - nothing more changed");
	}

	std::string wizardBanner(double now) {
		int left = std::max(0, (int) std::ceil(WIZ_TIMEOUT - (now - wizStepTime)));
		std::string tail = string::f("   (Esc cancels, %ds)", left);
		switch (wizStep) {
			case WIZ_RIGHT: return "Dial setup 1/5: turn your dial RIGHT, or scroll your mouse wheel UP - a few clicks, then stop" + tail;
			case WIZ_LEFT: return "Dial setup 2/5: now turn your dial LEFT, or scroll the wheel DOWN - then stop" + tail;
			case WIZ_PUSH: return "Dial setup 3/5: press the dial's push, or click the mouse wheel" + tail;
			case WIZ_HOLD: return "Dial setup 4/5: press and HOLD the push (or wheel button) for a second" + tail;
			case WIZ_LAYOUT: return "Dial setup 5/5: which way do you want to play?   1 = Standard (two hands, no modes)   "
				"2 = Numpad (one hand, modes)   3 = Laptop (no numpad)   Esc = keep the current keys" + string::f("   (%ds)", left);
		}
		return "";
	}

	// Scroll arrived during the wizard (turn steps only).
	void wizScroll(Vec delta) {
		if (wizStep != WIZ_RIGHT && wizStep != WIZ_LEFT) return;
		float d = (delta.y != 0.f) ? delta.y : delta.x;
		if (d == 0.f) return;
		double now = system::getTime();
		if (wizFirstTime < 0) wizFirstTime = now;
		wizLastTime = now;
		wizSum += d;
		// The smallest single scroll = one click (fast turns can arrive as
		// several clicks in one go, but some single ones always come through).
		wizMin = (wizCount == 0) ? std::fabs(d) : std::min(wizMin, std::fabs(d));
		wizCount++;
	}

	// A key (or the wheel button, key = -2) went down / up during the wizard.
	void wizPress(int key, int mods, bool down) {
		double now = system::getTime();
		if (wizStep == WIZ_RIGHT && down && key >= 0) {
			// The dial sends a KEY when turned: that's "turn up".
			settingKeys.turnUp = {key, mods};
			wizKeysTurn = true;
			wizBeginStep(WIZ_LEFT);
		}
		else if (wizStep == WIZ_LEFT && down && key >= 0) {
			settingKeys.turnDown = {key, mods};
			wizBeginStep(WIZ_PUSH);
		}
		else if (wizStep == WIZ_PUSH && down) {
			if (key == -2) wizWheelPush = settingWheelClickPush = true;
			else settingKeys.push = {key, mods};
			wizBeginStep(WIZ_HOLD);
		}
		else if (wizStep == WIZ_HOLD) {
			bool isPush = (key == -2) ? wizWheelPush : (key == settingKeys.push.key);
			if (!isPush) return;
			if (down) wizHoldDown = now;
			else if (wizHoldDown >= 0) wizFinish(now - wizHoldDown >= 0.3);
		}
		else if (wizStep == WIZ_LAYOUT && down && mods == 0) {
			// 1 / 2 / 3 on the number row or the numpad.
			int choice = -1;
			if (key >= GLFW_KEY_1 && key <= GLFW_KEY_3) choice = key - GLFW_KEY_1;
			if (key >= GLFW_KEY_KP_1 && key <= GLFW_KEY_KP_3) choice = key - GLFW_KEY_KP_1;
			if (choice >= 0) wizChooseLayout(choice);
		}
	}

	// Every frame while the wizard runs: finish one-click measurements,
	// HOLD detection, and time-outs.
	void wizFrame(double now) {
		// A turn is finished once nothing more arrives for a moment — however
		// many clicks it was.
		if ((wizStep == WIZ_RIGHT || wizStep == WIZ_LEFT) && wizLastTime >= 0 &&
				now - wizLastTime > WIZ_TURN_DONE) {
			if (wizStep == WIZ_RIGHT) {
				wizRightSum = wizSum;
				wizRightMin = wizMin;
				wizBeginStep(WIZ_LEFT);
			}
			else {
				if ((wizSum > 0) == (wizRightSum > 0)) {
					// Both turns scrolled the same way — try again.
					showKeyMessage("SETUP", "AGAIN", "Both turns went the same way - let's try again: RIGHT (or wheel UP) first");
					wizBeginStep(WIZ_RIGHT);
					return;
				}
				// One click = the smallest single step seen (averaged over both
				// turns). A smooth dial / wheel / touchpad has no clicks — its
				// steps are tiny — so it keeps Rack's normal scale (50) and the
				// feel is set with Sensitivity instead.
				float unit = (wizRightMin + wizMin) / 2.f;
				wizSmooth = unit < WIZ_SMOOTH_BELOW;
				settingScrollUnit = wizSmooth ? 50.f : clamp(unit, 1.f, 1000.f);
				// Turning RIGHT should turn knobs UP: if right scrolls down, reverse.
				settingReverseDial = (wizRightSum < 0);
				wizBeginStep(WIZ_PUSH);
			}
			return;
		}
		// HOLD step: still down after a second = it can hold.
		if (wizStep == WIZ_HOLD && wizHoldDown >= 0 && now - wizHoldDown >= 1.0) {
			wizFinish(true);
			return;
		}
		// Nothing arrived in time.
		if (now - wizStepTime > WIZ_TIMEOUT) {
			int step = wizStep;
			wizStep = -1;
			settingDialSetupDone = true;
			if (step == WIZ_RIGHT || step == WIZ_LEFT)
				showKeyMessage("NO DIAL", "SEEN", "Rack can't see your dial. In your keyboard's software, set the dial "
					"to SCROLL (or to keys) - volume keys never reach Rack. Then run Set up my dial again.", 12.0);
			else if (step == WIZ_PUSH)
				showKeyMessage("NO PUSH", "SEEN", "Turning is set up, but no push arrived. Set the push to a key in "
					"your keyboard's software, or use the mouse wheel click.", 10.0);
			else if (step == WIZ_HOLD)
				wizFinish(false);   // pressed, but never held long enough
			else
				wizChooseLayout(-1);   // no layout picked: keep the current keys
		}
	}

	// The HOLD step is done: on to the last question, the layout.
	void wizFinish(bool canHold) {
		wizCanHold = canHold;
		wizBeginStep(WIZ_LAYOUT);
	}

	// Layout chosen (1 = Standard, 2 = Numpad, 3 = Laptop; -1 = keep the current
	// keys). The keys the wizard just learned (push, and turn keys for a dial
	// that sends keys) are kept on top of the chosen layout.
	void wizChooseLayout(int choice) {
		if (choice >= 0) {
			static const int layouts[3] = {KeyMap::LAYOUT_STANDARD, KeyMap::LAYOUT_MIDI, KeyMap::LAYOUT_LAPTOP};
			KeyBinding push = settingKeys.push, up = settingKeys.turnUp, down = settingKeys.turnDown;
			keysUndoable("key layout", [&]() {
				settingKeys.setLayout(layouts[choice]);
				if (!wizWheelPush) settingKeys.push = push;
				if (wizKeysTurn) {
					settingKeys.turnUp = up;
					settingKeys.turnDown = down;
				}
			});
		}
		wizSummary(wizCanHold);
	}

	void wizSummary(bool canHold) {
		wizStep = -1;
		settingDialSetupDone = true;
		std::string turn = wizKeysTurn
			? "turning = keys " + keyName(settingKeys.turnUp.key, settingKeys.turnUp.mods) + " / " +
				keyName(settingKeys.turnDown.key, settingKeys.turnDown.mods)
			: wizSmooth
				? std::string("turning = scroll (smooth dial detected - use Sensitivity to adjust the feel)") +
					(settingReverseDial ? ", inverted" : "")
				: string::f("turning = scroll, 1 click = %.0f%s", settingScrollUnit, settingReverseDial ? ", inverted" : "");
		std::string push = wizWheelPush ? "push = mouse wheel click"
			: "push = " + keyName(settingKeys.push.key, settingKeys.push.mods);
		std::string hold = canHold ? "hold works" : "your push can't HOLD (tap and double-tap work)";
		std::string layout = std::string("layout: ") + KeyMap::layoutShortName(settingKeys.matchingLayout());
		showKeyMessage("DIAL", "READY", "Dial set up: " + turn + ";  " + push + ";  " + hold + ";  " + layout, 10.0);
	}

	// Numpad mode key: KNOBS -> BUTTONS -> SLOTS -> KNOBS. Lets go of any
	// held knob / button first, so nothing gets stuck across the switch.
	void cyclePadMode() {
		if (heldKey >= 0) numberReleased(heldKey);
		for (int i = 0; i < NUM_BUTTON_KEYS; i++) releaseButton(i);
		padMode = (padMode + 1) % NUM_PAD_MODES;
		showKeyMessage("MODE", PAD_MODE_NAMES[padMode],
			std::string("Numpad now plays ") + PAD_MODE_NAMES[padMode] +
			"   (numpad / switches: KNOBS > BUTTONS > SLOTS)");
	}

	// The single-key actions, by number (same order as KeyAction).
	static KeyBinding* actionBinding(int action) {
		KeyMap& k = settingKeys;
		switch (action) {
			case KA_PUSH: return &k.push;
			case KA_TURN_UP: return &k.turnUp;
			case KA_TURN_DOWN: return &k.turnDown;
			case KA_SENS_UP: return &k.sensUp;
			case KA_SENS_DOWN: return &k.sensDown;
			case KA_GROUP_PREV: return &k.groupPrev;
			case KA_GROUP_NEXT: return &k.groupNext;
			case KA_GROUP_NEW: return &k.groupNew;
			case KA_GROUP_CLEAR: return &k.groupClear;
			case KA_PAGE_NEXT: return &k.pageNext;
			case KA_BUTTON_PAGE: return &k.buttonPageNext;
			case KA_LOCK: return &k.lockKey;
			case KA_PAD_MODE: return &k.modeKey;
			case KA_PAD_SELECT: return &k.padSelectKey;
			case KA_PAD_ASSIGN: return &k.padAssignKey;
			default: return nullptr;
		}
	}
	static const char* actionName(int action) {
		static const char* names[] = {"Push", "Turn up", "Turn down", "Sensitivity +", "Sensitivity -",
			"Previous group", "Next group", "New group", "Clear this group", "Next page",
			"Next button page", "Lock on / off", "Numpad mode", "Select slot", "Assign slot",
			"Knob", "Button", "Slot", "this control"};
		return names[action];
	}

	// Every key combo the Encoder uses right now, with what it's for.
	struct UsedKey {
		KeyBinding b;
		int action, index;
	};
	std::vector<UsedKey> usedKeys() {
		std::vector<UsedKey> used;
		for (int a = KA_PUSH; a <= KA_LOCK; a++) used.push_back({*actionBinding(a), a, 0});
		// (Numpad layout: the mode, select and assign keys are in use too.)
		if (settingKeys.padModes) {
			for (int a = KA_PAD_MODE; a <= KA_PAD_ASSIGN; a++) used.push_back({*actionBinding(a), a, 0});
		}
		for (int i = 0; i < KEY_ROW; i++) {
			used.push_back({{settingKeys.knobs[i], 0}, KA_KNOB, i});
			used.push_back({{settingKeys.buttons[i], 0}, KA_BUTTON, i});
			used.push_back({{settingKeys.slots[i], settingKeys.slotSelectMods}, KA_SLOT, i});   // select
			used.push_back({{settingKeys.slots[i], settingKeys.slotAssignMods}, KA_SLOT, i});   // assign
		}
		return used;
	}

	// Rack's own shortcuts that we know of (so you're warned before taking
	// one). Not a complete list — Rack and other plugins have more.
	static bool isRackShortcut(int key, int mods) {
		if (mods == 0 && (key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE || key == GLFW_KEY_F3 || key == GLFW_KEY_F11))
			return true;
		if (mods == RACK_MOD_CTRL) {
			const int ctrlKeys[] = {GLFW_KEY_N, GLFW_KEY_O, GLFW_KEY_S, GLFW_KEY_Z, GLFW_KEY_Y, GLFW_KEY_Q,
				GLFW_KEY_C, GLFW_KEY_V, GLFW_KEY_X, GLFW_KEY_D, GLFW_KEY_I, GLFW_KEY_R, GLFW_KEY_E, GLFW_KEY_U,
				GLFW_KEY_F, GLFW_KEY_A, GLFW_KEY_0, GLFW_KEY_MINUS, GLFW_KEY_EQUAL};
			for (int k : ctrlKeys) if (k == key) return true;
		}
		if (mods == (RACK_MOD_CTRL | GLFW_MOD_SHIFT) &&
				(key == GLFW_KEY_S || key == GLFW_KEY_Z || key == GLFW_KEY_V || key == GLFW_KEY_O))
			return true;
		return false;
	}

	// The key that was pressed while learning.
	void learned(int key, int mods) {
		int action = learnAction, index = learnIndex;
		learnAction = -1;
		if (action == KA_CUSTOM) {   // custom mode: a control's own key (one undo step)
			remapUndoable(selectedModule(), "custom key", [&]() { customLearned(key, mods); });
			return;
		}

		// The combos this key will be used as (a slot key is used with
		// Shift AND with Alt; knob / button keys on their own).
		std::vector<KeyBinding> asUsed;
		if (action == KA_KNOB || action == KA_BUTTON) asUsed.push_back({key, 0});
		else if (action == KA_SLOT) {
			asUsed.push_back({key, settingKeys.slotSelectMods});
			asUsed.push_back({key, settingKeys.slotAssignMods});
		}
		else asUsed.push_back({key, mods});

		// Store it (one undo step).
		keysUndoable("learn key", [&]() {
			if (KeyBinding* b = actionBinding(action)) *b = {key, mods};
			else if (action == KA_KNOB) settingKeys.knobs[index] = key;
			else if (action == KA_BUTTON) settingKeys.buttons[index] = key;
			else if (action == KA_SLOT) settingKeys.slots[index] = key;
		});

		// Warn about clashes (but keep the new key — you may mean it).
		std::string clash;
		for (const UsedKey& u : usedKeys()) {
			if (u.action == action && u.index == index) continue;   // itself
			if (u.b.key < 0) continue;                              // unused key
			for (const KeyBinding& b : asUsed) {
				if (u.b == b && clash.empty()) {
					clash = (u.action >= 0) ? actionName(u.action)
						: (u.index == 0 ? "the numpad mode key" : "the numpad assign key");
					if (u.action >= KA_KNOB) clash += string::f(" %d", u.index + 1);
				}
			}
		}
		// (An extra dial's keys always win, so say so if this is one of them.)
		for (int i = 0; i < NUM_DIALS && clash.empty(); i++) {
			const DialSlot& d = settingDials[i];
			if (!d.learned()) continue;
			for (const KeyBinding& b : asUsed)
				if (d.up == b || d.down == b || (d.push.key >= 0 && d.push == b)) clash = string::f("extra dial %d", i + 1);
		}
		bool rack = false;
		for (const KeyBinding& b : asUsed) rack = rack || isRackShortcut(b.key, b.mods);

		std::string name = keyName(key, (action >= KA_KNOB) ? 0 : mods);
		std::string what = actionName(action);
		if (action >= KA_KNOB) what += string::f(" %d", index + 1);
		if (!clash.empty())
			showKeyMessage("CLASH", name, what + " = " + name + ", but " + name + " is also " + clash + " - change one of them");
		else if (rack)
			showKeyMessage("RACK KEY", name, what + " = " + name + ", but Rack also uses " + name + " for one of its own shortcuts");
		else
			showKeyMessage("SET", name, what + " = " + name);
	}

	// ---- Key menus (CONTROLS section) ----------------------------------------
	// Which layout the keys are based on: the matching one, or (once you've
	// changed some keys) the nearest — numpad modes on = Numpad, else Standard.
	int baseLayout() {
		int l = settingKeys.matchingLayout();
		if (l >= 0) return l;
		return settingKeys.padModes ? (int) KeyMap::LAYOUT_MIDI : (int) KeyMap::LAYOUT_STANDARD;
	}

	// What a key shows on the right of the menu: its name, "(default)" if
	// it's still the layout's own key (nothing to set), "not set" if empty.
	std::string keyLabel(const KeyBinding& b, const KeyBinding& def) {
		if (b.key < 0) return (def.key < 0) ? "not set (optional)" : "not set";
		std::string s = keyName(b.key, b.mods);
		if (b == def) s += "  (default)";
		return s;
	}
	// The same for a knob / button / slot key; -1 there means "the numpad
	// digit" (numpad layout).
	std::string rowKeyLabel(int key, int def, int i) {
		std::string s = (key >= 0) ? keyName(key) : "Numpad " + padLabel(i);
		if (key == def) s += "  (default)";
		return s;
	}

	// Layout submenu: the three ready-made layouts.
	void appendLayoutMenu(Menu* menu) {
		for (int l = 0; l < KeyMap::NUM_LAYOUTS; l++) {
			menu->addChild(stayCheck(KeyMap::layoutName(l), "",
				[=]() { return settingKeys.matchingLayout() == l; },
				[=]() {
					keysUndoable("key layout", [=]() { settingKeys.setLayout(l); });
					showKeyMessage("LAYOUT", KeyMap::layoutShortName(l),
						std::string("Keys set to the ") + KeyMap::layoutName(l) + " layout");
				}));
		}
	}

	// =========================================================================
	//  KEYS VIEW (CONTROLS > Keys)
	//  Every key the Encoder uses, on one screen — the same way of working as
	//  the MIDI grid:
	//
	//      Knobs     [1][2][3][4][5][6][7][8][9][0]
	//      Buttons   [Q][W][E][R][T][Y][U][I][O][P]
	//      Slots     [1][2][3][4][5][6][7][8][9][0]
	//      PLAYING                          GROUPS & SLOTS
	//      Push "dial press"       [End]    Previous group          [Shift+-]
	//      Turn up "dial right"    [ = ]    ...
	//      ...                              Select slot "+ slot key" [Shift]
	//      (line describing what's under the mouse)
	//
	//  Click a box or an action = learn its key (the menu closes; press the
	//  key). Right-click = back to the layout's default. A box / key lit in
	//  the mapping colour has been changed from the default.
	//  Select slot / Assign slot are modifiers (Shift, Alt ...), so clicking
	//  them opens a short list instead of learning.
	//
	//  NUMPAD LAYOUT: the numpad digits play knobs, buttons OR slots depending
	//  on the mode, so two plain lines explain that above the rows, and the
	//  rows are named by mode (KNOBS mode ...). A box can still be given an
	//  EXTRA key of its own (on top of its numpad digit). The list shows that
	//  layout's own keys — Numpad mode, and Select / Assign slot as
	//  "+ numpad digit" keys (learnable like any other key).
	// =========================================================================
	struct KeysPanel : widget::OpaqueWidget {
		EncoderWidget* owner = nullptr;
		static constexpr float LABEL_W = 100.f;  // grid row-name column ("BUTTONS mode" fits)
		static constexpr float CELL = 26.f;      // grid box pitch
		static constexpr float ROW = BND_WIDGET_HEIGHT + 2.f;
		static constexpr float COL_W = 280.f;    // one column of the action list
		static constexpr float HEAD_H = 18.f;    // the PLAYING / GROUPS & SLOTS headings
		static constexpr float KEY_W = 74.f;     // the key box at the end of each action row
		// Special "actions" in the list that aren't learnable keys.
		enum { ACT_SELECT_MODS = -1, ACT_ASSIGN_MODS = -2, ACT_NONE = -3,
			// Mouse / dial gestures (modifiers, picked from a list):
			ACT_REMAP_MODS = -4, ACT_PUSH_MODS = -5, ACT_CUSTOM_MODS = -6, ACT_NAV_MODS = -7,
			ACT_SETUP_HEAD = -8 };   // the small "SET UP" heading inside the right column
		bool pad = false;                        // Numpad layout (decided when the menu opens)
		Vec mouse = Vec(-1.f, -1.f);

		KeysPanel() {
			pad = settingKeys.padModes;
			box.size = Vec(2.f * COL_W, infoTop() + BND_WIDGET_HEIGHT + 4.f);
		}
		int gridRows() const { return 3; }
		float gridTop() const { return pad ? 2.f * ROW : 0.f; }   // Numpad: two explaining lines first
		float gridH() const { return gridTop() + 3.f * ROW; }
		int listRows() const { return pad ? 10 : 9; }   // per column (Numpad layout: + Numpad mode)
		float listTop() const { return gridH() + 6.f; }
		float infoTop() const { return listTop() + HEAD_H + listRows() * ROW + 4.f; }

		// The action list, column by column: left = PLAYING, right = GROUPS &
		// SLOTS, then a small SET UP heading and the three click gestures.
		int listCount() const { return 2 * listRows(); }
		int listAction(int i) const {
			// (No separate SET UP heading: it left a gap and pushed the right
			//  column down. The three "+ click" rows simply follow on, so both
			//  columns are the same length.)
			static const int standard[2 * 9] = {
				KA_PUSH, KA_TURN_UP, KA_TURN_DOWN, KA_SENS_UP, KA_SENS_DOWN, KA_PAGE_NEXT, KA_BUTTON_PAGE,
				ACT_NAV_MODS, KA_LOCK,
				KA_GROUP_PREV, KA_GROUP_NEXT, KA_GROUP_NEW, KA_GROUP_CLEAR, ACT_SELECT_MODS, ACT_ASSIGN_MODS,
				ACT_REMAP_MODS, ACT_PUSH_MODS, ACT_CUSTOM_MODS};
			static const int numpad[2 * 10] = {
				KA_PUSH, KA_TURN_UP, KA_TURN_DOWN, KA_SENS_UP, KA_SENS_DOWN, KA_PAGE_NEXT, KA_BUTTON_PAGE,
				KA_PAD_MODE, ACT_NAV_MODS, KA_LOCK,
				KA_GROUP_PREV, KA_GROUP_NEXT, KA_GROUP_NEW, KA_GROUP_CLEAR, KA_PAD_SELECT, KA_PAD_ASSIGN,
				ACT_REMAP_MODS, ACT_PUSH_MODS, ACT_CUSTOM_MODS, ACT_NONE};
			return pad ? numpad[i] : standard[i];
		}
		// Rows that are just there (an empty spot, or the SET UP heading).
		static bool isFiller(int a) { return a == ACT_NONE || a == ACT_SETUP_HEAD; }
		// The action's name, and its few words of context.
		// Drawn separately: the context dimmer, so the name stands out.
		static std::string listLabel(int a) {
			if (a == ACT_SELECT_MODS) return "Select slot";
			if (a == ACT_ASSIGN_MODS) return "Assign slot";
			if (a == ACT_REMAP_MODS) return "Swap module's hotkeys";
			if (a == ACT_PUSH_MODS) return "Assign knob press";
			if (a == ACT_CUSTOM_MODS) return "Assign hotkey manually";
			if (a == ACT_NAV_MODS) return "Navigation knob";
			return actionName(a);
		}
		static bool isModsItem(int a) { return a < 0 && !isFiller(a); }
		// The full sentence for the hover line (modifier rows), with the
		// modifier currently chosen.
		static std::string modsSentence(int a, const std::string& m) {
			switch (a) {
				case ACT_SELECT_MODS: return "Hold " + m + " + a slot key to select that slot";
				case ACT_ASSIGN_MODS: return "Hold " + m + " + a slot key to put the module under the mouse in that slot";
				case ACT_REMAP_MODS: return "Hold a knob's hotkey, then " + m + "-click any control: the two swap hotkeys";
				case ACT_PUSH_MODS: return "Hold a knob's hotkey, then " + m + "-click a button: pressing the knob presses it (again = unlink)";
				case ACT_CUSTOM_MODS: return m + "-click any control, then press a key: that key plays it (Backspace removes it)";
				case ACT_NAV_MODS: return "Hold " + m + " and turn the dial to turn the navigation knob";
			}
			return "";
		}
		static std::string listDesc(int a) {
			switch (a) {
				case KA_PUSH: return "\"dial press\"";
				case KA_TURN_UP: return "\"dial right\"";
				case KA_TURN_DOWN: return "\"dial left\"";
				case KA_SENS_UP: case KA_SENS_DOWN: return "\"dial speed\"";
				case KA_PAGE_NEXT: return "\"knobs 11-20...\"";
				case KA_BUTTON_PAGE: return "\"buttons 11-20...\"";
				case KA_LOCK: return "\"to type\"";
				case ACT_SELECT_MODS: case ACT_ASSIGN_MODS: return "\"+ slot key\"";
				case KA_PAD_MODE: return "\"knobs / buttons / slots\"";
				case KA_PAD_SELECT: case KA_PAD_ASSIGN: return "\"+ numpad digit\"";
				case ACT_REMAP_MODS: case ACT_PUSH_MODS: case ACT_CUSTOM_MODS: return "\"+ click\"";
				case ACT_NAV_MODS: return "\"hold + dial\"";
			}
			return "";
		}
		static std::string listName(int a) {   // both together (the hover line)
			std::string d = listDesc(a);
			return d.empty() ? listLabel(a) : listLabel(a) + " " + d;
		}
		Rect boxRect(int r, int c) const {
			return Rect(Vec(LABEL_W + c * CELL, gridTop() + r * ROW + 1.f), Vec(CELL - 3.f, BND_WIDGET_HEIGHT - 2.f));
		}
		Rect listRect(int i) const {
			int rows = listRows();
			return Rect(Vec((i / rows) * COL_W, listTop() + HEAD_H + (i % rows) * ROW), Vec(COL_W - 8.f, ROW));
		}
		// The key box inside an action row: one straight column, same look as the grid.
		Rect keyBoxRect(int i) const {
			Rect r = listRect(i);
			return Rect(Vec(r.pos.x + r.size.x - KEY_W, r.pos.y + 2.f), Vec(KEY_W, BND_WIDGET_HEIGHT - 2.f));
		}
		// What's under the mouse: a grid box (row, col) or a list entry (index).
		void hit(int& r, int& c, int& item) {
			r = c = item = -1;
			for (int rr = 0; rr < gridRows(); rr++)
				for (int cc = 0; cc < KEY_ROW; cc++)
					if (boxRect(rr, cc).contains(mouse)) { r = rr; c = cc; return; }
			for (int i = 0; i < listCount(); i++)
				if (!isFiller(listAction(i)) && listRect(i).contains(mouse)) { item = i; return; }
		}

		void onHover(const HoverEvent& e) override {
			mouse = e.pos;
			OpaqueWidget::onHover(e);
		}
		void onLeave(const LeaveEvent& e) override {
			mouse = Vec(-1.f, -1.f);
			OpaqueWidget::onLeave(e);
		}
		void onButton(const ButtonEvent& e) override;   // (below EncoderWidget)
		void draw(const DrawArgs& args) override;

		// While a key is being learned, the menu stays open and this panel
		// takes the keyboard (it's "selected" after the click), so the key
		// you press is learned here instead of going to the menu.
		// Esc = cancel; Shift / Ctrl / Alt alone are part of a combo, so ignored.
		void onSelectKey(const SelectKeyEvent& e) override {
			if (!owner || owner->learnAction < 0) {
				OpaqueWidget::onSelectKey(e);
				return;
			}
			if (e.action == GLFW_PRESS) {
				bool modifierOnly = (e.key >= GLFW_KEY_LEFT_SHIFT && e.key <= GLFW_KEY_RIGHT_SUPER);
				if (e.key == GLFW_KEY_ESCAPE) owner->cancelLearn();
				else if (!modifierOnly) owner->learned(e.key, e.mods & RACK_MOD_MASK);
			}
			e.consume(this);
		}
		// Is this box / row the one waiting for a key? (It blinks.)
		bool waitingFor(int action, int index) const {
			return owner && owner->learnAction == action && owner->learnIndex == index;
		}
	};

	// The layout default for the keys as they are now.
	KeyMap defaultKeys() {
		KeyMap d;
		d.setLayout(baseLayout());
		return d;
	}
	// The layout default for one single-key action.
	KeyBinding defaultBinding(int action) {
		KeyMap d = defaultKeys();
		KeyBinding* b = actionBinding(action);
		if (!b) return KeyBinding();
		return *(KeyBinding*) ((char*) &d + ((char*) b - (char*) &settingKeys));   // same field in the defaults
	}
	// A knob / button / slot row (0 / 1 / 2) of a KeyMap.
	static int* keyRow(KeyMap& k, int r) { return (r == 0) ? k.knobs : (r == 1) ? k.buttons : k.slots; }
	// A modifier combo's name: "Shift", "Alt", "Ctrl", "Shift+Alt".
	static std::string modsName(int mods) {
		std::string s;
		if (mods == 0) return "Off";
		if (mods & RACK_MOD_CTRL) s += "Ctrl";
		if (mods & GLFW_MOD_SHIFT) s += s.empty() ? "Shift" : "+Shift";
		if (mods & GLFW_MOD_ALT) s += s.empty() ? "Alt" : "+Alt";
		return s.empty() ? "none" : s;
	}

	// Choose the select or assign modifier. They can't be the same: picking
	// the other one's modifier swaps the two, so nothing ever breaks.
	void setSlotMods(bool select, int mods) {
		keysUndoable("slot modifier", [=]() {
			int& mine = select ? settingKeys.slotSelectMods : settingKeys.slotAssignMods;
			int& other = select ? settingKeys.slotAssignMods : settingKeys.slotSelectMods;
			if (other == mods) other = mine;
			mine = mods;
		});
		// Warn if Rack itself uses one of the new combos (e.g. Ctrl + 0 = zoom).
		std::string what = select ? "Select slot" : "Assign slot";
		for (int i = 0; i < KEY_ROW; i++) {
			int k = settingKeys.slots[i];
			if (k < 0) continue;
			for (int m : {settingKeys.slotSelectMods, settingKeys.slotAssignMods}) {
				if (isRackShortcut(k, m)) {
					showKeyMessage("RACK KEY", keyName(k, m), what + " = " + modsName(mods) + ", but Rack also uses " +
						keyName(k, m) + " for one of its own shortcuts - that slot won't work");
					return;
				}
			}
		}
		showKeyMessage("SET", modsName(mods), what + " = " + modsName(mods) + " + slot key");
	}

	// The MOUSE gestures' modifiers: renumber / knob push / custom key. All
	// three must differ (they'd fight over the same click), so picking one
	// that another already uses swaps the two.
	void setMouseMods(int which, int mods) {   // which = KeysPanel::ACT_REMAP_MODS ...
		static const char* names[3] = {"Swap module's hotkeys", "Assign knob press", "Assign hotkey manually"};
		int idx = KeysPanel::ACT_REMAP_MODS - which;   // 0, 1, 2
		keysUndoable("mouse modifier", [=]() {
			int* all[3] = {&settingKeys.remapMods, &settingKeys.pushLinkMods, &settingKeys.customMods};
			for (int o = 0; o < 3; o++)
				if (o != idx && *all[o] == mods) *all[o] = *all[idx];
			*all[idx] = mods;
		});
		showKeyMessage("SET", modsName(mods), std::string(names[idx]) + " = " + modsName(mods) + "-click");
	}
	// What a modifier row in the Key mapping view is set to, and its default.
	static int modsOf(int which) {
		switch (which) {
			case KeysPanel::ACT_SELECT_MODS: return settingKeys.slotSelectMods;
			case KeysPanel::ACT_ASSIGN_MODS: return settingKeys.slotAssignMods;
			case KeysPanel::ACT_REMAP_MODS: return settingKeys.remapMods;
			case KeysPanel::ACT_PUSH_MODS: return settingKeys.pushLinkMods;
			case KeysPanel::ACT_CUSTOM_MODS: return settingKeys.customMods;
			case KeysPanel::ACT_NAV_MODS: return settingNavModifier;
		}
		return 0;
	}
	static int defaultModsOf(int which) {
		switch (which) {
			case KeysPanel::ACT_SELECT_MODS: return GLFW_MOD_SHIFT;
			case KeysPanel::ACT_ASSIGN_MODS: return GLFW_MOD_ALT;
			case KeysPanel::ACT_REMAP_MODS: return GLFW_MOD_ALT;
			case KeysPanel::ACT_PUSH_MODS: return GLFW_MOD_ALT | RACK_MOD_CTRL;
			case KeysPanel::ACT_CUSTOM_MODS: return GLFW_MOD_ALT | GLFW_MOD_SHIFT;
			case KeysPanel::ACT_NAV_MODS: return GLFW_MOD_ALT;
		}
		return 0;
	}
	// Set any modifier row (slot, mouse or navigation).
	void setModsOf(int which, int mods) {
		if (which == KeysPanel::ACT_SELECT_MODS || which == KeysPanel::ACT_ASSIGN_MODS)
			setSlotMods(which == KeysPanel::ACT_SELECT_MODS, mods);
		else if (which == KeysPanel::ACT_NAV_MODS) {
			settingNavModifier = mods;
			showKeyMessage("SET", modsName(mods), mods ? "Navigation knob = hold " + modsName(mods) + " + turn the dial"
				: std::string("Navigation knob: dial turning switched off"));
		}
		else setMouseMods(which, mods);
	}

	// The Keys submenu: the panel above, then the two one-off commands.
	void appendKeysMenu(Menu* menu) {
		menu->addChild(createMenuLabel("Click a box or action, then press its new key (Esc cancels). Right-click = back to default."));
		KeysPanel* panel = new KeysPanel;
		panel->owner = this;
		menu->addChild(panel);
		menu->addChild(new MenuSeparator);
		menu->addChild(stayAction("No separate button page key", "buttons follow knob pages", [=]() {
			keysUndoable("button page key", [=]() { settingKeys.buttonPageNext = KeyBinding(); });
			showKeyMessage("BUTTON", "PAGES", "Buttons now follow the knob pages (when a module has more than 10)");
		}, settingKeys.buttonPageNext.key < 0));
		menu->addChild(new MenuSeparator);
		menu->addChild(stayAction("Reset keys to the layout's defaults", KeyMap::layoutShortName(baseLayout()), [=]() {
			int l = baseLayout();
			keysUndoable("reset keys", [=]() { settingKeys.setLayout(l); });
			showKeyMessage("KEYS", "RESET", std::string("All keys are back to the ") + KeyMap::layoutName(l) + " defaults");
		}));
	}


	// =========================================================================
	//  RIGHT-CLICK MENU
	//  One menu in sections, most-used first, set-once last:
	//    CONTROLS              Set up my dial..., Layout >, Dial & mouse wheel >,
	//                          Key mapping >, Lock keys
	//    GROUPS & SLOTS        organising this patch
	//    SELECTED MODULE       fixes for the module you're playing  (starts folded)
	//    HIGHLIGHTS & COLOURS  Highlights > (incl. brightness), Colours >  (starts folded)
	//    MIDI CONTROLLER       only for people with MIDI gear  (starts folded)
	//  Nothing is buried more than one level (Colours >, Layout >, ...).
	// =========================================================================

	// Reset everything in HIGHLIGHTS & COLOURS.
	void resetHighlights() {
		settingShowPlayedOutline = settingShowGroupModules = settingShowKnobBadges = settingShowButtonBadges = true;
		settingShowSlotNumbers = true;
		settingShowValue = true;
		settingColorPlayed = settingColorKnobBadges = settingColorButtonBadges = RGB_AMBER;
		settingColorGroup = RGB_TEAL;
		settingColorMapping = RGB_TEAL;
		settingHighlightBrightness = 1.f;
		settingFollowRoomBrightness = false;
	}

	void appendContextMenu(Menu* menu) override {
		menu->addChild(new SubmenuSide);   // submenus open to the left when there's no room on the right
		Encoder* e = enc();
		ModuleWidget* sel = selectedModule();
		section(menu, "CONTROLS", [=](Menu* m) { menuControls(m); });
		section(menu, string::f("GROUPS & SLOTS  (group %d of %d)", e ? e->currentGroup + 1 : 1, e ? (int) e->groups.size() : 1),
			[=](Menu* m) { menuGroups(m); }, "GROUPS & SLOTS");
		section(menu, sel ? "SELECTED MODULE: " + sel->model->getFullName() + "  (every one of this type)"
			: std::string("SELECTED MODULE  (select a module first)"), [=](Menu* m) { menuSelectedModule(m); }, "SELECTED MODULE");
		section(menu, "HIGHLIGHTS & COLOURS", [=](Menu* m) { menuHighlights(m); });
		section(menu, "GAMEPAD & MIDI  (controllers follow the selected module)", [=](Menu* m) { menuMidi(m); }, "MIDI CONTROLLER");
	}

	// One foldable section: a separator, its clickable heading, then its rows
	// (hidden while folded). `key` = what the fold is remembered by, when the
	// heading shows live info; otherwise the heading itself.
	void section(Menu* menu, const std::string& heading, std::function<void(Menu*)> body, const std::string& key = "") {
		menu->addChild(new MenuSeparator);
		SectionHeader* h = new SectionHeader;
		h->title = heading;
		h->key = key.empty() ? heading : key;
		menu->addChild(h);
		size_t before = menu->children.size();
		body(menu);
		auto it = menu->children.begin();
		std::advance(it, before);
		for (; it != menu->children.end(); ++it) h->items.push_back(*it);
		h->apply();
	}

	// ---- CONTROLS (all patches) -------------------------------------------
	void menuControls(Menu* menu) {
		// Set up my dial first: the first thing a new user needs.
		menu->addChild(createMenuItem("Set up my dial...", "", [=]() { startWizard(); }));
		// Layout next: it decides every key below it.
		menu->addChild(createSubmenuItem("Layout", KeyMap::layoutShortName(settingKeys.matchingLayout()),
			[=](Menu* menu) { appendLayoutMenu(menu); }));
		// Everything about how the keyboard dial / mouse wheel feels, together
		// in one place — so nobody mistakes it for the panel's navigation knob.
		menu->addChild(createSubmenuItem("Dial & mouse wheel", string::f("x%.1f", settingSensitivity),
			[=](Menu* menu) { appendDialMenu(menu); }));
		// Extra dials: keyboards / macropads with more knobs.
		menu->addChild(createSubmenuItem("Extra dials", dialsSetUp() ? string::f("%d set up", dialsSetUp()) : std::string("up to 4"),
			[=](Menu* menu) { appendExtraDialsMenu(menu); }));
		menu->addChild(createSubmenuItem("Key mapping", "every key, one screen", [=](Menu* menu) { appendKeysMenu(menu); }));
		// Lock last: there's also the LOCK button and the Scroll Lock key.
		menu->addChild(createMenuItem(isLocked() ? "Unlock keys" : "Lock keys (to type)",
			keyName(settingKeys.lockKey.key, settingKeys.lockKey.mods), [=]() { toggleLock(); }));
	}

	// ---- Gamepad mapping submenu ------------------------------------------------
	// One row per action, its gamepad control on the right. Click a row, then
	// press the control on the gamepad (menu stays open; the row blinks).
	// Right-click a row = back to default. Same way of working as Key mapping.
	// The view, in the same style as Key mapping:
	//   - on top, a simple controller drawing showing what each control does
	//     right now (generic shape — no brand artwork);
	//   - below, two columns of actions (NAVIGATE / PLAY), each with its
	//     gamepad control in a box: click = learn, right-click = default, lit
	//     = changed from the default;
	//   - a line at the bottom describing what's under the mouse.
	struct PadPanel : widget::OpaqueWidget {
		EncoderWidget* owner = nullptr;
		static constexpr float W = 560.f;
		static constexpr float PIC_H = 160.f;   // the controller drawing
		static constexpr float ROW = BND_WIDGET_HEIGHT + 2.f;
		static constexpr float HEAD_H = 18.f;
		static constexpr float COL_W = 280.f;
		static constexpr float KEY_W = 120.f;
		static constexpr int ROWS = 10;
		Vec mouse = Vec(-1.f, -1.f);
		PadPanel() { box.size = Vec(W, infoTop() + BND_WIDGET_HEIGHT + 4.f); }
		static float listTop() { return PIC_H + 6.f; }
		static float infoTop() { return listTop() + HEAD_H + ROWS * ROW + 4.f; }
		static int listAction(int i) {
			static const int order[2 * ROWS] = {
				PA_GROUP_PREV, PA_GROUP_NEXT, PA_MODULE_PREV, PA_MODULE_NEXT, PA_PAGE_PREV, PA_PAGE_NEXT,
				PA_HOME, PA_UNDO, -1, -1,
				PA_KNOB_A, PA_KNOB_B, PA_PRESS_A, PA_PRESS_B, PA_BTN1, PA_BTN2, PA_BTN3, PA_BTN4,
				PA_BANK_PREV, PA_BANK_NEXT};
			return order[i];
		}
		static Rect listRect(int i) {
			return Rect(Vec((i / ROWS) * COL_W, listTop() + HEAD_H + (i % ROWS) * ROW), Vec(COL_W - 8.f, ROW));
		}
		static Rect keyBoxRect(int i) {
			Rect r = listRect(i);
			return Rect(Vec(r.pos.x + r.size.x - KEY_W, r.pos.y + 2.f), Vec(KEY_W, BND_WIDGET_HEIGHT - 2.f));
		}
		int hitRow() {
			for (int i = 0; i < 2 * ROWS; i++)
				if (listAction(i) >= 0 && listRect(i).contains(mouse)) return i;
			return -1;
		}
		void onHover(const HoverEvent& e) override { mouse = e.pos; OpaqueWidget::onHover(e); }
		void onLeave(const LeaveEvent& e) override { mouse = Vec(-1.f, -1.f); OpaqueWidget::onLeave(e); }
		void onButton(const ButtonEvent& e) override {
			OpaqueWidget::onButton(e);
			e.consume(this);   // (right clicks too, or the menu would close)
			if (e.action != GLFW_PRESS || !owner) return;
			mouse = e.pos;
			int i = hitRow();
			if (i < 0) return;
			int a = listAction(i);
			if (e.button == GLFW_MOUSE_BUTTON_LEFT) owner->startPadLearn(a);   // menu stays open
			else if (e.button == GLFW_MOUSE_BUTTON_RIGHT) settingPadMap.control[a] = PadMap().control[a];
		}
		// Esc while a row waits for the gamepad = cancel.
		void onSelectKey(const SelectKeyEvent& e) override {
			if (owner && owner->padLearn >= 0 && e.action == GLFW_PRESS && e.key == GLFW_KEY_ESCAPE) {
				owner->padLearn = -1;
				e.consume(this);
				return;
			}
			OpaqueWidget::onSelectKey(e);
		}
		void draw(const DrawArgs& args) override;   // (below EncoderWidget)
	};
	// A very short name for the action on a control (the drawing's labels).
	static std::string padActionShort(int a) {
		static const char* s[NUM_PAD_ACTIONS] = {"Mod-", "Mod+", "Grp-", "Grp+", "J1", "J2", "J1 press", "J2 press",
			"1", "2", "3", "4", "J back", "J along", "Pg-", "Pg+", "Undo", "Home"};
		return (a >= 0 && a < NUM_PAD_ACTIONS) ? s[a] : "";
	}
	// Which action a control does ( -1 = none).
	static int padActionFor(int control) {
		for (int a = 0; a < NUM_PAD_ACTIONS; a++) if (settingPadMap.control[a] == control) return a;
		return -1;
	}
	void appendPadMenu(Menu* menu) {
		menu->addChild(stayBool("Use a game controller", "when one is plugged in", &settingPadOn));
		menu->addChild(createMenuLabel(!settingPadOn ? "Game controller: off"
			: padName.empty() ? "No game controller found - plug one in (Xbox, PlayStation ...)"
			: "Connected: " + padName));
		menu->addChild(createMenuLabel("Click a row, then press the control on the gamepad (Esc cancels). Right-click = default."));
		PadPanel* panel = new PadPanel;
		panel->owner = this;
		menu->addChild(panel);
		menu->addChild(new MenuSeparator);
		menu->addChild(stayAction("Reset to the default layout", "", [=]() {
			settingPadMap.setDefaults();
			padBank = 0;
		}, settingPadMap == PadMap()));
	}

	// Dial & mouse wheel submenu: the keyboard dial / mouse wheel (and the
	// = / - turn keys), not the navigation knob on the panel.
	void appendDialMenu(Menu* menu) {
		menu->addChild(createMenuLabel("How your keyboard dial / mouse wheel behaves"));
		menu->addChild(new SensitivitySlider);
		menu->addChild(stayBool("Acceleration", "turn faster = bigger steps", &settingAcceleration));
		menu->addChild(stayBool("Invert scroll / turn", "if it turns the wrong way", &settingReverseDial));
		menu->addChild(stayBool("Hold several numbers = turn them together", "", &settingMultiHold));
		menu->addChild(stayBool("Double-tap push = reset knob", "like double-clicking it", &settingDoubleTapReset));
		menu->addChild(stayBool("Mouse wheel click = push", "", &settingWheelClickPush));
		// (The navigation knob's "hold + dial" modifier lives in Key mapping >
		//  MOUSE & DIAL, with every other key and modifier.)
	}

	// ---- GROUPS & SLOTS (this patch) -----------------------------------------
	void menuGroups(Menu* menu) {
		Encoder* e = enc();
		int current = e ? e->currentGroup : 0;
		// Off (default) = you choose what goes in each slot. On = every module
		// in the patch, in rack order, 10 per group, kept up to date.
		menu->addChild(stayCheck("Fill slots automatically", "every module, 10 per group",
			[=]() { Encoder* en = enc(); return en && en->autoFill; },
			[=]() { Encoder* en = enc(); if (en) setAutoFill(!en->autoFill); }));
		// Slots stay put once filled; this tidies them back into rack order
		// when YOU want it (one undo step).
		if (e && e->autoFill) {
			menu->addChild(createMenuItem("Re-sort slots in rack order", "", [=]() { resortAutoFill(); }));
		}
		menu->addChild(stayBool("Scroll to the selected module", "when it's off screen", &settingFollowSelected));
		menu->addChild(createMenuItem("New group", keyName(settingKeys.groupNew.key, settingKeys.groupNew.mods), [=]() {
			newGroup();
		}));
		menu->addChild(createMenuItem("Clear this group", keyName(settingKeys.groupClear.key, settingKeys.groupClear.mods), [=]() {
			clearCurrentGroup();
		}));
		menu->addChild(createMenuItem("Clear all groups", "", [=]() {
			clearAllGroups();
		}));
		menu->addChild(createSubmenuItem(string::f("Slots in group %d", current + 1), "", [=](Menu* menu) {
			for (int i = 0; i < NUM_SLOTS; i++) {
				ModuleWidget* mw = slotModule(i);
				std::string label = string::f("Slot %d", i + 1);
				std::string right = mw ? mw->model->getFullName() : "empty";

				menu->addChild(createSubmenuItem(label, right, [=](Menu* menu) {
					menu->addChild(createMenuItem("Assign (then click a module)", "", [=]() {
						learningSlot = i;
					}));
					menu->addChild(stayAction("Clear", "", [=]() {
						assignSlot(i, nullptr);
					}, !mw));
				}));
			}
		}));
	}

	// ---- SELECTED MODULE (its type's set-up, all patches) --------------------
	void menuSelectedModule(Menu* menu) {
		ModuleWidget* sel = selectedModule();
		// (Each reset is one undo step — Ctrl+Z brings it back.)
		// Reset knob hotkeys = BOTH kinds of hotkey change for this module type:
		// swapped hotkeys (Swap knob hotkey) and manually assigned ones
		// (Assign hotkey manually). One item, because to the player they're
		// the same thing: "this module's hotkeys, back to how they started".
		auto selIt = sel ? settingRemaps.find(remapKey(sel)) : settingRemaps.end();
		bool hasCustom = selIt != settingRemaps.end() && !selIt->second.customKeys.empty();
		menu->addChild(createMenuItem("Reset module's hotkeys", "back to default", [=]() {
			remapUndoable(sel, "reset knob hotkeys", [=]() {
				resetRemap();   // swaps
				auto it = settingRemaps.find(remapKey(sel));
				if (it == settingRemaps.end()) return;
				it->second.customKeys.clear();   // manually assigned hotkeys
				if (it->second.empty()) settingRemaps.erase(it);
				knobsForModuleId = -1;
			});
		}, !hasNumberingRemap(sel) && !hasCustom));
		menu->addChild(createMenuItem("Reset knob sensitivities", "", [=]() {
			remapUndoable(sel, "reset knob sensitivities", [=]() { resetKnobSensitivities(); });
		}, !hasKnobSensitivities(sel)));
		menu->addChild(createMenuItem("Reset knob press", "back to default", [=]() {
			remapUndoable(sel, "reset push links", [=]() { clearPushLinks(); });
		}, !hasPushLinks(sel)));

		// Knobs that wrap round: every knob of this module, ticked if it goes
		// round past its end instead of stopping. Guessed automatically; tick
		// / untick any of them (for every module of this type, undoable).
		std::vector<ParamWidget*> knobs = sel ? currentKnobs() : std::vector<ParamWidget*>();
		int wrapping = 0;
		for (ParamWidget* pw : knobs) if (knobWraps(pw)) wrapping++;
		menu->addChild(createSubmenuItem("Knobs that wrap round", sel ? string::f("%d of %d", wrapping, (int) knobs.size()) : "",
				[=](Menu* menu) {
			menu->addChild(createMenuLabel("Ticked = goes round past its end, like an endless encoder"));
			for (int i = 0; i < (int) knobs.size(); i++) {
				ParamWidget* pw = knobs[i];
				ParamQuantity* pq = pw->getParamQuantity();
				if (!pq) continue;
				int paramId = pq->paramId;
				bool autoOn = autoWraps(pw);
				menu->addChild(stayCheck(string::f("%d  %s", i + 1, pq->getLabel().c_str()), autoOn ? "auto" : "",
					[=]() { return knobWraps(pw); },
					[=]() {
						bool now = !knobWraps(pw);
						remapUndoable(sel, "knob wrap round", [=]() {
							// Matching the automatic guess = no need to remember it.
							if (now == autoOn) {
								auto it = settingRemaps.find(remapKey(sel));
								if (it != settingRemaps.end()) {
									it->second.wrapKnobs.erase(paramId);
									if (it->second.empty()) settingRemaps.erase(it);
								}
							}
							else settingRemaps[remapKey(sel)].wrapKnobs[paramId] = now;
						});
					}));
			}
			auto rit = settingRemaps.find(remapKey(sel));
			bool changed = rit != settingRemaps.end() && !rit->second.wrapKnobs.empty();
			menu->addChild(new MenuSeparator);
			menu->addChild(stayAction("Back to automatic", "", [=]() {
				remapUndoable(sel, "knob wrap round", [=]() {
					auto it = settingRemaps.find(remapKey(sel));
					if (it == settingRemaps.end()) return;
					it->second.wrapKnobs.clear();
					if (it->second.empty()) settingRemaps.erase(it);
				});
			}, !changed));
		}, !sel || knobs.empty()));
	}

	// ---- HIGHLIGHTS & COLOURS (all patches) -----------------------------------
	void menuHighlights(Menu* menu) {
		// What's shown: six on / off switches, one level down (set once, so
		// they don't need to crowd the main menu). Right side: how many are on.
		int on = settingShowPlayedOutline + settingShowGroupModules + settingShowKnobBadges +
			settingShowButtonBadges + settingShowValue + settingShowSlotNumbers;
		menu->addChild(createSubmenuItem("Highlights", string::f("%d of 6 shown", on), [=](Menu* menu) {
			menu->addChild(stayBool("Played module outline", "", &settingShowPlayedOutline));
			menu->addChild(stayBool("Current group outlines + slot tags", "", &settingShowGroupModules));
			menu->addChild(stayBool("Knob badges", "", &settingShowKnobBadges));
			menu->addChild(stayBool("Button badges", "", &settingShowButtonBadges));
			menu->addChild(stayBool("Value while turning", "", &settingShowValue));
			menu->addChild(stayBool("Slot numbers on panel", "", &settingShowSlotNumbers));
			// How bright the highlights are — with them, since it's about them.
			menu->addChild(new MenuSeparator);
			menu->addChild(stayBool("Follow room brightness", "dim with Rack's room lights", &settingFollowRoomBrightness));
			menu->addChild(new BrightnessSlider);
		}));
		menu->addChild(createSubmenuItem("Colours", "", [=](Menu* menu) { appendColoursMenu(menu); }));
		menu->addChild(createMenuItem("Reset highlights & colours", "", [=]() { resetHighlights(); }));
	}

	// Colours submenu: one row per colour — slide it, or type a colour code.
	// The defaults are fine to leave alone.
	void appendColoursMenu(Menu* menu) {
		Encoder* e = enc();
		menu->addChild(createMenuLabel("Slide for any colour, or type a code like #40D0C0 + Enter. Double-click = default"));
		auto colorRow = [=](const char* label, int* setting, int defaultRgb) {
			menu->addChild(new ColorRow(label, [=]() { return *setting; }, [=](int c) { *setting = c; }, defaultRgb));
		};
		colorRow("Played module", &settingColorPlayed, RGB_AMBER);
		colorRow("Knob badges", &settingColorKnobBadges, RGB_AMBER);
		colorRow("Button badges", &settingColorButtonBadges, RGB_AMBER);
		colorRow("Mapping highlights", &settingColorMapping, RGB_TEAL);
		// Every group that exists in this patch, each with its own row — not
		// just the one you're on. (Group 1's colour is also the fallback for
		// any group left on "default".)
		colorRow("Group 1", &settingColorGroup, RGB_TEAL);
		EncoderWidget* self = this;
		int numGroups = e ? (int) e->groups.size() : 1;
		for (int g = 1; g < numGroups; g++) {
			menu->addChild(new ColorRow(string::f("Group %d", g + 1),
				[=]() { Encoder* en = self->enc(); return (en && g < (int) en->groupColors.size()) ? en->groupColor(g) : settingColorGroup; },
				[=](int c) { Encoder* en = self->enc(); if (en) en->setGroupColor(g, c); },
				e->autoGroupColor(g)));
		}
	}

	// ---- MIDI CONTROLLER (all patches) ------------------------------------------
	void menuMidi(Menu* menu) {
		// Gamepad first: it just works when plugged in (no set-up).
		menu->addChild(createSubmenuItem("Gamepad mapping", !settingPadOn ? "off" : padName.empty() ? "no controller" : "connected",
			[=](Menu* menu) { appendPadMenu(menu); }));
		menu->addChild(new MenuSeparator);
		menu->addChild(createSubmenuItem("MIDI input", "", [=](Menu* menu) { appendMidiMenu(menu, &midiInput); }));
		menu->addChild(createSubmenuItem("MIDI input 2", "a second controller", [=](Menu* menu) { appendMidiMenu(menu, &midiInput2); }));
		menu->addChild(createSubmenuItem("MIDI mapping", string::f("%d set", (int) settingMidiMap.size()),
			[=](Menu* menu) { appendMidiMappingsMenu(menu); }));
		// A controller set-up as a file — to keep, or to share (e.g. a ready-
		// made mapping for a popular controller).
		menu->addChild(createMenuItem("Save mapping to file...", "", [=]() { saveMidiMappingFile(); }, settingMidiMap.empty()));
		menu->addChild(createMenuItem("Load mapping file...", "", [=]() { loadMidiMappingFile(); }));
		static const char* potNames[NUM_POT_MODES] = {"Catch up", "Pickup", "Relative"};
		menu->addChild(createSubmenuItem("Pots & faders", potNames[clamp(settingMidiPotMode, 0, NUM_POT_MODES - 1)], [=](Menu* menu) {
			menu->addChild(createMenuLabel("For controller knobs with end stops (endless encoders don't need this)"));
			static const char* choices[NUM_POT_MODES] = {
				"Catch up - move straight away, meet at the end (default)",
				"Pickup - wait until the pot reaches the knob",
				"Relative - add each movement, like an encoder"};
			for (int i = 0; i < NUM_POT_MODES; i++)
				menu->addChild(stayCheck(choices[i], "",
					[=]() { return settingMidiPotMode == i; },
					[=]() { settingMidiPotMode = i; absState.clear(); }));
		}));
	}


	// =========================================================================
	//  EVERY SCREEN FRAME (UI thread, never the audio thread)
	// =========================================================================
	void step() override {
		claimIfFree();
		Encoder* e = enc();
		if (e && !isActive()) {
			showInactive();
			ModuleWidget::step();
			return;
		}
		if (e) {
			double now = system::getTime();

			// ---- Dial lights ------------------------------------------------
			upLight->brightness = (lastUpTime < 0) ? 0.f : flash(now - lastUpTime);
			downLight->brightness = (lastDownTime < 0) ? 0.f : flash(now - lastDownTime);
			// Push: lit while held, plus a flash on each press.
			bool pushDown = pushHeldNow();   // dial push key or mouse wheel button
			float pushFlash = (lastPushTime < 0) ? 0.f : flash(now - lastPushTime);
			pushLight->brightness = pushDown ? 1.f : pushFlash;

			// ---- MIDI controller: read what's arrived ------------------------
			midiFrame(now);
			// ---- Game controller: read its buttons and sticks ---------------
			padFrame(now);

			// ---- Panel controls (UI thread; the audio thread never looks) ----
			// Navigation encoder: every 1/24 turn = one step, however it was
			// moved (mouse, MIDI-MAP ...). Wrapping round counts the short way.
			float navValue = e->params[Encoder::NAV_PARAM].getValue();
			if (navLast < 0.f) navLast = navValue;
			float d = navValue - navLast;
			if (d > 0.5f) d -= 1.f;
			if (d < -0.5f) d += 1.f;
			navLast = navValue;
			navAccum += d * NAV_STEPS;
			while (navAccum >= 1.f) { navTurn(+1); navAccum -= 1.f; followSkip = true; }
			while (navAccum <= -1.f) { navTurn(-1); navAccum += 1.f; followSkip = true; }
			// Buttons: act on the moment they're pressed (works with MIDI-MAP too).
			bool lockNow = e->params[Encoder::LOCK_PARAM].getValue() > 0.5f;
			bool gPrevNow = e->params[Encoder::GROUP_PREV_PARAM].getValue() > 0.5f;
			bool gNextNow = e->params[Encoder::GROUP_NEXT_PARAM].getValue() > 0.5f;
			if (lockNow && !lockWas) toggleLock();
			if (gPrevNow && !groupPrevWas) { switchGroup(-1); followSkip = true; }
			if (gNextNow && !groupNextWas) { switchGroup(+1); followSkip = true; }
			lockWas = lockNow;
			groupPrevWas = gPrevNow;
			groupNextWas = gNextNow;

			// SENS knob <-> sensitivity, both ways: turning the knob sets it;
			// numpad + / - or the menu slider moving it moves the knob too.
			{
				float knob = e->params[Encoder::SENS_PARAM].getValue();
				if (sensKnobWritten < 0.f) {
					// First frame: your saved sensitivity wins (it's for all patches).
					e->params[Encoder::SENS_PARAM].setValue(sensToKnob(settingSensitivity));
				}
				else if (std::fabs(knob - sensKnobWritten) > 1e-5f) {
					settingSensitivity = sensFromKnob(knob);           // turned on the panel
					lastSensChangeTime = now;
				}
				else if (std::fabs(sensToKnob(settingSensitivity) - knob) > 1e-4f)
					e->params[Encoder::SENS_PARAM].setValue(sensToKnob(settingSensitivity));   // changed elsewhere
				sensKnobWritten = e->params[Encoder::SENS_PARAM].getValue();
			}

			// ---- Safety: if a number's release was missed (e.g. the Rack
			//      window lost focus while it was held), let go of it now. ---
			if (heldKey >= 0 && glfwGetKey(APP->window->win, heldKey) != GLFW_PRESS)
				numberReleased(heldKey);
			// ---- Safety: custom keys whose release was missed --------------
			if (heldCustomKey >= 0 && glfwGetKey(APP->window->win, heldCustomKey) != GLFW_PRESS)
				customReleased(heldCustomKey);
			for (auto it = customHeldButtons.begin(); it != customHeldButtons.end();) {
				int k = it->first;
				++it;   // (customReleased may erase the current entry)
				if (glfwGetKey(APP->window->win, k) != GLFW_PRESS) customReleased(k);
			}

			// ---- Undo: a turn paused for a second counts as finished ------
			if (turnGesture.moduleId >= 0 && now - turnGesture.lastTime > TURN_GESTURE_PAUSE) finishTurn();
			if (!multiGesture.knobs.empty() && now - multiGesture.lastTime > TURN_GESTURE_PAUSE) finishTurn();

			// ---- Dial push: let go once the key is up AND the press has
			//      lasted long enough for the module to notice it. --------
			if (activePush.moduleId >= 0) {
				int held = activePush.fromWheel
					? glfwGetMouseButton(APP->window->win, GLFW_MOUSE_BUTTON_MIDDLE)
					: activePush.dialKey >= 0 ? glfwGetKey(APP->window->win, activePush.dialKey)   // extra dial
					: (pushKey() >= 0 ? glfwGetKey(APP->window->win, pushKey()) : GLFW_RELEASE);
				// (A MIDI press is held until its own release arrives.)
				bool keyDown = !activePush.keyUp && (activePush.fromMidi || held == GLFW_PRESS);
				if (!keyDown && now - activePush.startTime >= MIN_PUSH_TIME) endPush();
			}

			// Same safety for push buttons held by a letter key.
			for (int i = 0; i < NUM_BUTTON_KEYS; i++) {
				int bk = heldButtons[i].key;
				if (heldButtons[i].moduleId >= 0 && (bk < 0 || glfwGetKey(APP->window->win, bk) != GLFW_PRESS))
					releaseButton(i);
			}

			// ---- Forget slots whose module was deleted from the patch -------
			// Checked 4 times a second (not every frame — with many groups
			// that's a lot of lookups for something that rarely changes),
			// using the engine's fast lookup. A group that loses its last
			// module disappears (unless it's the one you're on).
			// Auto-fill: rebuild the slots when the rack has changed (modules
			// added, removed or moved) — and that's all the tidying it needs.
			if (e->autoFill && now - lastCleanupTime > 0.25) {
				lastCleanupTime = now;
				if (autoFillFingerprint() != autoFillPrint) {
					autoFilling = true;
					// (First time in this session — e.g. a patch just opened with
					//  auto-fill on — still only UPDATES: the saved slots stay put.)
					autoFillUpdate();
					autoFilling = false;
				}
			}
			else if (now - lastCleanupTime > 0.25) {
				lastCleanupTime = now;
				autoFilling = true;   // (emptying slots of deleted modules isn't "by hand")
				restoreReturnedModules();
				std::vector<int64_t> gone;
				for (const auto& g : e->groups)
					for (int64_t id : g)
						if (id >= 0 && !APP->engine->getModule(id) &&
								std::find(gone.begin(), gone.end(), id) == gone.end())
							gone.push_back(id);
				if (!gone.empty()) {
					// Remember how everything looked, so Ctrl+Z (which brings
					// the module back) can put it back exactly where it was.
					Grave grave;
					grave.ids = gone;
					grave.before = snapshot();
					for (int g = 0; g < (int) e->groups.size(); g++)
						for (int s = 0; s < NUM_SLOTS; s++)
							if (std::find(gone.begin(), gone.end(), e->groups[g][s]) != gone.end())
								assignSlotIn(g, s, nullptr);
					dropEmptyGroups();
					grave.after = snapshot();
					graves.push_back(grave);
					if (graves.size() > MAX_GRAVES) graves.erase(graves.begin());
				}
				else dropEmptyGroups();
				autoFilling = false;
			}

			// ---- Follow: a newly selected module that's off screen -> glide
			//      the rack to it (only then; see followStep). ---------------
			{
				ModuleWidget* sm = selectedModule();
				int64_t sid = (sm && sm->module) ? sm->module->id : -1;
				if (sid != followLastId) {
					followLastId = sid;
					if (settingFollowSelected && !followSkip && sm && !moduleOnScreen(sm)) {
						followTargetId = sid;
						followActive = true;
						followUntil = now + 1.5;
					}
				}
				// Switched GROUP: if none of its modules is on screen, glide to
				// its first one (if any of them is visible, nothing moves).
				if (e->currentGroup != followLastGroup) {
					bool firstTime = (followLastGroup < 0);
					followLastGroup = e->currentGroup;
					if (!firstTime && settingFollowSelected && !followSkip) {
						ModuleWidget* first = nullptr;
						bool anyVisible = false;
						for (int s = 0; s < NUM_SLOTS; s++) {
							ModuleWidget* gm = slotModule(s);
							if (!gm) continue;
							if (!first) first = gm;
							if (moduleOnScreen(gm)) anyVisible = true;
						}
						if (first && !anyVisible) {
							followTargetId = first->module->id;
							followActive = true;
							followUntil = now + 1.5;
						}
					}
				}
				if (followActive) followStep(now);
				followSkip = false;
			}

			// ---- Slot lights (current group) --------------------------------
			//   off = empty, dim = has a module, bright = selected,
			//   blinking = waiting for a click to assign it.
			//   They glow in the CURRENT GROUP's colour, so the panel tells you
			//   which group you're on at a glance.
			bool blinkOn = std::fmod(now, 0.4) < 0.2;
			const std::vector<int64_t>& group = e->groups[e->currentGroup];
			NVGcolor groupColorNow = rgbColor(e->groupColor(e->currentGroup));
			for (int i = 0; i < NUM_SLOTS; i++) {
				float b = 0.f;
				if (group[i] >= 0) b = 0.2f;
				if (e->selectedGroup == e->currentGroup && i == e->selectedSlot) b = 1.f;
				if (i == learningSlot) b = blinkOn ? 1.f : 0.f;
				slotLights[i]->brightness = b;
				slotLights[i]->color = groupColorNow;
				slotNumbers->lit[i] = b;   // (the number inside picks a readable colour)
			}
			groupDisplay->color = groupColorNow;
			slotNumbers->color = groupColorNow;
			slotNumbers->visible = settingShowSlotNumbers;
			// Labels = the slot keys' names; only rebuilt when those keys change.
			if (std::memcmp(slotNumbers->keys, settingKeys.slots, sizeof(slotNumbers->keys)) != 0) {
				std::memcpy(slotNumbers->keys, settingKeys.slots, sizeof(slotNumbers->keys));
				for (int i = 0; i < NUM_SLOTS; i++)
					slotNumbers->labels[i] = settingKeys.slots[i] >= 0 ? keyName(settingKeys.slots[i]) : padLabel(i);
			}

			// ---- Group readout ----------------------------------------------
			// With numpad modes on, a second line ABOVE it shows what the
			// numpad is playing right now, e.g. "NUMPAD: BUTTONS".
			int modeShown = settingKeys.padModes ? padMode : -1;
			int groupKey[3] = {e->currentGroup, (int) e->groups.size(), modeShown};
			if (std::memcmp(groupKey, shownGroupKey, sizeof(groupKey)) != 0) {
				std::memcpy(shownGroupKey, groupKey, sizeof(groupKey));
				groupDisplay->text = string::f("GROUP %d/%d", groupKey[0] + 1, groupKey[1]);
				modeDisplay->text = (modeShown < 0) ? "" : std::string("NUMPAD: ") + PAD_MODE_NAMES[modeShown];
			}

			// ---- Dial setup wizard: its own readout while it runs ------------
			if (wizStep >= 0) {
				wizFrame(now);
				if (wizStep >= 0) {
					static const char* stepText[] = {"RIGHT / UP", "LEFT / DOWN", "PUSH", "HOLD PUSH", "LAYOUT 1-3"};
					pageDisplay->line1 = "DIAL SETUP";
					pageDisplay->line2 = stepText[wizStep];
					pageDisplay->line3 = string::f("step %d/5  Esc", wizStep + 1);
					pageDisplay->line3Bright = true;
					for (int i = 0; i < PAGE_KEY_LEN; i++) shownPageKey[i] = -1;
					ModuleWidget::step();
					return;
				}
			}

			// ---- MIDI learn: its own readout while it listens --------------
			if (!midiLearnRole.empty()) {
				int left = std::max(0, (int) std::ceil(MIDI_LEARN_TIMEOUT - (now - midiLearnStart)));
				pageDisplay->line1 = "MIDI LEARN";
				pageDisplay->line2 = midiRoleLabel(midiLearnRole);
				pageDisplay->line3 = (midiLearnFirst >= 0) ? "listening..." : string::f("Esc / %ds", left);
				pageDisplay->line3Bright = true;
				for (int i = 0; i < PAGE_KEY_LEN; i++) shownPageKey[i] = -1;
				ModuleWidget::step();
				return;
			}

			// ---- Extra dial learning: time out (on the push step, no push) --
			if (dialLearn >= 0 && now - dialLearnStart > DIAL_LEARN_TIMEOUT) {
				if (dialLearnStep == DL_PUSH) finishDialLearn();
				else cancelDialLearn("Dial learning timed out - nothing changed");
			}
			if (dialLearn >= 0) {
				pageDisplay->line1 = string::f("DIAL %d", dialLearn + 1);
				pageDisplay->line2 = (dialLearnStep == DL_RIGHT) ? "turn RIGHT" : (dialLearnStep == DL_LEFT) ? "turn LEFT" : "push?";
				pageDisplay->line3 = string::f("Esc / %ds", std::max(0, (int) std::ceil(DIAL_LEARN_TIMEOUT - (now - dialLearnStart))));
				pageDisplay->line3Bright = true;
				for (int i = 0; i < PAGE_KEY_LEN; i++) shownPageKey[i] = -1;   // normal text redraws after
				ModuleWidget::step();
				return;
			}

			// ---- Key learning gives up after LEARN_TIMEOUT seconds ----------
			if (learnAction >= 0 && now - learnStartTime > LEARN_TIMEOUT) {
				learnAction = -1;
				showKeyMessage("TIMED OUT", "", "Key learning timed out - nothing changed");
			}

			// ---- Key-learning messages take over the readout ----------------
			// While waiting for a key: "PRESS A KEY". For 2.5 s after: what
			// was set, or a clash warning. Then back to the normal readout.
			// (The full sentence is shown as a banner at the top of the Rack
			//  window — the panel only has room for a short version.)
			bool learning = learnAction >= 0;
			bool message = keyMessageTime >= 0 && now - keyMessageTime < 2.5;
			if (learning || message) {
				pageDisplay->line1 = learning ? "PRESS A KEY" : keyMessage1;
				pageDisplay->line2 = learning ? actionName(learnAction) : keyMessage2;
				if (learning && learnAction >= KA_KNOB && learnAction <= KA_SLOT) pageDisplay->line2 += string::f(" %d", learnIndex + 1);
				// After a few seconds with nothing arriving, the key you're
				// pressing is probably taken by Rack or Windows before the
				// Encoder can see it (e.g. Ctrl+S = Rack's Save).
				bool waitingLong = learning && now - learnStartTime > 3.0;
				int left = (int) std::ceil(LEARN_TIMEOUT - (now - learnStartTime));
				pageDisplay->line3 = !learning ? "" : waitingLong ? string::f("taken? Esc %ds", left)
					: string::f("Esc / %ds", left);
				pageDisplay->line3Bright = true;
				for (int i = 0; i < PAGE_KEY_LEN; i++) shownPageKey[i] = -1;   // normal text redraws after
				ModuleWidget::step();
				return;
			}

			// ---- Locked: say so, plainly, until unlocked ----------------------
			if (e->locked) {
				pageDisplay->line1 = "LOCKED";
				pageDisplay->line2 = keyName(settingKeys.lockKey.key, settingKeys.lockKey.mods);
				pageDisplay->line3 = "keys off";
				pageDisplay->line3Bright = false;
				for (int i = 0; i < PAGE_KEY_LEN; i++) shownPageKey[i] = -1;
				ModuleWidget::step();
				return;
			}

			// ---- Page readout -----------------------------------------------
			// Only rewritten when something it shows has changed.
			// When buttons page on their own (and need to), it shows both:
			// "K2/2 B1/3" = knob page 2 of 2, button page 1 of 3.
			int pages = selectedModule() ? pageCount() : 0;
			bool separate = buttonsPageSeparately() && buttonPageCount() > 1;
			int most = separate ? (int) currentKnobs().size()
				: (int) std::max(currentKnobs().size(), currentButtons().size());
			int key[PAGE_KEY_LEN] = {pages, page, most, settingPageFullBlock ? 1 : 0,
				separate ? currentButtonPage() : -1, separate ? buttonPageCount() : -1};
			if (std::memcmp(key, shownPageKey, sizeof(key)) != 0) {
				std::memcpy(shownPageKey, key, sizeof(key));
				if (pages == 0) {
					pageDisplay->line1 = "PAGE -";
					pageDisplay->line2 = "";
				}
				else {
					int first = page * PER_PAGE + 1;
					int last = settingPageFullBlock ? first + PER_PAGE - 1
						: std::min(first + PER_PAGE - 1, most);
					pageDisplay->line1 = separate
						? string::f("K%d/%d B%d/%d", page + 1, pages, currentButtonPage() + 1, buttonPageCount())
						: string::f("PAGE %d/%d", page + 1, pages);
					pageDisplay->line2 = (most == 0) ? "none" : string::f("%d-%d", first, last);
				}
			}

			// ---- Sensitivity line -------------------------------------------
			// While holding a number: that knob's sensitivity ("KNOB" if it
			// has its own). Otherwise the overall one. Brighter for a moment
			// after it changes, so a + / - press is easy to confirm.
			int held = heldIndex();
			bool knobView = held >= 0 && held < (int) currentKnobs().size();
			float s = knobView ? sensitivityFor(held) : settingSensitivity;
			bool own = knobView && knobHasOwnSensitivity(held);
			std::string sensText = string::f("%sx%.1f", own ? "KNOB " : "", s);
			if (settingAcceleration) sensText += " A";
			if (sensText != pageDisplay->line3) pageDisplay->line3 = sensText;
			pageDisplay->line3Bright = (lastSensChangeTime >= 0 && now - lastSensChangeTime < 1.0);
		}
		ModuleWidget::step();
	}
};


EncoderWidget* EncoderWidget::activeEncoder = nullptr;


// =============================================================================
//  UNDO for clearing slots (Alt+Delete = this group; menu = all groups)
//  Rack's undo list stores a snapshot of every group before and after.
//  (Rack's rule: undo steps must not hold pointers, only IDs — modules can
//  be deleted and restored in between.)
// =============================================================================
struct ClearSlotsAction : history::ModuleAction {
	EncoderWidget::SlotSnapshot before, after;

	void apply(const EncoderWidget::SlotSnapshot& s) {
		EncoderWidget* w = EncoderWidget::activeEncoder;
		if (w && w->module && w->module->id == moduleId) w->applySnapshot(s);
	}
	void undo() override { apply(before); }
	void redo() override { apply(after); }
};

// ---- Set-up changes for a module type (custom keys, remaps, push links,
//      sensitivity resets): the whole Remap before and after. --------------
struct RemapAction : history::Action {
	std::string key;          // "Plugin/Module"
	bool hadBefore = false, hasAfter = false;
	Remap before, after;

	static void put(const std::string& key, bool has, const Remap& r) {
		if (has) settingRemaps[key] = r;
		else settingRemaps.erase(key);
		if (EncoderWidget::activeEncoder) EncoderWidget::activeEncoder->knobsForModuleId = -1;   // rebuild lists
	}
	void undo() override { put(key, hadBefore, before); }
	void redo() override { put(key, hasAfter, after); }
};

void EncoderWidget::remapUndoable(ModuleWidget* mw, const std::string& name, std::function<void()> change) {
	if (!mw) {
		change();
		return;
	}
	std::string key = remapKey(mw);
	auto it = settingRemaps.find(key);
	bool had = it != settingRemaps.end();
	Remap before = had ? it->second : Remap();
	change();
	it = settingRemaps.find(key);
	bool has = it != settingRemaps.end();
	Remap after = has ? it->second : Remap();
	if (had == has && (!had || before == after)) return;   // nothing really changed
	RemapAction* a = new RemapAction;
	a->name = name;
	a->key = key;
	a->hadBefore = had;
	a->hasAfter = has;
	a->before = before;
	a->after = after;
	APP->history->push(a);
}

// ---- Learned keys / layouts: the whole key map before and after. ----------
struct KeyMapAction : history::Action {
	KeyMap before, after;
	void undo() override { settingKeys = before; }
	void redo() override { settingKeys = after; }
};

void EncoderWidget::keysUndoable(const std::string& name, std::function<void()> change) {
	KeyMap before = settingKeys;
	change();
	if (settingKeys == before) return;
	KeyMapAction* a = new KeyMapAction;
	a->name = name;
	a->before = before;
	a->after = settingKeys;
	APP->history->push(a);
}

// Make a change to the slots as one undoable step: snapshot, change, snapshot.
template <typename F>
static void undoableSlotChange(EncoderWidget* w, const char* name, F change) {
	if (!w->enc()) return;
	ClearSlotsAction* action = new ClearSlotsAction;
	action->name = name;
	action->moduleId = w->module->id;
	action->before = w->snapshot();
	change();
	action->after = w->snapshot();
	APP->history->push(action);
}

void EncoderWidget::clearCurrentGroup() {
	leaveAutoFill();
	undoableSlotChange(this, "clear Hotkey group", [this]() {
		Encoder* e = enc();
		std::vector<int64_t> removed = e->groups[e->currentGroup];
		e->groups[e->currentGroup] = Encoder::emptyGroup();
		if (e->selectedGroup == e->currentGroup) {
			e->selectedGroup = -1;
			e->selectedSlot = -1;
		}
		// The emptied group disappears (unless it's the only one); you land
		// on the group before it.
		int g = e->currentGroup;
		if (e->groups.size() > 1) {
			e->currentGroup = std::max(0, g - 1);
			eraseGroup(g);
		}
		backupSlots(removed);
	});
}

void EncoderWidget::setAutoFill(bool on) {
	Encoder* e = enc();
	if (!e) return;
	if (!on) {
		e->autoFill = false;   // the slots stay as they are; now they're yours
		showKeyMessage("AUTO-FILL", "OFF", "Auto-fill off - the slots stay as they are, arrange them by hand");
		return;
	}
	// Turning on replaces your slots: one undo step, so Ctrl+Z brings them back.
	undoableSlotChange(this, "auto-fill Hotkey slots", [this]() {
		Encoder* en = enc();
		en->autoFill = true;
		autoFilling = true;
		autoFillNow();
		autoFilling = false;
	});
	showKeyMessage("AUTO-FILL", "ON", string::f("Every module is in a slot: %d group(s) of 10, kept up to date",
		(int) e->groups.size()));
}

void EncoderWidget::resortAutoFill() {
	undoableSlotChange(this, "re-sort Hotkey slots", [this]() {
		autoFilling = true;
		autoFillNow();
		autoFilling = false;
	});
	showKeyMessage("SLOTS", "RE-SORTED", "Slots re-sorted in rack order (Ctrl+Z puts them back)");
}

void EncoderWidget::clearAllGroups() {
	leaveAutoFill();
	undoableSlotChange(this, "clear all Hotkey groups", [this]() {
		Encoder* e = enc();
		std::vector<int64_t> removed = e->flatten();
		e->groups.assign(1, Encoder::emptyGroup());
		e->groupColors.clear();
		e->syncColors();
		e->currentGroup = 0;
		e->selectedGroup = -1;
		e->selectedSlot = -1;
		backupSlots(removed);
	});
}


// =============================================================================
//  MIDI MAPPINGS GRID — clicks and drawing (see the struct in EncoderWidget)
// =============================================================================
void EncoderWidget::MidiGrid::onButton(const ButtonEvent& e) {
	// Keep EVERY click (Rack's OpaqueWidget only keeps left clicks). A right
	// click that got through would reach the menu's background, and Rack
	// closes the whole menu when its background is clicked.
	OpaqueWidget::onButton(e);
	e.consume(this);
	if (e.action != GLFW_PRESS || !owner) return;
	mouse = e.pos;
	int r, c;
	hovered(r, c);
	if (r < 0) return;
	std::string role = roleAt(r, c);
	if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
		// Learn, with the menu left OPEN: the box blinks until the controller
		// is moved / pressed (or Esc). The grid takes the keyboard meanwhile.
		owner->startMidiLearn(role);
		APP->event->setSelectedWidget(this);
	}
	else if (e.button == GLFW_MOUSE_BUTTON_RIGHT) {
		// The rarely needed options, opened as an ordinary submenu beside the
		// box (the same way every Rack submenu opens), so the menus stay one
		// tidy cascade. The grid shows any change straight away.
		ui::Menu* parentMenu = getAncestorOfType<ui::Menu>();
		if (!parentMenu) return;
		ui::Menu* options = new ui::Menu;
		owner->appendRoleOptions(options, role);
		options->box.pos = parentMenu->box.pos.plus(Vec(parentMenu->box.size.x, box.pos.y + boxRect(r, c).pos.y));
		parentMenu->activeEntry = nullptr;
		parentMenu->setChildMenu(options);
	}
}

void EncoderWidget::MidiGrid::onSelectKey(const SelectKeyEvent& e) {
	if (!owner) return;
	bool learning = !owner->midiLearnRole.empty();
	// A key we passed on earlier is let go: pass the release on too (even if
	// learning has finished — otherwise Rack's keyboard MIDI holds the note).
	if (e.action == GLFW_RELEASE && keysToRack.count(e.key)) {
		keyboard::release(e.key);
		keysToRack.erase(e.key);
		e.consume(this);
		return;
	}
	if (!learning) {
		OpaqueWidget::onSelectKey(e);
		return;
	}
	if (e.action == GLFW_PRESS) {
		if (e.key == GLFW_KEY_ESCAPE) owner->cancelMidiLearn();
		else {
			keyboard::press(e.key);   // -> Rack's computer-keyboard MIDI, if that's the input
			keysToRack.insert(e.key);
		}
	}
	e.consume(this);
}

void EncoderWidget::MidiGrid::draw(const DrawArgs& args) {
	NVGcontext* vg = args.vg;
	std::shared_ptr<window::Font> font = APP->window->uiFont;
	if (!font || font->handle < 0) return;
	static const char* rowNames[ROWS] = {"Knobs", "Knob press", "Buttons", "Slots", "Page / group", "More"};
	NVGcolor lit = rgbColor(settingColorMapping);   // (Colours > Mapping highlights)
	int hr, hc;
	hovered(hr, hc);

	nvgFontFaceId(vg, font->handle);
	nvgFontSize(vg, 12.f);
	for (int r = 0; r < ROWS; r++) {
		bndMenuLabel(vg, 0.f, r * ROW, LABEL_W, BND_WIDGET_HEIGHT, -1, rowNames[r]);
		for (int c = 0; c < KEY_ROW; c++) {
			std::string role = roleAt(r, c);
			if (role.empty()) continue;
			Rect b = boxRect(r, c);
			bool on = mapped(role);
			bool hot = (r == hr && c == hc);
			// Box: filled when mapped, outline when not; brighter under the mouse.
			nvgBeginPath(vg);
			nvgRoundedRect(vg, b.pos.x, b.pos.y, b.size.x, b.size.y, 3.f);
			if (on) {
				nvgFillColor(vg, hot ? nvgLerpRGBA(lit, nvgRGB(0xff, 0xff, 0xff), 0.3f) : lit);
				nvgFill(vg);
			}
			else {
				nvgFillColor(vg, hot ? nvgRGBA(0xff, 0xff, 0xff, 0x30) : nvgRGBA(0x00, 0x00, 0x00, 0x30));
				nvgFill(vg);
				nvgStrokeColor(vg, nvgRGBA(0x80, 0x80, 0x80, 0xc0));
				nvgStrokeWidth(vg, 1.f);
				nvgStroke(vg);
			}
			// Waiting for the controller: a blinking outline in the mapping colour.
			if (owner && owner->midiLearnRole == role && std::fmod(system::getTime(), 0.8) < 0.5) {
				nvgBeginPath(vg);
				nvgRoundedRect(vg, b.pos.x - 1.f, b.pos.y - 1.f, b.size.x + 2.f, b.size.y + 2.f, 3.f);
				nvgStrokeColor(vg, lit);
				nvgStrokeWidth(vg, 2.f);
				nvgStroke(vg);
			}
			nvgFillColor(vg, on ? nvgRGB(0x10, 0x10, 0x10) : bndGetTheme()->menuTheme.textColor);
			nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgText(vg, b.pos.x + b.size.x / 2.f, b.pos.y + b.size.y / 2.f, boxLabel(r, c).c_str(), nullptr);
		}
	}
	// The line under the grid: what the box under the mouse is, and what it's set to.
	std::string info = "Hover over a box to see its mapping";
	if (owner && !owner->midiLearnRole.empty()) {
		info = midiRoleLabel(owner->midiLearnRole) + ": move or press it on your controller   (Esc cancels)";
	}
	else if (hr >= 0) {
		std::string role = roleAt(hr, hc);
		auto it = settingMidiMap.find(role);
		info = midiRoleLabel(role) + ": " + midiMappingText(it != settingMidiMap.end() ? it->second : MidiMapping());
	}
	bndMenuLabel(vg, 0.f, ROWS * ROW + 2.f, box.size.x, BND_WIDGET_HEIGHT, -1, info.c_str());
	OpaqueWidget::draw(args);
}


// Text that shrinks to fit (down to 7 px) — for key names in small boxes.
static void drawFittedText(NVGcontext* vg, float x, float y, float maxW, const std::string& s, float size, int align) {
	nvgFontSize(vg, size);
	float w = nvgTextBounds(vg, 0.f, 0.f, s.c_str(), nullptr, nullptr);
	if (w > maxW && w > 0.f) nvgFontSize(vg, std::max(7.f, size * maxW / w));
	nvgTextAlign(vg, align);
	nvgText(vg, x, y, s.c_str(), nullptr);
}


// =============================================================================
//  KEYS VIEW — clicks and drawing (see the struct in EncoderWidget)
// =============================================================================
void EncoderWidget::KeysPanel::onButton(const ButtonEvent& e) {
	OpaqueWidget::onButton(e);
	e.consume(this);   // keep right clicks too, or the menu would close
	if (e.action != GLFW_PRESS || !owner) return;
	mouse = e.pos;
	int r, c, item;
	hit(r, c, item);
	bool left = (e.button == GLFW_MOUSE_BUTTON_LEFT), right = (e.button == GLFW_MOUSE_BUTTON_RIGHT);
	auto closeMenu = [&]() {
		if (ui::MenuOverlay* o = getAncestorOfType<ui::MenuOverlay>()) o->requestDelete();
	};

	// Learning keeps the menu OPEN (so several keys can be set in one go). The
	// panel takes the keyboard while it waits — see onSelectKey.
	auto learnHere = [&](int action, int index) {
		owner->startLearn(action, index);
		APP->event->setSelectedWidget(this);
	};
	(void) closeMenu;

	// ---- A knob / button / slot box ---------------------------------------
	if (r >= 0) {
		if (left) learnHere(KA_KNOB + r, c);   // (KA_KNOB, KA_BUTTON, KA_SLOT in a row)
		else if (right) {
			KeyMap d = owner->defaultKeys();
			int v = keyRow(d, r)[c];
			owner->keysUndoable("reset key", [=]() { keyRow(settingKeys, r)[c] = v; });
			std::string what = string::f("%s %d", actionName(KA_KNOB + r), c + 1);
			std::string name = (v >= 0) ? keyName(v) : "Numpad " + padLabel(c);
			owner->showKeyMessage("DEFAULT", name, what + " is back to " + name);
		}
		return;
	}
	if (item < 0) return;

	// ---- A single-key action ----------------------------------------------
	int a = listAction(item);
	if (a >= 0) {
		if (left) learnHere(a, 0);
		else if (right) {
			KeyBinding d = owner->defaultBinding(a);
			owner->keysUndoable("reset key", [=]() { *actionBinding(a) = d; });
			std::string name = (d.key >= 0) ? keyName(d.key, d.mods) : "no key";
			owner->showKeyMessage("DEFAULT", name, std::string(actionName(a)) + " is back to " + name);
		}
		return;
	}

	// ---- A modifier (slot select / assign, mouse gestures, navigation):
	//      picked from a short list --------------------------------------
	if (!isModsItem(a)) return;
	if (right) {
		owner->setModsOf(a, defaultModsOf(a));
		return;
	}
	if (!left) return;
	// Opened as an ordinary submenu beside the entry, like the MIDI grid's options.
	ui::Menu* parentMenu = getAncestorOfType<ui::Menu>();
	if (!parentMenu) return;
	ui::Menu* choices = new ui::Menu;
	std::string title;
	std::vector<int> options;
	const int SHIFT = GLFW_MOD_SHIFT, ALT = GLFW_MOD_ALT, CTRL = RACK_MOD_CTRL;
	switch (a) {
		case ACT_SELECT_MODS: title = "Hold with a slot key to SELECT it"; options = {SHIFT, ALT, CTRL, SHIFT | ALT}; break;
		case ACT_ASSIGN_MODS: title = "Hold with a slot key to ASSIGN it"; options = {SHIFT, ALT, CTRL, SHIFT | ALT}; break;
		case ACT_REMAP_MODS: title = "Hold a knob's hotkey, then this + click a control: the two swap hotkeys"; break;
		case ACT_PUSH_MODS: title = "Hold a knob's hotkey, then this + click a button: the knob's press presses it"; break;
		case ACT_CUSTOM_MODS: title = "This + click a control, then press a key: that key plays it"; break;
		case ACT_NAV_MODS: title = "Hold this + turn the dial = turn the navigation knob"; options = {ALT, SHIFT, CTRL, 0}; break;
	}
	if (options.empty()) options = {ALT, SHIFT, CTRL, SHIFT | ALT, CTRL | ALT, CTRL | SHIFT};   // mouse gestures
	choices->addChild(createMenuLabel(title));
	EncoderWidget* w = owner;
	bool mouseItem = (a == ACT_REMAP_MODS || a == ACT_PUSH_MODS || a == ACT_CUSTOM_MODS);
	for (int m : options) {
		// Warnings for combos Rack itself uses.
		std::string note;
		if (m == CTRL && mouseItem) note = "Rack uses Ctrl + drag for fine turning";
		else if (m == CTRL && a == ACT_NAV_MODS) note = "not with a dial that sends Ctrl, like the K70";
		else if (m == CTRL) note = "Rack uses Ctrl + 0 / - / = to zoom";
		else if (m == SHIFT && a == ACT_NAV_MODS) note = "takes over Rack's Shift + scroll";
		else if (m == 0) note = "dial never turns it";
		choices->addChild(stayCheck(modsName(m), note,
			[=]() { return modsOf(a) == m; },
			[=]() { w->setModsOf(a, m); }));
	}
	choices->box.pos = parentMenu->box.pos.plus(Vec(parentMenu->box.size.x, box.pos.y + listRect(item).pos.y));
	parentMenu->activeEntry = nullptr;
	parentMenu->setChildMenu(choices);
}

// =============================================================================
//  GAMEPAD MAPPING VIEW — drawing (see PadPanel in EncoderWidget)
//  A simple, generic controller (our own drawing — no brand artwork), each
//  control labelled with what it does right now; then the rows.
// =============================================================================
void EncoderWidget::PadPanel::draw(const DrawArgs& args) {
	NVGcontext* vg = args.vg;
	std::shared_ptr<window::Font> font = APP->window->uiFont;
	if (!font || font->handle < 0 || !owner) return;
	nvgFontFaceId(vg, font->handle);
	NVGcolor text = bndGetTheme()->menuTheme.textColor;
	NVGcolor dim = nvgTransRGBAf(text, 0.6f);
	NVGcolor lit = rgbColor(settingColorMapping);
	NVGcolor body = nvgRGBA(0x80, 0x80, 0x80, 0x30), edge = nvgRGBA(0x90, 0x90, 0x90, 0xc0);
	int hover = hitRow();
	int hoverAction = (hover >= 0) ? listAction(hover) : -1;
	bool blink = std::fmod(system::getTime(), 0.8) < 0.5;
	bool ps = owner->padIsPlayStation();

	// ---- The controller ------------------------------------------------------
	float cx = W / 2.f;
	// Body + grips.
	nvgBeginPath(vg);
	nvgRoundedRect(vg, cx - 150.f, 34.f, 300.f, 86.f, 40.f);
	nvgCircle(vg, cx - 112.f, 112.f, 38.f);
	nvgCircle(vg, cx + 112.f, 112.f, 38.f);
	nvgFillColor(vg, body);
	nvgFill(vg);
	// One control: a shape, lit if it's the one under discussion (hovered
	// row / waiting to learn), and its action's short name.
	auto label = [&](float x, float y, const std::string& s, int align) {
		nvgFillColor(vg, text);
		drawFittedText(vg, x, y, 70.f, s, 10.f, align);
	};
	auto mark = [&](int control) -> NVGcolor {
		int a = padActionFor(control);
		bool on = (a >= 0 && a == hoverAction) || (a >= 0 && owner->padLearn == a && blink);
		return on ? lit : edge;
	};
	auto roundBox = [&](float x, float y, float w, float h, int control) {
		nvgBeginPath(vg);
		nvgRoundedRect(vg, x, y, w, h, 4.f);
		nvgStrokeColor(vg, mark(control));
		nvgStrokeWidth(vg, 1.5f);
		nvgStroke(vg);
	};
	auto circle = [&](float x, float y, float r, int control) {
		nvgBeginPath(vg);
		nvgCircle(vg, x, y, r);
		nvgStrokeColor(vg, mark(control));
		nvgStrokeWidth(vg, 1.5f);
		nvgStroke(vg);
	};
	auto actionOn = [&](int control) { return padActionShort(padActionFor(control)); };
	// Triggers and bumpers (along the top), labels outside.
	roundBox(cx - 140.f, 4.f, 56.f, 12.f, PAD_AXIS + 4);   // LT
	roundBox(cx + 84.f, 4.f, 56.f, 12.f, PAD_AXIS + 5);    // RT
	roundBox(cx - 140.f, 20.f, 56.f, 9.f, 4);              // LB
	roundBox(cx + 84.f, 20.f, 56.f, 9.f, 5);               // RB
	label(cx - 146.f, 10.f, "LT  " + actionOn(PAD_AXIS + 4), NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
	label(cx + 146.f, 10.f, actionOn(PAD_AXIS + 5) + "  RT", NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	label(cx - 146.f, 25.f, "LB  " + actionOn(4), NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
	label(cx + 146.f, 25.f, actionOn(5) + "  RB", NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	// Left stick (and its click), D-pad, right stick.
	circle(cx - 95.f, 62.f, 14.f, PAD_AXIS + 1);
	label(cx - 95.f, 62.f, actionOn(PAD_AXIS + 1), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	label(cx - 95.f, 84.f, "click: " + actionOn(9), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	circle(cx + 45.f, 100.f, 14.f, PAD_AXIS + 3);
	label(cx + 45.f, 100.f, actionOn(PAD_AXIS + 3), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	label(cx + 45.f, 122.f, "click: " + actionOn(10), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	float dx = cx - 45.f, dy = 100.f;
	roundBox(dx - 6.f, dy - 20.f, 12.f, 14.f, 11);   // up
	roundBox(dx - 6.f, dy + 6.f, 12.f, 14.f, 13);    // down
	roundBox(dx - 20.f, dy - 6.f, 14.f, 12.f, 14);   // left
	roundBox(dx + 6.f, dy - 6.f, 14.f, 12.f, 12);    // right
	label(dx, dy - 26.f, actionOn(11), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	label(dx, dy + 27.f, actionOn(13), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	label(dx - 24.f, dy, actionOn(14), NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
	label(dx + 24.f, dy, actionOn(12), NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	// Face buttons (A bottom, B right, X left, Y top).
	static const char* xbox[4] = {"A", "B", "X", "Y"};
	static const char* psym[4] = {"\xe2\x9c\x95", "\xe2\x97\x8b", "\xe2\x96\xa1", "\xe2\x96\xb3"};
	float fx = cx + 95.f, fy = 62.f;
	const Vec facePos[4] = {Vec(fx, fy + 16.f), Vec(fx + 16.f, fy), Vec(fx - 16.f, fy), Vec(fx, fy - 16.f)};
	for (int b = 0; b < 4; b++) {
		circle(facePos[b].x, facePos[b].y, 8.f, b);
		nvgFillColor(vg, dim);
		drawFittedText(vg, facePos[b].x, facePos[b].y, 14.f, ps ? psym[b] : xbox[b], 9.f, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		// what it plays, just outside
		Vec out = facePos[b].plus(facePos[b].minus(Vec(fx, fy)).mult(1.0f));
		label(out.x, out.y, actionOn(b), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	}
	// View / Menu in the middle.
	roundBox(cx - 26.f, 54.f, 18.f, 9.f, 6);
	roundBox(cx + 8.f, 54.f, 18.f, 9.f, 7);
	label(cx - 17.f, 72.f, actionOn(6), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	label(cx + 17.f, 72.f, actionOn(7), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

	// ---- The rows (same look as Key mapping) ---------------------------------
	static const char* heads[2] = {"NAVIGATE", "PLAY"};
	for (int col = 0; col < 2; col++) {
		nvgFillColor(vg, dim);
		drawFittedText(vg, col * COL_W + 6.f, listTop() + HEAD_H / 2.f, COL_W, heads[col], 11.f, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	}
	for (int i = 0; i < 2 * ROWS; i++) {
		int a = listAction(i);
		if (a < 0) continue;
		Rect rr = listRect(i);
		nvgBeginPath(vg);
		nvgMoveTo(vg, rr.pos.x + 4.f, rr.pos.y + 0.5f);
		nvgLineTo(vg, rr.pos.x + rr.size.x, rr.pos.y + 0.5f);
		nvgStrokeColor(vg, nvgTransRGBAf(text, 0.12f));
		nvgStrokeWidth(vg, 1.f);
		nvgStroke(vg);
		if (i == hover) {
			nvgBeginPath(vg);
			nvgRoundedRect(vg, rr.pos.x, rr.pos.y, rr.size.x, rr.size.y, 3.f);
			nvgFillColor(vg, nvgRGBA(0xff, 0xff, 0xff, 0x20));
			nvgFill(vg);
		}
		// Name (normal) + its context in quotes (dimmer), like Key mapping.
		std::string full = padActionName(a), name = full, desc;
		size_t q = full.find(" \"");
		if (q != std::string::npos) { name = full.substr(0, q); desc = full.substr(q + 1); }
		float cy = rr.pos.y + rr.size.y / 2.f, room = rr.size.x - KEY_W - 12.f;
		nvgFillColor(vg, text);
		drawFittedText(vg, rr.pos.x + 6.f, cy, room, name, 12.f, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		float nameW = nvgTextBounds(vg, 0.f, 0.f, name.c_str(), nullptr, nullptr);
		if (!desc.empty()) {
			nvgFillColor(vg, nvgTransRGBAf(text, 0.5f));
			drawFittedText(vg, rr.pos.x + 12.f + nameW, cy, std::max(10.f, room - nameW - 6.f), desc, 11.f,
				NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		}
		// Its control, in a box: lit when changed from the default; blinking
		// "press on the gamepad" while it waits.
		Rect kb = keyBoxRect(i);
		bool changed = settingPadMap.control[a] != PadMap().control[a];
		bool waiting = owner->padLearn == a;
		nvgBeginPath(vg);
		nvgRoundedRect(vg, kb.pos.x, kb.pos.y, kb.size.x, kb.size.y, 3.f);
		if (changed && !waiting) { nvgFillColor(vg, lit); nvgFill(vg); }
		else {
			nvgFillColor(vg, nvgRGBA(0x00, 0x00, 0x00, 0x30));
			nvgFill(vg);
			nvgStrokeColor(vg, (waiting && blink) ? lit : nvgRGBA(0x80, 0x80, 0x80, 0xc0));
			nvgStrokeWidth(vg, (waiting && blink) ? 2.f : 1.f);
			nvgStroke(vg);
		}
		std::string ctl = waiting ? "press on gamepad" : padControlName(settingPadMap.control[a]);
		nvgFillColor(vg, (changed && !waiting) ? nvgRGB(0x10, 0x10, 0x10) : text);
		drawFittedText(vg, kb.pos.x + kb.size.x / 2.f, kb.pos.y + kb.size.y / 2.f, kb.size.x - 4.f, ctl, 11.f,
			NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	}

	// ---- Hover line ----------------------------------------------------------
	std::string info = "Hover over a row: its control lights up on the drawing.";
	if (hoverAction >= 0)
		info = std::string(padActionName(hoverAction)) + ": " + padControlName(settingPadMap.control[hoverAction]) +
			"   (click = change, right-click = default)";
	bndMenuLabel(vg, 0.f, infoTop(), box.size.x, BND_WIDGET_HEIGHT, -1, info.c_str());
	OpaqueWidget::draw(args);
}

void EncoderWidget::KeysPanel::draw(const DrawArgs& args) {
	NVGcontext* vg = args.vg;
	std::shared_ptr<window::Font> font = APP->window->uiFont;
	if (!font || font->handle < 0 || !owner) return;
	nvgFontFaceId(vg, font->handle);
	const KeyMap& k = settingKeys;
	KeyMap d = owner->defaultKeys();
	NVGcolor changed = rgbColor(settingColorMapping);   // "changed from the default" (Colours > Mapping highlights)
	NVGcolor text = bndGetTheme()->menuTheme.textColor;
	NVGcolor dim = nvgTransRGBAf(text, 0.6f);
	int hr, hc, hitem;
	hit(hr, hc, hitem);
	bool blinkOn = std::fmod(system::getTime(), 0.8) < 0.5;   // for the box / row waiting for a key

	// ---- The grid: knobs / buttons / slots --------------------------------
	static const char* rowNames[3] = {"Knobs", "Buttons", "Slots"};
	static const char* padRowNames[3] = {"KNOBS mode", "BUTTONS mode", "SLOTS mode"};
	if (pad) {
		// Numpad layout: the same ten digits play whichever the mode says.
		nvgFillColor(vg, text);
		drawFittedText(vg, 6.f, ROW / 2.f, box.size.x - 12.f,
			"Numpad 1-0 play knobs, buttons or slots, depending on the mode.", 12.f, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		drawFittedText(vg, 6.f, ROW * 1.5f, box.size.x - 12.f,
			"Click a box to give it an extra key of its own.", 12.f, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	}
	for (int r = 0; r < gridRows(); r++) {
		bndMenuLabel(vg, 0.f, gridTop() + r * ROW, LABEL_W, BND_WIDGET_HEIGHT, -1, pad ? padRowNames[r] : rowNames[r]);
		const int* row = keyRow(const_cast<KeyMap&>(k), r);
		const int* defRow = keyRow(d, r);
		for (int c = 0; c < KEY_ROW; c++) {
			Rect b = boxRect(r, c);
			bool isChanged = row[c] != defRow[c];
			bool hot = (r == hr && c == hc);
			nvgBeginPath(vg);
			nvgRoundedRect(vg, b.pos.x, b.pos.y, b.size.x, b.size.y, 3.f);
			if (isChanged) {
				nvgFillColor(vg, hot ? nvgLerpRGBA(changed, nvgRGB(0xff, 0xff, 0xff), 0.3f) : changed);
				nvgFill(vg);
			}
			else {
				nvgFillColor(vg, hot ? nvgRGBA(0xff, 0xff, 0xff, 0x30) : nvgRGBA(0x00, 0x00, 0x00, 0x30));
				nvgFill(vg);
				nvgStrokeColor(vg, nvgRGBA(0x80, 0x80, 0x80, 0xc0));
				nvgStrokeWidth(vg, 1.f);
				nvgStroke(vg);
			}
			// Waiting for its new key: a blinking outline in the mapping colour.
			if (waitingFor(KA_KNOB + r, c) && blinkOn) {
				nvgBeginPath(vg);
				nvgRoundedRect(vg, b.pos.x - 1.f, b.pos.y - 1.f, b.size.x + 2.f, b.size.y + 2.f, 3.f);
				nvgStrokeColor(vg, changed);
				nvgStrokeWidth(vg, 2.f);
				nvgStroke(vg);
			}
			// A key of -1 means "the numpad digit" (Numpad layout): N1 ... N0.
			std::string label = (row[c] >= 0) ? keyName(row[c]) : "N" + padLabel(c);
			nvgFillColor(vg, isChanged ? nvgRGB(0x10, 0x10, 0x10) : text);
			drawFittedText(vg, b.pos.x + b.size.x / 2.f, b.pos.y + b.size.y / 2.f, b.size.x - 2.f, label, 12.f,
				NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		}
	}
	// ---- The action list, two columns -------------------------------------
	// Headings, then rows: name (normal), its context (dimmer, smaller), and
	// the key in a box like the grid's — all keys in one straight column.
	static const char* heads[2] = {"PLAYING", "GROUPS & SLOTS"};
	for (int col = 0; col < 2; col++) {
		nvgFillColor(vg, dim);
		drawFittedText(vg, col * COL_W + 6.f, listTop() + HEAD_H / 2.f, COL_W, heads[col], 11.f,
			NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	}
	for (int i = 0; i < listCount(); i++) {
		int a = listAction(i);
		if (a == ACT_NONE) continue;   // (an empty spot at the end of a column)
		Rect rr = listRect(i);
		if (a == ACT_SETUP_HEAD) {
			// A small heading inside the right column, like PLAYING above.
			nvgFillColor(vg, dim);
			drawFittedText(vg, rr.pos.x + 6.f, rr.pos.y + rr.size.y / 2.f + 2.f, rr.size.x, "SET UP", 11.f,
				NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
			continue;
		}
		// A faint line above each row, so the eye can follow across.
		nvgBeginPath(vg);
		nvgMoveTo(vg, rr.pos.x + 4.f, rr.pos.y + 0.5f);
		nvgLineTo(vg, rr.pos.x + rr.size.x, rr.pos.y + 0.5f);
		nvgStrokeColor(vg, nvgTransRGBAf(text, 0.12f));
		nvgStrokeWidth(vg, 1.f);
		nvgStroke(vg);
		if (i == hitem) {
			nvgBeginPath(vg);
			nvgRoundedRect(vg, rr.pos.x, rr.pos.y, rr.size.x, rr.size.y, 3.f);
			nvgFillColor(vg, nvgRGBA(0xff, 0xff, 0xff, 0x20));
			nvgFill(vg);
		}
		std::string keyText;
		bool isChanged = false, unused = false;
		if (a >= 0) {
			const KeyBinding* b = actionBinding(a);
			keyText = (b->key >= 0) ? keyName(b->key, b->mods) : "not set";
			isChanged = !(*b == owner->defaultBinding(a));
		}
		else {
			int m = modsOf(a);   // a modifier row: slot, mouse or navigation
			keyText = modsName(m);
			isChanged = m != defaultModsOf(a);
		}
		float cy = rr.pos.y + rr.size.y / 2.f;
		float textRoom = rr.size.x - KEY_W - 12.f;
		// Name, then its context just after it, dimmer and smaller.
		std::string label = listLabel(a);
		nvgFillColor(vg, unused ? dim : text);
		drawFittedText(vg, rr.pos.x + 6.f, cy, textRoom, label, 12.f, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		float labelW = nvgTextBounds(vg, 0.f, 0.f, label.c_str(), nullptr, nullptr);
		std::string desc = listDesc(a);
		if (!desc.empty()) {
			nvgFillColor(vg, nvgTransRGBAf(text, 0.5f));
			drawFittedText(vg, rr.pos.x + 6.f + labelW + 6.f, cy, std::max(10.f, textRoom - labelW - 6.f), desc, 11.f,
				NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		}
		// The key, in a box: lit in the mapping colour when changed from the default.
		Rect kb = keyBoxRect(i);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, kb.pos.x, kb.pos.y, kb.size.x, kb.size.y, 3.f);
		if (isChanged && !unused) {
			nvgFillColor(vg, changed);
			nvgFill(vg);
		}
		else {
			nvgFillColor(vg, nvgRGBA(0x00, 0x00, 0x00, 0x30));
			nvgFill(vg);
			nvgStrokeColor(vg, nvgRGBA(0x80, 0x80, 0x80, unused ? 0x60 : 0xc0));
			nvgStrokeWidth(vg, 1.f);
			nvgStroke(vg);
		}
		// Waiting for its new key: "press a key", blinking in the mapping colour.
		bool waiting = (a >= 0) && waitingFor(a, 0);
		if (waiting) {
			if (blinkOn) {
				nvgBeginPath(vg);
				nvgRoundedRect(vg, kb.pos.x - 1.f, kb.pos.y - 1.f, kb.size.x + 2.f, kb.size.y + 2.f, 3.f);
				nvgStrokeColor(vg, changed);
				nvgStrokeWidth(vg, 2.f);
				nvgStroke(vg);
			}
			keyText = "press a key";
		}
		nvgFillColor(vg, unused ? dim : (isChanged && !waiting ? nvgRGB(0x10, 0x10, 0x10) : text));
		drawFittedText(vg, kb.pos.x + kb.size.x / 2.f, kb.pos.y + kb.size.y / 2.f, kb.size.x - 4.f, keyText, 12.f,
			NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	}

	// ---- The line at the bottom: what's under the mouse -------------------
	std::string info = "Hover over a key for details. Lit = changed from the layout's default.";
	if (hr >= 0) {
		const int* row = keyRow(const_cast<KeyMap&>(k), hr);
		info = string::f("%s %d: ", actionName(KA_KNOB + hr), hc + 1) +
			owner->rowKeyLabel(row[hc], keyRow(d, hr)[hc], hc);
		// Numpad layout: an extra key works alongside the numpad digit.
		if (pad && row[hc] >= 0) info += " + Numpad " + padLabel(hc) + " in " + padRowNames[hr];
		info += "   (click = change, right-click = default)";
	}
	else if (hitem >= 0) {
		int a = listAction(hitem);
		if (a >= 0) {
			info = listName(a) + ": " + owner->keyLabel(*actionBinding(a), owner->defaultBinding(a)) +
				"   (click = change, right-click = default)";
		}
		else info = modsSentence(a, modsName(modsOf(a))) + "   (click = choose, right-click = default)";
	}
	bndMenuLabel(vg, 0.f, infoTop(), box.size.x, BND_WIDGET_HEIGHT, -1, info.c_str());
	OpaqueWidget::draw(args);
}


// =============================================================================
//  OVERLAY — behaviour
// =============================================================================
void Overlay::step() {
	// Always cover the whole Rack window, even after it's resized.
	if (parent) box = Rect(Vec(0, 0), parent->box.size);
	Widget::step();
}

// ---- Outline around the selected module ------------------------------------
void Overlay::draw(const DrawArgs& args) {
	if (!owner) return;
	Encoder* e = owner->enc();
	if (!e) return;
	// Don't draw over the module browser when it's open.
	if (APP->scene->browser && APP->scene->browser->isVisible()) return;

	std::shared_ptr<window::Font> groupFont = APP->window->uiFont;

	// ---- Outlines and badges, clipped to the RACK AREA ---------------------
	// so they never draw over Rack's menu bar (File, Edit ...) or outside the
	// rack when you scroll.
	nvgSave(args.vg);
	if (APP->scene->rackScroll) {
		Rect r = APP->scene->rackScroll->box;
		nvgScissor(args.vg, r.pos.x, r.pos.y, r.size.x, r.size.y);
	}
	drawHighlights(args);
	nvgRestore(args.vg);

	// ---- Banner: the full key-learning message, top-centre of the window ---
	// (Drawn last, on top of everything — it's meant to be seen.)
	std::string banner = owner->currentBanner(system::getTime());
	if (!banner.empty() && groupFont && groupFont->handle >= 0) {
		nvgFontFaceId(args.vg, groupFont->handle);
		nvgFontSize(args.vg, 14.f);
		float bounds[4];
		nvgTextBounds(args.vg, 0, 0, banner.c_str(), NULL, bounds);
		float w = bounds[2] - bounds[0] + 24.f, h = 26.f;
		float x = box.size.x / 2.f - w / 2.f, y = 40.f;   // just under Rack's menu bar
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, x, y, w, h, 5.f);
		nvgFillColor(args.vg, nvgRGBA(0x10, 0x10, 0x10, 0xf0));
		nvgFill(args.vg);
		nvgStrokeColor(args.vg, SLOT_COLOR);
		nvgStrokeWidth(args.vg, 1.5f);
		nvgStroke(args.vg);
		nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgFillColor(args.vg, nvgRGB(0xf0, 0xf0, 0xf0));
		nvgText(args.vg, box.size.x / 2.f, y + h / 2.f, banner.c_str(), NULL);
	}
}

void Overlay::drawHighlights(const DrawArgs& args) {
	Encoder* e = owner->enc();
	std::shared_ptr<window::Font> groupFont = APP->window->uiFont;

	// Drawn in layers, bottom to top:
	//   1. outlines of the CURRENT group's modules (teal)
	//   2. the outline of the module you're playing (amber, thicker)
	//   3. the slot tags — on top, so an outline never covers them
	//   4. the played module's control badges and value box
	// Every outline is drawn just INSIDE the module's edge, so thick and thin
	// outlines share the same outer edge and nothing spills past it.
	bool showGroup = settingShowGroupModules && groupFont && groupFont->handle >= 0;
	ModuleWidget* mw = (e->selectedSlot >= 0) ? owner->selectedModule() : nullptr;

	auto outline = [&](ModuleWidget* m, NVGcolor color, float width) {
		Vec tl = m->getRelativeOffset(Vec(0, 0), parent);
		Vec br = m->getRelativeOffset(m->box.size, parent);
		float in = width / 2.f;   // keep the whole stroke inside the module
		nvgBeginPath(args.vg);
		nvgRect(args.vg, tl.x + in, tl.y + in, br.x - tl.x - width, br.y - tl.y - width);
		nvgStrokeColor(args.vg, color);
		nvgStrokeWidth(args.vg, width);
		nvgStroke(args.vg);
	};

	// ---- 1. The current group's modules (menu: "Show current group") ------
	if (showGroup) {
		for (int s = 0; s < NUM_SLOTS; s++) {
			ModuleWidget* gm = owner->slotModule(s);
			if (gm && gm != mw) outline(gm, hlColor(e->groupColor(e->currentGroup), 0.8f), 2.f);
		}
	}

	// ---- 2. The module being played (Highlights: "Played module outline") --
	if (mw && settingShowPlayedOutline) outline(mw, hlColor(settingColorPlayed), 3.f);

	// ---- 3. Slot tags: e.g. "3" (its slot key), centred ON the module's top
	//         edge (half outside), so they never cover controls or badges. ---
	if (showGroup) {
		for (int s = 0; s < NUM_SLOTS; s++) {
			ModuleWidget* gm = owner->slotModule(s);
			if (!gm) continue;
			bool playingThis = (gm == mw);
			NVGcolor color = playingThis ? hlColor(settingColorPlayed) : hlColor(e->groupColor(e->currentGroup));
			Vec tl = gm->getRelativeOffset(Vec(0, 0), parent);
			Vec br = gm->getRelativeOffset(gm->box.size, parent);
			std::string tag = settingKeys.slots[s] >= 0 ? keyName(settingKeys.slots[s]) : padLabel(s);
			nvgFontFaceId(args.vg, groupFont->handle);
			nvgFontSize(args.vg, 11.f);
			float bounds[4];
			nvgTextBounds(args.vg, 0, 0, tag.c_str(), NULL, bounds);
			float w = std::max(16.f, bounds[2] - bounds[0] + 8.f), h = 14.f;
			float x = (tl.x + br.x) / 2.f - w / 2.f, y = tl.y - h / 2.f;
			nvgBeginPath(args.vg);
			nvgRoundedRect(args.vg, x, y, w, h, 3.f);
			nvgFillColor(args.vg, playingThis ? color : nvgRGBAf(0.06f, 0.06f, 0.06f, 0.92f * highlightAlpha()));
			nvgFill(args.vg);
			nvgStrokeColor(args.vg, color);
			nvgStrokeWidth(args.vg, 1.f);
			nvgStroke(args.vg);
			nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFillColor(args.vg, playingThis ? nvgRGB(0x10, 0x10, 0x10) : color);
			nvgText(args.vg, x + w / 2.f, y + h / 2.f, tag.c_str(), NULL);
		}
	}

	if (!mw) return;

	std::shared_ptr<window::Font> font = APP->window->uiFont;
	if (!font || font->handle < 0) return;

	// ---- Badges: numbers on knobs, letters on buttons -----------------------
	// Drawn at each control's top-left corner: round for knobs, square for
	// buttons. The one being held / just tapped is filled in.
	// EVERY control (up to 4 pages' worth) gets a badge, so nothing is
	// hidden. The current page's badges are bright — those are what the keys
	// play right now; other pages' badges are dimmed.
	//   Knobs show their real number 1-40; the key is always the LAST digit
	//   (knob 15 = hold 5 on page 2; knob 20 = hold 0 on page 2).
	//   Buttons show their letter, plus the page number from page 2 on
	//   ("W2" = W on page 2).
	//   (If you've changed the knob keys away from the number row, knob
	//    badges show the key's name + page, like buttons do.)
	refreshKeyLabels();
	bool numberBadges = settingKeys.knobsShowNumbers();
	// Numpad modes: the kind of control the numpad ISN'T playing right now
	// is dimmed, so the bright badges are always what the digits do.
	bool knobsLive = !settingKeys.padModes || padMode == PAD_KNOBS;
	bool buttonsLive = !settingKeys.padModes || padMode == PAD_BUTTONS;
	// Gamepad in use: the two knobs the sticks play show J1 / J2, the four
	// buttons the pad plays show its button names (A B X Y, or the
	// PlayStation symbols), and everything else dims until D-pad left /
	// right moves onto it — like pages.
	bool pad = owner->padInUse();
	int padKnob0 = owner->padBank * 2, padButton0 = owner->padBank * 4;
	const int limit = PER_PAGE * MAX_PAGES;
	std::vector<ParamWidget*>& knobs = owner->currentKnobs();
	NVGcolor knobColor = hlColor(settingColorKnobBadges);
	NVGcolor buttonColor = hlColor(settingColorButtonBadges);
	// (Highlights: "Knob badges" — off draws none, but the knobs still play.)
	for (int i = 0; i < (settingShowKnobBadges ? std::min((int) knobs.size(), limit) : 0); i++) {
		int pageOf = i / PER_PAGE, key = i % PER_PAGE;
		bool onThisPage = (pageOf == owner->page);
		Vec at = knobs[i]->getRelativeOffset(Vec(0, 0), parent);
		std::string label = numberBadges ? string::f("%d", i + 1) : knobLabels[key];
		if (!numberBadges && pageOf > 0) label += string::f("%d", pageOf + 1);
		// Custom mode: a knob with its own key shows THAT key, always bright
		// (its key works on any page).
		bool custom = i < (int) owner->knobCustomLabel.size() && !owner->knobCustomLabel[i].empty();
		if (custom) label = owner->knobCustomLabel[i];
		bool dim = custom ? false : (!onThisPage || !knobsLive);
		if (pad) {
			bool isJ = onThisPage && (key == padKnob0 || key == padKnob0 + 1);
			if (isJ) label = (key == padKnob0) ? "J1" : "J2";
			dim = !isJ;
		}
		drawBadge(args.vg, font, at, label, knobColor,
			false, owner->knobHeld(i), dim);

		// A small dot under the badge = this knob has a push (found
		// automatically, or set by you). HOLLOW = it has one; it goes SOLID
		// while you push it, so you can see the push happen.
		if (ParamWidget* target = owner->pushTargetFor(i)) {
			NVGcolor c = nvgTransRGBAf(knobColor, onThisPage ? 1.f : 0.35f);
			nvgBeginPath(args.vg);
			if (owner->pushShowing(target)) {
				nvgCircle(args.vg, at.x, at.y + 10.f, 2.5f);
				nvgFillColor(args.vg, c);
				nvgFill(args.vg);
			}
			else {
				nvgCircle(args.vg, at.x, at.y + 10.f, 2.f);
				nvgStrokeColor(args.vg, c);
				nvgStrokeWidth(args.vg, 1.2f);
				nvgStroke(args.vg);
			}
		}
	}

	double now = system::getTime();
	std::vector<ParamWidget*>& buttons = owner->currentButtons();
	int buttonPageNow = owner->currentButtonPage();   // 10 or fewer buttons: always 0
	// (Highlights: "Button badges" — off draws none, but the buttons still play.)
	for (int i = 0; i < (settingShowButtonBadges ? std::min((int) buttons.size(), limit) : 0); i++) {
		int pageOf = i / PER_PAGE, key = i % PER_PAGE;
		bool onThisPage = (pageOf == buttonPageNow);
		bool active = onThisPage && (owner->heldButtons[key].moduleId >= 0 ||
			(i == owner->lastButton && now - owner->lastButtonTime < 0.15));
		std::string label = buttonLabels[key];
		if (pageOf > 0) label += string::f("%d", pageOf + 1);
		bool custom = i < (int) owner->buttonCustomLabel.size() && !owner->buttonCustomLabel[i].empty();
		if (custom) label = owner->buttonCustomLabel[i];
		bool dim = custom ? false : (!onThisPage || !buttonsLive);
		if (pad) {
			int n = key - padButton0;   // 0-3 = played by the gamepad's four buttons
			bool isPad = onThisPage && n >= 0 && n < 4;
			if (isPad) label = owner->padButtonLabel(n);
			dim = !isPad;
		}
		drawBadge(args.vg, font, buttons[i]->getRelativeOffset(Vec(0, 0), parent),
			label, buttonColor, true, active, dim);
	}

	// ---- Value readout (menu: "Show value while turning") -------------------
	// Knob: shown while its number is held. Button: shown briefly after a tap.
	if (!settingShowValue) return;
	int heldIndex = owner->heldIndex();
	if (heldIndex >= 0 && heldIndex < (int) knobs.size())
		drawValueBox(args.vg, font, knobs[heldIndex]);
	// A knob moved by a MIDI controller: shown briefly, the same way.
	else if (owner->lastMidiKnob >= 0 && owner->lastMidiKnob < (int) knobs.size() &&
			now - owner->lastMidiKnobTime < BUTTON_VALUE_SHOW_TIME)
		drawValueBox(args.vg, font, knobs[owner->lastMidiKnob]);
	else if (owner->lastButton >= 0 && owner->lastButton < (int) buttons.size() &&
			now - owner->lastButtonTime < BUTTON_VALUE_SHOW_TIME)
		drawValueBox(args.vg, font, buttons[owner->lastButton]);
}

// A small labelled badge: round (knobs) or square-cornered (buttons). It
// widens into a pill for two-character labels like "15" or "W2".
// active = filled in (being played). dim = on another page.
void Overlay::drawBadge(NVGcontext* vg, std::shared_ptr<window::Font> font, Vec at,
		const std::string& label, NVGcolor base, bool square, bool active, bool dim) {
	nvgFontFaceId(vg, font->handle);
	nvgFontSize(vg, 11.f);
	float bounds[4];
	nvgTextBounds(vg, 0, 0, label.c_str(), NULL, bounds);
	const float r = 7.f;
	float halfW = std::max(r, (bounds[2] - bounds[0]) / 2.f + 3.f);

	float alpha = dim ? 0.35f : 1.f;
	NVGcolor color = nvgTransRGBAf(base, alpha);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, at.x - halfW, at.y - r, 2 * halfW, 2 * r, square ? 2.f : r);
	nvgFillColor(vg, active ? base : nvgRGBAf(0.06f, 0.06f, 0.06f, 0.88f * alpha * highlightAlpha()));
	nvgFill(vg);
	nvgStrokeColor(vg, color);
	nvgStrokeWidth(vg, 1.f);
	nvgStroke(vg);

	nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFillColor(vg, active ? nvgRGB(0x10, 0x10, 0x10) : color);
	nvgText(vg, at.x, at.y, label.c_str(), NULL);
}

// The same text Rack shows when you hover a control, e.g. "Frequency: 440 Hz",
// drawn just below it in a small dark box.
void Overlay::drawValueBox(NVGcontext* vg, std::shared_ptr<window::Font> font, ParamWidget* pw) {
	ParamQuantity* pq = pw->getParamQuantity();
	if (!pq) return;
	std::string text = pq->getString();
	Vec below = pw->getRelativeOffset(Vec(pw->box.size.x / 2.f, pw->box.size.y), parent);

	nvgFontFaceId(vg, font->handle);
	nvgFontSize(vg, 13.f);
	float bounds[4];
	nvgTextBounds(vg, 0, 0, text.c_str(), NULL, bounds);
	float w = bounds[2] - bounds[0] + 10.f;
	float h = 18.f;
	float x = below.x - w / 2.f;
	float y = below.y + 4.f;

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, w, h, 3.f);
	nvgFillColor(vg, nvgRGBA(0x10, 0x10, 0x10, 0xe8));
	nvgFill(vg);
	nvgStrokeColor(vg, hlColor(settingColorPlayed));
	nvgStrokeWidth(vg, 1.f);
	nvgStroke(vg);

	nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFillColor(vg, nvgRGB(0xf0, 0xf0, 0xf0));
	nvgText(vg, below.x, y + h / 2.f, text.c_str(), NULL);
}

// ---- Scroll ------------------------------------------------------------------
void Overlay::onHoverScroll(const HoverScrollEvent& e) {
	if (!owner) return;
	owner->scrollSeen(e.scrollDelta);

	// Dial setup wizard: every scroll is a measurement, not a turn — and
	// it's kept from Rack so the rack doesn't scroll while setting up.
	if (owner->wizStep >= 0) {
		owner->wizScroll(e.scrollDelta);
		e.consume(this);
		return;
	}

	// Locked: the Encoder leaves the scroll alone entirely.
	if (owner->isLocked()) return;

	// Nav modifier held (default Shift), no number held: the dial turns the
	// panel's navigation encoder, one step per dial click.
	int heldMods = APP->window->getMods() & RACK_MOD_MASK;
	if (settingNavModifier && heldMods == settingNavModifier && !owner->playing()) {
		float d = (e.scrollDelta.y != 0.f) ? e.scrollDelta.y : e.scrollDelta.x;
		if (settingReverseDial) d = -d;
		owner->navScrollAccum += d / settingScrollUnit;
		while (owner->navScrollAccum >= 1.f) { owner->navTurn(+1); owner->navScrollAccum -= 1.f; }
		while (owner->navScrollAccum <= -1.f) { owner->navTurn(-1); owner->navScrollAccum += 1.f; }
		e.consume(this);
		return;
	}

	// Holding a number with a module selected: this scroll turns the knob,
	// and Rack doesn't get it. Otherwise Rack handles the scroll as normal.
	if (owner->playing()) {
		float d = (e.scrollDelta.y != 0.f) ? e.scrollDelta.y : e.scrollDelta.x;
		// Some dials/wheels feel backwards; the menu setting flips them.
		// (Arrow keys are never flipped: Up is always up.)
		if (settingReverseDial) d = -d;
		float clicks = d / settingScrollUnit;   // one click of YOUR dial = 1
		owner->turnHeldKnob(clicks, owner->accelerationFor(clicks));
		e.consume(this);
	}
}

// ---- Keys --------------------------------------------------------------------
void Overlay::onHoverKey(const HoverKeyEvent& e) {
	if (!owner) return;
	// A key pressed: the badges go back to keyboard labels (from gamepad ones).
	if (e.action == GLFW_PRESS) owner->keysLastUsed = system::getTime();

	int mods = e.mods & RACK_MOD_MASK;

	// ---- Dial setup wizard: keys are what the dial sends -------------------
	// Shift / Ctrl / Alt on their own are ignored (a dial may send them as
	// part of a combo, e.g. the K70's Ctrl+End). Esc cancels. All kept from
	// Rack while the wizard runs.
	if (owner->wizStep >= 0) {
		bool modifierOnly = (e.key >= GLFW_KEY_LEFT_SHIFT && e.key <= GLFW_KEY_RIGHT_SUPER);
		if (e.key == GLFW_KEY_ESCAPE && e.action == GLFW_PRESS) {
			// On the last question Esc means "keep my keys", not "cancel".
			if (owner->wizStep == EncoderWidget::WIZ_LAYOUT) owner->wizChooseLayout(-1);
			else owner->wizCancel();
		}
		else if (!modifierOnly && (e.action == GLFW_PRESS || e.action == GLFW_RELEASE)) {
			if (e.action == GLFW_PRESS) owner->lastPushTime = system::getTime();
			owner->wizPress(e.key, mods, e.action == GLFW_PRESS);
		}
		e.consume(this);
		return;
	}

	// ---- Learning an extra dial (CONTROLS > Extra dials > Learn) ----------
	// Same rules as learning a key: Shift / Ctrl / Alt on their own are
	// ignored, Esc cancels, everything is kept from Rack meanwhile.
	if (owner->dialLearn >= 0) {
		if (e.action == GLFW_PRESS) {
			bool modifierOnly = (e.key >= GLFW_KEY_LEFT_SHIFT && e.key <= GLFW_KEY_RIGHT_SUPER);
			if (e.key == GLFW_KEY_ESCAPE && mods == 0) owner->cancelDialLearn("Dial learning cancelled - nothing changed");
			else if (!modifierOnly) owner->dialLearnKey(e.key, mods);
		}
		e.consume(this);
		return;
	}

	// ---- Learning a key (right-click > Keys > ...) ------------------------
	// The next real key pressed becomes the new key. Shift / Ctrl / Alt on
	// their own are ignored (they're part of a combo, not the key). Escape
	// cancels. Everything is kept from Rack while learning.
	if (owner->learnAction >= 0) {
		if (e.action == GLFW_PRESS) {
			bool modifierOnly = (e.key >= GLFW_KEY_LEFT_SHIFT && e.key <= GLFW_KEY_RIGHT_SUPER);
			if (e.key == GLFW_KEY_ESCAPE) {
				owner->customLearnParam = -1;
				owner->cancelLearn();
			}
			else if (!modifierOnly) owner->learned(e.key, mods);
		}
		e.consume(this);
		return;
	}

	// ---- MIDI learn: Esc cancels (other keys carry on as normal) -----------
	if (!owner->midiLearnRole.empty() && e.key == GLFW_KEY_ESCAPE && e.action == GLFW_PRESS) {
		owner->cancelMidiLearn();
		e.consume(this);
		return;
	}

	// ---- Lock key (default Scroll Lock): always works, even when locked ---
	if (e.action == GLFW_PRESS && keyMatches(settingKeys.lockKey, e.key, mods)) {
		owner->toggleLock();
		e.consume(this);
		return;
	}
	// Locked: every other key goes straight to Rack, untouched.
	if (owner->isLocked()) return;

	// ---- Extra dials (only ones you've learned; none by default) -----------
	// Their keys are theirs alone (learning refuses keys Hotkey already
	// uses), so they're always kept from Rack. A held key repeats.
	{
		int dial = -1;
		int what = owner->dialForKey(e.key, mods, dial);
		if (what == 0 && e.action == GLFW_RELEASE && e.key >= 0 && owner->activePush.dialKey == e.key) what = 2;   // (mods changed)
		if (what != 0) {
			bool typing = dynamic_cast<ui::TextField*>(APP->event->selectedWidget) != nullptr;
			if (what == 2) {
				if (e.action == GLFW_PRESS && !typing) owner->dialPushed(dial);
				else if (e.action == GLFW_RELEASE && owner->activePush.dialKey == e.key) owner->activePush.keyUp = true;
			}
			else if ((e.action == GLFW_PRESS || e.action == GLFW_REPEAT) && !typing) owner->dialTurned(dial, what);
			if (!typing) e.consume(this);
			return;
		}
	}

	// ---- Dial push ----------------------------------------------------------
	// Always flashes the push light. If a number is held and that knob has a
	// push link, it presses the linked control (and the key is kept from
	// Rack). Any modifier is fine — the K70 dial sends Ctrl+End.
	// With the nav modifier held (default Shift) and no number held, it's
	// the navigation encoder's push instead.
	if (e.key == EncoderWidget::pushKey()) {
		if (e.action == GLFW_PRESS) {
			owner->lastPushTime = system::getTime();
			if (settingNavModifier && (mods & settingNavModifier) && !owner->playing()) {
				owner->navPush();
				e.consume(this);
				return;
			}
			if (owner->pushPressed()) e.consume(this);
		}
		else if (e.action == GLFW_RELEASE && !owner->activePush.fromWheel) {
			owner->activePush.keyUp = true;   // let go (after the minimum time)
		}
		return;
	}

	// Custom key let go: release its knob / button (always noticed).
	if (e.action == GLFW_RELEASE) owner->customReleased(e.key);

	// Number key let go: always noticed, so a number can never get "stuck".
	int knob = knobForKey(e.key);
	if (knob >= 0 && e.action == GLFW_RELEASE) {
		owner->numberReleased(e.key);
		return;
	}
	// Letter key let go: release the push button it was holding (always).
	int button = buttonForKey(e.key);
	if (button >= 0 && e.action == GLFW_RELEASE) {
		owner->releaseButton(button);
		return;
	}

	// Don't steal keys while someone is typing in a text box (e.g. the
	// module browser search).
	if (dynamic_cast<ui::TextField*>(APP->event->selectedWidget)) return;

	const KeyMap& keys = settingKeys;

	// ---- Sensitivity + / - (default numpad + / -), on the fly ---------------
	// Alone = overall sensitivity. While holding a number = that knob only.
	// Holding the key repeats.
	bool sensUp = keyMatches(keys.sensUp, e.key, mods);
	if (sensUp || keyMatches(keys.sensDown, e.key, mods)) {
		if (e.action == GLFW_PRESS || e.action == GLFW_REPEAT)
			owner->changeSensitivity(sensUp);
		e.consume(this);
		return;
	}

	// ---- Turn up / down (default = / -) while a number is held ------------
	// For keyboards without a dial. Holding the key repeats. Without a number
	// held, these keys work as normal.
	bool turnUp = keyMatches(keys.turnUp, e.key, mods);
	if ((turnUp || keyMatches(keys.turnDown, e.key, mods)) && owner->playing()) {
		if (e.action == GLFW_PRESS || e.action == GLFW_REPEAT)
			owner->turnHeldKnob(turnUp ? 1.f : -1.f);
		e.consume(this);
		return;
	}

	if (e.action != GLFW_PRESS) return;

	// ---- Groups (defaults: Shift+- / Shift+= switch, Alt+= new,
	//      Alt+Delete clear this group — undo with Ctrl+Z) --------------------
	if (keyMatches(keys.groupClear, e.key, mods)) { owner->clearCurrentGroup(); e.consume(this); return; }
	if (keyMatches(keys.groupPrev, e.key, mods)) { owner->switchGroup(-1); e.consume(this); return; }
	if (keyMatches(keys.groupNext, e.key, mods)) { owner->switchGroup(+1); e.consume(this); return; }
	if (keyMatches(keys.groupNew, e.key, mods)) { owner->newGroup(); e.consume(this); return; }

	// ---- Next page (default numpad /): one tap flips the selected module
	//      to its next page of knobs + buttons (wraps round). -------------
	//      (Checked before the numpad modes, in case someone gives the page
	//       and mode keys different keys — they're different by default.)
	if (keyMatches(keys.pageNext, e.key, mods) && !(keys.padModes && keyMatches(keys.modeKey, e.key, mods))) {
		owner->nextPage();
		e.consume(this);
		return;
	}
	// ---- Next BUTTON page (default numpad *): modules with more than 10
	//      buttons page them on their own with this key. --------------------
	if (keyMatches(keys.buttonPageNext, e.key, mods) && !(keys.padModes && keyMatches(keys.padAssignKey, e.key, mods))) {
		owner->nextButtonPage();
		e.consume(this);
		return;
	}

	// ---- Numpad modes (MIDI-friendly layout) --------------------------------
	if (keys.padModes) {
		// Mode key (numpad /): KNOBS -> BUTTONS -> SLOTS -> KNOBS ...
		if (keyMatches(keys.modeKey, e.key, mods)) {
			owner->cyclePadMode();
			e.consume(this);
			return;
		}
		// The assign key (numpad *) and select key (numpad Enter) are only
		// ever HELD with a digit.
		if (keyMatches(keys.padAssignKey, e.key, mods) || keyMatches(keys.padSelectKey, e.key, mods)) {
			e.consume(this);
			return;
		}
		// In ANY mode: hold the assign key + tap a digit = put the module
		// under the mouse in that slot; hold the select key + tap a digit =
		// select that slot (you stay in the mode you were in, so you can
		// play it straight away). In SLOTS mode a digit on its own selects.
		int digit = padDigit(e.key);
		if (digit >= 0 && mods == 0) {
			auto held = [&](const KeyBinding& b) {
				return b.key >= 0 && glfwGetKey(APP->window->win, b.key) == GLFW_PRESS;
			};
			bool assigning = held(keys.padAssignKey);
			bool selecting = held(keys.padSelectKey) || padMode == PAD_SLOTS;
			if (assigning || selecting) {
				if (assigning) owner->assignSlot(digit, owner->moduleUnderMouse());
				else owner->selectSlot(digit);
				e.consume(this);
				return;
			}
		}
	}

	// ---- Custom keys (custom mode) come first: on this module type, a
	//      control's own key wins over the normal number / letter. ---------
	if (mods == 0 && owner->customPressed(e.key)) {
		e.consume(this);
		return;
	}

	// ---- A knob key on its own: pick that knob for as long as it's held ----
	// (Not consumed: the key still reaches Rack as normal.)
	if (knob >= 0 && mods == 0) {
		owner->numberPressed(e.key);
		return;
	}

	// ---- A button key on its own: press that button of the selected module -
	// Only kept from Rack if there really is a button with that key.
	if (button >= 0 && mods == 0) {
		if (owner->buttonPressed(button, e.key)) e.consume(this);
		return;
	}

	int slot = slotForKey(e.key);
	if (slot < 0) return;

	// (The modifiers are changeable: Keys menu > Select slot / Assign slot.)
	if (mods == keys.slotAssignMods) {
		// Alt + slot key: put the module under the mouse in this slot
		// (or clear the slot, if the mouse is over empty space).
		owner->assignSlot(slot, owner->moduleUnderMouse());
		e.consume(this);
	}
	else if (mods == keys.slotSelectMods) {
		// Shift + slot key: select this slot.
		owner->selectSlot(slot);
		e.consume(this);
	}
}

// ---- Clicks: only two kinds are ours, everything else passes through --------
void Overlay::onButton(const ButtonEvent& e) {
	if (!owner) return;

	// ---- Dial setup wizard: the mouse wheel button can be the push --------
	if (owner->wizStep >= 0 && e.button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (e.action == GLFW_PRESS) owner->lastPushTime = system::getTime();
		owner->wizPress(-2, 0, e.action == GLFW_PRESS);
		e.consume(this);
		return;
	}

	// ---- Mouse wheel click = dial push (menu: "Mouse wheel click = push") --
	// Always flashes the push light. With a number held it does exactly what
	// the dial push does (and Rack doesn't get the click); otherwise the
	// click carries on to Rack as normal. Mouse buttons report a real hold.
	// Locked: clicks are left entirely to Rack (no remap / link / assign).
	if (owner->isLocked()) return;

	if (e.button == GLFW_MOUSE_BUTTON_MIDDLE && settingWheelClickPush) {
		if (e.action == GLFW_PRESS) {
			owner->lastPushTime = system::getTime();
			int heldMods = APP->window->getMods() & RACK_MOD_MASK;
			if (settingNavModifier && heldMods == settingNavModifier && !owner->playing()) {
				owner->navPush();   // nav modifier + wheel click = navigation push
				e.consume(this);
				return;
			}
			if (owner->playing() && owner->pushPressed(true)) e.consume(this);
		}
		else if (e.action == GLFW_RELEASE && owner->activePush.fromWheel) {
			owner->activePush.keyUp = true;
		}
		return;
	}

	// Holding a number + clicking a control of the selected module with:
	//   Alt        -> remap (give it that number / letter)
	//   Ctrl+Alt   -> push link (link / unlink it as that knob's push)
	// (Hold the number FIRST, then the modifier — Alt + number is "assign slot".)
	//   Shift+Alt  -> custom mode: learn this control's own key (no number
	//                 needs to be held)
	// (Those are the defaults: all three are changeable in Key mapping > MOUSE.)
	int mods = e.mods & RACK_MOD_MASK;
	const KeyMap& k = settingKeys;
	bool remapClick = (mods == k.remapMods) && owner->heldKnob >= 0;
	bool linkClick = (mods == k.pushLinkMods) && owner->heldIndex() >= 0;
	bool customClick = (mods == k.customMods);
	if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT && (remapClick || linkClick || customClick)) {
		ParamWidget* clicked = nullptr;
		for (Widget* w = APP->event->hoveredWidget; w && !clicked; w = w->parent)
			clicked = dynamic_cast<ParamWidget*>(w);
		bool used = false;
		if (customClick) used = owner->customClicked(clicked);   // (its key is undoable once learned)
		else owner->remapUndoable(owner->selectedModule(), remapClick ? "remap control" : "push link", [&]() {
			used = remapClick ? owner->remapClicked(clicked) : owner->linkPushClicked(clicked);
		});
		if (used) {
			e.consume(this);   // so the click doesn't also move/press it
			return;
		}
	}

	// "Assign (then click a module)" from the Slots menu.
	if (owner->learningSlot < 0) return;   // not assigning: pass through
	if (e.action != GLFW_PRESS) return;

	if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
		ModuleWidget* mw = owner->moduleUnderMouse();
		if (mw) owner->assignSlot(owner->learningSlot, mw);
	}
	// Any click ends assigning; right-click (or clicking empty space) cancels.
	owner->learningSlot = -1;
	// Keep this click, so it doesn't also press a button on that module.
	e.consume(this);
}


Model* modelEncoder = createModel<Encoder, EncoderWidget>("HOTKEY");
