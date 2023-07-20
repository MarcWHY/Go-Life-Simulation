// Dev:       希
// Date:      2023/07/20
// Function:  依据五段模型模拟定段赛结果

#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <algorithm> // Added for std::sort
using namespace std;

enum level
{
	beginner = 1, challenger = 2, mid = 3, top = 4, pro = 5
};

class Participant
{
public:
	string name;
	int pNo;		  //自己的编号
	int scoreLG = 0;  //大分
	int scoreTB = 0;  //小分
	level lvl;
	double totScore(int maxScoreLG, int rounds, int scoreLG, int scoreTB)
	{
		return double(scoreLG) + double(scoreTB) / maxScoreLG - rounds; //总分计算方式
	}
	double TS;
	bool winLoss[15];
	int oppName;					//当前轮对手编号
};

bool match(Participant p1, Participant p2)
{
	int lvlDiff = (p1.lvl - p2.lvl);   //选手水平差距
	double rnd1 = 0, rnd2 = 0;
	random_device rd;
	mt19937 same(rd());
	uniform_real_distribution<double> dist(0.0, 10.0); //生成随机
	rnd1 = dist(same);
	rnd2 = dist(same);
	if (lvlDiff == 0)
		rnd2 = rnd2;
	else if (lvlDiff > 0 && lvlDiff <= 4)
		rnd2 /= (1 + pow(4, lvlDiff));
	else if (lvlDiff < 0 && lvlDiff >= -4)
		rnd1 /= (1 + pow(4, abs(lvlDiff)));
	return rnd1 > rnd2;
}

int max(int var1, int var2)
{
	if (var1 >= var2)
		return var1;
	else
		return var2;
}

const int participantNum = 400;
const int rounds = 15;
Participant allRank[participantNum][rounds]; //所有轮次排行信息

int main()
{
	Participant pt[participantNum];
	string nmk;
	bool temp;
	int lgComp = 0; //两大分中的最大大分
	for (int i = 0; i < 30; i++)
	{
		nmk = to_string(i + 1);
		pt[i].name = "pro" + nmk;
		pt[i].lvl = pro;
	}
	for (int i = 30; i < 80; i++)
	{
		nmk = to_string(i - 29);
		pt[i].name = "top" + nmk;
		pt[i].lvl = top;
	}
	for (int i = 80; i < 240; i++)
	{
		nmk = to_string(i - 79);
		pt[i].name = "mid" + nmk;
		pt[i].lvl = mid;
	}
	for (int i = 240; i < 340; i++)
	{
		nmk = to_string(i - 239);
		pt[i].name = "challenger" + nmk;
		pt[i].lvl = challenger;
	}
	for (int i = 340; i < participantNum; i++)
	{
		nmk = to_string(i - 339);
		pt[i].name = "beginner" + nmk;
		pt[i].lvl = beginner;
	}
	for (int i = 0; i < participantNum; i++)
		pt[i].pNo = i;
	// 五级别选手定义完成

	int maxScoreLG = 0; //最大大分

	cout << "                           定段赛第" << rounds << "轮最终名词表：" << endl;

	for (int i = 0; i < participantNum; i++)
	{
		pt[i].scoreTB = 0;							//初始化所有选手的小分, 将其15个都设为0
		pt[i].TS = 0;								//初始化所有选手的总分, 将其15个都设为0
	}


	for (int j = 0; j < rounds; j++) //15 轮比赛
	{
		for (int i = 0; i < participantNum / 2; i++) //对所有经过上一轮排序的选手进行如下操作
		{
			temp = match(pt[2 * i], pt[2 * i + 1]);
			//比赛并判断胜负
			if (temp)
			{
				pt[2 * i].scoreLG += 2;
				pt[2 * i].winLoss[j] = true;
				pt[2 * i + 1].winLoss[j] = false;
			}
			else
			{
				pt[2 * i + 1].scoreLG += 2;
				pt[2 * i].winLoss[j] = false;
				pt[2 * i + 1].winLoss[j] = true;
			}
			//胜者大分加2，并输入对应胜负


			pt[2 * i].oppName = pt[2 * i + 1].pNo;
			pt[2 * i + 1].oppName = pt[2 * i].pNo; //将对手编号存入当前轮次的对手名称


			lgComp = max(pt[2 * i + 1].scoreLG, pt[2 * i].scoreLG);
			if (maxScoreLG < lgComp)
				maxScoreLG = lgComp;
			//maxScoreLG与两对局者大分相比大分, 每次迭代都如此，直到选出最大maxScoreLG。

			pt[2 * i].scoreTB = 0;
			pt[2 * i + 1].scoreTB = 0;   //小分在每一轮对局初始归零
			allRank[2 * i][j] = pt[2 * i];
			allRank[2 * i + 1][j] = pt[2 * i + 1];
		}
		//第j轮对局结束，现在所有选手大分，对局胜负，与第j轮对手编号被记录在allRank中了。

		for (int i = 0; i < participantNum; i++)
		{
			for (int k = 0; k <= j; k++)											//从第0轮开始，统计至当前轮
			{
				int countingNumeral = allRank[i][k].oppName;						//countingNumeral是i号选手第k轮的对手编号
				pt[i].scoreTB += allRank[countingNumeral][j].scoreLG;				//第k对手大分累加至当前轮的小分
			}
		}
		// 现在所有选手小分出来了，接下来计算总分


		for (int i = 0; i < participantNum; i++)
			pt[i].TS = pt[i].totScore(maxScoreLG, rounds, pt[i].scoreLG, pt[i].scoreTB);
		//所有选手的总分出来了，进行排位准备下一轮对战


		// 选手进行排行
		sort(pt, pt + participantNum, [](const Participant& p1, const Participant& p2)
			{
				return p1.TS > p2.TS;
			});
	}

	for (int i = 0; i < participantNum; i++)
	{
		cout << "选手名：" << pt[i].name << "  大分：" << pt[i].scoreLG << "  小分：" << pt[i].scoreTB << "  总分:" << pt[i].TS << "  排名：" << i + 1 << endl;
	}
	// 打印排位表

	for (int k = 0; k < 15; k++)
		cout << allRank[0][k].oppName << endl;

}
