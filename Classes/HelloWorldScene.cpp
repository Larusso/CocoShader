#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "GPUTileMap.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	FileUtils::getInstance()->addSearchPath("res/shader", false);

	auto director = Director::getInstance();

    //auto background = CSLoader::createNode("MainScene.csb");
	//addChild(background);
	auto rootNode = GPUTileMap::create("testMap.json");
	auto winsize = director->getWinSize();
	//rootNode->setContentSize(director->getWinSize());
	rootNode->setContentSize(Size(200,300));
	//rootNode->setMapOffset(Vec2(20,100));
	//rootNode->setPosition((winsize.width - 200)/2, (winsize.height - 300) / 2);
    addChild(rootNode);
	auto moveAction = MoveBy::create(10.0, winsize / 4);
	rootNode->runAction(RepeatForever::create(Sequence::createWithTwoActions(moveAction, moveAction->reverse())));

    return true;
}
