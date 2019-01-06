#include "AppDelegate.h"

#include <vector>
#include <string>

#include "HelloWorldScene.h"
#include "AppMacros.h"

//Uncomment the following line to use localize manager
//#include "editor-support/cocostudio/LocalizationManager.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
#if 0
    unsigned char ivec[] = { 0xfd,0xf0,0x93,0x6a,0xe1,0x7f,0x48,0x2d,0xa7,0xd7,0x65,0x64,0xf6,0x3e,0xb7,0x88,};
    unsigned char secret[] = { 0x70,0xbf,0x4d,0xce,0xa3,0x38,0xc6,0xf0,0x17,0xf8,0xa7,0xee,0x4c,0xd7,0x76,0x73,0x91,0x68,0x2f,0x50,0x42,0x2a,0x06,0x47,0xcf,0x92,0x41,0x4a,0x1d,0x1c,0xfa,0x16, };

    EncryptManager::getInstance()->setEncryptEnabled(true,
                                                     std::string_view((char*)secret, sizeof(secret)),
                                                     std::string_view((char*)ivec, sizeof(ivec)),
                                                     EncryptManager::ENCF_SIGNATURE | 5 << 16);
#endif
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Cpp Empty Test");
        director->setOpenGLView(glview);
    }


    director->setOpenGLView(glview);
    glview->setFrameSize(1024, 768);
    // Set the design resolution
    glview->setDesignResolutionSize(1024, 768, ResolutionPolicy::SHOW_ALL);

	Size frameSize = glview->getFrameSize();
    
    vector<string> searchPath;

#if defined(_WIN32)
    char appPath[512];
    GetModuleFileNameA(GetModuleHandleA(NULL), appPath, 512);
    auto n = strlen(appPath);
    char* last_slash = nullptr;
    for (auto i = n - 1; i > 0; --i) {
        if (appPath[i] == '\\' || appPath[i] == '/') {
            last_slash = &appPath[i];
            break;
        }
    }
    if (last_slash) (*last_slash) = '\0';
    searchPath.push_back(appPath);
#endif

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

#if 0
    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{
        searchPath.push_back(largeResource.directory);

        director->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(mediumResource.directory);
        
        director->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPath.push_back(smallResource.directory);

        director->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
#endif

    // set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);

    // Uncomment follow block to use localize manager to set localize strings

    //  If you want to load json localize data, use follow block
    /*
    cocostudio::ILocalizationManager * lm = cocostudio::JsonLocalizationManager::getInstance();
    lm->initLanguageData("your localize file name.lang.json");
    cocostudio::LocalizationHelper::setCurrentManager(lm, false);
    */

    //  If you want to load binary localize data, use follow block
    /*
    cocostudio::ILocalizationManager * lm = cocostudio::BinLocalizationManager::getInstance();
    lm->initLanguageData("your localize file name.lang.csb");
    cocostudio::LocalizationHelper::setCurrentManager(lm, true);
    */

    // to enable VR, uncomment the following lines
//    auto vrImpl = new VRGenericRenderer;
//    glview->setVR(vrImpl);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::scene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
