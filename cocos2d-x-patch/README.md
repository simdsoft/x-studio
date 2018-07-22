# The cocos2d-x patch, support cocos2d-x 3.15 or later.
**Features:**
1. x-studio365 runtime extension ui loader (based on cocostudio, compatible with cocostudio).
2. Refactor audio engine module, replace backend with OpenAL for android platform.
3. Fix some audio engine issue, delay execute audio finish callback(When application play a new audio at the callback, my cause crash).
4. Integrate the dragonbonse 5.5 runtime.

**Usage:**
1.Delete folder cocos/audio/android of your project.  
2.Copy all folders of this directory to your project & overrite.
