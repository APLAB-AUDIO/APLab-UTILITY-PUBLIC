# Hotkey — play VCV Rack without the mouse

**Hotkey** (plugin *APLab Utility* by AP Lab) turns your computer keyboard, its volume dial or a mouse wheel, a game controller or a MIDI controller into a hands-on control surface for **any** module in your patch. It makes no sound and has no jacks, so it costs nothing on the audio thread.

Free and open source (GPL-3.0-or-later). Contact: support@aplabaudio.com

---

## Quick start in 4 keys

1. **Alt + 1**: point at a module and press Alt+1 to put it in slot 1.
2. **Shift + 1**: select slot 1. The module gets an amber outline, and badges show which key does what.
3. **Hold 1 and turn**: hold a number key and turn your dial, scroll the mouse wheel, or tap **=** / **-**. Only that knob moves.
4. **Q**: the letter row Q…P presses buttons 1–10.

New here? Right-click Hotkey > **Set up my dial...** It works out what your dial or wheel sends and asks which key layout you want.

## Playing

| What | How (Standard layout) |
|---|---|
| Turn knob N | Hold **1…0** + dial / wheel / **=** **-** (1 click = 1% of the range; stepped knobs move one step) |
| Push a push-encoder | Hold its number + press the dial (**End**) or click the mouse wheel |
| Reset a knob | Hold its number + double-tap the push |
| Press button N | **Q…P** (momentary buttons are held as long as the key is) |
| Next page (modules with more than 10 knobs) | **Num /** |
| Sensitivity | **Num +** / **Num -** (hold a number to change that knob only) |
| Lock (so you can type) | **Scroll Lock** or the LOCK button |

## Slots and groups

- 10 slots per group, up to 99 groups. **Alt + slot key** assigns the module under the mouse (over empty space, it clears the slot). **Shift + slot key** selects it.
- **Shift + -** / **Shift + =** switch groups, **Alt + =** makes a new group, and **Alt + Delete** clears the group (Ctrl+Z undoes it).
- Slots and groups are saved with the patch. Your keys, colours and sensitivity are saved once for all patches.

## Make it yours

- **Layouts:** Standard (two hands), Laptop (no numpad) and Numpad (one hand, safe alongside Rack's computer-keyboard MIDI). Every key can be re-learned in **Key mapping**.
- **Renumber:** hold a number + Alt-click a control. This applies to every module of that type.
- **Push links:** hold a number + Ctrl+Alt-click the button that should be that knob's push. Push encoders link themselves.
- **Own key:** Shift+Alt-click any control, then press a key.
- **Highlights & colours:** turn each overlay on or off, including a colour-blind-safe set.

## Extra dials (keyboards and macropads with more knobs)

Some keyboards and macropads have 2, 3 or 4 knobs. Set each knob to send its own keys in the keyboard's software (VIA, QMK and similar). F13–F24 are ideal, because nothing else uses them. Volume keys never reach Rack. Then go to right-click > **Extra dials** > Dial N > **Learn**: turn the knob right, then left, then press it (Backspace if it has no push).

Extra dial N turns **knob N** of the selected module directly, with no number to hold. Each dial can instead turn the held knob, or work as navigation. Each one has its own speed and invert.

## Panel

- **Big dial:** picks the next or previous module in the group. Its push is next page.
- **< >:** previous or next group.
- **SENS:** overall sensitivity.
- **LOCK:** ignore all of Hotkey's keys.

The lights show the dial's up, push and down, the 10 slots, the group, and the page. Alt + your dial turns the big dial from the keyboard.

## Game controller and MIDI

- **Gamepad:** plug in an Xbox or PlayStation style controller. By default the sticks turn two knobs, A B X Y press buttons, the bumpers pick modules, the triggers switch groups, and the d-pad flips pages (up/down) and moves the sticks along the knobs (left/right). View = undo and Menu = home. Remap it all in the menu.
- **MIDI controller:** map your controller's knobs and buttons to *roles* (knob 3, button 5, slot 2, next page…), so the same physical knob plays knob 3 of whatever module is selected. Learn detects absolute and relative encoders, and pickup stops jumps.

## Good to know

- Only one Hotkey per patch is active. Extra copies wait on standby.
- Turns, button presses and setup changes go into Rack's undo (Ctrl+Z).
- **Mac:** Ctrl means Cmd and Alt means Option. Many Mac keyboards have no End, numpad or Scroll Lock, so use the Laptop layout, re-learn the keys you're missing, and use the panel's LOCK button.
- **Inside a DAW** (Rack Pro as a plugin), the host may take some keys first.
- **"My dial does nothing":** run **Set up my dial...**. If Rack can't see the dial at all (it sends volume keys), set it to scroll or to normal keys in the keyboard's software.

## Building

Standard Rack 2 plugin: `make` with `RACK_DIR` pointing at the Rack SDK, then `make install`.
