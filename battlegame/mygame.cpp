/*

Part of a little game
Liuqing Yang<yllqq#outlook.com>

Compile with VS2010+ or gcc(std=c++11)

2015/3/30 relize shopping function

*/

#include "allstruct.h"

bag mybag;

void clearscreen()
{
#ifdef _WIN32
		system("cls");
#else
		cout << "\033[H""\033[J";
#endif
}

bool useitem(bag& mybag, warrior* w, monster* m)
{
	int sn;
	mybag.show();
	cout << "\n按对应序号使用 0返回\n";
	cin >>sn;
	if(sn == 0) return false;
	return mybag.use(sn, w, m);
}

void battle(warrior& mywarrior, monster& mymonster)
{
	char ch;
	while (!mywarrior.dead()){
		mywarrior.show();
		mymonster.show();
		cout << "\n1.攻击 2.背包\n";
		cin >> ch;
		clearscreen();
		switch(ch){
		case '1':
			mymonster.hurt(mywarrior.autoattack());
			break;
		case '2':
			if (useitem(mybag, &mywarrior, &mymonster)) break;
			continue;
		default:
			continue;
		}
		if (mymonster.dead()){
			int exp = mymonster.power.exp;
			int gold = mymonster.gold;
			cout << "战胜怪物， 获得经验" << exp << "获得金币" << gold << '\n';
			mywarrior.gainexp(exp, gold);
			return;
		}
		mywarrior.hurt(mymonster.autoattack());
	}
}

bool shopping(bag& mybag, warrior* w)
{
	int sn;
	int count;
	int price;
	int gold = w->gold;

	mybag.showshop();
	cout << "\n现有金币：" << w->gold << "请输入要购买的序号与数量(0 0不购买)\n";
	cin >> sn;
	cin >> count;
	price = mybag.getprice(sn, count);

	if (price < 0) {
		return false;
	}
	if (gold < price) {
		cout << "\n金币不够\n";
		return false;
	}
	mybag.add(sn, count);
	w->gold -= price;

	return true;
}

int main(int argc, char* argv[])
{
	warrior warr;
	cout << "送你999RMB大礼包！包括10手雷，10伤药！\n\n";
	mybag.add(1, 10);
	mybag.add(2, 10);
	char ch;
	while (!warr.dead()){
		warr.show();
		warr.power.hp = warr.power.maxhp;
		cout << "\n1.打小怪 2.打BOSS 3.背包 4. 商店 5.退出\n";
		cin >> ch;
		clearscreen();
		switch(ch){
		case '1':{
			monster mons;
			battle(warr, mons);
			break;
		}
		case '2':{
			boss bos;
			battle(warr, bos);
			break;
		}
		case '3':
			useitem(mybag, &warr, 0);
			break;
		case '4':
			shopping(mybag, &warr);
			break;
		case '5':
			return 0;
		default:
			continue;
		}
	}
	cout << "游戏结束！\n";
#ifdef _WIN32
	system("pause");
#endif
	return 0;
}


