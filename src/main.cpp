#define NAME "Blip'n Blop"
#define CONFIG_FILE "config/settings.json"
#define HISCORES_FILE "config/hiscores.json"

#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
// #include <mmsystem.h> TODO?
#include <stdio.h>
#include <string.h>

#include "ben_debug.h"
#include "ben_divers.h"
#include "config.h"
#include "dd_gfx.h"
#include "fmod.h"
#include "fonte.h"
#include "game.h"
#include "globals.h"
#include "input.h"
#include "music_bank.h"
#include "picture.h"
#include "picture_bank.h"
#include "scroll.h"
#include "sound_bank_bb.h"
#include "txt_data.h"

#ifdef _MSC_VER
// TODO
#pragma warning(disable:4996)
#endif


Game game;

#if _WIN32
#include <windows.h>
HWND WinHandle = NULL;
#endif

// FSOUND_SAMPLE * samp_test = NULL;

static bool safeMode = false;

void ReleaseAll(void) {
    if (graphicInstance != NULL) {
        if (videoA != NULL) {
            videoA->Release();
            videoA = NULL;
        }

        if (systemSurface != NULL) {
            systemSurface->Release();
            systemSurface = NULL;
        }

        if (primSurface != NULL) {
            primSurface->Release();
            primSurface = NULL;
        }

        DDCloseDirectDraw();
    }

    FSOUND_Close();

    in.close();
}

void Bug(const char* txt) {
#if _WIN32
    int size = MultiByteToWideChar(CP_UTF8, 0, txt, -1, NULL, 0);
    wchar_t* wideMessage = new wchar_t[size];
    MultiByteToWideChar(CP_UTF8, 0, txt, -1, wideMessage, size);

    MessageBox(
        WinHandle, wideMessage, L"Blip'n Blop : Error reporting", MB_OK | MB_ICONERROR);
#else
    std::cerr << txt << "\n";
#endif
}

void Warning(const char* txt) {
#if _WIN32
    int size = MultiByteToWideChar(CP_UTF8, 0, txt, -1, NULL, 0);
    wchar_t* wideMessage = new wchar_t[size];
    MultiByteToWideChar(CP_UTF8, 0, txt, -1, wideMessage, size);

    MessageBox(WinHandle, wideMessage, L"Blip'n Blop : Warning", MB_OK | MB_ICONWARNING);
#else
    std::cerr << txt << "\n";
#endif
}

#if 0
// FIXME: Can you still work properly under Windows without this?
long WINAPI WinProc(HWND WinHandle, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) {
        case WM_SETCURSOR:  // CURSEUR --------
            SetCursor(NULL);
            return TRUE;
            break;

        case WM_ACTIVATE:
            active = !((BOOL)HIWORD(wParam));
            break;

        case WM_CLOSE:
            app_killed = TRUE;
            //              want_to_kill_app = true;
            return 0;
            break;

        case WM_DESTROY:  // ARG! -----------
            app_killed = TRUE;
            PostQuitMessage(0);
            break;

        case WM_TIMER:
            fps_count = fps_current_count;
            fps_current_count = 0;
            break;
    }

    return DefWindowProc(WinHandle, Msg, wParam, lParam);
}
#endif

static void analyseCmdLine(char* cmd) {
    static const char sep[] = " ";
    char* token;

    token = strtok(cmd, sep);

    while (token != NULL) {
        if (strcmp(token, "/safe") == 0) {
            safeMode = true;
        }

        if (strcmp(token, "/cheat") == 0) {
            cheat_on = true;
        }

        if (strcmp(token, "/quiet") == 0) {
            music_on = false;
        }

        token = strtok(NULL, sep);
    }
}

static bool InitApp(int nCmdShow) {
    struct {
        int height;
        int width;
    } win_size = {480, 640};

    //------------------------------------------------------------------
    //                      Histoire d'avoir un joli fichier log
    //------------------------------------------------------------------

    debug
        << "---------------------------------------------------------------\n";
    debug << "Blip & Blop - (c) LOADED Studio - " << __DATE__ << "\n";
    debug
        << "---------------------------------------------------------------\n";

    //------------------------------------------------------------------
    //                      FMOD / Sons
    //------------------------------------------------------------------

    if (!FSOUND_Init(44100, CHANNEL_NUMBER, 0)) {
        Warning("Cannot initialise FMOD. Sound will be turned off");
        sound_on = false;
        music_on = false;
    } else {
        // Pour éviter que les musiques ne se fassent écraser par les sons
        //
        FSOUND_SetPriority(0, 255);
    }

    //------------------------------------------------------------------
    //                      Charge la configuration
    //------------------------------------------------------------------

    load_BB3_config(CONFIG_FILE);

    //------------------------------------------------------------------
    //                      Charge les hi scores
    //------------------------------------------------------------------

    hi_scores.init();

    if (!hi_scores.load(HISCORES_FILE)) {
        debug << "Cannot load hi-scores file. Use default hi-scores\n";
        hi_scores.init();
    } else {
        debug << "Using " << HISCORES_FILE << " as hiscores file\n";
    }

    //------------------------------------------------------------------
    //                      Les fichiers textes
    //------------------------------------------------------------------

    if (lang_type == LANG_UK) {
        if (!loadTxtData("uk.dat")) {
            Bug("Cannot open the file 'uk.dat'");
            return false;
        }
    } else {
        if (!loadTxtData("fr.dat")) {
            Bug("Cannot open the file 'fr.dat'");
            return false;
        }
    }

    //------------------------------------------------------------------
    //                      SDL 2
    //------------------------------------------------------------------

    DDInitDirectDraw();

    //------------------------------------------------------------------
    //                      Input (SDL Events)
    //------------------------------------------------------------------

    if (!in.open()) {
        Bug("Cannot initialise DirectInput. Make sure DirectX 7 or better is "
            "installed.");
        return false;
    }

    debug << "DI Initialized\n";

    /*
            // -------- Mode Windows ----------


            DDSURFACEDESC2 ddsd;
            ZeroMemory(&ddsd, sizeof(ddsd));

            ddsd.dwSize = sizeof( ddsd );
            ddsd.dwFlags = DDSD_CAPS;
            ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

            if ( graphicInstance->CreateSurface( &ddsd, &primSurface, NULL) !=
       DD_OK)
            {
                    debug<<"Shit!\n";
                    return false;
            }

            ZeroMemory(&ddsd, sizeof(ddsd));

            ddsd.dwSize = sizeof( ddsd );
            ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
            ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
            ddsd.dwWidth = 640;  // whatever you want
            ddsd.dwHeight = 480; // whatever you want

            if ( graphicInstance->CreateSurface( &ddsd, &backSurface, NULL) !=
       DD_OK)
            {
                    debug<<"Shit2!\n";
                    return false;
            }

    */

    // Mode EXCLUSIF (bourrin quoi...)

    active = true;  // Activate the game loop

    debug << "Exclusive mode set\n";

    static const int BEST_RATE = 85;

    if (safeMode) {
        debug << "Safe mode enabled, using default 640x480x16 refresh rate.\n";
        winSet = false;

        DDSetGfxMode(
            win_size.width, win_size.height, 16, false /* fullscreen */);
    } else {
        /*DEVMODE dm;

        ZeroMemory(&dm, sizeof(dm));
        dm.dmSize = sizeof(dm);
        dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT |
        DM_DISPLAYFREQUENCY; dm.dmPelsWidth  = 640; dm.dmPelsHeight = 480;
        dm.dmBitsPerPel = 16;
        dm.dmDisplayFrequency = BEST_RATE;

        if (ChangeDisplaySettings(&dm, CDS_TEST) != DISP_CHANGE_SUCCESSFUL ||
                ChangeDisplaySettings(&dm, CDS_FULLSCREEN) !=
        DISP_CHANGE_SUCCESSFUL) { debug << "Cannot set 640x480x16 at " <<
        BEST_RATE << " Hz.\n"; winSet = false;*/
        debug << "Trying to create window\n";
        DDSetGfxMode(
            win_size.width, win_size.height, 16, fullscreen /* fullscreen */);
        debug << "Window creation done\n";
        /*debug << "Using default 640x480x16 refresh rate.\n";
        vSyncOn = true;
} else {
        debug << "Set 640x480x16 at " << BEST_RATE << " Hz.\n";
        winSet = true;
        vSyncOn = true;
}*/
    }

    /*DDSCAPS2              ddscaps2dummy;
    DWORD                   vid_mem1;
    DWORD                   vid_mem2;

    ZeroMemory(&ddscaps2dummy, sizeof(ddscaps2dummy));
    ddscaps2dummy.dwCaps = DDSCAPS_VIDEOMEMORY;
    graphicInstance->GetAvailableVidMem(&ddscaps2dummy, &vid_mem1, &vid_mem2);
    debug << "Available video memory : " << (vid_mem2 >> 10) << " Ko\n";
    mem_flag = DDSURF_BEST;
    video_buffer_on = true;

    if (vid_mem2 >> 10 >= 20) {
            mustFixGforceBug = true;
    }*/

    //------------------------------------------------------------------
    //                      Direct Draw (suite)
    //------------------------------------------------------------------

    // Crée la surface primaire avec double buffer
    debug << "Creating primSurface\n";
    primSurface = DDCreatePrimary(backSurface);

    if (primSurface == NULL || backSurface == NULL) {
        Bug("Cannot get a primary surface. Please reboot your PC and try to "
            "launch Blip'n Blop again.\nBe sure you have at least 2 Mb of "
            "video memory on your video card.");
        return false;
    }
    debug << "primSurface created\n";

    //------------------------------------------------------------------
    //                      Surface système
    //------------------------------------------------------------------
    debug << "Creating systemSurface\n";
    systemSurface =
        DDCreateSurface(win_size.width, win_size.height, DDSURF_SYSTEM);

    if (systemSurface == NULL) {
        Bug("Not enough memory. Blip'n Blop needs 32 Mo of free memory. Try to "
            "close all other applications and launch Blip'n Blop again.");
        return false;
    }
    debug << "systemSurface created\n";

    //------------------------------------------------------------------
    //                      Scroll buffers
    //------------------------------------------------------------------

    for (int width_off = 0; width_off >= 200; width_off += 100) {
        debug << "Creating video buffer of size "
              << (WANTED_VBUFFER_WIDE - width_off) << "...";
        videoA =
            DDCreateSurface(WANTED_VBUFFER_WIDE - width_off, 480, DDSURF_VIDEO);

        if (videoA) {
            debug << "Ok\n";
            vbuffer_wide = WANTED_VBUFFER_WIDE - width_off;
            break;
        }

        debug << "Failed\n";
    }

    if (videoA == NULL) {
        videoA = DDCreateSurface(WANTED_VBUFFER_WIDE, 480, DDSURF_SYSTEM);

        if (videoA == NULL) {
            // Laughing in 2019
            Bug("Not enough memory. Blip'n Blop needs 32 Mo of free "
                "memory. Try to close all other applications and "
                "launch Blip'n Blop again.");
            return false;
        }

        debug << "Cannot create video buffer. Use system buffer "
                 "instead.\n";
        vbuffer_wide = WANTED_VBUFFER_WIDE;
        mem_flag = DDSURF_SYSTEM;
        video_buffer_on = false;
    }

    //------------------------------------------------------------------
    //                      Chargement des fontes
    //------------------------------------------------------------------

    if (!fnt_menu.load("menu.lft")) {
        Bug("Cannot open the file menu.lft");
        return false;
    }

    if (!fnt_menus.load("menus.lft")) {
        Bug("Cannot open the file menus.lft");
        return false;
    }

    if (!fnt_cool.load("cool.lft")) {
        Bug("Cannot open the file cool.lft");
        return false;
    }

    if (!fnt_rpg.load("rpg.lft")) {
        Bug("Cannot open the file rpg.lft");
        return false;
    }

    //------------------------------------------------------------------
    //                      Chargement de l'interface
    //------------------------------------------------------------------

    if (!pbk_inter.loadGFX("inter.gfx")) {
        debug << "Cannot load interface.\n";
        return false;
    } else {
        debug << "Successfully loaded interface.\n";
    }

    if (!mbk_inter.open("inter.mbk", false)) {
        debug << "Cannot load interface musics.\n";
        return false;
    }

    if (!mbk_interl.open("interl.mbk", true)) {
        debug << "Cannot load interface musics (p2).\n";
        return false;
    }

    //------------------------------------------------------------------
    //                      Mémoire video restante
    //------------------------------------------------------------------

    /*ZeroMemory(&ddscaps2dummy, sizeof(ddscaps2dummy));
    ddscaps2dummy.dwCaps = DDSCAPS_VIDEOMEMORY;
    graphicInstance->GetAvailableVidMem(&ddscaps2dummy, &vid_mem1, &vid_mem2);
    debug << "Available video memory : " << (vid_mem2 >> 10) << " Ko\n";*/

    return true;  // C'est fini!
}

#undef main

void cleanup() {
    //------------------------------------------------------------------
    //                      On quitte plus ou moins proprement
    //------------------------------------------------------------------

    debug
        << "---------------------------------------------------------------\n";
    debug << "Releasing Blip & Blop\n";
    debug
        << "---------------------------------------------------------------\n";

    // Désalloue tout ce qu'il faut (en théorie)
    //

    // Sauvegarde les hi-scores
    //
    if (!hi_scores.save(HISCORES_FILE)) {
        debug << "Cannot save hi-scores\n";
    } else {
        debug << "Saving " << HISCORES_FILE << " as hi-scores file.\n";
    }

    // Sauvegarde la configuration
    //
    save_BB3_config(CONFIG_FILE);
}

int main(int argc, char** argv) {
    char lpCmdLine[512] = {0};
    for (int i = 1; i < argc; i++) {
        strcat(lpCmdLine, argv[i]);
        strcat(lpCmdLine, " ");
    }
    int nCmdShow = 0;

    //------------------------------------------------------------------
    //                      Safe mode ?
    //------------------------------------------------------------------

    analyseCmdLine(lpCmdLine);

    //------------------------------------------------------------------
    //                      Initialise l'application
    //------------------------------------------------------------------

    if (!InitApp(nCmdShow)) {
        return -1;
    }

    atexit(cleanup);

    //------------------------------------------------------------------
    //                      Joue la partie
    //------------------------------------------------------------------
    debug << "Game.go" << "\n";
    
    game.go();

    return 0;

}  // WinMain ------------------------
