*This project has been created as part of the 42 curriculum by <login1>, <login2>.*

# cub3D

## Description

cub3D is a small first-person 3D maze renderer inspired by *Wolfenstein 3D*,
written in C with the [MiniLibX](https://github.com/42Paris/minilibx-linux)
graphics library. It reads a scene description file (`.cub`), checks it
carefully, and opens a window where the player can walk around a maze seen
from inside.

The 3D view is produced with **ray casting**: for every column of the window
one ray is cast from the player through a 2D grid using the **DDA** (Digital
Differential Analysis) algorithm. The perpendicular distance to the first wall
hit gives the height of the wall slice (no fish-eye distortion), and the slice
is drawn with the texture that matches the direction the wall faces
(north, south, east or west). Ceiling and floor are painted with the colors
given in the scene file.

Main features (mandatory part):

- `.cub` parser: 4 wall textures (`NO`, `SO`, `WE`, `EA`), floor (`F`) and
  ceiling (`C`) colors, and the map, with strict validation (closed map,
  exactly one player, valid characters, RGB range, duplicated/missing
  elements, element after the map, ...).
- Any error prints `Error\n` followed by an explicit message on stderr and
  exits with status 1, freeing everything that was allocated.
- Textured walls (XPM), one texture per wall direction.
- Smooth, frame-rate independent movement (`W`/`A`/`S`/`D`) and rotation
  (arrow keys).
- Clean exit with `ESC` or the window's close button (no leaks).

Bonus part (built with `make bonus`, see [Bonus](#bonus)):

- Wall collisions: the player cannot walk through walls and slides along
  them.
- Minimap: the top-left corner shows the map, the player and where they are
  looking.
- Mouse look: moving the mouse horizontally rotates the point of view.
- Doors: `D` cells in the map are doors that open and close with the `E`
  key; closed doors block the player and are drawn with their own texture.
- Animated sprites: `2` cells in the map hold an animated object (a fire
  brazier) drawn as a billboard, hidden behind walls and closed doors.

## Instructions

### Requirements

Linux with an X server (X11 or XWayland/WSLg), `cc`, `make` and the X11
development headers used by the MiniLibX:

```sh
sudo apt-get install gcc make xorg libxext-dev libx11-dev
```

The MiniLibX sources are included in `minilibx/` and the `libft` sources in
`libft/`; both are built automatically by the Makefile.

### Build

```sh
make          # builds libft, MiniLibX and ./cub3D
make clean    # removes object files
make fclean   # removes object files, libraries and the executable
make re       # fclean + all
make bonus    # builds ./cub3D with the bonus features
```

The project is compiled with `cc -Wall -Wextra -Werror`.

### Run

Run it from the repository root (texture paths in the maps are relative to
it):

```sh
./cub3D maps/valid_demo.cub
./cub3D maps/valid_minimal.cub      # the example scene from the subject
```

### Controls

| Key | Action |
|---|---|
| `W` / `S` | move forward / backward |
| `A` / `D` | strafe left / right |
| `←` / `→` | look left / right |
| mouse (bonus only) | look left / right (move the mouse horizontally) |
| `E` (bonus only) | open / close the door in front of you |
| `ESC` or window close button | quit |

### The `.cub` file

```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm

F 220,100,0
C 225,30,0

        1111111111111111111111111
        1000000000110000000000001
        1011000001110000000000001
        1001000000000000000000001
111111111011000001110000000000001
100000000011000001110111111111111
11110111111111011100000010001
11110111111111011101010010001
11000000110101011100000010001
10000000000000001100000010001
10000000000000001101010010001
11000001110101011111011110N0111
11110111 1110101 101111010001
11111111 1111111 111111111111
```

- The six elements may appear in any order, before the map, separated by
  any number of empty lines (or none). Spaces/tabs separate an identifier
  from its value; spaces around the commas of a color are accepted.
- `NO`/`SO`/`WE`/`EA`: path to an XPM texture shown on walls facing the
  player when looking north/south/west/east. The file must exist.
- `F`/`C`: floor/ceiling color, `R,G,B` with each value in `[0,255]`.
- The map comes last and is made of `0` (floor), `1` (wall), space (void,
  outside the map) and exactly one of `N`/`S`/`E`/`W` (player start and
  orientation). It must be closed by walls: no floor cell may touch the
  void or the edge of the map. Nothing but empty lines may follow it.

`maps/` contains the scenes used to test the program: `valid_*.cub` must
open a window, `invalid_*.cub` must be rejected with `Error`.

### Project layout

```
includes/cub3d.h   structures, constants and prototypes
src/parse/         reading the file, elements (textures, colors) and map
src/validate/      closed-map checks (neighbourhood + flood fill)
src/mlx/           window, framebuffer image, pixels, event hooks
src/render/        DDA ray casting, projection, textures, frame drawing
src/game/          player, keyboard state, movement, frame timing
src/cleanup/       freeing the scene
src/error/         Error\n messages
includes/cub3d_bonus.h, src/bonus/   bonus version (*_bonus.c / *_bonus.h)
textures/door.xpm  bonus door texture (`DO`), maps/bonus_*.cub  bonus maps
textures/sprite_0..3.xpm  bonus sprite animation frames (`SP`)
```

### Bonus

`make bonus` produces the same executable name, `./cub3D`, compiled only
from `src/bonus/**/*_bonus.c` and `includes/cub3d_bonus.h` (the mandatory
objects are not reused, so the mandatory part is never affected). Running
`make` afterwards relinks the mandatory version, and vice versa; repeating
the same rule relinks nothing.

| Bonus | How |
|---|---|
| Wall collisions | The player is a square of half-side 0.2 cell. Each axis is moved separately (x with the old y, then y with the new x), so a blocked axis does not stop the other one: the player slides along walls. When blocked, the player is placed right against the wall. Movement is split into sub-steps of at most 0.25 cell, so walls cannot be tunnelled through. |
| Minimap | Drawn into the frame buffer after the walls (no `mlx_pixel_put`, no allocation per frame), inside a box of at most 200x200 px at the top-left corner. Scale: the largest one (6 to 16 px per cell) that fits the whole map; when the map does not fit at 6 px per cell, the view scrolls with the player, clamped to the map edges. Walls are light grey, floor dark grey, the empty area outside an irregular map is transparent; the player is a red dot with a yellow segment pointing where they look. |
| Doors | Map symbol `D` (every door starts closed). `E` toggles the door the player is looking at: the centre ray of the camera is followed through the grid and the first non-floor cell is taken if it is a door whose near side is at most 1.5 cells away. A closed door is a solid block for the ray caster (hit, drawn with the `DO` texture, never mirrored, and stored in the depth buffer like a wall) and for the collision check; an open door is invisible to the rays and walkable. A door is not closed while the player's collision square still overlaps its cell, so the player can never be trapped inside it. After a toggle, `E` is ignored for 0.3 s (holding the key with auto-repeat does not make the door flicker). On the minimap a closed door is orange, an open one green. |
| Animated sprites | Map symbol `2`: one sprite at the centre of each such cell. The list of sprites is allocated once, while parsing (nothing is allocated per frame). Every frame, after the walls: the sprites are sorted from the farthest to the nearest (squared distance, insertion sort: the order barely changes between frames), each one is projected with the inverse of the camera matrix `[plane dir]` (gives its screen column and its depth, the perpendicular distance), sized like a one-cell block at that depth and drawn row by row; a column of the sprite is drawn only if it is nearer than the wall or closed door stored in the depth buffer (`z_buffer`) for that column, so walls and closed doors hide sprites and open doors do not. Transparent texels (XPM colour `None`, or the key colour magenta `#FF00FF`) are skipped. Sprites behind the camera or closer than 0.1 cell to the camera plane are not drawn, and huge projections are clipped to the window. Animation is driven by the clock, not by the frame count: frame = (time in ms / 150 + phase) mod number of frames, with the time taken from `gettimeofday` (the same call that measures the frame delta); each sprite has its own phase so neighbours are not synchronised. A sprite is a solid object: the player collides with its cell like with a wall (it is not an obstacle for the rays, nor a target for `E`). On the minimap a sprite cell is purple. |
| Mouse look | Rotation proportional to the horizontal mouse movement (0.003 rad per pixel; right = turn right), applied in the game loop together with the arrow keys (both work at the same time). Only while the window has the keyboard focus: on focus loss the mouse stops rotating and any pending movement is discarded. When the pointer gets within 100 px of a window edge it is moved back to the centre (`mlx_mouse_move`); a jump of more than 250 px between two events (the re-centring itself, stale events, the pointer leaving and re-entering the window) is never turned into rotation, so the view never "jumps". |

Extra functions and map symbols used by the bonus (allowed by the subject
when justified):

- No function outside the mandatory list is used (`floor`, `ceil` and
  `fabs` belong to the math library, already allowed; `gettimeofday`, used
  for the sprite animation, is in the mandatory list). From the MiniLibX the bonus
  also uses `mlx_mouse_move` (re-centring the pointer) and hooks the X11
  `MotionNotify`, `FocusIn` and `FocusOut` events through `mlx_hook`.
- Map symbol `D` (door) and scene element `DO <path>` (XPM texture of the
  doors), both bonus only; the mandatory program rejects them. Why: the
  subject asks for doors that open and close, allows extra map symbols for
  the bonus and allows the `.cub` format to change, and a door needs both a
  map position and a texture of its own. `DO` follows the same rules as
  `NO`/`SO`/`WE`/`EA` (path must exist, no duplicates, any order); it is
  optional, so every mandatory `.cub` still runs with `make bonus`, and
  required as soon as the map contains a `D`.
- Door validation: for the closed-map check a door counts as floor (it can
  be opened, so the map must be closed by walls with every door open: a
  door on the border or next to the void is an error), and every door must
  stand between two aligned walls (walls left and right, or above and
  below), i.e. it always closes a one-cell passage. Example:
  `maps/bonus_doors.cub`; `maps/bonus_invalid_door_*.cub` must be rejected.
- Map symbol `2` (sprite) and scene element `SP <frame> [<frame> ...]`,
  bonus only (the mandatory program rejects them). Why: animated sprites
  need a position in the map and a sequence of textures. `2` is the symbol
  used for sprites in earlier versions of the cub3D subject. `SP` takes
  1 to 8 XPM paths separated by spaces/tabs, in animation order (example:
  `SP ./textures/sprite_0.xpm ./textures/sprite_1.xpm ./textures/sprite_2.xpm ./textures/sprite_3.xpm`);
  each path follows the `NO`/`SO`/`WE`/`EA` rules, `SP` may not be
  repeated, and it is optional unless the map contains a `2`. For the
  closed-map check a sprite cell counts as floor (it must be surrounded by
  walls like any floor cell). Example: `maps/bonus_sprites.cub`;
  `maps/bonus_invalid_sprite_*.cub` must be rejected.

Mouse look and platforms: this project targets Linux/X11 (the MiniLibX in
`minilibx/`). On a native X server `mlx_mouse_move` warps the pointer, so the
view can rotate indefinitely. On XWayland (e.g. WSLg on Windows) a pointer
warp may not move the real cursor while it is visible; there the view
rotates while the pointer moves inside the window and simply stops when it
leaves the window (no jump) — move it back in to keep turning. The cursor is
not hidden on purpose: `mlx_mouse_hide` of this MiniLibX sends an
uninitialised color to the X server and loads an extra library at run time,
which Valgrind reports. On macOS the MiniLibX API is different (e.g.
`mlx_mouse_move` takes no `mlx` pointer) and this code is not meant to build
there.

Floor/ceiling textures, weapons or a HUD (shown in the subject's figures)
are not part of the bonus list and are not implemented as bonuses.

## Resources

### References

- MiniLibX documentation: the man pages in `minilibx/man/` and
  [harm-smits' MiniLibX guide](https://harm-smits.github.io/42docs/libs/minilibx).
- Lode Vandevenne, *Raycasting* tutorial (DDA, camera plane, textured walls):
  <https://lodev.org/cgtutor/raycasting.html>
- F. Permadi, *Ray-Casting Tutorial for Game Development and Other Purposes*.
- Amanatides & Woo, *A Fast Voxel Traversal Algorithm for Ray Tracing* (1987),
  the grid traversal behind DDA.
- X Window System documentation for key symbols and events
  (`X11/keysym.h`, `X11/X.h`).

### How AI was used

AI (Claude, through the Claude Code agent tool) was used extensively in this
project, under our supervision:

- **Planning:** an AI agent read the subject and produced a master plan
  (requirements matrix, ambiguities and decisions to take, architecture,
  data structures, math of the DDA, test strategy) and split it into phases.
  We reviewed the plan and confirmed the design decisions (e.g. exactly one
  player, empty lines between elements are optional, error messages on
  stderr).
- **Implementation:** a chain of AI agents, one or two phases each, wrote the
  code of the mandatory part (Makefile, parser, map validation, MiniLibX
  setup, ray casting and textures, movement, error handling and cleanup)
  and of the bonus part (collisions, minimap, mouse look, doors, animated
  sprites), following
  the 42 Norm, and left notes for the next one.
- **Testing and review:** the agents also wrote test programs and scripts
  (kept outside of the delivered files): parser unit tests, valgrind runs on
  every map, fault injection on `malloc`/`open`/`read`/MiniLibX calls, and
  window tests that drive the program with synthetic X events and check the
  rendered pixels. They also checked the Norm, the compilation flags and
  the list of external functions.
- **Assets and docs:** the wall, door and sprite textures in `textures/` were generated by a
  small script written by the AI, and the AI wrote the first draft of this
  README.

The AI did not replace our own work of understanding the project: all of the
generated code has to be reviewed by both of us, and we must be able to
explain and modify any part of it (DDA, projection, texture mapping, map
validation, memory management) during the evaluation, using the references
above.
