/******************************************************************
*
* 
*		----------------
*		  Sound.cpp
*		----------------
*			
*
*		Classe "Sound" pour g�rer les sons
*
*
*		Prosper / LOADED -   V 0.1
*
*
*
******************************************************************/


//-----------------------------------------------------------------------------
//		Protection pour �viter de d�clarer 'dsound' 2 fois (1 fois dans le .h)
//-----------------------------------------------------------------------------

#define BENSOUND_CPP_FILE

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include <io.h>
#include <fcntl.h>
#include "bendebug.h"
#include "sound.h"


//-----------------------------------------------------------------------------
//		Constructeur
//-----------------------------------------------------------------------------

Sound::Sound() : sample(NULL), channel(-1)
{
}

//-----------------------------------------------------------------------------
//		Constructeur
//-----------------------------------------------------------------------------

Sound::~Sound()
{
	close();
}

//-----------------------------------------------------------------------------
// Nom: Sound::load(const char * fic, int n)
// Desc: Load un fichier .WAV de nom 'fic'. Ce fichier peut �tre jou�
//		 simultan�ment 'n' fois. Remarque : il prend �galement 'n' fois
//		 sa taille en m�moire !!!
//-----------------------------------------------------------------------------

bool Sound::load(const char * fic)
{
	sample = FSOUND_Sample_Load( FSOUND_FREE, fic, FSOUND_LOOP_OFF, 0);

	if ( sample == NULL)
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// Nom: Sound::load(void * ptr, int n)
// Desc: Load un fichier .WAV de nom 'fic'. Le fichier est d�j� ouvert
//		 et son contenu copi� en m�moire
//-----------------------------------------------------------------------------

bool Sound::loadFromMem( void * ptr, int taille)
{
	sample = FSOUND_Sample_Load( FSOUND_FREE, (char*)ptr, FSOUND_LOADMEMORY | FSOUND_LOOP_OFF, taille);

	if ( sample == NULL)
	{
		debug<<"Sound::loadFromMem\n";
		return false;
	}

	channel = -1;
	cpt_loop = 0;

	return true;
}


//-----------------------------------------------------------------------------
// Nom: Sound::play(int flags)
// Desc: Joue le son avec des drapeaux
//-----------------------------------------------------------------------------

void Sound::play(int flags)
{
	if ( flags & SOUND_LOOP)
	{
		FSOUND_Sample_SetLoopMode( sample, FSOUND_LOOP_NORMAL);

		if ( cpt_loop++ == 0)
		{
			channel = FSOUND_PlaySound( FSOUND_FREE, sample);
		}
	}
	else
	{
		FSOUND_Sample_SetLoopMode( sample, FSOUND_LOOP_OFF);
		channel = FSOUND_PlaySound( FSOUND_FREE, sample);
	}
}

//-----------------------------------------------------------------------------
// Nom: Sound::setVolume(int v)
// Desc: R�gle le volume. 'v' doit �tre n�gatif et repr�sente l'att�nuation
//		 en 100i�me de d�cibels (conseil : utiliser les constantes)
//-----------------------------------------------------------------------------

void Sound::setVolume(int v)
{
}

//-----------------------------------------------------------------------------
// Nom: Sound::stop()
// Desc: Arr�te le son. Il recommencera alors au d�but au prochain d�part
//-----------------------------------------------------------------------------

void Sound::stop()
{
	if ( channel != -1)
	{
		if ( cpt_loop > 0)
		{
			if ( --cpt_loop == 0)
			{
				FSOUND_StopSound( channel);
				channel = -1;
			}
		}
		else
		{
			FSOUND_StopSound( channel);
			channel = -1;
		}
	}
}

//-----------------------------------------------------------------------------
// Nom: Sound::close()
// Desc: Referme le tout
//-----------------------------------------------------------------------------

void Sound::close()
{
	FSOUND_Sample_Free( sample);
}






