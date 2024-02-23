#pragma once

//https://manpages.ubuntu.com/manpages/focal/man7/virkeycode-linux.7.html
/*
#define K_ESCAPE  0x01
#define K_MINUS   0x0C
#define K_EQUAL   0x0D
#define K_BACK    0x0E  
#define K_TAB     0x0F

#define K_0       0x0B
#define K_1       0x02
#define K_2       0x03
#define K_3       0x04
#define K_4       0x05
#define K_5       0x06
#define K_6       0x07
#define K_7       0x08
#define K_8       0x09
#define K_9       0x0A

#define K_A       0x1E
#define K_B       0x30
#define K_C       0x2E
#define K_D       0x20
#define K_E       0x12
#define K_F       0x21
#define K_G       0x22
#define K_H       0x23
#define K_I       0x17
#define K_J       0x24
#define K_K       0x25
#define K_L       0x26
#define K_M       0x32
#define K_N       0x31
#define K_O       0x18
#define K_P       0x19
#define K_Q       0x10
#define K_R       0x13
#define K_S       0x1F
#define K_T       0x14
#define K_U       0x16
#define K_V       0x2F
#define K_W       0x11
#define K_X       0x2D
#define K_Y       0x15
#define K_Z       0x2C

#define K_F1       0x3B
#define K_F2       0x3C
#define K_F3       0x3D
#define K_F4       0x3E
#define K_F5       0x3F
#define K_F6       0x40
#define K_F7       0x41
#define K_F8       0x42
#define K_F9       0x43
#define K_F10      0x44
#define K_F11      0x57
#define K_F12      0x58
#define K_F13      0xB7
#define K_F14      0xB8
#define K_F15      0xB9
#define K_F16      0xBA
#define K_F17      0xBB
#define K_F18      0xBC
#define K_F19      0xBD
#define K_F20      0xBE
#define K_F21      0xBF
#define K_F22      0xC0
#define K_F23      0xC1
#define K_F24      0xC2

#define K_ENTER   0x1C
#define K_LCTRL   0x1D
*/

/*
26 (0x1a) Key name KEY_LEFTBRACE
27 (0x1b) Key name KEY_RIGHTBRACE
39 (0x27) Key name KEY_SEMICOLON
40 (0x28) Key name KEY_APOSTROPHE
41 (0x29) Key name KEY_GRAVE
42 (0x2a) Key name KEY_LEFTSHIFT
43 (0x2b) Key name KEY_BACKSLASH
51 (0x33) Key name KEY_COMMA
52 (0x34) Key name KEY_DOT
53 (0x35) Key name KEY_SLASH
54 (0x36) Key name KEY_RIGHTSHIFT
55 (0x37) Key name KEY_KPASTERISK
56 (0x38) Key name KEY_LEFTALT
57 (0x39) Key name KEY_SPACE
58 (0x3a) Key name KEY_CAPSLOCK
69 (0x45) Key name KEY_NUMLOCK
70 (0x46) Key name KEY_SCROLLLOCK
71 (0x47) Key name KEY_KP7
72 (0x48) Key name KEY_KP8
73 (0x49) Key name KEY_KP9
74 (0x4a) Key name KEY_KPMINUS
75 (0x4b) Key name KEY_KP4
76 (0x4c) Key name KEY_KP5
77 (0x4d) Key name KEY_KP6
78 (0x4e) Key name KEY_KPPLUS
79 (0x4f) Key name KEY_KP1
80 (0x50) Key name KEY_KP2
81 (0x51) Key name KEY_KP3
82 (0x52) Key name KEY_KP0
83 (0x53) Key name KEY_KPDOT
85 (0x55) Key name KEY_ZENKAKUHANKAKU
86 (0x56) Key name KEY_102ND
89 (0x59) Key name KEY_RO
90 (0x5a) Key name KEY_KATAKANA
91 (0x5b) Key name KEY_HIRAGANA
92 (0x5c) Key name KEY_HENKAN
93 (0x5d) Key name KEY_KATAKANAHIRAGANA
94 (0x5e) Key name KEY_MUHENKAN
95 (0x5f) Key name KEY_KPJPCOMMA
96 (0x60) Key name KEY_KPENTER
97 (0x61) Key name KEY_RIGHTCTRL
98 (0x62) Key name KEY_KPSLASH
99 (0x63) Key name KEY_SYSRQ
100 (0x64) Key name KEY_RIGHTALT
101 (0x65) Key name KEY_LINEFEED
102 (0x66) Key name KEY_HOME
103 (0x67) Key name KEY_UP
104 (0x68) Key name KEY_PAGEUP
105 (0x69) Key name KEY_LEFT
106 (0x6a) Key name KEY_RIGHT
107 (0x6b) Key name KEY_END
108 (0x6c) Key name KEY_DOWN
109 (0x6d) Key name KEY_PAGEDOWN
110 (0x6e) Key name KEY_INSERT
111 (0x6f) Key name KEY_DELETE
112 (0x70) Key name KEY_MACRO
113 (0x71) Key name KEY_MUTE
114 (0x72) Key name KEY_VOLUMEDOWN
115 (0x73) Key name KEY_VOLUMEUP
116 (0x74) Key name KEY_POWER
117 (0x75) Key name KEY_KPEQUAL
118 (0x76) Key name KEY_KPPLUSMINUS
119 (0x77) Key name KEY_PAUSE
120 (0x78) Key name KEY_SCALE
121 (0x79) Key name KEY_KPCOMMA
122 (0x7a) Key name KEY_HANGEUL
123 (0x7b) Key name KEY_HANJA
124 (0x7c) Key name KEY_YEN
125 (0x7d) Key name KEY_LEFTMETA
126 (0x7e) Key name KEY_RIGHTMETA
127 (0x7f) Key name KEY_COMPOSE
128 (0x80) Key name KEY_STOP
129 (0x81) Key name KEY_AGAIN
130 (0x82) Key name KEY_PROPS
131 (0x83) Key name KEY_UNDO
132 (0x84) Key name KEY_FRONT
133 (0x85) Key name KEY_COPY
134 (0x86) Key name KEY_OPEN
135 (0x87) Key name KEY_PASTE
136 (0x88) Key name KEY_FIND
137 (0x89) Key name KEY_CUT
138 (0x8a) Key name KEY_HELP
139 (0x8b) Key name KEY_MENU
140 (0x8c) Key name KEY_CALC
141 (0x8d) Key name KEY_SETUP
142 (0x8e) Key name KEY_SLEEP
143 (0x8f) Key name KEY_WAKEUP
144 (0x90) Key name KEY_FILE
145 (0x91) Key name KEY_SENDFILE
146 (0x92) Key name KEY_DELETEFILE
147 (0x93) Key name KEY_XFER
148 (0x94) Key name KEY_PROG1
149 (0x95) Key name KEY_PROG2
150 (0x96) Key name KEY_WWW
151 (0x97) Key name KEY_MSDOS
152 (0x98) Key name KEY_SCREENLOCK
153 (0x99) Key name KEY_DIRECTION
154 (0x9a) Key name KEY_CYCLEWINDOWS
155 (0x9b) Key name KEY_MAIL
156 (0x9c) Key name KEY_BOOKMARKS
157 (0x9d) Key name KEY_COMPUTER
158 (0x9e) Key name KEY_BACK
159 (0x9f) Key name KEY_FORWARD
160 (0xa0) Key name KEY_CLOSECD
161 (0xa1) Key name KEY_EJECTCD
162 (0xa2) Key name KEY_EJECTCLOSECD
163 (0xa3) Key name KEY_NEXTSONG
164 (0xa4) Key name KEY_PLAYPAUSE
165 (0xa5) Key name KEY_PREVIOUSSONG
166 (0xa6) Key name KEY_STOPCD
167 (0xa7) Key name KEY_RECORD
168 (0xa8) Key name KEY_REWIND
169 (0xa9) Key name KEY_PHONE
170 (0xaa) Key name KEY_ISO
171 (0xab) Key name KEY_CONFIG

172 (0xac) Key name KEY_HOMEPAGE

173 (0xad) Key name KEY_REFRESH

174 (0xae) Key name KEY_EXIT

175 (0xaf) Key name KEY_MOVE

176 (0xb0) Key name KEY_EDIT

177 (0xb1) Key name KEY_SCROLLUP

178 (0xb2) Key name KEY_SCROLLDOWN

179 (0xb3) Key name KEY_KPLEFTPAREN

180 (0xb4) Key name KEY_KPRIGHTPAREN

181 (0xb5) Key name KEY_NEW

182 (0xb6) Key name KEY_REDO

200 (0xc8) Key name KEY_PLAYCD

201 (0xc9) Key name KEY_PAUSECD

202 (0xca) Key name KEY_PROG3

203 (0xcb) Key name KEY_PROG4

204 (0xcc) Key name KEY_DASHBOARD

205 (0xcd) Key name KEY_SUSPEND

206 (0xce) Key name KEY_CLOSE

207 (0xcf) Key name KEY_PLAY

208 (0xd0) Key name KEY_FASTFORWARD

209 (0xd1) Key name KEY_BASSBOOST

210 (0xd2) Key name KEY_PRINT

211 (0xd3) Key name KEY_HP

212 (0xd4) Key name KEY_CAMERA

213 (0xd5) Key name KEY_SOUND

214 (0xd6) Key name KEY_QUESTION

215 (0xd7) Key name KEY_EMAIL

216 (0xd8) Key name KEY_CHAT

217 (0xd9) Key name KEY_SEARCH

218 (0xda) Key name KEY_CONNECT

219 (0xdb) Key name KEY_FINANCE

220 (0xdc) Key name KEY_SPORT

221 (0xdd) Key name KEY_SHOP

222 (0xde) Key name KEY_ALTERASE

223 (0xdf) Key name KEY_CANCEL

224 (0xe0) Key name KEY_BRIGHTNESSDOWN

225 (0xe1) Key name KEY_BRIGHTNESSUP

226 (0xe2) Key name KEY_MEDIA

227 (0xe3) Key name KEY_SWITCHVIDEOMODE

228 (0xe4) Key name KEY_KBDILLUMTOGGLE

229 (0xe5) Key name KEY_KBDILLUMDOWN

230 (0xe6) Key name KEY_KBDILLUMUP

231 (0xe7) Key name KEY_SEND

232 (0xe8) Key name KEY_REPLY

233 (0xe9) Key name KEY_FORWARDMAIL

234 (0xea) Key name KEY_SAVE

235 (0xeb) Key name KEY_DOCUMENTS

236 (0xec) Key name KEY_BATTERY

237 (0xed) Key name KEY_BLUETOOTH

238 (0xee) Key name KEY_WLAN

239 (0xef) Key name KEY_UWB

240 (0xf0) Key name KEY_UNKNOWN

241 (0xf1) Key name KEY_VIDEO_NEXT

242 (0xf2) Key name KEY_VIDEO_PREV

243 (0xf3) Key name KEY_BRIGHTNESS_CYCLE

244 (0xf4) Key name KEY_BRIGHTNESS_ZERO

245 (0xf5) Key name KEY_DISPLAY_OFF

246 (0xf6) Key name KEY_WIMAX

256 (0x100) Key name BTN_0

257 (0x101) Key name BTN_1

258 (0x102) Key name BTN_2

259 (0x103) Key name BTN_3

260 (0x104) Key name BTN_4

261 (0x105) Key name BTN_5

262 (0x106) Key name BTN_6

263 (0x107) Key name BTN_7

264 (0x108) Key name BTN_8

265 (0x109) Key name BTN_9

272 (0x110) Key name BTN_LEFT

273 (0x111) Key name BTN_RIGHT

274 (0x112) Key name BTN_MIDDLE

275 (0x113) Key name BTN_SIDE

276 (0x114) Key name BTN_EXTRA

277 (0x115) Key name BTN_FORWARD

278 (0x116) Key name BTN_BACK

279 (0x117) Key name BTN_TASK

288 (0x120) Key name BTN_TRIGGER

289 (0x121) Key name BTN_THUMB

290 (0x122) Key name BTN_THUMB2

291 (0x123) Key name BTN_TOP

292 (0x124) Key name BTN_TOP2

293 (0x125) Key name BTN_PINKIE

294 (0x126) Key name BTN_BASE

295 (0x127) Key name BTN_BASE2

296 (0x128) Key name BTN_BASE3

297 (0x129) Key name BTN_BASE4

298 (0x12a) Key name BTN_BASE5

299 (0x12b) Key name BTN_BASE6

303 (0x12f) Key name BTN_DEAD

304 (0x130) Key name BTN_A

305 (0x131) Key name BTN_B

306 (0x132) Key name BTN_C

307 (0x133) Key name BTN_X

308 (0x134) Key name BTN_Y

309 (0x135) Key name BTN_Z

310 (0x136) Key name BTN_TL

311 (0x137) Key name BTN_TR

312 (0x138) Key name BTN_TL2

313 (0x139) Key name BTN_TR2

314 (0x13a) Key name BTN_SELECT

315 (0x13b) Key name BTN_START

316 (0x13c) Key name BTN_MODE

317 (0x13d) Key name BTN_THUMBL

318 (0x13e) Key name BTN_THUMBR

320 (0x140) Key name BTN_TOOL_PEN

321 (0x141) Key name BTN_TOOL_RUBBER

322 (0x142) Key name BTN_TOOL_BRUSH

323 (0x143) Key name BTN_TOOL_PENCIL

324 (0x144) Key name BTN_TOOL_AIRBRUSH

325 (0x145) Key name BTN_TOOL_FINGER

326 (0x146) Key name BTN_TOOL_MOUSE

327 (0x147) Key name BTN_TOOL_LENS

330 (0x14a) Key name BTN_TOUCH

331 (0x14b) Key name BTN_STYLUS

332 (0x14c) Key name BTN_STYLUS2

333 (0x14d) Key name BTN_TOOL_DOUBLETAP

334 (0x14e) Key name BTN_TOOL_TRIPLETAP

335 (0x14f) Key name BTN_TOOL_QUADTAP

336 (0x150) Key name BTN_GEAR_DOWN

337 (0x151) Key name BTN_GEAR_UP

352 (0x160) Key name KEY_OK

353 (0x161) Key name KEY_SELECT

354 (0x162) Key name KEY_GOTO

355 (0x163) Key name KEY_CLEAR

356 (0x164) Key name KEY_POWER2

357 (0x165) Key name KEY_OPTION

358 (0x166) Key name KEY_INFO

359 (0x167) Key name KEY_TIME

360 (0x168) Key name KEY_VENDOR

361 (0x169) Key name KEY_ARCHIVE

362 (0x16a) Key name KEY_PROGRAM

363 (0x16b) Key name KEY_CHANNEL

364 (0x16c) Key name KEY_FAVORITES

365 (0x16d) Key name KEY_EPG

366 (0x16e) Key name KEY_PVR

367 (0x16f) Key name KEY_MHP

368 (0x170) Key name KEY_LANGUAGE

369 (0x171) Key name KEY_TITLE

370 (0x172) Key name KEY_SUBTITLE

371 (0x173) Key name KEY_ANGLE

372 (0x174) Key name KEY_ZOOM

373 (0x175) Key name KEY_MODE

374 (0x176) Key name KEY_KEYBOARD

375 (0x177) Key name KEY_SCREEN

376 (0x178) Key name KEY_PC

377 (0x179) Key name KEY_TV

378 (0x17a) Key name KEY_TV2

379 (0x17b) Key name KEY_VCR

380 (0x17c) Key name KEY_VCR2

381 (0x17d) Key name KEY_SAT

382 (0x17e) Key name KEY_SAT2

383 (0x17f) Key name KEY_CD

384 (0x180) Key name KEY_TAPE

385 (0x181) Key name KEY_RADIO

386 (0x182) Key name KEY_TUNER

387 (0x183) Key name KEY_PLAYER

388 (0x184) Key name KEY_TEXT

389 (0x185) Key name KEY_DVD

390 (0x186) Key name KEY_AUX

391 (0x187) Key name KEY_MP3

392 (0x188) Key name KEY_AUDIO

393 (0x189) Key name KEY_VIDEO

394 (0x18a) Key name KEY_DIRECTORY

395 (0x18b) Key name KEY_LIST

396 (0x18c) Key name KEY_MEMO

397 (0x18d) Key name KEY_CALENDAR

398 (0x18e) Key name KEY_RED

399 (0x18f) Key name KEY_GREEN

400 (0x190) Key name KEY_YELLOW

401 (0x191) Key name KEY_BLUE

402 (0x192) Key name KEY_CHANNELUP

403 (0x193) Key name KEY_CHANNELDOWN

404 (0x194) Key name KEY_FIRST

405 (0x195) Key name KEY_LAST

406 (0x196) Key name KEY_AB

407 (0x197) Key name KEY_NEXT

408 (0x198) Key name KEY_RESTART

409 (0x199) Key name KEY_SLOW

410 (0x19a) Key name KEY_SHUFFLE

411 (0x19b) Key name KEY_BREAK

412 (0x19c) Key name KEY_PREVIOUS

413 (0x19d) Key name KEY_DIGITS

414 (0x19e) Key name KEY_TEEN

415 (0x19f) Key name KEY_TWEN

416 (0x1a0) Key name KEY_VIDEOPHONE

417 (0x1a1) Key name KEY_GAMES

418 (0x1a2) Key name KEY_ZOOMIN

419 (0x1a3) Key name KEY_ZOOMOUT

420 (0x1a4) Key name KEY_ZOOMRESET

421 (0x1a5) Key name KEY_WORDPROCESSOR

422 (0x1a6) Key name KEY_EDITOR

423 (0x1a7) Key name KEY_SPREADSHEET

424 (0x1a8) Key name KEY_GRAPHICSEDITOR

425 (0x1a9) Key name KEY_PRESENTATION

426 (0x1aa) Key name KEY_DATABASE

427 (0x1ab) Key name KEY_NEWS

428 (0x1ac) Key name KEY_VOICEMAIL

429 (0x1ad) Key name KEY_ADDRESSBOOK

430 (0x1ae) Key name KEY_MESSENGER

431 (0x1af) Key name KEY_DISPLAYTOGGLE

432 (0x1b0) Key name KEY_SPELLCHECK

433 (0x1b1) Key name KEY_LOGOFF

434 (0x1b2) Key name KEY_DOLLAR

435 (0x1b3) Key name KEY_EURO

436 (0x1b4) Key name KEY_FRAMEBACK

437 (0x1b5) Key name KEY_FRAMEFORWARD

438 (0x1b6) Key name KEY_CONTEXT_MENU

439 (0x1b7) Key name KEY_MEDIA_REPEAT

448 (0x1c0) Key name KEY_DEL_EOL

449 (0x1c1) Key name KEY_DEL_EOS

450 (0x1c2) Key name KEY_INS_LINE

451 (0x1c3) Key name KEY_DEL_LINE

464 (0x1d0) Key name KEY_FN

465 (0x1d1) Key name KEY_FN_ESC

466 (0x1d2) Key name KEY_FN_F1

467 (0x1d3) Key name KEY_FN_F2

468 (0x1d4) Key name KEY_FN_F3

469 (0x1d5) Key name KEY_FN_F4

470 (0x1d6) Key name KEY_FN_F5

471 (0x1d7) Key name KEY_FN_F6

472 (0x1d8) Key name KEY_FN_F7

473 (0x1d9) Key name KEY_FN_F8

474 (0x1da) Key name KEY_FN_F9

475 (0x1db) Key name KEY_FN_F10

476 (0x1dc) Key name KEY_FN_F11

477 (0x1dd) Key name KEY_FN_F12

478 (0x1de) Key name KEY_FN_1

479 (0x1df) Key name KEY_FN_2

480 (0x1e0) Key name KEY_FN_D

481 (0x1e1) Key name KEY_FN_E

482 (0x1e2) Key name KEY_FN_F

483 (0x1e3) Key name KEY_FN_S

484 (0x1e4) Key name KEY_FN_B

497 (0x1f1) Key name KEY_BRL_DOT1

498 (0x1f2) Key name KEY_BRL_DOT2

499 (0x1f3) Key name KEY_BRL_DOT3

500 (0x1f4) Key name KEY_BRL_DOT4

501 (0x1f5) Key name KEY_BRL_DOT5

502 (0x1f6) Key name KEY_BRL_DOT6

503 (0x1f7) Key name KEY_BRL_DOT7

504 (0x1f8) Key name KEY_BRL_DOT8

505 (0x1f9) Key name KEY_BRL_DOT9

506 (0x1fa) Key name KEY_BRL_DOT10

512 (0x200) Key name KEY_NUMERIC_0

513 (0x201) Key name KEY_NUMERIC_1

514 (0x202) Key name KEY_NUMERIC_2

515 (0x203) Key name KEY_NUMERIC_3

516 (0x204) Key name KEY_NUMERIC_4

517 (0x205) Key name KEY_NUMERIC_5

518 (0x206) Key name KEY_NUMERIC_6

519 (0x207) Key name KEY_NUMERIC_7

520 (0x208) Key name KEY_NUMERIC_8

521 (0x209) Key name KEY_NUMERIC_9

522 (0x20a) Key name KEY_NUMERIC_STAR

523 (0x20b) Key name KEY_NUMERIC_POUND

524 (0x20c) Key name KEY_RFKILL
*/

/*────────────────────────────────────────────────────────────────────────────*/

#include <limits.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class input_t {
protected:

    struct image_t {
        int width, height;
        ptr_t<char> data;
    };

    struct NODE {
        XEvent   event; int state = 0;
    	array_t<uint> button, key;
        Display* dpy = nullptr;
        Screen*  scr = nullptr;
        Window   root;
        Window   win;
        int      id;
    };  ptr_t<NODE>  obj;

    ptr_t<float> screen_ref( const float& x, const float& y ) const noexcept{
        auto size = get_screen_size(); return {{
            x * size[0] / 100, y * size[1] / 100
        }};
	};

public: 

    event_t<uint>      onButtonRelease;
    event_t<uint>      onButtonPress;

    event_t<uint,uint> onMouseMotion;

    event_t<uint>      onKeyRelease;
    event_t<uint>      onKeyPress;

    event_t<except_t>  onError;
    event_t<>          onClose;
	
    /*─······································································─*/

    input_t() noexcept : obj( new NODE() ) {
        obj->dpy = XOpenDisplay( NULL );
        obj->id  = DefaultScreen( obj->dpy );
        obj->root= DefaultRootWindow( obj->dpy );
        obj->win = XRootWindow( obj->dpy, obj->id );
        obj->scr = DefaultScreenOfDisplay( obj->dpy );
    }
	
    /*─······································································─*/

    Display* get_Display(){ return obj->dpy; }
    void     set_Display( Display* dpy ){ obj->dpy = dpy; }

    Screen*  get_Screen(){ return obj->scr; }
    void     set_Screen( Screen* scr ){ obj->scr = scr; }

    XEvent&  get_XEvent(){ return obj->event; }
    void     set_XEvent( XEvent ev ){ obj->event = ev; }

    Window&  get_Window(){ return obj->win; }
    void     set_window( Window win ){ obj->win = win; }

    int&     get_ID(){ return obj->id; }
    void     set_ID( int id ){ obj->id = id; }
	
    /*─······································································─*/

	virtual ~input_t() noexcept { 
		if( obj.count() > 1 ){ return; } 
			force_close(); 
	}

    /*─······································································─*/

    void close() const noexcept { if( obj->state == -1 ){ return; } obj->state = -1; onClose.emit(); }

    bool is_closed() const noexcept { return obj==nullptr ? 1 : obj->state==-1; }

    virtual void force_close() const noexcept {
        if( obj->state == -1 ){ return; } obj->state = -1; 
        	XDestroyWindow( obj->dpy, obj->win ); 
			XCloseDisplay( obj->dpy );
    }

    /*─······································································─*/

    image_t take_screenshot() noexcept { auto size = this->get_screen_size();
        XImage *img = XGetImage( obj->dpy, obj->root, 0, 0, size[0], size[1], AllPlanes, ZPixmap );
        ptr_t<char> data ( img->bytes_per_line * img->height, 0 ); 
        memcpy( &data, img->data, data.size() );

        image_t image; 
                image.data   = data;
                image.width  = img->width;
                image.height = img->height;

        XDestroyImage(img); return image;
    }

    /*─······································································─*/

    string_t get_clipboard() const noexcept {
        Atom utf8String = XInternAtom( obj->dpy, "UTF8_STRING", 0 );
        Atom clipboard  = XInternAtom( obj->dpy, "CLIPBOARD"  , 0 );
        Atom type; int format; ulong length; uchar* data = nullptr;

        XGetWindowProperty( obj->dpy, obj->win, clipboard, 0, LONG_MAX, 0, utf8String, &type, &format, &length, &length, &data);

        if ( type == utf8String && format == 8 ) {
            string_t result = { (char*) data, length };
            XFree( data ); return result;
        } else { return nullptr; }
    }

    int set_clipboard( string_t msg ) const noexcept {
        Atom utf8String = XInternAtom( obj->dpy, "UTF8_STRING", 0 ); 
        Atom clipboard  = XInternAtom( obj->dpy, "CLIPBOARD"  , 0 );
        return XChangeProperty( obj->dpy, obj->win, clipboard, utf8String, 8, PropModeReplace, (uchar*)(msg.c_str()), msg.size() );
    }

    /*─······································································─*/

	ptr_t<int> get_screen_size() const noexcept { return {{ obj->scr->width, obj->scr->height }}; }

	int      get_screen_length() const noexcept { return XScreenCount( obj->dpy ); }

    /*─······································································─*/

    ptr_t<int> get_mouse_position() const noexcept { int rootX, rootY, winX, winY; uint mask; Window child;
        XQueryPointer( obj->dpy, obj->win, &obj->win, &child, &rootX, &rootY, &winX, &winY, &mask);
             return {{ winX, winY, rootX, rootY }};
    }

	void set_mouse_position( float x, float y ) const noexcept {
        auto sr = screen_ref( x, y ); ptr_t<float> r ({ sr[0], sr[1] });
		XTestFakeMotionEvent(obj->dpy,obj->id,r[0],r[1],CurrentTime);
		XFlush( obj->dpy );
	}

	void release_mouse_button( int btn ) const noexcept { 
		XTestFakeButtonEvent(obj->dpy,btn,0,CurrentTime);
		XFlush( obj->dpy );
	}

	void press_mouse_button( int btn ) const noexcept { 
		XTestFakeButtonEvent(obj->dpy,btn,1,CurrentTime);
		XFlush( obj->dpy );
	}

	void scroll_mouse_up() const noexcept {
		 XTestFakeButtonEvent(obj->dpy,4,1,CurrentTime);
		 XTestFakeButtonEvent(obj->dpy,4,0,CurrentTime);
		 XFlush( obj->dpy );
	}

	void scroll_mouse_down() const noexcept {
		 XTestFakeButtonEvent(obj->dpy,5,1,CurrentTime);
		 XTestFakeButtonEvent(obj->dpy,5,0,CurrentTime);
		 XFlush( obj->dpy );
	}

    /*─······································································─*/

	void release_key_button( int key ) const noexcept { 
		auto k = XKeysymToKeycode(obj->dpy,key);
		XTestFakeButtonEvent(obj->dpy,k,0,CurrentTime);
		XFlush( obj->dpy );
	}

	void press_key_button( int key ) const noexcept {
		auto k = XKeysymToKeycode(obj->dpy,key);
		XTestFakeButtonEvent(obj->dpy,k,1,CurrentTime);
		XFlush( obj->dpy );
	}

    /*─······································································─*/

    bool is_button_released( uint btn ) const noexcept {
        if( obj->button.empty() ) return 1; 
        else { for( auto x : obj->button ){
           if( x == btn ) return 0;
        }}                return 1;
    }

    bool is_button_pressed( uint btn ) const noexcept {
        if( obj->button.empty() ) return 0; 
        else { for( auto x : obj->button ){
           if( x == btn ) return 1;
        }}                return 0;
    }

    bool is_key_released( uint btn ) const noexcept {
        if( obj->key.empty() ) return 1; 
        else { for( auto x : obj->key ){
           if( x == btn ) return 0;
        }}                return 1;
    }

    bool is_key_pressed( uint btn ) const noexcept {
        if( obj->key.empty() ) return 0; 
        else { for( auto x : obj->key ){
           if( x == btn ) return 1;
        }}                return 0;
    }
    
    /*─······································································─*/

	void pipe(){ if( obj->state == 1 ){ return; } auto inp = type::bind( this );

        if( obj->dpy == NULL )
          { process::error( onError, "can't start X11 server" ); close(); return; }

        auto events = ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
                      KeyReleaseMask  | KeyPressMask      ;

        XSelectInput( obj->dpy, obj->win, events ); obj->state = 1;

        process::loop::add([=](){ 
        coStart 
		
        	while( XPending( inp->obj->dpy ) <= 0 ){ coNext; } 
                 XNextEvent( inp->obj->dpy, &inp->obj->event );

    /*─······································································─*/

            if( inp->obj->event.type == MotionNotify ) { 
                auto bt = inp->obj->event.xmotion;
                inp->onMouseMotion.emit( bt.x, bt.y ); 
            }

    /*─······································································─*/

            elif( inp->obj->event.type == ButtonRelease ) { 
                     auto bt = inp->obj->event.xbutton.button;
                for( ulong x=inp->obj->button.size(); x--; ){
                 if( inp->obj->button[x] == bt ) 
                   { inp->obj->button.erase(x); }
                }    inp->onButtonRelease.emit( bt ); 
            }

            elif( inp->obj->event.type == ButtonPress ) { 
                     auto bt = inp->obj->event.xbutton.button;
                for( ulong x=inp->obj->button.size(); x--; ){
                 if( inp->obj->button[x] == bt ){ return 1; }
                }    inp->obj->button.push( bt ); 
                     inp->onButtonPress.emit( bt );
            }

    /*─······································································─*/

            elif( inp->obj->event.type == KeyRelease ) { 
                     auto bt = inp->obj->event.xkey.keycode;
                for( ulong x=inp->obj->key.size(); x--; ){
                 if( inp->obj->key[x] == bt ) 
                   { inp->obj->key.erase(x); }
                }    inp->onKeyRelease.emit( bt ); 
            }

            elif( inp->obj->event.type == KeyPress ) { 
                     auto bt = inp->obj->event.xkey.keycode;
                for( ulong x=inp->obj->key.size(); x--; ){
                 if( inp->obj->key[x] == bt ){ return 1; }
                }    inp->obj->key.push( bt ); 
                     inp->onKeyPress.emit( bt );
            }

    /*─······································································─*/

            if( !inp->is_closed() ) coGoto(0);
			
		coStop 
        });

    }

};}

/*────────────────────────────────────────────────────────────────────────────*/