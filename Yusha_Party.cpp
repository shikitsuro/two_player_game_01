////メモ
// 
//
////
//preprocesser
#include "DxLib.h"
//ウィンドウサイズ
#define MIN_X 0
#define MIN_Y 0
#define MAX_X 640
#define MAX_Y 480
//カラー
#define WHITE GetColor(255,255,255)//白
#define BLACK GetColor(110,110,110)//黒
#define RED GetColor(255,0,0)//赤
#define GREEN GetColor(0,255,0)//緑
#define BLUE GetColor(0,0,255)//青
#define CYAN GetColor(0,255,255)//シアン
#define MAGENTA GetColor(255,0,255)//マゼンタ
#define YELLOW GetColor(255,255,0)//イエロー

//Menuの操作
//上下左右で移動 Enterで決定
class Menu{
	//Operate()はstate,decisionの値を変える命令
	//Backはメニューに戻る用
	//Decide()State()は画面遷移用
private:
	int state;//0:バトル 1:1Pキャラメイク 2:2Pキャラメイク
	int decide;//(0bit)0:選択中 1:決定済み (1bit)Enterについて 0:離れた 1:長押し (2bit)Enter以外について 0:離れた 1:長押し
public:
	Menu(){
		this->state=0;
		this->decide=0;
	}
	~Menu();
	void Operate(){
		if (((this->decide)&(2))==(2)){
			((CheckHitKey(KEY_INPUT_RETURN)) ? (this->decide) &= ~0 : (this->decide) &= ~2);
			if(((this->decide) & (2)) == 0)(this->decide)|=1;
		}
		else{
			if (CheckHitKey(KEY_INPUT_LEFT))
				this->state=1;
			if (CheckHitKey(KEY_INPUT_RIGHT))
				this->state=2;
			if (CheckHitKey(KEY_INPUT_UP))
				this->state=1;
			if (CheckHitKey(KEY_INPUT_DOWN))
				this->state=0;
			if (CheckHitKey(KEY_INPUT_RETURN)){
				(this->decide)|=(2);
			}
		}
	}
	int State(){
		return this->state;
	}
	int Decide(){
		return this->decide;
	}
	void Back(){
		this->decide&=(~1);
	}
};
//Personの操作
//上下でステータス調整 左右で移動 Enterで決定
class Person{
private:
	int decide;//
	int x;     //キャラメイク時 位置ポインタ
	int score;//総合勝利点
	int hp;   //体力
	int attack;  //攻撃力
	int defence;//防御力
	int speed;  //攻撃回数
	int point;  //勝利点
public:
	Person(){
		this->decide=0;
		this->x=0;
		this->score=0;
		this->hp=100;
		this->attack=0;
		this->defence=0;
		this->speed=0;
		this->point=0;
	}
	~Person();
	void Operate(){
		if (((this->decide)&(2))==(2)){
			((CheckHitKey(KEY_INPUT_RETURN)) ? (this->decide) &= ~0 : (this->decide) &= ~2);
			if(((this->decide) & (2)) == 0)(this->decide)|=1;
		}
		else{
			if(((this->decide)&4)==4){
				((CheckHitKeyAll()!=0) ? (this->decide) &= ~0 : (this->decide) &= ~4);
			}
			else if (CheckHitKey(KEY_INPUT_LEFT)){
				this->x=(x+7)%8;
				(this->decide)|=4;
			}
			else if (CheckHitKey(KEY_INPUT_RIGHT)){
				this->x=(x+1)%8;
				(this->decide)|=4;
			}
			else if (CheckHitKey(KEY_INPUT_UP)){
				switch(this->x){
				case 0: (attack/10==9 ? this->attack=this->attack-90 : this->attack=this->attack+10);break;
				case 1: (attack%10==9 ? this->attack=this->attack-9 : this->attack=this->attack+1);break;
				case 2: (defence/10==9 ? this->defence=this->defence-90 : this->defence=this->defence+10);break;
				case 3: (defence%10==9 ? this->defence=this->defence-9 : this->defence=this->defence+1);break;
				case 4: (speed/10==9 ? this->speed=this->speed-90 : this->speed=this->speed+10);break;
				case 5: (speed%10==9 ? this->speed=this->speed-9 : this->speed=this->speed+1);break;
				case 6: (point/10==9 ? this->point=this->point-90 : this->point=this->point+10);break;
				case 7: (point%10==9 ? this->point=this->point-9 : this->point=this->point+1);break;
				default:break;
				}
				(this->decide)|=4;
			}
			else if (CheckHitKey(KEY_INPUT_DOWN)){
				switch(this->x){
				case 0: (attack/10==0 ? this->attack=this->attack+90 : this->attack=this->attack-10);break;
				case 1: (attack%10==0 ? this->attack=this->attack+9 : this->attack=this->attack-1);break;
				case 2: (defence/10==0 ? this->defence=this->defence+90 : this->defence=this->defence-10);break;
				case 3: (defence%10==0 ? this->defence=this->defence+9 : this->defence=this->defence-1);break;
				case 4: (speed/10==0 ? this->speed=this->speed+90 : this->speed=this->speed-10);break;
				case 5: (speed%10==0 ? this->speed=this->speed+9 : this->speed=this->speed-1);break;
				case 6: (point/10==0 ? this->point=this->point+90 : this->point=this->point-10);break;
				case 7: (point%10==0 ? this->point=this->point+9 : this->point=this->point-1);break;
				default:break;
				}
				(this->decide)|=4;
			}
			else if (CheckHitKey(KEY_INPUT_RETURN)){
				this->decide|=(2);
			}
		}
	}
	int Sum(){return this->attack + this->defence + this->speed + this->point;}
	void Back(){
		this->decide&=(~1);
	}
	void Damege(int hit , int speed_defference){
		this->hp -= (hit * speed_defference);
	}
	void Win(){
		this->score+=this->point;
	}
	void End(){
		this->hp=100;
	}
	
	int Decide(){return this->decide;}
	int X(){return this->x;}
	int Score(){return this->score;}
	int Hp(){return this->hp;}
	int Attack(){return this->attack;}
	int Defence(){return this->defence;}
	int Speed(){return this->speed;}
	int Point(){return this->point;}
};
//バトルメソッド系クラス
//
class Battle{
private:
	//p1がくらうダメージは(p1hit * s_d_p1)
	int winner;
	int p1hit;
	int p2hit;
	int speed_defference_p1;
	int speed_defference_p2;
public:
	Battle(){
		winner=-1;
		p1hit=0;
		p2hit=0;
		speed_defference_p1=0;
		speed_defference_p2=0;
	}
	~Battle();
	void Damege(Person *p1,Person *p2){
		//速度差と消耗の算出
		p1hit = ((p2->Attack() - p1->Defence() < 0) ? 0 : (p2->Attack() - p1->Defence()));
		p2hit = ((p1->Attack() - p2->Defence() < 0) ? 0 : (p1->Attack() - p2->Defence()));
		(( p2->Speed() - p1->Speed() ) < 1 ) ? (speed_defference_p1 = 1) : (speed_defference_p1 = p2->Speed() - p1->Speed());
		(( p1->Speed() - p2->Speed() ) < 1 ) ? (speed_defference_p2 = 1) : (speed_defference_p2 = p1->Speed() - p2->Speed());
	}
	void End(Person *p1,Person *p2){
		if(p1hit==0 && p2hit==0){
			this->winner=0;
		}
		else if(p1->Hp()<=0 && p2->Hp()<=0){
			this->winner=0;
		}
		else if(p1->Hp()<=0){
			this->winner=2;
		}
		else if(p2->Hp()<=0){
			this->winner=1;
		}
		else winner=-1;
	}
	void End2(Person *p1,Person *p2){//winnerリセット用
		if(p1->Hp()<=0 && p2->Hp()<=0){
			this->winner=0;
		}
		else if(p1->Hp()<=0){
			this->winner=2;
		}
		else if(p2->Hp()<=0){
			this->winner=1;
		}
		else winner=-1;
	}
	int Winner(){return this->winner;}
	int P1hit(){return this->p1hit;}
	int P2hit(){return this->p2hit;}
	int Speed_defference_p1(){return this->speed_defference_p1;}
	int Speed_defference_p2(){return this->speed_defference_p2;}
};
//表示系
//Output
class View{
private:
	int backimg;
	int back2img;
public:
	View(){
		backimg=LoadGraph("image/back.png");
		back2img=LoadGraph("image/back2.png");
	}
	~View();
	void Drawback(Menu *m){
		if(m->State()==0 && (m->Decide()&1)==1){
		}
		else{
			DrawGraph( MIN_X, MIN_Y , this->backimg, false );
			if(m->State()==0)DrawGraph( MIN_X, MIN_Y , this->back2img, false );
		}
	}
	void Drawmenu(Menu *m,Person *p1,Person *p2){
		DrawString(MIN_X,MIN_Y,"上下左右で移動 Enterで決定",YELLOW);
		if(p1->Sum()>100){DrawString(MIN_X,MIN_Y+20,"1Pの合計ステータスは100を超えています。それでもいいですか？",RED);}
		if(p2->Sum()>100){DrawString(MIN_X,MIN_Y+40,"2Pの合計ステータスは100を超えています。それでいいのですね？",RED);}
		DrawString((MIN_X+MAX_X)*1/2-100 ,(MIN_Y+MAX_Y)*2/3,"バトル開始",BLACK);
		DrawString((MIN_X+MAX_X)*1/3-100,(MIN_Y+MAX_Y)*1/3,"1P_キャラメイク",BLACK);
		DrawString((MIN_X+MAX_X)*2/3-100,(MIN_Y+MAX_Y)*1/3,"2P_キャラメイク",BLACK);
		DrawFormatString((MIN_X+MAX_X)*1/3-100,(MIN_Y+MAX_Y)*1/6,BLUE,"1P_現在スコア:%d",p1->Score());
		DrawFormatString((MIN_X+MAX_X)*2/3-100,(MIN_Y+MAX_Y)*1/6,BLUE,"2P_現在スコア:%d",p2->Score());
		switch(m->State()){
		case 0:DrawString((MIN_X+MAX_X)*1/2-100 ,(MIN_Y+MAX_Y)*2/3,"バトル開始",RED);break;
		case 1:DrawString((MIN_X+MAX_X)*1/3-100,(MIN_Y+MAX_Y)*1/3,"1P_キャラメイク",RED);break;
		case 2:DrawString((MIN_X+MAX_X)*2/3-100,(MIN_Y+MAX_Y)*1/3,"2P_キャラメイク",RED);break;
		}
	} 
	void Drawset(Menu *m,Person *p){
		DrawString(MIN_X,MIN_Y,"上下でステータス調整 左右で移動 Enterで決定",YELLOW);
		DrawFormatString((MIN_X+MAX_X)*1/2-100,(MIN_Y+MAX_Y)*1/6,BLACK,"%dPキャラメイク",m->State());
		DrawFormatString((MIN_X+MAX_X)*0/4,(MIN_Y+MAX_Y)*1/2-20,BLACK,"攻撃");
		DrawFormatString((MIN_X+MAX_X)*1/4,(MIN_Y+MAX_Y)*1/2-20,BLACK,"防御");
		DrawFormatString((MIN_X+MAX_X)*2/4,(MIN_Y+MAX_Y)*1/2-20,BLACK,"速度");
		DrawFormatString((MIN_X+MAX_X)*3/4,(MIN_Y+MAX_Y)*1/2-20,BLACK,"報酬");
		DrawFormatString((MIN_X+MAX_X)*0/4,(MIN_Y+MAX_Y)*1/2,BLACK,"%02d",p->Attack());
		DrawFormatString((MIN_X+MAX_X)*1/4,(MIN_Y+MAX_Y)*1/2,BLACK,"%02d",p->Defence());
		DrawFormatString((MIN_X+MAX_X)*2/4,(MIN_Y+MAX_Y)*1/2,BLACK,"%02d",p->Speed());
		DrawFormatString((MIN_X+MAX_X)*3/4,(MIN_Y+MAX_Y)*1/2,BLACK,"%02d",p->Point());
		DrawFormatString((MIN_X+MAX_X)*1/2,(MIN_Y+MAX_Y)*3/4,(p->Sum()>100 ? RED : BLUE),"上限(100)合計値:%d",p->Sum());
		switch(p->X()){
		case 0:DrawFormatString((MIN_X+MAX_X)*0/4,(MIN_Y+MAX_Y)*1/2,RED,"%1d ",(p->Attack()/10));break;
		case 1:DrawFormatString((MIN_X+MAX_X)*0/4,(MIN_Y+MAX_Y)*1/2,RED," %1d",(p->Attack()%10));break;
		case 2:DrawFormatString((MIN_X+MAX_X)*1/4,(MIN_Y+MAX_Y)*1/2,RED,"%1d ",(p->Defence()/10));break;
		case 3:DrawFormatString((MIN_X+MAX_X)*1/4,(MIN_Y+MAX_Y)*1/2,RED," %1d",(p->Defence()%10));break;
		case 4:DrawFormatString((MIN_X+MAX_X)*2/4,(MIN_Y+MAX_Y)*1/2,RED,"%1d ",(p->Speed()/10));break;
		case 5:DrawFormatString((MIN_X+MAX_X)*2/4,(MIN_Y+MAX_Y)*1/2,RED," %1d",(p->Speed()%10));break;
		case 6:DrawFormatString((MIN_X+MAX_X)*3/4,(MIN_Y+MAX_Y)*1/2,RED,"%1d ",(p->Point()/10));break;
		case 7:DrawFormatString((MIN_X+MAX_X)*3/4,(MIN_Y+MAX_Y)*1/2,RED," %1d",(p->Point()%10));break;
		}

	}
	void Drawbattle(Person *p1,Person *p2,Battle *b){
		DrawString(MIN_X,MIN_Y,"Enterで進める BackSpaceでメニューに戻る",YELLOW);		
		DrawFormatString((MIN_X+MAX_X)*0/4,(MIN_Y+MAX_Y)*1/24,RED,"1P");
		DrawFormatString((MIN_X+MAX_X)*0/4,(MIN_Y+MAX_Y)*1/12, (p1->Hp()>=80) ? GREEN : ( (p1->Hp()>=20) ? YELLOW : RED) ,"体力");
		DrawFormatString((MIN_X+MAX_X)*0/4,(MIN_Y+MAX_Y)*2/12,BLACK,"攻撃");
		DrawFormatString((MIN_X+MAX_X)*0/4,(MIN_Y+MAX_Y)*3/12,BLACK,"防御");
		DrawFormatString((MIN_X+MAX_X)*0/4,(MIN_Y+MAX_Y)*4/12,BLACK,"速度");
		DrawFormatString((MIN_X+MAX_X)*0/4,(MIN_Y+MAX_Y)*5/12,BLACK,"報酬");
		DrawFormatString((MIN_X+MAX_X)*0/4+40,(MIN_Y+MAX_Y)*1/12, (p1->Hp()>=80) ? GREEN : ( (p1->Hp()>=20) ? YELLOW : RED) ,"%02d",p1->Hp());
		DrawFormatString((MIN_X+MAX_X)*0/4+40,(MIN_Y+MAX_Y)*2/12,BLACK,"%02d",p1->Attack());
		DrawFormatString((MIN_X+MAX_X)*0/4+40,(MIN_Y+MAX_Y)*3/12,BLACK,"%02d",p1->Defence());
		DrawFormatString((MIN_X+MAX_X)*0/4+40,(MIN_Y+MAX_Y)*4/12,BLACK,"%02d",p1->Speed());
		DrawFormatString((MIN_X+MAX_X)*0/4+40,(MIN_Y+MAX_Y)*5/12,BLACK,"%02d",p1->Point());
		DrawFormatString((MIN_X+MAX_X)*3/4,(MIN_Y+MAX_Y)*1/24,BLUE,"2P");
		DrawFormatString((MIN_X+MAX_X)*3/4,(MIN_Y+MAX_Y)*1/12, (p2->Hp()>=80) ? GREEN : ( (p2->Hp()>=20) ? YELLOW : RED) ,"体力");
		DrawFormatString((MIN_X+MAX_X)*3/4,(MIN_Y+MAX_Y)*2/12,BLACK,"攻撃");
		DrawFormatString((MIN_X+MAX_X)*3/4,(MIN_Y+MAX_Y)*3/12,BLACK,"防御");
		DrawFormatString((MIN_X+MAX_X)*3/4,(MIN_Y+MAX_Y)*4/12,BLACK,"速度");
		DrawFormatString((MIN_X+MAX_X)*3/4,(MIN_Y+MAX_Y)*5/12,BLACK,"報酬");
		DrawFormatString((MIN_X+MAX_X)*3/4+40,(MIN_Y+MAX_Y)*1/12, (p2->Hp()>=80) ? GREEN : ( (p2->Hp()>=20) ? YELLOW : RED) ,"%02d",p2->Hp());
		DrawFormatString((MIN_X+MAX_X)*3/4+40,(MIN_Y+MAX_Y)*2/12,BLACK,"%02d",p2->Attack());
		DrawFormatString((MIN_X+MAX_X)*3/4+40,(MIN_Y+MAX_Y)*3/12,BLACK,"%02d",p2->Defence());
		DrawFormatString((MIN_X+MAX_X)*3/4+40,(MIN_Y+MAX_Y)*4/12,BLACK,"%02d",p2->Speed());
		DrawFormatString((MIN_X+MAX_X)*3/4+40,(MIN_Y+MAX_Y)*5/12,BLACK,"%02d",p2->Point());
		switch(b->Winner()){
			case 0:
				DrawString((MIN_X+MAX_X)*1/2-80,(MIN_Y+MAX_Y)*2/3,"引 き 分 け ! ! ",GREEN);
				break;
			case 1:
				DrawString((MIN_X+MAX_X)*1/2-80,(MIN_Y+MAX_Y)*2/3,"1 P の 勝 ち ! ",GREEN);
				break;
			case 2:
				DrawString((MIN_X+MAX_X)*1/2-80,(MIN_Y+MAX_Y)*2/3,"2 P の 勝 ち ! ",GREEN);
				break;
			default:
				DrawFormatString((MIN_X+MAX_X)*1/2-160,(MIN_Y+MAX_Y)*2/3,GREEN,"(被ダメージ/1ターン) 1P:%d 2P:%d",b->P1hit()*b->Speed_defference_p1(),b->P2hit()*b->Speed_defference_p2());
				break;
		}
	}
	void Warning(){
		DrawString((MIN_X+MAX_X)*1/2-80,(MIN_Y+MAX_Y)*2/3+20,"決着がついていませんよ？",RED);
	}
};
//リソース確保は初期化らしい
//失敗
/*
class RaiiMenu{
public:
	Menu *menu;
	RaiiMenu():menu(new Menu){};
	~RaiiMenu(){delete menu;};
};
class RaiiPerson{
public:
	Person *person;
	RaiiPerson():person(new Person){};
	~RaiiPerson(){delete person;};
};
class RaiiView{
public:
	View *view;
	RaiiView():view(new View){};
	~RaiiView(){delete view;};
};
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow )
{
	// タイトルを 変更
	SetMainWindowText( "Yusha_Party" ) ;
	// ウインドウモードに変更
	ChangeWindowMode( TRUE ) ;
	// ＤＸライブラリ初期化処理
	if( DxLib_Init() == -1 )return -1;
	// 描画先画面を裏にする
	SetDrawScreen(DX_SCREEN_BACK);

	Menu *menu=new Menu;
	View *view=new View;
	Battle *battle=new Battle;
	Person *p1=new Person;
	Person *p2=new Person;
	while(1)
	{
		if ( ProcessMessage() == -1 ){DxLib_End();return 0;}
//
		if((menu->Decide()&1)==1){
			switch(menu->State()){
			case 0:
				//バトル画面
				battle->Damege(p1,p2);
				if(CheckHitKey(KEY_INPUT_RETURN)){
					if(battle->Winner()<0){
						p1->Damege(battle->P1hit(),battle->Speed_defference_p1());
						p2->Damege(battle->P2hit(),battle->Speed_defference_p2());
						battle->End(p1,p2);
					}else{

					}
				}
				if(CheckHitKey(KEY_INPUT_BACK)){
					if(battle->Winner()==-1){
						view->Warning();
					}else{
						switch(battle->Winner()){
							case 0:
								break;
							case 1:
								p1->Win();
							break;
							case 2:
								p2->Win();
								break;
							default:
								break;
						}
						p1->End();
						p2->End();
						battle->End2(p1,p2);
						menu->Back();
					}
				}
				break;
			case 1:
				p1->Operate();
				if((p1->Decide()&1)==1){
					p1->Back();
					menu->Back();
				}
				break;
			case 2:
				p2->Operate();
				if((p2->Decide()&1)==1){
					p2->Back();
					menu->Back();
				}
				break;
			}
		}
		else{
			menu->Operate();
		}
//
		view->Drawback(menu);
		switch(menu->Decide()){
			case 0:
				view->Drawmenu(menu,p1,p2);
				break;
			case 1:
				switch(menu->State()){
				case 0:
					view->Drawbattle(p1,p2,battle);
					break;
				case 1:
					view->Drawset(menu,p1);
					break;
				case 2:
					view->Drawset(menu,p2);
					break;
				}
			break;
		}
		ScreenFlip();
		ClearDrawScreen();
		WaitTimer(1000/60);
	}
	/*delete menu;
	delete view;
	delete p1;
	delete p2;*///失敗
	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;
}