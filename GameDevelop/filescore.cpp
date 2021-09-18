#define _CRT_SECURE_NO_WARNINGS 0
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<Windows.h>
#include<time.h>

using namespace std;



int main2() {

	vector<pair<int, string>> userScore;
	fstream file("score.txt", ios::in);
	int score;
	string name;

	while (file >> name >> score) {
		userScore.push_back(make_pair(score,name));
	}

	file.close();
	userScore.push_back(make_pair(7000,"ran")); // push back data in vector
	for (int i = userScore.size() - 2; i >= 0; i--) {
		if (userScore[i].second == userScore[userScore.size()-1].second && userScore[i].first == userScore[userScore.size()-1].first) {
			userScore.erase(userScore.begin() + i);
		}
	}
	sort(userScore.begin(), userScore.end());

	file.open("score.txt", ios::out);

	for (int i = userScore.size()-1; i >= 0; i--) {
		file << userScore[i].second <<" ";
		file << userScore[i].first << endl;
	}

	file.close();

	return 0;

}