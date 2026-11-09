# Blip'n Blop

A run and gun game for one or two players, released for free on the PC in 2002 by LOADED Studio. This fork ports it to SDL2 so it runs on Windows, Linux and in the browser.

Play it in the browser: https://k-adam.github.io/blip-blop/

## Playing

Choose START GAME. Each player joins by pressing jump on the keyboard, the mouse fire button, or A or Start on a controller. Press again on the same device to start, or press Escape to go back.

Default controls, all changeable in OPTIONS:

| Action | Keyboard | Mouse and keyboard | Controller          |
| ------ | -------- | ------------------ | ------------------- |
| Move   | Arrows   | A / D, S down      | D-pad or left stick |
| Aim    | Arrows   | Mouse              | Right stick         |
| Jump   | Z        | W                  | Left trigger        |
| Fire   | X        | Left click         | Right trigger       |
| Super  | C        | Middle click       | Right shoulder      |
| Pause  | Escape   | Escape             | Start or Back       |

OPTIONS also toggles fullscreen and vsync. HIGH SCORES on the main menu shows the table.

Settings and high scores are saved in `config/` next to the game. The browser version keeps them in browser storage.

## Building

Requires CMake 3.18+, a C++20 compiler, and SDL2 with SDL2_image and SDL2_mixer. The first configure downloads GoogleTest for the tests.

Linux:

    sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j

Windows, with vcpkg:

    vcpkg install sdl2 sdl2-image sdl2-mixer --triplet x64-windows
    cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg root>/scripts/buildsystems/vcpkg.cmake
    cmake --build build --config Release

Web, with Emscripten: `./web_build.sh` builds into `build-web/bin`.

The game loads `assets/` and `config/` from the working directory. Run it from the repository root, or run `cmake --install build --prefix <dir>` to get a folder that runs on its own. Running from the repository writes your settings into the tracked `config/` files, so don't commit those.

## Contributing

- **Formatting:** clang-format 13, the version CI checks. Enable the hook once per clone:

      pip install clang-format==13.0.1
      git config core.hooksPath .githooks

- **Tests:** `ctest --test-dir build -C Release`. These GoogleTest end to end tests run the game headless with scripted input. They check the log for expected lines and that two runs draw identical frames.
- **CI:** every push builds on Linux, Windows and Emscripten, and checks formatting. Pushes to `main` deploy the web build to GitHub Pages.

### Command line

| Flag                     | Effect                                                       |
| ------------------------ | ------------------------------------------------------------ |
| `/safe`                  | Windowed 640x480, ignoring the fullscreen setting            |
| `/quiet`                 | No music                                                     |
| `/cheat`                 | Enables the cheat keys                                       |
| `/autoquit <frames>`     | Exits after that many frames                                 |
| `/input "frame:Key,..."` | Presses keys at given frames, for example `200:Return,260:Z` |

Cheat keys: F1/F5 ammo (hold), F2/F6 cow bomb, F3/F7 extra life (player 1/player 2), F9 skip level, F12 FPS.

To replay a test run exactly, set `BB_FIXEDSTEP=1`. For headless runs, also set `SDL_VIDEODRIVER=dummy` and `SDL_AUDIODRIVER=dummy`.

### Layout

| Path                       | Contents                                                                   |
| -------------------------- | -------------------------------------------------------------------------- |
| `src/core`                 | Startup, main loop, config, high scores                                    |
| `src/engine`               | Graphics, input, sound, sprite banks, fonts                                |
| `src/entities`             | Players, enemies, shots, bonuses                                           |
| `src/level`                | Level loading, scrolling, events                                           |
| `src/menus`                | Title, pause, options, key binding and join screens                        |
| `src/cinematics`           | Cutscenes and in-level dialogues                                           |
| `assets/gfx`               | Sprite banks: a `.gfx.json` of named animation sheets, with its PNG folder |
| `assets/lvl`, `cin`, `rpg` | Levels, cutscenes and dialogues as JSON                                    |
| `assets/lft`, `sfx`, `mbk` | Fonts, sound effects, music                                                |
| `assets/uk.dat.json`       | Game text                                                                  |

## Possible improvements

Help is welcome on any of these:

- **Resolution:** support resolutions other than 640x480. It is fixed in `src/core/main.cpp`.
- **Translations:** restore the French translation. The code can load `fr.dat`, but only the English text ships.
- **Touch controls:** add them to the web build, for phones and tablets.
- **Tests:** cover more ground. The tests reach only the first world.
- **Data formats:** document the JSON formats of levels, cutscenes and dialogues, or add an editor for them.
- **Global state:** group it into objects, starting with `src/level/world.h` and `src/core/assets.h`.
- **Legacy shims:** replace the `DIK_` key aliases with direct SDL calls.
