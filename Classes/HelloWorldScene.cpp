#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

int MapX = 256, MapY = 256, Offset = 1;

float PaddleStartSpeed = 5;
float BallStartSpeed = 2;
float PaddleStartWidth = 100;


void getRandom(int &x, int &y){
	x = rand() % MapX, y = rand() % MapY;
}

bool Brick::init(){
	if (!Layer::init()){
		return false;
	}
	return true;
}

void Brick::reset(float _x, float _y, float _w, float _h, int _t = 0){
	x = _x, y = _y, h = _h, w = _w, type = _t;
	setPosition(x + w / 2, y + h / 2);

	addChild(Sprite::create("gray.png", Rect(0, 0, w, h)));

	auto t = DrawNode::create();
	Point lt = Point(-w/2, h/2), rt = Point(w/2, h/2),
		lb = Point(-w/2, -h/2), rb = Point(w/2,-h/2);
	t->drawSegment(lb, rb, 0.5, Color4F(1, 1, 1, 1));
	t->drawSegment(lb, lt, 0.5, Color4F(1, 1, 1, 1));
	t->drawSegment(lt, rt, 0.5, Color4F(1, 1, 1, 1));
	t->drawSegment(rb, rt, 0.5, Color4F(1, 1, 1, 1));
	addChild(t);
}

bool Bricks::init(){
	if (!Layer::init()){
		return false;
	}
	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 8; ++j){
			Brick* t = Brick::create();
			t->reset(j*32, 250-i*8, 32, 8);
			body.push_back(t);
			addChild(t);
		}
	}

	return true;
}

bool Ball::move(){
	if (attached) return 1;

	x += dx; y += dy;
	body->runAction(MoveBy::create(0.05, Point(dx, dy)));
	if (dy < 0 && y <= r / 2) return 0;

	if (dx < 0 && x <= r/2) dx = -dx;
	if (dx > 0 && x > MapX - r/2) dx = -dx;
	if (dy > 0 && y > MapY - r/2) dy = -dy;
	return 1;
}

void Ball::reset(float _x, float _y, float _r = 4){
	r = _r; x = _x + r/2, y = _y + r/2 + 4;
	body = Sprite::create("yellow.png", Rect(0, 0, r, r));
	body->setPosition(x, y);
	this->addChild(body);
}

bool Ball::init(){
	if (!Layer::init()){
		return false;
	}
	r = 4; x = r / 2; y = r/2 + 4; attached = 1;
	dx = (M_PI/4)*BallStartSpeed, dy = sin(M_PI/4)*BallStartSpeed;
	return true;
}

void Ball::setReflection(float alpha){
	float d = sqrt(dx*dx + dy*dy);
	dx = d*cos(alpha);
	dy = d*sin(alpha);
}

void Paddle::bounce(Ball* ball){ // 挡板的碰撞
	if (ball->dy < 0 && ball->y <= ball->r/2+4 && x - width/2 <= ball->x && ball->x <= x + width/2){
		ball->setReflection((M_PI/3)*(x-ball->x)/(width/2) + M_PI/2);
	}
}

void Paddle::move(){ // 处理移动。

	if (dir == -1){
		ddx = -0.2f;
	}
	else if (dir == 1){
		ddx = 0.2f;
	}
	else{
		ddx = 0;
	}

	if (fabs(dx + ddx) <= mdx) dx += ddx;

	if (dx >= 0){
		float d = min(dx, 0.1f);
		ddx += -d;
	}
	else if (dx <= 0){
		float d = min(-dx, 0.1f);
		ddx += d;
	}

	if (fabs(dx + ddx) <= mdx) dx += ddx;


	if (x + dx <= width / 2 || x + width / 2 + dx > MapX) dx = 0;
	x += dx; body->runAction(MoveBy::create(0.05, Point(dx, 0)));

	for each(auto t in attach){
		t->x += dx;
		t->runAction(MoveBy::create(0.05, Point(dx, 0)));
	}
}

bool Paddle::init(){
	if (!Layer::init()){
		return false;
	}
	dir = 0;
	ddx = dx = 0;
	mdx = PaddleStartSpeed; width = PaddleStartWidth;

	x = width / 2;
	//setPosition(Offset,0);
	//setPosition(width/2 + 0.5, 2 + 0.5);
	body = Sprite::create("white.png", Rect(0, 0, width, 4));
	body->setPosition(x + 0.5, 2 + 0.5);

	this->addChild(body);
	return true;
}


bool Ball::bounce(Brick* b){ 

	CCLOG("%f %f %f %f\n", x, y, b->x, b->y);

	
	if (b->x <= x+r 
		&& x-r <= b->x + b->w && 		
		b->y <= y+r
		&& y-r <= b->y + b->h ){
		dx = -dx; dy = -dy;
		return 1;
	}

	/*if (b->x-b->w <= x+r && x-r<=b->x+b->w && b->y-b->h <= y+r && y-r <= b->y+b->h){
		dx = -dx; dy = -dy;
		return 1;
	}*/
	

	/*if (b->x - b->w <= x + r && x - r <= b->x + b->w && b->y - b->h <= y + r && y - r <= b->y + b->y){
		dx = -dx; dy = -dy;
		return 1;
	}*/
	return 0;
}

void Ball::bounce(Bricks* bs){
	
	/*for each(auto t in bs->body){
		if (bounce(t)){
			bs->removeChild(t);
			bs->body.slice(t);
		}
	}*/

	/*for (vector<Brick*>::iterator it = bs->body.begin(), jt; it != bs->body.end(); it = jt){
		jt = it; ++jt;
		if (bounce(*it)){
			bs->removeChild(*it);
			//bs->body.erase(it);
		}
	}*/

	for (int i = 0; i < bs->body.size();){
		if (bounce(bs->body[i])){
			bs->removeChild(bs->body[i]);
			bs->body.erase(bs->body.begin() + i);
		}
		else{
			++i;
		}
	}
}

void Game::gameLogic(float t){
	pad->move(); pad->bounce(ball);

	if (!ball->move()){
		Label* t = (Label *)getChildByTag(2);
		t->setVisible(1);
		Director::getInstance()->pause();
	}

	ball->bounce(bricks);
}

float cur, aim;

void backgroundMusicDieDown(){

}

void Game::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){

	//CCLOG("Key with keycode %d pressed", keyCode);

	if (keyCode == EventKeyboard::KeyCode::KEY_A){
		if (pad->dir == -1) pad->dir = 0;
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_D){
		if (pad->dir == 1) pad->dir = 0;
	}
}




void Game::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){

	//CCLOG("Key with keycode %d pressed", keyCode);

	if (keyCode == EventKeyboard::KeyCode::KEY_A){
		pad->dir = -1;
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_D){
		pad->dir = 1;
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE){


		//SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(t);

			//playBackgroundMusic("yumeorugo_arr2.wav", true);

		for each(auto t in pad->attach){
			t->attached = 0;
		}
		pad->attach.clear();
	}
}
Scene* Game::scene(){
	auto scene = Scene::create();
	Game *layer = Game::create();
	scene->addChild(layer);
	return scene;
}

bool Game::init(){
	if (!Layer::init()){
		return false;
	}

	auto size = Director::getInstance()->getWinSize();
	setPosition(Point(40, 45));

	

	// 四壁

	auto wall = DrawNode::create();

	Point lt = Point(-Offset, MapY+Offset), rt = Point(MapX+Offset, MapY+Offset),
		lb = Point(-Offset, -Offset), rb = Point(MapX+Offset, -Offset);
	
	wall->drawSegment(lb, rb, 1, Color4F(1, 1, 1, 1));
	wall->drawSegment(lb, lt, 1, Color4F(1, 1, 1, 1));
	wall->drawSegment(lt, rt, 1, Color4F(1, 1, 1, 1));
	wall->drawSegment(rb, rt, 1, Color4F(1, 1, 1, 1));
	addChild(wall);

	pad = Paddle::create(); addChild(pad);
	ball = Ball::create(); ball->reset(pad->width/3, 0); addChild(ball); 
	pad->attach.push_back(ball);

	bricks = Bricks::create(); addChild(bricks);

	auto labelScore = Label::create("Score: 0", "宋体", 14);
	labelScore->setTag(1); score = 0; labelScore->setPosition(Point(size.width - 80, size.height - 50));
	addChild(labelScore);

	auto labelGameOver = Label::create("Game Over", "宋体", 42);
	labelGameOver->setVisible(0);
	labelGameOver->setTag(2);
	labelGameOver->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(labelGameOver);

	auto labelPause = Label::create("Pause", "宋体", 42);
	labelPause->setVisible(0);
	labelPause->setTag(3);
	labelPause->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(labelPause);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("yumeorugo_arr2.wav", true);

	//计划任务
	//this->schedule(schedule_selector(Game::gameLogic), 1); // (float)1 / GameSpeed);
	//this->schedule(schedule_selector(Game::gameLogic), (float)1 / GameSpeed);
	this->schedule(schedule_selector(Game::gameLogic), 0);
	//监听键盘事件
	auto e = EventListenerKeyboard::create();
	e->onKeyPressed = CC_CALLBACK_2(Game::onKeyPressed, this);
	e->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);;
	return true;
}
