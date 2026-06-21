# ALBERT Qt/C++ Rewrite - Feature TODO List

This is an ordered, dependency-aware checklist of features to re-implement when porting ALBERT
from GTK/C to Qt/C++. It's derived from the current GTK feature set (user manual, changelog,
and source code), not the implementation details of the GTK codebase.

Each phase builds on the previous one. Within a phase, items are roughly in the order they
should be tackled, but most are independent of each other once the phase's prerequisites are
done: that's where contributors can parallelize. **Do not start a phase before its
prerequisites are checked off**. E.g. selections and affine transformations both depend on a
stable canvas/colormap representation and coordinate-transform layer, so doing them early just
means redoing the work later.

Starting point: Qt's official
[Scribble example](https://code.qt.io/cgit/qt/qtbase.git/tree/examples/widgets/widgets/scribble)
for the basic QWidget + QPainter + QImage + mouse-event skeleton, the same way the GTK version
started from the GTK2 drawing-area demo.

---

## Phase 0 - Foundation (no user-visible features yet)

- [x] Project scaffolding: CMake build, Qt Widgets app skeleton, window with a central drawing
      widget (à la Scribble)
- [ ] **Colormap** module: 8-bit indexed image buffer (width, height, stride, data), independent
      of any Qt type, analogous to a `QImage` with an indexed-8 format but under app control
      for fast direct pixel access
- [ ] **Palette** module: 16-color C64 VIC-II palette with multiple variants (Pixcen, Pepto,
      Colodore, CCS64, PC64, VICE, Jampal, DeeKay, High contrast, NTSC) and a way to map a
      palette index to display RGB
- [ ] Colormap -> `QImage`/pixmap conversion (for blitting to screen via `QPainter`)
- [ ] **Transform** module: coordinate mapping between canvas space and widget/device space,
      wrapping `QTransform` (pan, zoom, rotate), needed by virtually everything else
      (drawing, selection, preview)
- [ ] Basic canvas widget: fixed-size indexed bitmap, zoom (in/out/100%/best-fit), pan
      (space-drag or scroll wheel)
- [ ] Application/document model skeleton: one open "project" holding the colormap, palette
      state, and transform, even before most of those things do anything yet

## Phase 1 - Core Canvas Modes & Drawing

- [ ] Hires mode (320x200 / 416x200 with side borders, 8x8 char blocks, fg/bg per block)
- [ ] Multicolor mode (double-width pixels, 4x8 char blocks, 3 colors + background per block)
- [ ] Bitmap-only mode vs. bitmap + side-borders mode (canvas width 320 vs 416)
- [ ] Bitmap + sprites in upper/lower borders mode (canvas height 200 vs 284) - alpha GTK
      feature, keep in mind while designing the canvas model so it isn't bolted on awkwardly
- [ ] Color clash / per-cell color-count enforcement consistent with the active mode
- [ ] Background, border, and sprite multicolor (MC0/MC1) color assignment, plus individual
      per-sprite colors (with the hardware constraint that rows 5-6 and rows 8-9 share colors
      in side-borders mode)
- [ ] Pen tool: adjustable brush size (1-24px), primary/secondary color via mouse button,
      Shift-drag for connecting lines
- [ ] Eraser tool
- [ ] Fill (flood fill) tool, primary/secondary color
- [ ] Eyedropper tool (color sampling into primary/secondary)
- [ ] Pixel-perfect toggle (lenient vs. strict stroke rendering)
- [ ] 16-color palette widget: left-click = primary, right-click = secondary

## Phase 2 - Selection & Affine Transformations

*(Depends on Phase 1's canvas representation and transform module. Do this early. Selection
and transforms are foundational for symmetry drawing, copy/paste, flip, and the import/convert
pipeline later.)*

- [ ] Selection tool: rectangular marquee, drag-and-drop of selection contents, Alt-key
      transparency option
- [ ] Select all / select none / deselect
- [ ] Cut / copy / paste / paste-with-transparency
- [ ] Pan tool (space-drag or scroll wheel); formalize from the Phase 0 prototype
- [ ] Canvas rotation tool: free rotation with Shift-key snapping (22.5° increments), reset
      rotation
- [ ] Flip horizontally / vertically; whole canvas and selection-only variants
- [ ] Symmetry drawing modes: horizontal, vertical, quadrant, bi-quadrant, kaleidoscope
- [ ] Color swap and color replace tools (canvas-wide and within a selection)

## Phase 3 - History (Undo/Redo) & Project Files

*(Depends on a stable colormap representation from Phase 1. Do this before Phase 4+ so that
import/export/scripting work can be tested with safe undo at every step.)*

- [ ] Checkpoint/history module: linear undo/redo stack over colormap states
- [ ] Unlimited undo/redo back to the first edit
- [ ] Clear history (reduce file size); explicit user action
- [ ] Project file format: persists colormap + full workstage history + color state
- [ ] Save / Save As
- [ ] Open / Open Recent (last 10 files)
- [ ] Project properties dialog (workstage count, color usage, memory footprint, timestamps)
- [ ] Save workstages as a PNG sequence (for external time-lapse assembly)

## Phase 4 - Native Import/Export

*(Depends on Phases 1-3: needs the canvas modes finalized and a way to undo a botched import.)*

- [ ] Import hires images: Art Studio 1.x, Doodle, Image System, Interpaint Hires
- [ ] Import multicolor images: Advanced Art Studio, Blazing Paddles, Koala Painter 2,
      Vidcom 64, Wigmore Artist 64
- [ ] Import native sprites: PRG and PMAP (Pixcen) formats, optional use of stored sprite
      attribute byte ("Sprite Pad" format) vs. ignoring it
- [ ] Export to native formats: Koala Painter (multicolor), Art Studio (hires)
- [ ] Export to PNG (with/without borders where applicable to the mode)
- [ ] Export to executable PRG (Exomized, PAL/NTSC/Drean-aware startup code)
- [ ] Export to Ultimate device over the network (REST API), with IP/hostname + timeout
      settings

## Phase 5 - PNG Conversion Pipeline

*(Depends on Phase 1's palette/color-clash logic and Phase 3's undo, since conversion is
effectively a big destructive edit.)*

- [ ] Import 24/32-bit PNG as a non-clashing, non-editable reference layer overlay
- [ ] Convert non-native 24/32-bit PNG into the active C64 mode, with a live preview
- [ ] Color distance modes: natural (Euclidean) and weighted (luminance-based)
- [ ] Dithering: none (nearest match), error diffusion (Floyd-Steinberg), ordered (Bayer)
- [ ] Histogram with curves adjustment (combined RGB or individual R/G/B channels),
      including logarithmic scale option
- [ ] Aspect-ratio emulation during conversion: PAL (0.9375), NTSC (0.75), square (1:1)

## Phase 6 - Preview Window & Display Aids

*(Mostly independent of Phases 4-5; depends only on Phase 1's canvas + Phase 0's transform.)*

- [ ] Preview window: separate, independently zoomable view of the canvas without reference
      grids
- [ ] CRT emulation filter in the preview window
- [ ] Mirror preview (horizontal flip for composition checking)
- [ ] Character/sprite/pixel grid toggles, independent of each other
- [ ] Reference grid shading toggle
- [ ] VICE border safe-area overlays (green = normal borders, amber = full borders)
- [ ] Canvas filtering mode for zoomed display: nearest vs. bilinear

## Phase 7 - Scripting (Lua)

*(Depends on Phases 1-3 being stable, since scripts drive the same drawing/checkpoint
primitives interactively.)*

- [ ] Embed a Lua interpreter and a script editor window (syntax highlighting is a nice-to-have,
      not a blocker)
- [ ] Drawing primitives: `drawclear`, `drawfill`, `drawbrush`, `drawline`, `drawrect`,
      `drawrectf`, `drawcircle`, `drawcirclef`, `drawellipse`, `drawellipsef`, `drawstar`
- [ ] Control primitives: `brushsize`, `checkpointon`/`checkpointoff`/`checkpointsave`,
      `invalidateon`/`invalidateoff`, `drawrefresh`
- [ ] Color primitives: `bgcolor`, `bordercolor`, `mcspritecolors`
- [ ] `exportpng` scripting function (with optional border inclusion)
- [ ] Max instruction count safeguard against infinite loops

## Phase 8 - Extras, Settings & Platform Polish

*(Lowest priority; mostly independent leaf items; good first/small contributions once earlier
phases exist.)*

- [ ] Pareidolia generator (random pattern inspiration tool)
- [ ] Keyboard shortcuts: palette selection (1-8, with/without Shift), brush size adjustment
      (comma/period, brackets), Delete to clear selection, Escape to deselect, arrow keys to
      pan
- [ ] Settings: scroll-wheel zoom direction invert + canvas-binding option, native vs. Qt file
      dialogs (if still a meaningful distinction under Qt), scripting-window font choice
- [ ] Open Recent list management, including pruning invalid entries
- [ ] About dialog, offline documentation viewer/shortcut
- [ ] Sample projects bundled with the app
- [ ] Cross-platform packaging (Windows installer, macOS bundle/signing, Linux packaging) -
      defer until the app is otherwise feature-complete

---

## Notes for contributors

- Pick an unchecked item whose prerequisites (the phases above it) are already checked off.
- A pull request should implement one item (or a tightly related small group of items) at a
  time: small, reviewable, testable changes are much easier to accept than large ones.
- If a feature's exact behavior is unclear, the published
  [user manual](https://www.albertpixels.com/documentation/) and the original GTK project's
  [change log](https://www.albertpixels.com/hosted/software/changelog.txt) are the sources
  of truth for *what* it should do.
