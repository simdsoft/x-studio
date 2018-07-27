# The cocos2d-x patch, support cocos2d-x 3.15 or later.
**Features:**
1. x-studio365 runtime extension ui loader (based on cocostudio, compatible with cocostudio).
2. Refactor audio engine module, replace backend with OpenAL for android and linux. Unify win32,android,linux framework implementation sources.
3. Fix some audio engine issue, delay execute audio finish callback(When application play a new audio at the callback, may cause crash)
4. Support play wav sound file.
5. Integrate the dragonbonse 5.5 runtime.

**Install Full:**
1. Delete folder ```cocos/audio``` of your project.  
2. Copy all folders of this directory to your project & overrite.

**Install Audio:**
1. Delete folder ```cocos/audio``` of your project. 
2. Copy the folder ```external/android-specific``` to your project at the same location.
