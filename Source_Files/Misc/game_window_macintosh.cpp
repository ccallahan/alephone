/*

	macintosh_game_window.c
	Tuesday, August 29, 1995 6:09:18 PM- rdm created.

	May 28, 2000 (Loren Petrich): Added support for buffering the Heads-Up Display

Jul 2, 2000 (Loren Petrich):
	The HUD is now always buffered

Aug 22, 2000 (Loren Petrich):
	Added object-oriented resource handling
*/

#include "macintosh_cseries.h"
#include "my32bqd.h"

#include "HUDRenderer_SW.h"

#include "shell.h"
#include "preferences.h"
#include "screen.h"
#include "screen_definitions.h"
#include "images.h"

extern GrafPtr screen_window;
extern GWorldPtr world_pixels;
extern HUD_SW_Class HUD_SW;
extern bool OGL_HUDActive;

extern void draw_panels(void);


/*
 *  Draw HUD (to HUD buffer)
 */

void draw_panels(void)
{
	// LP change: the drawing is now done into a special buffer (HUD_Buffer),
	// and not to the screen or to world_pixels

	struct screen_mode_data new_mode= graphics_preferences->screen_mode;
	LoadedResource PictRsrc;
	PicHandle picture;
	Rect destination= {320, 0, 480, 640};
	Rect source= {0, 0, 160, 640};
	
	if (get_picture_resource_from_images(INTERFACE_PANEL_BASE,PictRsrc))
	{
		picture = PicHandle(PictRsrc.GetHandle());
		
		// LP addition: use HUD buffer
		_set_port_to_HUD();

		HLock((Handle) picture);
		DrawPicture(picture, &destination);
		HUnlock((Handle) picture);

		HUD_SW.update_everything(NONE);
		// LP: handled inside of the resource wrapper object
		// ReleaseResource((Handle) picture);	
		
		_restore_port();
	} else {
		/* Either they don't have the picture, or they are out of memory.  Most likely no memory */
		alert_user(fatalError, strERRORS, outOfMemory, ResError());
	}

	/* Note that you don't get here if the picture failed.. */
	// LP addition: redirecting the HUD drawing to its own buffer
	// if that buffer is available
	RequestDrawingHUD();
}
