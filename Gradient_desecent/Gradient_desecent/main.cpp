#include<iostream>
#include<fstream>
#include<istream>
#include<ostream>
#include<string>
#include<sstream>
#include<vector>
using namespace std;
double theta[385] = { 0 };
double old_theta[385] = { 0 };
double vx[250002][385] = { 0 };
double vy[250002] = { 0 };
double inner_product[250002] = { 0 };
int main() {
	ifstream file("file.csv"); // declare file stream
	ofstream outfile;
	ofstream outtheta_file;
	ifstream intheta_file;
	outfile.open("result3.csv");
	outtheta_file.open("theta.csv");

	ifstream testfile("test.csv");
	ifstream thetafile("theta2.csv");
	int row = 0;
	double cost = INT_MAX;
	double step = 0.01;

	bool firstflag = true;
	while (file.good())
	{
		string line;
		getline(file, line);
		if (firstflag) {
			firstflag = false;
			continue;
		}

		stringstream ss(line);
		for (int col = 0; col < 386; col++) {
			string val;
			double double_val;
			getline(ss, val, ',');
			double_val = stof(val);
			if (col == 0) {
				vx[row][col] = 1;
			}
			if (col > 0 && col < 385) {
				vx[row][col] = double_val;
			}
			if (col == 385) {
				vy[row] = double_val;
			}
		}
		row++;

	}
	int cnt = 0;
	while (thetafile.good()) {
		string line;
		getline(thetafile, line);
		if (line.empty()) {
			continue;
		}
		double double_val = stod(line);
		theta[cnt] = double_val;
		cnt++;
	}

	bool converge_flag = false;
	double tempstep = step / 25000;
	for (int i = 0; i < 140000; i++) {
		if (converge_flag) break;
		for (int j = 0; j < 385; j++) {
			old_theta[j] = theta[j];
		}
		if (i == 100000 || i == 139999) {
			for (int col = 0; col < 385; col++) {
				double temp_theta = theta[col];
				string s = to_string(temp_theta);
				outtheta_file << s << endl;
			}
		}
		for (int j = 0; j < 25000; j++) {
			double tempresult = 0;
			for (int k = 0; k < 385; k++) {
				tempresult += theta[k] * vx[j][k];
			}
			inner_product[j] = tempresult-vy[j];
		}
		for (int j = 0; j < 385; j++) {	
			double tempnum = 0;
			for (int k = 0; k < 25000; k++) {
				tempnum += inner_product[k] * vx[k][j];
			}
			theta[j] -= (tempstep*tempnum);
		}
		double resultcost = 0.0;
		for (int j = 0; j < 25000; j++) {
			double tempcost = 0;
			for (int k = 0; k < 385; k++) {
				tempcost += theta[k] * vx[j][k];
			}
			tempcost -= vy[j];
			tempcost *= tempcost;
			resultcost += tempcost;
		}
		double eps = abs(resultcost - cost);
		cout << "第 " << i << "次迭代，代价为差为" << eps << endl;
		if (resultcost > cost) {
			for (int j = 0; j < 385; j++) {
				theta[j] = old_theta[j];
			}
			converge_flag = true;
		}
		else {
			cost = resultcost;
		}
	}
	
	row = 0;
	firstflag = true;
	while (testfile.good()) {
		string line;
		getline(testfile, line);
		if (firstflag) {
			outfile << "id,reference" << endl;
			firstflag = false;
			continue;
		}

		double y;
		double inner_product = 0.0;
		stringstream ss(line);
		for (int col = 0; col < 385; col++) {
			string val;
			double double_val;
			getline(ss, val, ',');
			stringstream convertor(val);
			if (col == 0) {
				inner_product += theta[0];
			}
			if (col > 0 && col < 385) {
				convertor >> double_val;
				inner_product += theta[col] * double_val;
			}
		}
		y = inner_product;
		int id = row;
		outfile << to_string(id) << "," << to_string(y) << endl;
		row++;
	}
	outfile.close();
	
	getchar();
	getchar();
	return 0;
}