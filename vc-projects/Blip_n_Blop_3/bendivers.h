/******************************************************************
*
*
*		----------------
*		   Game.h
*		----------------
*
*
*		Fonction tr�s diverses
*
*
*		Prosper / LOADED -   V 0.2
*
*
*
******************************************************************/

#ifndef _BEN_DIVERS_
#define _BEN_DIVERS_

extern BOOL active;
extern BOOL app_killed;
extern bool want_to_kill_app;

//
// G�re les messages. Eh oui, Windows pue du cul
//
int manageMsg();

//
// Attends t millisecondes
//
void wait(int t);

#endif
