#include <stdio.h> // sprintf()
#include <pspsdk.h>
#include <string.h> // strlen()
#include <pspctrl.h>
#include <pspkernel.h>
#include <psputility_sysparam.h> // lang

#include "libpspexploit.h"

PSP_MODULE_INFO("KeyTester_Yoti_and_Krazynez", 0, 4, 3);
PSP_MAIN_THREAD_ATTR(0);
PSP_HEAP_SIZE_KB(20480);

#define printf pspDebugScreenPrintf
#define printfc pspDebugScreenPrintfCenter
#define printfe pspDebugScreenPrintfEx
#define COLOR_BLACK 0xff000000
#define COLOR_WHITE 0xffFFFFFF
#define COLOR_GREY 0xff777777

// DEFAULT KEY: 1A04000
// LT         : 1A04100

static KernelFunctions _ktbl; KernelFunctions*k_tbl = &_ktbl;

int lang = 0; // язык
int mode = 1; // режим ofw
int home = 0; // kernel < 6

void pspDebugScreenPrintfEx(int x, int y, const char*text)
{
	int x_backup, y_backup;
	x_backup = pspDebugScreenGetX();
	y_backup = pspDebugScreenGetY();
	pspDebugScreenSetXY(x, y);
	printf(text);
	pspDebugScreenSetXY(x_backup, y_backup);
}

void pspDebugScreenPrintfCenter(int y, const char*text)
{
	int x, len, x_backup, y_backup;
	x_backup = pspDebugScreenGetX();
	y_backup = pspDebugScreenGetY();
	if ((strlen(text) % 2) == 0)
		len = strlen(text);
	else
		len = strlen(text)-1;
	x = (68 - len) / 2;
	pspDebugScreenSetXY(x, y);
	printf(text);
	pspDebugScreenSetXY(x_backup, y_backup);
}

void ExitTimerEx(void)
{
	int i = 0;
	pspDebugScreenSetTextColor(COLOR_BLACK);
	pspDebugScreenSetBackColor(COLOR_WHITE);
	for (i = 5; i > 0; i--)
	{
		if (lang == 8)
		{
			printfc(15, "                               ");
			printfc(16, " Завершено, выход через 5 сек. ");
			printfc(17, "                               ");
		}
		else
		{
			printfc(15, "                                 ");
			printfc(16, " All done, autoexit after 5 sec. ");
			printfc(17, "                                 ");
		}
		sceKernelDelayThread(1*1000*1000);
	}
	sceKernelExitGame();
}

int ReadFile(char*file, void*buff, int size)
{
	int ret = 0;

	SceUID fd = sceIoOpen(file, PSP_O_RDONLY, 0777);
	if (fd < 0)
		ret = fd;
	else
		ret = sceIoRead(fd, buff, size);
	sceIoClose(fd);

	return ret;
}

// --------------------------------------------------
void PrintScreen_L(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(2, 0, "/       |");
	printfe(2, 1, "|   L   |");
	printfe(2, 2, "|_______|");
}

void PrintScreen_R(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(58, 0, "|       \\");
	printfe(58, 1, "|   R   |");
	printfe(58, 2, "|_______|");
}

// --------------------------------------------------
void PrintScreen_Up(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(8, 12, "/   \\");
	printfe(8, 13, "| ^ |");
	printfe(8, 14, "\\   /");
}

void PrintScreen_Left(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(2, 15, "/   \\");
	printfe(2, 16, "| < |");
	printfe(2, 17, "\\   /");
}

void PrintScreen_Right(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(14, 15, "/   \\");
	printfe(14, 16, "| > |");
	printfe(14, 17, "\\   /");
}

void PrintScreen_Down(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(8, 18, "/   \\");
	printfe(8, 19, "| v |");
	printfe(8, 20, "\\   /");
}

// --------------------------------------------------
void PrintScreen_Triangle(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(56, 12, "/   \\");
	printfe(56, 13, "| \3 |");
	printfe(56, 14, "\\   /");
}

void PrintScreen_Square(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(50, 15, "/   \\");
	printfe(50, 16, "| \6 |");
	printfe(50, 17, "\\   /");
}

void PrintScreen_Circle(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(62, 15, "/   \\");
	printfe(62, 16, "| \4 |");
	printfe(62, 17, "\\   /");
}

void PrintScreen_Cross(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(56, 18, "/   \\");
	printfe(56, 19, "| \5 |");
	printfe(56, 20, "\\   /");
}

// --------------------------------------------------
void PrintScreen_Wlan(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(2, 24, "-");
	printfe(2, 25, " ");
	printfe(2, 26, "W");
	printfe(2, 27, "L");
	printfe(2, 28, "A");
	printfe(2, 29, "N");
	printfe(2, 30, " ");
	printfe(2, 31, "-");
}

void PrintScreen_Home(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(2, 33, "[ HOME ]");
}

void PrintScreen_VolDown(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(11, 33, "[ - ]");
}

void PrintScreen_VolUp(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(17, 33, "[ + ]");
}

void PrintScreen_Screen(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(35, 33, "[ \1 ]"); // 17
}

void PrintScreen_Note(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(41, 33, "[ \2 ]"); // 16
}

void PrintScreen_Select(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(47, 33, "[ SELECT ]");
}

void PrintScreen_Start(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(58, 33, "[ START ]");
}

void PrintScreen_Hold(unsigned int col1, unsigned int col2)
{
	pspDebugScreenSetTextColor(col1);
	pspDebugScreenSetBackColor(col2);
	printfe(66, 24, "-");
	printfe(66, 25, " ");
	printfe(66, 26, "H");
	printfe(66, 27, "O");
	printfe(66, 28, "L");
	printfe(66, 29, "D");
	printfe(66, 30, " ");
	printfe(66, 31, "-");
}

void PrintScreen(SceCtrlData Key)
{
	// --------------------------------------------------
	if (Key.Buttons & PSP_CTRL_LTRIGGER)
		PrintScreen_L(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_L(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_RTRIGGER)
		PrintScreen_R(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_R(COLOR_WHITE, COLOR_BLACK);

	// --------------------------------------------------
	if (Key.Buttons & PSP_CTRL_UP)
		PrintScreen_Up(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Up(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_LEFT)
		PrintScreen_Left(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Left(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_RIGHT)
		PrintScreen_Right(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Right(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_DOWN)
		PrintScreen_Down(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Down(COLOR_WHITE, COLOR_BLACK);

	// --------------------------------------------------
	if (Key.Buttons & PSP_CTRL_TRIANGLE)
		PrintScreen_Triangle(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Triangle(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_CIRCLE)
		PrintScreen_Circle(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Circle(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_CROSS)
		PrintScreen_Cross(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Cross(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_SQUARE)
		PrintScreen_Square(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Square(COLOR_WHITE, COLOR_BLACK);

	// --------------------------------------------------
	if (Key.Buttons & PSP_CTRL_WLAN_UP)
		PrintScreen_Wlan(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Wlan(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_HOME)
		PrintScreen_Home(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Home(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_VOLUP)
		PrintScreen_VolUp(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_VolUp(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_VOLDOWN)
		PrintScreen_VolDown(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_VolDown(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_SCREEN)
		PrintScreen_Screen(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Screen(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_NOTE)
		PrintScreen_Note(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Note(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons &  PSP_CTRL_SELECT)
		PrintScreen_Select(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Select(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_START)
		PrintScreen_Start(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Start(COLOR_WHITE, COLOR_BLACK);

	if (Key.Buttons & PSP_CTRL_HOLD)
		PrintScreen_Hold(COLOR_BLACK, COLOR_WHITE);
	else
		PrintScreen_Hold(COLOR_WHITE, COLOR_BLACK);

	// --------------------------------------------------
	// PSP_CTRL_REMOTE = 0x00080000
	// PSP_CTRL_DISC = 0x01000000
	// PSP_CTRL_MS = 0x02000000
}

void PrintScreen_Pad(SceCtrlData Key)
{
	pspDebugScreenSetTextColor(COLOR_WHITE);
	pspDebugScreenSetBackColor(COLOR_BLACK);

	int dead = 16; // 256/16
	char*text="";
	int x = Key.Lx;
	int y = Key.Ly;
	int x_coord = 0;
	int y_coord = 0;

	printfc(20, "+-----+");
	printfc(21, "|     |");
	printfc(22, "|     |");
	printfc(23, "|     |");
	printfc(24, "|     |");
	printfc(25, "|     |");
	printfc(26, "+-----+");

	if ((x >= 127 - dead) && (x <= 127 + dead))
		x_coord = 0;
	else if ((x >= 63) && (x <= 127))
		x_coord = -1;
	else if ((x >= 0) && (x <= 63))
		x_coord = -2;
	else if ((x >= 127) && (x <= 191))
		x_coord = 1;
	else if ((x >= 191) && (x <= 255))
		x_coord = 2;

	if ((y >= 127 - dead) && (y <= 127 + dead))
		y_coord = 0;
	else if ((y >= 63) && (y <= 127))
		y_coord = -1;
	else if ((y >= 0) && (y <= 63))
		y_coord = -2;
	else if ((y >= 127) && (y <= 191))
		y_coord = 1;
	else if ((y >= 191) && (y <= 255))
		y_coord = 2;

	printfe(34 + x_coord, 23 + y_coord, "\17");
	sprintf(text, "%03d/%03d", x, y);
	printfc(27, text);
}

int (*_sceCtrlReadBufferPositive)(SceCtrlData *pad, int count) = NULL;
int (*_sceCtrlSetSamplingCycle)(int) = NULL;
int (*_sceCtrlSetSamplingMode)(int) = NULL;
int prxCtrlReadBufferPositive(SceCtrlData*pad_data, int count)
{
	_sceCtrlSetSamplingCycle(0);
	_sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG); // без этого в кастоме нет отслеживания аналога
	int ret = _sceCtrlReadBufferPositive(pad_data, count);

	return ret;
}
int kthread() {

	SceCtrlData Pad;
	while(1)
	{
		prxCtrlReadBufferPositive(&Pad, 1);
		PrintScreen(Pad);
		PrintScreen_Pad(Pad);
		//printf("0x%8X\n", Pad.Buttons);

		if ((Pad.Buttons & PSP_CTRL_LTRIGGER) && (Pad.Buttons & PSP_CTRL_RTRIGGER) && (Pad.Buttons & PSP_CTRL_START))
			break;
		k_tbl->KernelDelayThread(.15 * 1000*1000); // 0.15
	}

}

int kmain() {

	int k1 = pspSdkSetK1(0);
	int ul = pspXploitSetUserLevel(8);

	pspXploitRepairKernel();
	pspXploitScanKernelFunctions(k_tbl);


	_sceCtrlReadBufferPositive = pspXploitFindFunction("sceController_Service", "sceCtrl", 0x1F803938); 
	if(!_sceCtrlReadBufferPositive)
		printf("ERR: can't find _sceCtrlReadBufferPositive\n");
	_sceCtrlSetSamplingCycle = pspXploitFindFunction("sceController_Service", "sceCtrl", 0x6A2774F3);
	_sceCtrlSetSamplingMode = pspXploitFindFunction("sceController_Service", "sceCtrl", 0x1F4011E6);



	SceUID kthreadID = k_tbl->KernelCreateThread("keystester_thread", (void*)KERNELIFY(&kthread), 1, 0x20000, PSP_THREAD_ATTR_VFPU, NULL);
	if (kthreadID >= 0){
		k_tbl->KernelStartThread(kthreadID, 0, NULL);
		k_tbl->waitThreadEnd(kthreadID, NULL);
	}

	pspXploitSetUserLevel(ul);
	pspSdkSetK1(k1);

}

int main(int argc, char*argv[])
{

	pspDebugScreenInit();
	pspDebugScreenClear(); // особо не нужно
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &lang);

	
	

	if (sceKernelDevkitVersion() >= 0x06000010)
		home = 1; // k6+

	pspDebugScreenSetTextColor(COLOR_GREY);
	pspDebugScreenSetBackColor(COLOR_BLACK);
	if (lang == 8)
		printfc(15, "Key[s]Tester V5 от Yoti & Krazynez");
	else
		printfc(15, "Key[s]Tester V5 by Yoti and Krazynez");
	if (lang == 8)
		printfc(17, "L + R + START = выход");
	else
		printfc(17, "L + R + START = exit!");
	if (mode == 1)
	{
		pspDebugScreenSetTextColor(COLOR_WHITE);
		pspDebugScreenSetBackColor(COLOR_BLACK);
		if (lang == 8)
			printfc(16, "= кастомный режим =");
		else
			printfc(16, "= custom mode =");
	}

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	int ret = pspXploitInitKernelExploit();
	if(ret == 0) {
		ret = pspXploitDoKernelExploit();
		if(ret == 0)
			pspXploitExecuteKernel(kmain);
	}
	

	ExitTimerEx();
	return 0;
}
