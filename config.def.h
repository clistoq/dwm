#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Fantasque Sans Mono Regular Nerd Font Complete:size=14:style=heavy" };
static const char dmenufont[]       = "Fantasque Sans Mono Regular Nerd Font Complete:size=14:style=heavy";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_dark_blue[]   = "#1565C0";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_dark_blue, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray1,  col_dark_blue },
};

/* tagging */
static const char *tags[] = { "", "", "/", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance    title       tags mask     isfloating   monitor */
	{ "st-256color",        NULL,       NULL,       1,            0,           -1 },
    { "firefox",            NULL,       NULL,       1 << 1,       0,           -1 },
	{ "goland",             NULL,       NULL,       1 << 2,       0,            2 },
	{ "idea",               NULL,       NULL,       1 << 2,       0,            2 },
	{ "datagrip",           NULL,       NULL,       1 << 3,       0,            2 },
	{ "TelegramDesktop",    NULL,       NULL,       1 << 4,       0,            1 },
	{ "Spotify",            NULL,       NULL,       1 << 5,       0,            1 },

};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *betterlockscreen[] = { "betterlockscreen", "-l", NULL };
static const char *suspend[] = { "systemctl", "suspend", NULL };

static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_dark_blue, "-sb", col_gray2, "-sf", col_dark_blue , NULL };
static const char *termcmd[]  = { "st", NULL };

// for pulse compatible //
static const char *upvol[] = { "amixer", "-q", "sset", "Master", "1%+", NULL };
static const char *downvol[] = { "amixer", "-q", "sset", "Master", "1%-", NULL };
static const char *mute[] = { "amixer", "-q", "-D", "pulse", "sset", "Master", "toggle", NULL };

static const char *brightnessup[] = { "xbacklight", "-inc", "10", NULL };
static const char *brightnessdown[] = { "xbacklight", "-dec", "10", NULL };

static Key keys[] = {
	/* modifier                     key                         function            argument */
    { 0,                            XF86XK_MonBrightnessUp,     spawn,              {.v = brightnessup } },
    { 0,                            XF86XK_MonBrightnessDown,   spawn,              {.v = brightnessdown } },
    { 0,                            XF86XK_AudioRaiseVolume,    spawn,              {.v = upvol } },
    { 0,                            XF86XK_AudioLowerVolume,    spawn,              {.v = downvol } },
    { 0,                            XF86XK_AudioMute,           spawn,              {.v = mute } },
	{ MODKEY,                       XK_d,                       spawn,              {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                  spawn,              {.v = termcmd } },
	//{ MODKEY,                       XK_b,                       togglebar,          {0} },
	{ MODKEY,                       XK_j,                       focusstack,         {.i = +1 } },
	{ MODKEY,                       XK_k,                       focusstack,         {.i = -1 } },
	{ MODKEY,                       XK_i,                       incnmaster,         {.i = +1 } },
	{ MODKEY,                       XK_d,                       incnmaster,         {.i = -1 } },
	{ MODKEY,                       XK_h,                       setmfact,           {.f = -0.05} },
	{ MODKEY,                       XK_l,                       setmfact,           {.f = +0.05} },
	{ MODKEY,                       XK_Return,                  zoom,               {0} },
	{ MODKEY,                       XK_Tab,                     view,               {0} },
	{ MODKEY|ShiftMask,             XK_c,                       killclient,         {0} },
	{ MODKEY,                       XK_t,                       setlayout,          {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                       setlayout,          {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                       setlayout,          {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                   setlayout,          {0} },
	{ MODKEY|ShiftMask,             XK_space,                   togglefloating,     {0} },
	{ MODKEY,                       XK_0,                       view,               {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                       tag,                {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                   focusmon,           {.i = -1 } },
	{ MODKEY,                       XK_period,                  focusmon,           {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                   tagmon,             {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                  tagmon,             {.i = +1 } },
	TAGKEYS(                        XK_1,                                           0)
	TAGKEYS(                        XK_2,                                           1)
	TAGKEYS(                        XK_3,                                           2)
	TAGKEYS(                        XK_4,                                           3)
	TAGKEYS(                        XK_5,                                           4)
	TAGKEYS(                        XK_6,                                           5)
	TAGKEYS(                        XK_7,                                           6)
	TAGKEYS(                        XK_8,                                           7)
	TAGKEYS(                        XK_9,                                           8)
	{ MODKEY|ShiftMask,             XK_q,                       quit,               {0} },
	{ MODKEY|ShiftMask,             XK_x,                       spawn,              {.v = suspend } },
	{ ControlMask|Mod1Mask,         XK_x,                       spawn,              {.v = betterlockscreen } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

