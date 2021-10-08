#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

class Rectangle
{
private:
    void rotate_plus90()
    {
        unsigned int t;
        t = width;
        width = height;
        height = t;
        bool t2 = north_canted;
        north_canted = west_canted;
        west_canted = south_canted;
        south_canted = east_canted;
        east_canted = t2;
        rotated = true;
    }

    void rotate_minus90()
    {
        unsigned int t;
        t = width;
        width = height;
        height = t;

        bool t2 = north_canted;
        north_canted = east_canted;
        east_canted = south_canted;
        south_canted = west_canted;
        west_canted = t2;

        rotated = false;
    }

    bool rotated;
    bool rotatable;
public:
    Rectangle(bool nc, bool ec, bool sc, bool wc, unsigned int width, unsigned int height)
    {
        north_canted = nc;
        east_canted = ec;
        south_canted = sc;
        west_canted = wc;
        rotated = false;
        this->width = width;
        this->height = height;
        x = 0; y = 0;
        rotatable = true;
    }
    bool north_canted;
    bool east_canted;
    bool south_canted;
    bool west_canted;


    unsigned int width;
    unsigned int height;
    unsigned int x;
    unsigned int y;
    void rotate()
    {
        if (rotatable)
            (rotated) ? rotate_minus90() : rotate_plus90();
    }
    void lock_rotation()
    {
        rotatable = false;
    }
};

vector <Rectangle> read_input(string filepath, int& dsp_w, int& dsp_h, int& saw_width)
{
    ifstream is(filepath);
    vector <Rectangle> data;
    is >> dsp_w >> dsp_h >> saw_width;
    int n = 0;
    is >> n;
    bool nc, ec, sc, wc;
    int w, h;

    for (int i = 0; i < n; ++i)
    {
        is >> w >> h >> nc >> ec >> sc >> wc;
        Rectangle r(nc, ec, sc, wc, w, h);
        data.push_back(r);
    }
    is.close();
    return data;
}

vector <Rectangle> cast_input_vector(vector<vector<int>> input)
{
	vector <Rectangle> data;
	for (int i = 0; i < input.size(); ++i)
	{
		int nc = input[i][0];
		int ec = input[i][1];
		int sc = input[i][2];
		int wc = input[i][3];
		int w = input[i][4];
		int h = input[i][5];
		Rectangle r(nc, ec, sc, wc, w, h);
		data.push_back(r);
	}
	return data;
}
void write_output(vector <Rectangle> data, int total_count, double percantage)
{
	cout << "Total DSP count = " << total_count << " , Fill percentage = " << percantage << endl;
	for (int i = 0; i < data.size(); ++i)
	{
		Rectangle r = data[i];
		cout << "Rectangle w/h = " << r.width << " " << r.height << "Rectangle x/y =" << r.x << " " << r.y << endl;
	}
}
void generate_some_file(string path)
{
	ofstream os(path);
	os << 3000 << " " << 2000 << " " << 10 << endl;
	int n = 5000;
	os << n << endl;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> uid(50, 1500);

	for (int i = 0; i < n; ++i)
	{
		os << uid(gen) << " " << uid(gen) << " " <<  0 << " " << 0 << " " << 0 << " " << 0 << endl;
	}
	os.close();
}
bool comp(Rectangle r1, Rectangle r2)
{
	return r1.height > r2.height;
}
struct level
{
	int cell_h = 0;
	int curr_w = 0;
	int space_left = 0;
	int height = 0;

};
long long total_square(vector <Rectangle> data)
{
	long long total = 0;
	for (int i = 0; i < data.size(); ++i)
	{
		total += data[i].width * data[i].height;
	}
	return total;
}
vector <Rectangle> BF(vector <Rectangle> data, int dsp_w, int dsp_h, int& total_count, double& percentage, bool rotate)
{
	sort(data.begin(), data.end(), comp);
	vector<level> levels;
	level curr;
	curr.curr_w = data[0].width;
	curr.space_left = data[0].height * (dsp_w - data[0].width);
	curr.height = data[0].height;
	//First highest is placed beforehand, also 1 level is created
	levels.push_back(curr);

	//Try to put each rectangle on each lvl it can be put to
	//and compare how much space is left after placing rectangle there
	//If rectangle cannot be placed a new lvl is added
	for (int i = 1; i < data.size(); ++i)
	{
		int least_space = 2000000000;
		int best_lvl_number = -1;
		int curr_space_left = 0;
		//Search for best lvl to fit current rect
		for (int j = 0; j < levels.size(); ++j)
		{
			if ((data[i].height <= levels[j].height) && (data[i].width <= (dsp_w - levels[j].curr_w))) //if rect can fit on curr lvl
			{
				curr_space_left = levels[j].space_left - (data[i].height * data[i].width);
				if (curr_space_left < least_space)
				{
					least_space = curr_space_left;
					best_lvl_number = j;
				}
			}
		}
		//rotation addition makes algorythm rotate current rectangle in search for its better place
		if (rotate)
		{
			bool find_best_rotated = false;
			data[i].rotate();
			for (int j = 0; j < levels.size(); ++j)
			{
				if ((data[i].height <= levels[j].height) && (data[i].width <= (dsp_w - levels[j].curr_w))) //if rect can fit on curr lvl
				{
					curr_space_left = levels[j].space_left - (data[i].height * data[i].width);
					if ((curr_space_left < least_space) || ((curr_space_left == least_space) && (data[i].height > data[i].width)))
					{
						least_space = curr_space_left;
						best_lvl_number = j;
						find_best_rotated = true;
					}
				}
			}
			if (!find_best_rotated) data[i].rotate();
		}
		
		if (best_lvl_number == -1)
		{
			//If cant fit anywhere add new lvl and fit it there
			level newlevel;
			newlevel.cell_h = levels[levels.size() - 1].cell_h + levels[levels.size() - 1].height;
			bool cant_fit = false;
			//if it cant fit on that dsp list we place this rectangle on next list
			if ((newlevel.cell_h + data[i].height) / dsp_h > levels[levels.size() - 1].cell_h / dsp_h)
			{
				newlevel.cell_h = newlevel.cell_h + dsp_h - newlevel.cell_h % dsp_h;
				cant_fit = true;
			}
			newlevel.curr_w += data[i].width;
			newlevel.height = data[i].height;
			newlevel.space_left = (newlevel.height * (dsp_w - newlevel.curr_w));
			data[i].x = 0;
			data[i].y = newlevel.cell_h;
			if (cant_fit)
			{
				//however we still can use that space left
				level extra;
				extra.cell_h = levels[levels.size() - 1].cell_h + levels[levels.size() - 1].height;
				extra.curr_w = 0;
				extra.height = newlevel.cell_h - extra.cell_h;
				extra.space_left = (extra.height * dsp_w );
				levels.push_back(extra);
			}
			levels.push_back(newlevel);
		}
		else
		{
			//Fit rect on that lvl
			data[i].x = levels[best_lvl_number].curr_w;
			data[i].y = levels[best_lvl_number].cell_h;
			levels[best_lvl_number].curr_w += data[i].width;
			levels[best_lvl_number].space_left -= (data[i].height * data[i].width);
		}
	}
	//After evey rectangle is set we calculate dsp count and fill percentage
	int total_height = (levels[levels.size() - 1].cell_h + levels[levels.size() - 1].height);
	total_count = total_height / dsp_h;
	if ((levels[levels.size() - 1].cell_h + levels[levels.size() - 1].height) % dsp_h > 0) total_count++;
	percentage = ((double)total_square(data) / dsp_w) / total_height;
	return data;
}
vector <Rectangle> logic_part(vector <Rectangle> data, int dsp_w, int dsp_h, int saw_width, int& min_total, double& max_percent)
{
	int total_count = 0;  min_total = 100000000;
	double percentage = 0; max_percent = 0;
	//Add saw width to dsp sizes so we dont care about inner and outer rectangles
	dsp_w += saw_width;
	dsp_h += saw_width;
	//Add 1/2 sw to rectangle from each side. If 2 rectangles contact we are sure between them is exactly sw
	for (int i = 0; i < data.size(); ++i)
	{
		data[i].width += saw_width;
		data[i].height += saw_width;
	}
	//First try without rotations
	data = BF(data, dsp_w, dsp_h, total_count, percentage,false);
	cout << total_count << " " << percentage << endl;
	vector <Rectangle> best(data); min_total = total_count; max_percent = percentage;
	//Rotate every Rectangle so height < width
	//DO bf  and remember stats
	for (int i = 0; i < data.size(); ++i)
	{
		if (data[i].height > data[i].width)
			swap(data[i].height, data[i].width);

	}
	data = BF(data, dsp_w, dsp_h, total_count, percentage,false);
	cout << total_count << " " << percentage << endl;
	if ((total_count < min_total) || ((total_count == min_total) && (percentage > max_percent)))
	{
		best.clear(); best.insert(best.begin(), data.begin(), data.end());
		min_total = total_count; max_percent = percentage;
	}
	


	//Do bf with rotations
	data = BF(data, dsp_w, dsp_h, total_count, percentage, true);
	cout << total_count << " " << percentage << endl;
	if ((total_count < min_total) || ((total_count == min_total) && (percentage > max_percent)))
	{
		best.clear(); best.insert(best.begin(), data.begin(), data.end());
		min_total = total_count; max_percent = percentage;
	}
	//Rotate every Rectangle so height < width
	//DO bf  and remember stats
	for (int i = 0; i < data.size(); ++i)
	{
		if (data[i].height > data[i].width)
			swap(data[i].height, data[i].width);

	}
	data = BF(data, dsp_w, dsp_h, total_count, percentage, true);
	cout << total_count << " " << percentage << endl;
	if ((total_count < min_total) || ((total_count == min_total) && (percentage > max_percent)))
	{
		best.clear(); best.insert(best.begin(), data.begin(), data.end());
		min_total = total_count; max_percent = percentage;
	}
	
	return best;
}
void algorythm()
{
	string in_path = "D:\\1.txt";
	//generate_some_file(in_path);
	int dsp_w  = 0 , dsp_h = 0, saw_width = 0;
	vector <Rectangle> data = read_input(in_path, dsp_w, dsp_h, saw_width);
	for (int i = 0; i < data.size(); ++i)
	{
		if ((data[i].width > dsp_w) && (data[i].height <= dsp_w))
		{
			data[i].rotate();
			data[i].lock_rotation();
		}
		if ((data[i].width > dsp_w) && (data[i].height > dsp_w))
		{
			throw "One of rectangles can't fit in dsp";
		}
		if ((data[i].height > dsp_h) && (data[i].width <= dsp_h))
		{
			data[i].rotate();
			data[i].lock_rotation();
		}
		if ((data[i].height > dsp_h) && (data[i].width <= dsp_h))
		{
			throw "One of rectangles can't fit in dsp";
		}
	}
	int min_total = 0; double max_percent = 0;
		auto start = chrono::steady_clock::now();
	vector <Rectangle> best = logic_part(data, dsp_w, dsp_h, saw_width, min_total, max_percent);
		cout << min_total << " " << max_percent << endl;
		auto finish = chrono::steady_clock::now();
		cout << "Time : " << chrono::duration_cast <chrono::milliseconds>(finish - start).count()  << "ms "<< endl;
	write_output(best, min_total, max_percent);
}
void algorythm(vector<vector<int>> input, int dsp_w, int dsp_h, int saw_width = 10)
{
	vector <Rectangle> data = cast_input_vector(input);
	for (int i = 0; i < data.size(); ++i)
	{
		if ((data[i].width > dsp_w) && (data[i].height <= dsp_w))
		{
			data[i].rotate();
			data[i].lock_rotation();
		}
		if ((data[i].width > dsp_w) && (data[i].height > dsp_w))
		{
			throw "One of rectangles can't fit in dsp";
		}
		if ((data[i].height > dsp_h) && (data[i].width <= dsp_h))
		{
			data[i].rotate();
			data[i].lock_rotation();
		}
		if ((data[i].height > dsp_h) && (data[i].width <= dsp_h))
		{
			throw "One of rectangles can't fit in dsp";
		}
	}
	int min_total = 0; double max_percent = 0;
	auto start = chrono::steady_clock::now();
	vector <Rectangle> best = logic_part(data, dsp_w, dsp_h, saw_width, min_total, max_percent);
	cout << min_total << " " << max_percent << endl;
	auto finish = chrono::steady_clock::now();
	cout << "Time : " << chrono::duration_cast <chrono::milliseconds>(finish - start).count() << "ms " << endl;
	write_output(best, min_total, max_percent);
}