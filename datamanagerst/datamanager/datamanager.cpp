//库的调用
#include<iostream>
#include<sstream>
#include<string>
#include<cmath>
#include<cassert>
#include<fstream>
#include<vector>
#include <cstring>
#include <fstream>
using namespace std;
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
	unsigned char r, b, g;
};
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

	virtual void printType() = 0;
	virtual void setbordercolor(int r, int b, int g) = 0;
	virtual void setborderstyle(string bs) = 0;
	virtual void setfillcolor(int r, int b, int g) = 0;
	virtual void setfillstlye(string fs) = 0;
};
shape::~shape() {}
//点类
class point : public shape {
public:
	int code = 0;
	int x, y;
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
	void setbordercolor(int r, int b, int g) {};
	void setborderstyle(string bs) {};
	void setfillcolor(int r, int b, int g) {};
	void setfillstlye(string fs) {};
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

	void setbordercolor(int r, int b, int g) { bordercolor.r = r; bordercolor.b = b;bordercolor.g = g; }
	void setborderstyle(string bs) { this->borderstyle = bs; }
	void setfillcolor(int r, int b, int g) {}
	void setfillstlye(string fs) {}
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
};

//矩形类
class rectangle :public shape {
public:
	int code = 3;
	point p;
	point q;
	rectangle() {}
	rectangle(float X, float Y, float X1, float Y1) :p(X, Y), q(X1, Y1) { if (X == X1 && Y == Y1) { assert(false); } }
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
	void printType() { cout << "this is a rectangle." << endl; }
	void setbordercolor(int r, int b, int g) { bordercolor.r = r; bordercolor.b = b;bordercolor.g = g; };
	void setborderstyle(string bs) { borderstyle = bs; };
	void setfillcolor(int r, int b, int g) { fillcolor.r = r; fillcolor.b = b;fillcolor.g = g; };
	void setfillstlye(string fs) { fillstyle = fs; };
};
//圆形类
class circle :public shape {


public:
	int code = 4;
	point center;
	int r;
	circle() {}
	circle(float X, float Y, float R) :center(X, Y), r(R) { if (R == 0) { assert(false); } }
	circle(const point& p, float R) : center(p), r(R) { if (R == 0) { assert(false); } }
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

	void setbordercolor(int r, int b, int g) { bordercolor.r = r; bordercolor.b = b;bordercolor.g = g; };
	void setborderstyle(string bs) { borderstyle = bs; };
	void setfillcolor(int r, int b, int g) { fillcolor.r = r; fillcolor.b = b;fillcolor.g = g; };
	void setfillstlye(string fs) { fillstyle = fs; };
};
//三角形类
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

class FileWriter {
public:
	virtual bool Write(const string& data, const string& filename) = 0;
};

class pointFileWriter :
	public FileWriter {
public:
	bool Write(const string& data, const string& filename) {}
	bool save(point* p) {
		ofstream fout("data.txt");
		if (!fout) {
			cerr << "Error: cannot open file " << "data.txt" << endl;
			return false;
		}
		fout << "point" << endl << p->code << endl << p->x << endl << p->y << endl;

		return true;
	}
};
//几何工厂类
class savefactory {
public:
	static FileWriter* Create(shape* sh) {
		int co = sh->code;
		switch (co)
				case(0): {return new pointFileWriter();

		}

	}


	static void Destory(shape* Sh) {
		Sh->~shape();

	}

};

//主函数
int main() {
	point x(1, 1);
	shape* g[3] = {
	 Shapefactory::Create("Circle 0 0 1"),
	 Shapefactory::Create("Triangle -1 -1 2 2 2 -2"),
	 Shapefactory::Create("Rectangle 0 2 1 0")
	};

	cout << endl;
	g[0]->printType();
	cout << "The area of it is:" << g[0]->getArea() << endl;
	cout << "The circumference of it is:" << g[0]->getCir() << endl;
	if (g[0]->isContain(&x))cout << "The point in the Circle." << endl;
	else cout << "The point not in the Circle." << endl;
	g[0]->isInter(g[2]);
	g[0]->isInter(g[1]);

	cout << endl;
	g[1]->printType();
	cout << "The area of it is:" << g[1]->getArea() << endl;
	cout << "The circumference of it is:" << g[1]->getCir() << endl;
	if (g[0]->isContain(&x))cout << "The point in the Triangle." << endl;
	else cout << "The point not in the Triangle." << endl;
	g[1]->isInter(g[0]);
	g[1]->isInter(g[2]);

	cout << endl;
	g[2]->printType();
	cout << "The area of it is:" << g[2]->getArea() << endl;
	cout << "The circumference of it is:" << g[2]->getCir() << endl;
	if (g[0]->isContain(&x))cout << "The point in the Rectangle." << endl;
	else cout << "The point not in the Rectangle." << endl;
	g[2]->isInter(g[0]);
	g[2]->isInter(g[1]);

	cout << endl;
	Shapefactory::Destory(g[0]);
	Shapefactory::Destory(g[1]);
	Shapefactory::Destory(g[2]);

	cout << endl;
	vector<string> vec;
	Getxt("D:\\Visual Studiofile\\SXnum2\\geometry_data.txt", vec);
	int n = vec.size();
	for (int i = 0; i < n; i++)Shapefactory::Create(vec[i]);
	cout << "A total of " << n << " graphics were produced." << endl;

}