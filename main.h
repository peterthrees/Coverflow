
enum currentState { EmulatorSelectState, CoverFlowState };

SDL_Surface screen;

/* Init classes */
Configuration _config;
FileHandler _fileHandler;
CoverFlowOnScreen _CoverFlowOnScreen;	
Helper _helper;

int quiteCoverFlow;
//int FrameStartTimeMs = 0;
SDL_Event event;
int FPS = 60;
CTime gametime;
//int global_data = 0;
bool pauseTread = false;
SDL_Thread *thread;
int threadDelay = 5;
int currentThreadDelay = 5;
string emulatorType = "NES";
string screenType = "EMULATOR";

CoverFlowOnScreen::coverFlowState _CFState = CoverFlowOnScreen::EMULATOR;

int main(int argc, char** argv);
void Init(int posX, int posY);
void InitEmulator();
void InitConsole();
void CheckController();
int ThreadLoadImages(void *unused);
void Quit();




