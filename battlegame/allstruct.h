/*

Part of a little game
Liuqing Yang<yllqq#outlook.com>

Compile with VS2010+ or gcc(std=c++11)

*/

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>
using std::cin;
using std::cout;
using std::string;
using std::map;
using std::endl;


struct powernum{
	int attack;
	int defence;
	int hp;
	int maxhp;
	int exp, level;
	int nextexp() {return level * level * 100;}
};

class warrior{
public:
	warrior(){
		power.attack = 10;
		power.defence = 1;
		power.hp = power.maxhp = 1000;
		power.exp = 0;
		power.level = 1;
		gold = 0;
	}
	void hurt(int damage){
		damage -= power.defence;
		if (damage < 0) damage = 1;
		if (damage > power.hp) damage = power.hp;
		power.hp -= damage;
		cout << "你受到" << damage << "点伤害" << '\n';
		if (power.hp == 0) cout << "你阵亡了\n";
	}
	void show(){
		cout << "\n勇士 ：\n"
			<< "\n等级 ： " << power.level
			<< "\nHP/HPMAX ： " << power.hp << '/' << power.maxhp
			<< "\n攻击/防御 ： " <<power.attack << '/' << power.defence
			<< "\n经验/升级经验 ： " << power.exp << '/' << power.nextexp() << '\n';
	}
	int autoattack() {return power.attack;}
	bool dead() {return power.hp == 0;}
	void gainexp(int n, int gaingold)
	{
		power.exp += n;
		while (power.exp >= power.nextexp()){
			power.level++;
			power.attack += 2 * power.level + 2;
			power.defence += power.level + 1;
			power.maxhp += 100 * power.level + 100;
			cout << "恭喜达到" << power.level << "级！\n\n";
		}
		gold += gaingold;
	}

	powernum power;
	int gold;
};

class monster{
public:
	monster(){
		power.attack = 50;
		power.defence = 1;
		power.hp = power.maxhp = 200;
		power.exp = 10;
		power.level = 1;
		gold = 10;
		name = "怪物";
	}
	void hurt(int damage){
		damage -= power.defence;
		if (damage < 0) damage = 1;
		if (damage > power.hp) damage = power.hp;
		power.hp -= damage;
		std::cout << "怪物受到" << damage << "点伤害" << '\n';
		if (power.hp == 0) std::cout << "怪物阵亡了\n";
	}
	int autoattack() {return power.attack;}
	bool dead() {return power.hp == 0;}
	void show(){
		cout << "\n" << name << " ：\n"
			<< "\n等级 ： " << power.level
			<< "\nHP/HPMAX ： " << power.hp << '/' << power.maxhp
			<< "\n攻击/防御 ： " <<power.attack << '/' << power.defence << '\n';
	}
	powernum power;
	int gold;
	string name;
};

class boss : public monster{
public:
	boss(){
		power.attack *= 2;
		power.defence *= 2;
		power.maxhp *= 2;
		power.hp *= 2;
		gold *= 10;
		name = "怪物领主";
	}
};

class object{
public:
	object(string s, int p) : name(s), price(p) { }
	virtual bool use(warrior*, monster*) = 0;
	int price;
	string name;
};

class potion : public object{
public:
	potion() : object("伤药", 10) { }
	bool use(warrior* w, monster* m){
		if (w->power.maxhp == w->power.hp) return false;
		if (w->power.maxhp - w->power.hp < recover){
			cout << "\n恢复" << w->power.maxhp - w->power.hp << "点生命值\n";
			w->power.hp = w->power.maxhp;
		}
		else{
			cout << "\n恢复" << recover << "点生命值\n";
			w->power.hp += recover;
		}
		return true;
	}
private:
	static const int recover = 100;
};

class bomb : public object{
public:
	bomb() : object("手雷", 20) { }
	bool use(warrior* w, monster* m){
		if(!m) return false;
		cout << "\n你使用了手雷\n";
		m->hurt(damage);
		return true;
	}
private:
	static const int damage = 200;
};

class bag{
public:
	bag() {
		allitem[1] = new potion;
		allitem[2] = new bomb;
	}
	~bag() {
		delete allitem[2];
		delete allitem[1];
	}
	int getprice(int sn, int num){
		if (sn != 1 && sn != 2){
			cout << "无此物品\n";
			return -1;
		}
		return allitem[sn]->price * num;
	}
	void add(int sn, int num){
		items[sn] += num;
	}
	bool use(int num, warrior* w, monster* m){
		auto it = items.find(num);
		if (it == items.end()){
			cout << "使用失败：无此物品";
			return false;
		}
		if (it->second == 0){
			cout << "使用失败 : 物品数量不足！\n";
			return false;
		}
		if (allitem[num]->use(w, m)){
			items[num]--;
			return true;
		}
		else{
			cout << "使用失败！ \n";
			return false;
		}
	}
	void show() {
		for(auto it = items.begin(); it != items.end(); ++it)
			if (it->second != 0)
				cout << it->first << '\t' << allitem[it->first]->name 
					<< "\t数量：\t" << it->second << endl;
	}
	void showshop() {
		int n = 0;
		for (auto it = allitem.begin(); it != allitem.end(); ++it) {
			cout << it->first << '\t' << it->second->name << "\t价格：\t" << it->second->price << endl;
			n++;
		}
	}
private:
	map<int, int> items;
	map<int, object*> allitem;
};

class area{

};

