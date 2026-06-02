// eMemCard::DeleteList(void) static  @ 0x00055c78  (eAll_psp.obj)
// Emits: __0fIeMemCardKDeleteListvT
//
// Sets up a SceUtilitySavedataParam on the stack and kicks the delete-list
// save utility via eMemCardPlatform::DoSaveUtility.  Split-TU: the param
// struct is the game's (older/smaller, sizeof == 0x600) revision modelled
// here with exact field offsets; eMemCard is declared locally.

typedef unsigned int u32;

// --- helpers (declared locally; relocations are masked by compare_func) ---
extern "C" void *memset(void *, int, unsigned int);
extern "C" void *memcpy(void *, const void *, unsigned int);

void cGetLanguageAndButton(int *, int *);
char cGetParentalLevel(void);
const char *cGetProductId(void);
char *cStrCopy(char *, const char *, int);

// Game-revision SceUtilitySavedataParam (sizeof == 0x600 == 1536).
struct XSceUtilityParamBase
{
	u32 size;
	int message_lang;
	int ctrl_assign;
	int main_thread_priority;
	int sub_thread_priority;
	int font_thread_priority;
	int sound_thread_priority;
	int result;
	int reserved1;
	int reserved2;
	int reserved3;
	int reserved4;
};

struct XSceUtilitySavedataParam
{
	XSceUtilityParamBase base;       // 0   (48 bytes)
	int  type;                       // 48
	u32  bind;                       // 52
	u32  overWriteMode;              // 56
	char titleId[13];                // 60
	char reserved[3];                // 73
	char userId[20];                 // 76
	char pad0[1312];                 // 96  -> 1408
	char parental;                   // 1408
	char pad1[91];                   // 1409 -> 1500
	char secureFileId[16];           // 1500 -> 1516
	u32  dataVersion;                // 1516
	u32  mcStatus;                   // 1520
	char pad2[12];                   // 1524 -> 1536
};

class eMemCard
{
public:
	static void DeleteList(void);
};

class eMemCardPlatform
{
public:
	static void DoSaveUtility(XSceUtilitySavedataParam &);
};

#pragma control sched=1

void eMemCard::DeleteList(void)
{
	XSceUtilitySavedataParam param;
	int zeros[4];
	int langButton[2];

	memset(&param, 0, sizeof(param));
	memset(&param.base, 0, sizeof(param.base));

	cGetLanguageAndButton(&langButton[0], &langButton[1]);

	param.base.size = sizeof(param);
	__asm__ volatile("" ::: "memory");
	param.base.message_lang = langButton[0];
	__asm__ volatile("" ::: "memory");
	param.base.ctrl_assign = langButton[1];
	param.base.main_thread_priority = 0x11;
	param.base.sub_thread_priority = 0x13;
	param.base.font_thread_priority = 0x12;
	param.base.sound_thread_priority = 0x10;

	zeros[0] = 0;
	zeros[1] = 0;
	zeros[2] = 0;
	zeros[3] = 0;

	param.parental = cGetParentalLevel();
	param.mcStatus = 0;
	param.dataVersion = 0;
	memcpy(param.secureFileId, zeros, 16);

	cStrCopy(param.titleId, cGetProductId(), 13);
	cStrCopy(param.userId, 0, 20);

	param.type = 7;
	eMemCardPlatform::DoSaveUtility(param);
}
