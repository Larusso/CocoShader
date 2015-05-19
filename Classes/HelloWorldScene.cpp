#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
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
    
    //auto rootNode = CSLoader::createNode("MainScene.csb");
	auto rootNode = GPUTileMap::create("testMap.json");
	rootNode->setContentSize(this->getContentSize());
    addChild(rootNode);
    
    //Sprite* ground = rootNode->getChildByName<Sprite *>("map_ground");
    //setCustomShader(ground);

	//ground->runAction(MoveBy::create(10.0, Vec2(20,20)));
    return true;
}

void HelloWorld::setCustomShader(Sprite *node)
{
    printf("add shader");
    
    //create my own program
    auto program = new GLProgram;
    program->initWithFilenames("map.vs", "map.fs");
    program->link();
    //set uniform locations
    program->updateUniforms();
    
    
    //    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    node->setGLProgram(program);
}
