#include <fstream>
#include <iostream>
#include <vector>
#include <limits.h>
#include <memory>
//#include "test.h"
using namespace std;

class bst {
public:
	bst(istream &is) {
		is >> n;
		p = new double[n + 1];
		for (int i = 1; i <= n; i++) {
			is >> p[i];
		}

		q = new double[n + 1];
		for (int i = 0; i <= n; i++) {
			is >> q[i];
		}
		e.resize(n+2);
		for (int i = 0; i <= n + 1; i++) {
			e[i].resize(n+2);
		}
		w.resize(n+2);
		for (int i = 0; i <= n + 1; i++) {
			w[i].resize(n+2);
		}
		root.resize(n+2);
		for (int i = 0; i <= n + 1; i++) {
			root[i].resize(n+2);
		}
	}

	void optimal_bst() {
		for (int i = 1; i <= n + 1; i++) {
			e[i][i - 1] = q[i - 1];
			w[i][i - 1] = q[i - 1];
		}

		for (int i = 1; i <= n; i++) {
			root[i][n - i + 1] = i;
			e[i][i] = INT_MAX;
			w[i][i] = w[i][i - 1] + p[i] + q[i];
			e[i][i] = e[i][i - 1] + e[i + 1][i] + w[i][i];
			root[i][i] = i;
		}

		for (int l = 2; l <= n; l++) {
			for (int i = 1; i <= n - l + 1; i++) {
				int j = i + l - 1;
				e[i][j] = INT_MAX;
				w[i][j] = w[i][j - 1] + p[j] + q[j];
				for (int r = root[i][j - 1]; r <=  root[i + 1][j]; r++) {
					double t = e[i][r - 1] + e[r + 1][j] + w[i][j];
					if (t < e[i][j]) {
						e[i][j] = t;
						root[i][j] = r;
					}
				}
			}
		}
	}

	void printdata () {
		for (auto &ele : e) {
			for (auto &element : ele) {
				cout << '\t' << element;
			}
			cout << endl;
		}
		cout << "\n\n";

		for (auto &ele : w) {
			for (auto &element : ele) {
				cout << '\t' << element;
			}
			cout << endl;
		}
		cout << "\n\n";

		for (auto &ele : root) {
			for (auto &element : ele) {
				cout << '\t' << element;
			}
			cout << endl;
		}
	}

	void print() {
		cout << 'k' << root[1][n] << "是根\n";
		print_rec(1, n);
	}

	void print_rec(int m, int n) {
		int k = root[m][n];
		if (m == k)
			cout << 'd' << m - 1 << "是k" << k << "的左孩子\n";
		else {
			cout << 'k' << root[m][k -1] << "是k" << k << "的左孩子\n";
			print_rec(m, k -1);
		}

		if (k == n)
			cout << 'd' << n << "是k" << k << "的右孩子\n";
		else {
			cout << 'k' << root[k + 1][n] << "是k" << k << "的左孩子\n";
			print_rec(k + 1, n);
		}

	}

	~bst() {
		delete p;
		delete q;
	}

private:
	int n;
	double *p;
	double *q;
	vector<vector<double>> e;
	vector<vector<double>> w;
	vector<vector<int>> root;
};

struct a{
	union {
		int i;
		double d;
	};
	int v;
};

int main()
{
	a a1;
	a1.i = 9;
	double d = a1.d;
	cout << d;

	vector<string> v1;
	{
		vector<string> v2;
		v2.push_back("a");
		v2.push_back("an");
		v1 = v2;
	}
	
	ifstream is("E://input.txt");
	bst b(is);
	b.optimal_bst();
	b.printdata();
	cout << "\n\n";
	b.print();
	system("pause");
	return 0;
}
