/******************************************************************
*
*
*		----------------
*		   Game.h
*		----------------
*
*
*		Fonction très diverses
*
*
*		Prosper / LOADED -   V 0.2
*
*
*
******************************************************************/

#pragma once

extern bool active;
extern bool app_killed;
extern bool want_to_kill_app;

//
// Gère les messages. Eh oui, Windows pue du cul
//
int manageMsg();
