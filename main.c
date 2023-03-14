#include <wiiuse.h>
#include <stdio.h>
#include <stdbool.h>

#define WIIMOTE_PATH "~/sys/bus/hid/devices/0005:057E:0306.0001"
#define MAX_WIIMOTES 1
// struct xwii_iface* dev;

// source: wiiuse example code by Michael Laforest
//  https://github.com/wiiuse/wiiuse
/**
 *	@brief Callback that handles an event.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *
 *	This function is called automatically by the wiiuse library when an
 *	event occurs on the specified wiimote.
 */
void handle_event(struct wiimote_t* wm) {
	printf("\n\n--- EVENT [id %i] ---\n", wm->unid);

	/* if a button is pressed, report it */
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)) {
		printf("A pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_B)) {
		printf("B pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
		printf("UP pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN))	{
		printf("DOWN pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT))	{
		printf("LEFT pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT))	{
		printf("RIGHT pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS))	{
		printf("MINUS pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS))	{
		printf("PLUS pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE)) {
		printf("ONE pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO)) {
		printf("TWO pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME))	{
		printf("HOME pressed\n");
	}

	/*
	 *	Pressing minus will tell the wiimote we are no longer interested in movement.
	 *	This is useful because it saves battery power.
	 */
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) {
		wiiuse_motion_sensing(wm, 0);
	}

	/*
	 *	Pressing plus will tell the wiimote we are interested in movement.
	 */
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) {
		wiiuse_motion_sensing(wm, 1);
	}

	/*
	 *	Pressing B will toggle the rumble
	 *
	 *	if B is pressed but is not held, toggle the rumble
	 */
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_B)) {
		wiiuse_toggle_rumble(wm);
	}

	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
		wiiuse_set_ir(wm, 1);
	}
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_DOWN)) {
		wiiuse_set_ir(wm, 0);
	}

	/*
	 * Motion+ support
	 */
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_ONE)) {
		if (WIIUSE_USING_EXP(wm)) {
			wiiuse_set_motion_plus(wm, 2);    // nunchuck pass-through
		} else {
			wiiuse_set_motion_plus(wm, 1);    // standalone
		}
	}

	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_TWO)) {
		wiiuse_set_motion_plus(wm, 0); // off
	}

	/* if the accelerometer is turned on then print angles */
	if (WIIUSE_USING_ACC(wm)) {
		printf("wiimote roll  = %f [%f]\n", wm->orient.roll, wm->orient.a_roll);
		printf("wiimote pitch = %f [%f]\n", wm->orient.pitch, wm->orient.a_pitch);
		printf("wiimote yaw   = %f\n", wm->orient.yaw);
	}

	/*
	 *	If IR tracking is enabled then print the coordinates
	 *	on the virtual screen that the wiimote is pointing to.
	 *
	 *	Also make sure that we see at least 1 dot.
	 */
	if (WIIUSE_USING_IR(wm)) {
		int i = 0;

		/* go through each of the 4 possible IR sources */
		for (; i < 4; ++i) {
			/* check if the source is visible */
			if (wm->ir.dot[i].visible) {
				printf("IR source %i: (%u, %u)\n", i, wm->ir.dot[i].x, wm->ir.dot[i].y);
			}
		}

		printf("IR cursor: (%u, %u)\n", wm->ir.x, wm->ir.y);
		printf("IR z distance: %f\n", wm->ir.z);
	}

	/* show events specific to supported expansions */
	if (wm->exp.type == EXP_NUNCHUK || wm->exp.type == EXP_MOTION_PLUS_NUNCHUK) {
		/* nunchuk */
		struct nunchuk_t* nc = (nunchuk_t*)&wm->exp.nunchuk;

		if (IS_PRESSED(nc, NUNCHUK_BUTTON_C)) {
			printf("Nunchuk: C pressed\n");
		}
		if (IS_PRESSED(nc, NUNCHUK_BUTTON_Z)) {
			printf("Nunchuk: Z pressed\n");
		}

		printf("nunchuk roll  = %f\n", nc->orient.roll);
		printf("nunchuk pitch = %f\n", nc->orient.pitch);
		printf("nunchuk yaw   = %f\n", nc->orient.yaw);

		printf("nunchuk joystick angle:     %f\n", nc->js.ang);
		printf("nunchuk joystick magnitude: %f\n", nc->js.mag);

		printf("nunchuk joystick vals:      %f, %f\n", nc->js.x, nc->js.y);
		printf("nunchuk joystick calibration (min, center, max): x: %i, %i, %i  y: %i, %i, %i\n",
		    nc->js.min.x,
		    nc->js.center.x,
		    nc->js.max.x,
		    nc->js.min.y,
		    nc->js.center.y,
		    nc->js.max.y);
	} else if (wm->exp.type == EXP_CLASSIC) {
		/* classic controller */
		struct classic_ctrl_t* cc = (classic_ctrl_t*)&wm->exp.classic;

		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_ZL)) {
			printf("Classic: ZL pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_B)) {
			printf("Classic: B pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_Y)) {
			printf("Classic: Y pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_A)) {
			printf("Classic: A pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_X)) {
			printf("Classic: X pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_ZR)) {
			printf("Classic: ZR pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_LEFT)) {
			printf("Classic: LEFT pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_UP)) {
			printf("Classic: UP pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_RIGHT)) {
			printf("Classic: RIGHT pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_DOWN)) {
			printf("Classic: DOWN pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_FULL_L)) {
			printf("Classic: FULL L pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_MINUS)) {
			printf("Classic: MINUS pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_HOME)) {
			printf("Classic: HOME pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_PLUS)) {
			printf("Classic: PLUS pressed\n");
		}
		if (IS_PRESSED(cc, CLASSIC_CTRL_BUTTON_FULL_R)) {
			printf("Classic: FULL R pressed\n");
		}

		printf("classic L button pressed:         %f\n", cc->l_shoulder);
		printf("classic R button pressed:         %f\n", cc->r_shoulder);
		printf("classic left joystick angle:      %f\n", cc->ljs.ang);
		printf("classic left joystick magnitude:  %f\n", cc->ljs.mag);
		printf("classic right joystick angle:     %f\n", cc->rjs.ang);
		printf("classic right joystick magnitude: %f\n", cc->rjs.mag);
	} else if (wm->exp.type == EXP_GUITAR_HERO_3) {
		/* guitar hero 3 guitar */
		struct guitar_hero_3_t* gh3 = (guitar_hero_3_t*)&wm->exp.gh3;

		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_STRUM_UP)) {
			printf("Guitar: Strum Up pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_STRUM_DOWN))	{
			printf("Guitar: Strum Down pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_YELLOW)) {
			printf("Guitar: Yellow pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_GREEN)) {
			printf("Guitar: Green pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_BLUE)) {
			printf("Guitar: Blue pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_RED)) {
			printf("Guitar: Red pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_ORANGE)) {
			printf("Guitar: Orange pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_PLUS)) {
			printf("Guitar: Plus pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_MINUS)) {
			printf("Guitar: Minus pressed\n");
		}

		printf("Guitar whammy bar:          %f\n", gh3->whammy_bar);
		printf("Guitar joystick angle:      %f\n", gh3->js.ang);
		printf("Guitar joystick magnitude:  %f\n", gh3->js.mag);
	} else if (wm->exp.type == EXP_WII_BOARD) {
		/* wii balance board */
		struct wii_board_t* wb = (wii_board_t*)&wm->exp.wb;
		float total = wb->tl + wb->tr + wb->bl + wb->br;
		float x = ((wb->tr + wb->br) / total) * 2 - 1;
		float y = ((wb->tl + wb->tr) / total) * 2 - 1;
		printf("Weight: %f kg @ (%f, %f)\n", total, x, y);
		printf("Interpolated weight: TL:%f  TR:%f  BL:%f  BR:%f\n", wb->tl, wb->tr, wb->bl, wb->br);
		printf("Raw: TL:%d  TR:%d  BL:%d  BR:%d\n", wb->rtl, wb->rtr, wb->rbl, wb->rbr); 
	}

	if (wm->exp.type == EXP_MOTION_PLUS ||
	        wm->exp.type == EXP_MOTION_PLUS_NUNCHUK) {
		printf("Motion+ angular rates (deg/sec): pitch:%03.2f roll:%03.2f yaw:%03.2f\n",
		       wm->exp.mp.angle_rate_gyro.pitch,
		       wm->exp.mp.angle_rate_gyro.roll,
		       wm->exp.mp.angle_rate_gyro.yaw);
	}
}

// source: wiiuse example code by Michael Laforest
//  https://github.com/wiiuse/wiiuse
short any_wiimote_connected(wiimote** wm, int wiimotes) {
	int i;
	if (!wm) {
		return 0;
	}

	for (i = 0; i < wiimotes; i++) {
		if (wm[i] && WIIMOTE_IS_CONNECTED(wm[i])) {
			return 1;
		}
	}

	return 0;
}

int main()
{
    printf("Hello World!\n");

    wiimote** wiimotes;
    int found, connected;

    wiimotes = wiiuse_init(MAX_WIIMOTES);


    found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
	if (!found) {
		printf("No wiimotes found.\n");
		return 0;
	}


    connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
	if (connected) {
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	} else {
		printf("Failed to connect to any wiimote.\n");
		return 0;
	}
    
    wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);

    while(any_wiimote_connected(wiimotes, MAX_WIIMOTES)) {
        if(wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
            

            for(int i = 0; i < MAX_WIIMOTES; i++) {
                switch (wiimotes[i]->event) {
					case WIIUSE_EVENT:
						/* a generic event occurred */
						handle_event(wiimotes[i]);
						break;

					default:
						break;
                }
            }
        }
    }

    wiiuse_cleanup(wiimotes, MAX_WIIMOTES);

    return 0;
}