#include "item_tasks.h"

static int trapped_error_code = 0;
static int (*old_error_handler) (Display *d, XErrorEvent *e);

static int
error_handler(Display     *display,
	      XErrorEvent *error)
{
   trapped_error_code = error->error_code;
   return 0;
}

static void
trap_errors(void)
{
   trapped_error_code = 0;
   old_error_handler = XSetErrorHandler(error_handler);
}

static int
untrap_errors(void)
{
   XSetErrorHandler(old_error_handler);
   return trapped_error_code;
}


static void *
get_win_prop_data (MBDesktop *mb, Window win, Atom prop, Atom type, int *items)
{
	Atom type_ret;
	int format_ret;
	unsigned long items_ret;
	unsigned long after_ret;
	unsigned char *prop_data;

	prop_data = 0;

	XGetWindowProperty (mb->dpy, win, prop, 0, 0x7fffffff, False,
			    type, &type_ret, &format_ret, &items_ret,
			    &after_ret, &prop_data);
	if (items)
		*items = items_ret;

	return prop_data;
}


void
item_tasks_activate_cb(void *data1, void *data2)
{
  MBDesktop *mb = (MBDesktop *)data1; 
  MBDesktopItem *item = (MBDesktopItem *)data2; 
  Atom atom_net_active = XInternAtom(mb->dpy, "_NET_ACTIVE_WINDOW", False);
  XEvent ev;

  memset(&ev, 0, sizeof ev);
  ev.xclient.type = ClientMessage;
  ev.xclient.window = item->win;
  ev.xclient.message_type = atom_net_active;
  ev.xclient.format = 32;

  ev.xclient.data.l[0] = 0; 

  XSendEvent(mb->dpy, mb->root, False, SubstructureRedirectMask, &ev);
}

void
item_tasks_get_icon_cb(void *data1, void *data2)
{
  //MBDesktop *mb = (MBDesktop *)data1; 
  ;
}

void
item_tasks_populate_cb(void *data1, void *data2)
{
 MBDesktop *mb = (MBDesktop *)data1; 
 MBDesktopItem *item = (MBDesktopItem *)data2, *cur; 
 MBPixbufImage *img = NULL;
 Window *wins, win_trans_for;
 XWMHints *wmhints;
 XWindowAttributes winattr; 
 int i, num;

 Atom atom_client_list = XInternAtom(mb->dpy, "_NET_CLIENT_LIST",False);
 Atom atom_net_name    = XInternAtom(mb->dpy, "_NET_WM_NAME",    False);
 Atom atom_net_win_type = XInternAtom(mb->dpy, "_NET_WM_WINDOW_TYPE", False);
 Atom atom_net_wm_icon = XInternAtom(mb->dpy, "_NET_WM_ICON", False);
 Atom atom_net_win_type_normal 
   = XInternAtom(mb->dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False);
 Atom atom_utf8_str    = XInternAtom(mb->dpy, "UTF8_STRING",    False);
 Atom *atom_tmp;

 int *wm_icon_data;

 wins = get_win_prop_data (mb, mb->root, atom_client_list, XA_WINDOW, &num);

 if (!wins) return;

 cur = item->item_child; /* UP item */

 if (cur->item_next_sibling)
   {
     mbdesktop_items_free(mb, cur->item_next_sibling);
     cur->item_next_sibling = NULL;
   }

 for (i = 0; i < num; i++)
   {
     unsigned char *win_name = NULL;
     img = NULL;

     trap_errors();

     XGetWindowAttributes(mb->dpy, wins[i], &winattr);


     if (winattr.map_state != IsViewable)
       {
	 continue;
       }

     atom_tmp = get_win_prop_data (mb, wins[i], atom_net_win_type, 
				   XA_ATOM, NULL);
     if (atom_tmp && atom_tmp[0] != atom_net_win_type_normal)
       {
	 continue;
       }

     XGetTransientForHint(mb->dpy, wins[i], &win_trans_for);
   
     if ( win_trans_for && (win_trans_for != wins[i]))
       {
	 continue;
       }

     if ((win_name = get_win_prop_data(mb, wins[i], 
				       atom_net_name, atom_utf8_str, 
				       NULL)) == NULL)
       {
	 XFetchName(mb->dpy, wins[i], (char **)&win_name); 
	 if (!win_name) win_name = (unsigned char *)"<unnamed>";
       }

     wmhints = XGetWMHints(mb->dpy, wins[i]);

     if ((wm_icon_data = get_win_prop_data(mb, wins[i], 
					  atom_net_wm_icon, XA_CARDINAL, 
					   NULL)) != NULL)
       {
	 unsigned char *p;
	 int i;
	 MBPixbufImage *tmp_img;

	 img = mb_pixbuf_img_new(mb->pixbuf,wm_icon_data[0],wm_icon_data[1] ); 

	 p = img->rgba;
	 for (i =0 ; i < (wm_icon_data[0]*wm_icon_data[1]); i++)
	   {
	     *p++ = (wm_icon_data[i+2] >> 16) & 0xff;  
	     *p++ = (wm_icon_data[i+2] >> 8) & 0xff;  
	     *p++ = wm_icon_data[i+2] & 0xff;  
	     *p++ = wm_icon_data[i+2] >> 24; 
	   }

	 if (img)
	   {
	     if (wm_icon_data[0] != 32 || wm_icon_data[1] != 32) 
	       {
		 tmp_img = mb_pixbuf_img_scale(mb->pixbuf, img, 32, 32); 
		 mb_pixbuf_img_free(mb->pixbuf, img);
		 img = tmp_img;
	       }
	   }

	 XFree(wm_icon_data);
       }
     else if (wmhints && wmhints->flags & IconPixmapHint
	 && wmhints->flags & IconMaskHint)
       {
	 Window duh;
	 int x,y,w,h,b,d;
	 MBPixbufImage *tmp_img;

	 if (XGetGeometry(mb->dpy, wmhints->icon_pixmap, &duh, 
			  &x, &y, &w, &h, &b, &d))
	   {
	     img = mb_pixbuf_img_new_from_drawable(mb->pixbuf, 
						   (Drawable)wmhints->icon_pixmap, 
						   (Drawable)wmhints->icon_mask,
						   0, 0, w, h);

	 
	     if ( (img) && (w != 32 || h != 32))
	       {
		 tmp_img = mb_pixbuf_img_scale(mb->pixbuf, img, 32, 32); 
		 
		 mb_pixbuf_img_free(mb->pixbuf, img);
		 img = tmp_img;
	       }
	   }

       } 

     if (img == NULL || untrap_errors())
       {
	 char *no_app_fn = NULL;
	 if (img != NULL) mb_pixbuf_img_free(mb->pixbuf, img);
	 
	 no_app_fn = mb_dot_desktop_icon_get_full_path (mb->theme_name, 
							32, 
							NO_APP_ICON );


	 img = mb_pixbuf_img_new_from_file(mb->pixbuf, no_app_fn );
	 if (img == NULL)
	   {
	     fprintf(stderr, "mbdesktop: failed to load %s ." 
		     "- please make sure this exists\n", NO_APP_ICON );
	     continue;
	   }

	 free(no_app_fn);
       }

     cur->item_next_sibling 
       = mbdesktop_item_new_with_params(mb,
				        win_name, 
					NULL,
					img,
					NULL,
					item_tasks_activate_cb,
					NULL,
					ITEM_TYPE_APP, 0 );
     cur->item_next_sibling->win = wins[i];
     cur->item_next_sibling->item_prev_sibling = cur;
     cur = cur->item_next_sibling;

     mb_pixbuf_img_free(mb->pixbuf, img);

   }

 XFree(wins);

}

void
item_tasks_empty_cb(void *data1, void *data2)
{
  ;
}
