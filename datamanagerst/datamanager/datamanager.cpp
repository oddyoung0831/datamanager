﻿//库的调用

#include<ostream>
#include<iostream>
#include<sstream>
#include<string>
#include<cmath>
#include<cassert>
#include<fstream>
#include<vector>
#include <cstring>
#include <fstream>
#include "iomanip"
#include "windows.h"
using namespace std;
#import "msado15.dll" no_namespace rename("EOF","EndOfFile")
//Π的声明和定义
const double pi = 2 * std::asin(1);
//图形类的声明
class polyline;
class point;
class Circle;
class Rectangle;
class Triangle;
class color {
public:
	string r, g, b;
};
template<class T>
inline string toString(const T& v) {
	ostringstream os;
	os << v;
	return os.str();
}

//抽象图形基类
class shape {
public:
	int code;
	color bordercolor;
	string borderstyle;
	color fillcolor;
	string fillstyle;
	virtual ~shape() = 0;
	virtual float getArea() = 0;
	virtual float getCir() = 0;
	virtual bool isContain(point* p) = 0;
	virtual int getcode() = 0;
	virtual double* getdata() = 0;
	virtual string* getdraw() =0;
	virtual void printType() = 0;
	virtual void setbc(string r, string g, string b) = 0;
	virtual void setbs(string bs) = 0;
	virtual void setfc(string r, string g, string b) = 0;
	virtual void setfs(string fs) = 0;
};
shape::~shape() {}
//点类
class point : public shape {
public:
	int code = 0;
	double x, y;
	point() {}
	point(const point& p) { x = p.x; y = p.y; }
	point(float X, float Y) :x(X), y(Y) {}
	~point() {};
	point getXY() {
		return *this;

	}
	float getArea() { return 0; }
	float getCir() { return 0; }
	bool isContain(point* p) { return false; }
	void printType() { cout << "this is a point." << endl; }

	double distance(point p) {
		double dx = x - p.x;
		double dy = y - p.y;
		return sqrt(dx * dx + dy * dy);
	}
	int getcode() { return code; }
	double* getdata() { double* arr = new double[2] {x, y}; return arr; }
	string* getdraw() { return 0; }
	void setbc(string r, string g, string b) {};
	void setbs(string bs) {};
	void setfc(string r, string g, string b) {};
	void setfs(string fs) {};
};
//线类
class polyline :public shape {
public:
	int code = 1;
	point* points;
	int numPoints;
	point p, q;
	polyline();

	polyline(point* _points, int _numPoints) {
		points = new point[_numPoints];
		numPoints = _numPoints;
		for (int i = 0; i < numPoints; i++) {
			points[i] = _points[i];
		}
	}
	~polyline() {
		delete[] points;
	}

	float getArea() { return 0; }
	float getCir() {
		double totalLength = 0.0;
		for (int i = 0; i < numPoints - 1; i++) {
			totalLength += points[i].distance(points[i + 1]);
		}
		return totalLength;
	}

	bool isContain(point* p) { return false; }
	void printType() { cout << "this is a polyline." << endl; }
	int getcode() { return code; }
	double* getdata() {
		double* arr = new double[2 * numPoints + 5];
		arr[0] = numPoints;
		for (int i = 0;i < numPoints;i++) { arr[2 * i + 1] = points[i].x; arr[2 * i+2] = points[i].y; }
						return arr; }
	string* getdraw() {
		string* str = new string[8]{ bordercolor.r ,bordercolor.g ,bordercolor.b,borderstyle};
		return str;
	}
	void setbc(string r, string g, string b) { bordercolor.r = r; bordercolor.b = b;bordercolor.g = g; }
	void setbs(string bs) { this->borderstyle = bs; }
	void setfc(string r, string g, string b) {}
	void setfs(string fs) {}
};

class polygon : public polyline {
public:
	int code = 2;
	polygon(point* _points, int _numpoints) : polyline(_points, _numpoints) {}

	float getArea() {
		double totalArea = 0.0;
		for (int i = 0; i < numPoints - 1; i++) {
			totalArea += points[i].x * points[i + 1].y - points[i + 1].x * points[i].y;
		}
		return 0.5 * fabs(totalArea);
	}
	float getCir() {
		double totalLength = 0.0;
		for (int i = 0; i < numPoints - 1; i++) {
			totalLength += points[i].distance(points[i + 1]);
		}
		totalLength += points[numPoints - 1].distance(points[1]);
		return totalLength;
	}
	bool isContain(point* p) {
		int count = 0;
		for (int i = 0; i < numPoints - 1; i++) {
			const point& p1 = points[i];
			const point& p2 = points[(i + 1) % numPoints];

			// 如果点在多边形的边界上，则认为点在多边形内部
			if ((p->x == p1.x && p->y == p1.y) || (p->x == p2.x && p->y == p2.y)) {
				return true;
			}

			// 判断射线是否和线段相交
			if ((p1.y > p->y) != (p2.y > p->y)
				&& p->x < (p2.x - p1.x) * (p->y - p1.y) / (p2.y - p1.y) + p1.x) {
				++count;
			}
		}

	}
	void printType() { cout << "this is a polygon." << endl; }
	int getcode() { return code; }
	string* getdraw() {
		string* str = new string[8]{ bordercolor.r ,bordercolor.g ,bordercolor.b,borderstyle,
										fillcolor.r, fillcolor.g, fillcolor.b,fillstyle };
		return str;
	}
	void setfc(string r, string g, string b) { fillcolor.r = r;fillcolor.g = g;fillcolor.b = b;
	}
	void setfs(string fs) { this->fillstyle = fs; }
};

//矩形类
class rectangle :public shape {
public:
	int code = 3;
	point p;
	point q;
	rectangle() {}
	rectangle(double X, double Y, double X1, double Y1) :p(X, Y), q(X1, Y1) { if (X == X1 && Y == Y1) { assert(false); } }
	rectangle(const point& p, const point& Q) : p(p), q(Q) { if (p.x == q.x && p.y == q.y) { assert(false); } }
	rectangle(const rectangle& r) {
		p = r.p;
		q = r.q;

	}
	~rectangle() { cout << "A rectangle has been destroyed." << endl; };
	point& getPoint() {
		return p, q;

	}
	float getArea() {
		return abs(p.x - q.x) * abs(p.y - q.y);

	}
	float getCir() {
		return 2 * (abs(p.x - q.x) + abs(p.y - q.y));

	}
	bool isContain(point* t) {
		return (t->x > min(p.x, q.x) && t->x<max(p.x, q.x) && t->y>min(p.y, q.y) && t->x < max(p.y, q.x));

	}
	bool isContain(float X, float Y) {
		return (X > min(p.x, q.x) && X <max(p.x, q.x) && Y>min(p.y, q.y) && Y < max(p.y, q.x));

	}
	int getcode() { return code; }
	double* getdata() {
		double* arr = new double[4] {p.x,p.y,q.x,q.y};

		return arr;
	}
	string* getdraw() {
		string* str = new string[8]{ bordercolor.r ,bordercolor.g ,bordercolor.b,borderstyle,
										fillcolor.r, fillcolor.g, fillcolor.b,fillstyle };
		return str;
	}
	void printType() { cout << "this is a rectangle." << endl; }
	void setbc(string r, string g, string b) { bordercolor.r = r; bordercolor.b = b;bordercolor.g = g; };
	void setbs(string bs) { borderstyle = bs; };
	void setfc(string r, string g, string b) { fillcolor.r = r; fillcolor.b = b;fillcolor.g = g; };
	void setfs(string fs) { fillstyle = fs; };
};
//圆形类
class circle :public shape {


public:
	int code = 4;
	point center;
	double r;
	circle() {}
	circle(double X, double Y, double R) :center(X, Y), r(R) { if (R == 0) { assert(false); } }
	circle(const point& p, double R) : center(p), r(R) { if (R == 0) { assert(false); } }
	circle(const circle& c) {
		center = c.center;
		r = c.r;

	}
	~circle() { cout << "A circle has been destroyed." << endl; };
	point& getPoint() {
		return this->center;

	}
	float getR() {
		return r;

	}
	float getArea() {
		return pi * r * r;

	}
	float getCir() {
		return 2 * pi * r;

	}
	bool isContain(point* p) {
		return pow((p->x - center.x), 2) + pow((p->y - center.y), 2) < pow(r, 2);

	}
	bool isContain(float X, float Y) {
		return pow((X - center.x), 2) + pow((Y - center.y), 2) < pow(r, 2);

	}
	void printType() { cout << "this is a circle." << endl; }
	int getcode() { return code; }
	double* getdata() {
		double* arr = new double[3] {r,center.x,center.y};
		return arr;
	}
	string* getdraw() {
		string* str = new string[8]{ bordercolor.r ,bordercolor.g ,bordercolor.b,borderstyle,
										fillcolor.r, fillcolor.g, fillcolor.b,fillstyle };
		return str;
	}
	void setbc(string r, string g, string b) { bordercolor.r = r; bordercolor.b = b;bordercolor.g = g; };
	void setbs(string bs) { borderstyle = bs; };
	void setfc(string r, string g, string b) { fillcolor.r = r; fillcolor.b = b;fillcolor.g = g; };
	void setfs(string fs) { fillstyle = fs; };
};

class sector :public circle {


public:
	int code = 5;
	double startAngle;
	double endAngle;
	sector() {}
	sector(double r, point c, double sa, double ea) :circle(c, r) {
		startAngle = sa;
		endAngle = ea;
	}
	sector(const sector& t) {
		center = t.center;
		r = t.r;
		startAngle = t.startAngle;
		endAngle = t.endAngle;
	}
	~sector() { cout << "A Sector has been destroyed." << endl; };
	point& getPoint() {
		return this->center;
	}
	float angle() {
		return endAngle - startAngle;
	}
	float getArea() {
		return 0.5 * angle() * r * r;
	}
	float getCir() {
		return r + r + angle() * r;

	}
	bool isContain(point* p) {
		double dx = p->x - center.x;
		double dy = p->y - center.y;
		double distance = sqrt(dx * dx + dy * dy);
		if (distance > r) {  // 如果点到圆心的距离超过了半径，则不在扇形内部
			return false;
		}

		double angle = atan2(dy, dx);  // 计算点对应的极角
		angle = angle >= 0 ? angle : angle + 2 * pi;  // 将角度转换为正值

		// 判断点的极角是否在扇形的角度范围内
		double diff1 = fmin(fabs(angle - startAngle), 2 * pi - fabs(angle - startAngle));
		double diff2 = fmin(fabs(angle - endAngle), 2 * pi - fabs(angle - endAngle));
		double diff = diff1 + diff2;
		return diff <= fabs(endAngle - startAngle) + 1e-8;
	}
	int getcode() { return code; }
	double* getdata() {
			double* arr = new double[5] {r, center.x, center.y,startAngle,endAngle};
		return arr;
	}
	void printType() { cout << "this is a sector." << endl; }


};

class Text {
public:
	// 构造函数
	Text() = default;
	Text(const string& content) : content_(content) {}

	// 插入文本
	void insert(size_t pos, const string& str) {
		content_.insert(pos, str);
	}

	// 删除文本
	void erase(size_t pos, size_t len) {
		content_.erase(pos, len);
	}

	// 替换文本
	void replace(size_t pos, size_t len, const string& str) {
		content_.replace(pos, len, str);
	}

	// 获取文本
	string get_content() const {
		return content_;
	}

	// 把文本按行分成多个子串
	vector<string> split_lines() const {
		vector<string> lines;

		size_t start = 0;
		while (start < content_.size()) {
			size_t end = content_.find('\n', start);
			if (end == string::npos) {
				end = content_.size();
			}
			lines.push_back(content_.substr(start, end - start));
			start = end + 1;
		}

		return lines;
	}
private:
	// 存储文本内容
	string content_;
};

class BitMap {
public:
	// 构造函数
	BitMap(int width, int height) : width_(width), height_(height) {
		int size = (width * height + 7) / 8;
		data_.resize(size);
	}

	// 获取指定位置像素的颜色
	bool get_pixel(int x, int y) const {
		if (x < 0 || x >= width_ || y < 0 || y >= height_) {
			return false;
		}

		int index = y * width_ + x;
		int offset = index % 8;
		int byte = index / 8;
		return (data_[byte] >> offset) & 1u;
	}

	// 设置指定位置的像素颜色
	void set_pixel(int x, int y, bool color) {
		if (x < 0 || x >= width_ || y < 0 || y >= height_) {
			return;
		}

		int index = y * width_ + x;
		int offset = index % 8;
		int byte = index / 8;

		if (color) {
			data_[byte] |= (1u << offset);
		}
		else {
			data_[byte] &= ~(1u << offset);
		}
	}

	// 清除所有像素
	void clear() {
		memset(&data_[0], 0x00, data_.size());
	}

private:
	int width_;            // 图像宽度
	int height_;           // 图像高度
	vector<unsigned char> data_;  // 存储像素数据
};
class Writer {
public:
	virtual bool  save(shape& sh) = 0;
};
class SqlWriter:
public Writer{
public:
	virtual bool save(shape& sh) { return false; }
};
class pointSqlWriter:
	public SqlWriter{
	bool save(shape& sh) {
		::CoInitialize(NULL);
		_RecordsetPtr m_pRecordset("ADODB.Recordset");
		_ConnectionPtr m_pConnection("ADODB.Connection");
		_bstr_t bstrSQL("select * from point");
		try
		{
			m_pConnection.CreateInstance("ADODB.Connection");
			_bstr_t strConnect = "Provider=SQLOLEDB;Server=(local);Database=tuxing;uid=sa;pwd=20030831;";
			m_pConnection->Open(strConnect, "", "", adModeUnknown);
			if (m_pConnection == NULL)
			{
				cerr << "Lind data ERROR!\n";
			}
			//创建记录集
			m_pRecordset.CreateInstance(_uuidof(Recordset));
			//取得表中的记录
			m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
			
			m_pRecordset->AddNew(); ///添加新记录
			m_pRecordset->PutCollect("x", _variant_t(sh.getdata()[0]));
			m_pRecordset->PutCollect("y", _variant_t(sh.getdata()[1]));
			m_pRecordset->Update();
			m_pRecordset->Close(); // 关闭记录集
		}
		// 捕捉异常
		catch (_com_error c)
		{
			cerr << "\nERROR:" << (char*)c.Description();
		}
		if (m_pConnection->State)
			m_pConnection->Close();

		::CoUninitialize();
		return TRUE;
	}
};
class FileWriter :
public Writer{
public:
	virtual bool save(shape& sh) { return false; };
};

class pointFileWriter :
	public FileWriter {
public:
	bool save(shape& p) {
		
		ofstream fout;
		fout.open("tuxing.txt", ios::app);
		if (!fout.is_open()){
			cerr << "Error: cannot open file " << "data.txt" << endl;
			return false;
		}
		else {
			fout << "point" << endl << toString(p.getcode()) << endl << toString(p.getdata()[0]) << " " << toString(p.getdata()[1]) << endl<<endl;
			cout << "save successfully";
			fout << flush;

			fout.close();
			return true;
		}
	}
};
class polylineFileWriter :
	public FileWriter {
public:
	bool save(shape& p) {
		ofstream fout;
		fout.open("tuxing.txt", ios::app);
		if (!fout.is_open()) {
			cerr << "Error: cannot open file " << "data.txt" << endl;
			return false;
		}
		else {
			fout << "polyline" << endl << toString(p.getcode()) << endl<<toString(p.getdata()[0])<<endl;
			for (int i = 1;i < 2 * p.getdata()[0] + 1;i += 2)
				fout << toString(p.getdata()[i]) << " " << toString(p.getdata()[i + 1])<<endl;
			for (int k = 0;k < 4;k++) 
			{ 
				if (k < 2)fout << p.getdraw()[k] << ",";
				else fout << p.getdraw()[k] << endl;
				
			}
	
							
			
			fout << endl;
			cout << "save successfully"<<endl;
			fout << flush;
			
			fout.close();
			return true;
		}
	}
};
class polygonFileWriter :
	public FileWriter {
public:
	bool save(shape& p) {
		ofstream fout;
		fout.open("tuxing.txt", ios::app);
		if (!fout.is_open()) {
			cerr << "Error: cannot open file " << "data.txt" << endl;
			return false;
		}
		else {
			fout << "polygon" << endl << toString(p.getcode()) << endl << toString(p.getdata()[0]) << endl;
			for (int i = 1;i < 2 * p.getdata()[0] + 1;i += 2)
				fout << toString(p.getdata()[i]) << " " << toString(p.getdata()[i + 1]) << endl;
			fout << endl;
			cout << "save successfully"<<endl;
			fout << flush;

			fout.close();
			return true;
		}
	}
};
//几何工厂类
class savefactory {
public:
	static Writer* Create(shape& sh, string t) {
		int co = sh.getcode();
		if (t == "t" || t == "T") {
			switch (co) {
			case(0): {return new pointFileWriter;}
			case(1): {return new polylineFileWriter;}
			case(2): {return new polygonFileWriter;}
			case(3): {return new pointFileWriter;}
			case(4): {return new pointFileWriter;}
			case(5): {return new pointFileWriter;}
			default: cout << "error:unknown type";return nullptr;
			}

		}
		else if (t == "s" || t == "S") {
			switch (co) {
			case(0): {return new pointSqlWriter;}
			default: cout << "error:unknown type";return nullptr;

			}

		}
		else return nullptr;
	}
	static void Destory(shape& Sh) {
		Sh.~shape();

	}

};

//主函数
int main() {
	
	point p=point(3, 1);
	point q = point(19, 26);
	point* ps = new point[2]{q,p};
	polyline pl(ps, 2);
	pl.setbc("1","2","3");
	pl.setbs("window");
	Writer* ka = savefactory::Create(pl,"t");
	ka->save(pl);
	return 0;
}