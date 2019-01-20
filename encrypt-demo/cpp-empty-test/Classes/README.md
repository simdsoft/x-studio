**This runitime now compatible with cocos2d-x-3.10 or later**

**Know issue:**
 All cocos2d-x version, for apple platform, if you use FileUtils::getValueVectorFromFile, you can't encrypt that file, because  
getValueVectorFromFile at apple platfom use object-c API to read file data, not FileUtils::getStringFormFile, so the runtime no chance to decrypt  
your encrypted file.  
In the future, cocos2d-x engine or this runtime may be cover & fix this issue.  
I very Suggestion official engine team use the same implement getValueMapFromFile, getValueVectorFromFile as same for all platforms.  
Also, other suggestion, deprecated tinyxml, use more fast xml parsers rapidxml/pugixml to instead.
