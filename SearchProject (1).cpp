#define _GLIBCXX_USE_CXX11_ABI 0
#include <utility>
#include <math.h>
#include <iostream>
#include <string>
#include <set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include "Point.h"
#include "Maze.h"
#include <stddef.h>

using namespace std;

/*
 * Assignment Written by Varick Erickson.
 * Inspired by PACMAN assignment written/developed by Dan Klein and Ron Parr 
 */

// Direction related constants
enum Direction {NORTH, SOUTH, EAST, WEST};
const string directions[4] = {"North", "South", "East", "West"};
vector<vector<Point>> temps;

// Searches you need to implement
vector<Point>  DFS(Maze&, bool(*isGoalState)(Point pt, Maze& maze));
vector<Point> BFS(Maze&, bool(*isGoalState)(Point pt, Maze& maze));
vector<Point> UCS(Maze&, bool(*isGoalState)(Point pt, Maze& maze), double(*CostFunction)(Point pt));
vector<Point> AStar(Maze&, bool(*isGoalState)(Point pt, Maze& maze), double(*Heuristic)(Point pt, Maze maze));
bool visited[100][100];
// Goal functions
bool FinalPositionGoal(Point , Maze& );
bool CornerGoal(Point , Maze& );

// Cost functions
double StayEast(Point pt);
double StayWest(Point pt);
double Ecluidian(Point pt, Maze maze);
double Manhattan(Point pt, Maze maze);

// Cost heuristics
double NullHeuristic(Point pt, Maze maze) {
	return 0;
}
///////  Add your Heuristic prototypes here

// Used for priority queue
bool ComparePoints(Point &a, Point &b)
{
	return a.cost < b.cost;
}

// A couple of convenience functionss
bool ValidDirection(int, Point&, Maze&);
bool NotInSet(Point, set<Point>&);

int main()
{
	string filename;
	//cout << "Enter Maze file name: ";
	//cin >> filename;
	filename = "openMaze.txt"; 
	// Initialize the maze
	Maze maze(filename);

	cout << maze << endl;
	const int height = maze.Height();
	const int width = maze.Width();
	for (int i = 0; i < maze.Height(); i++)
	{
		for (int j = 0; j < maze.Width(); j++)
		{
			visited[i][j] = false;
		}
	}
	vector<Point> soln;
	// Examples of calls
	//soln = DFS(maze, FinalPositionGoal);
	//soln = UCS(maze, FinalPositionGoal, StayEast);
	soln = AStar(maze, FinalPositionGoal, Ecluidian);

	//cout << "\nTotal Cost: " << soln->back().cost << endl;
	
	for (int i = 0; i < soln.size(); i++) {
		//cout << soln->at(i).row << endl;
		//cout << soln->at(i).col << endl;
		maze.SetVal(soln.at(i), '*');
	}

	cout << maze << endl;
	cout << "Total cost : " << soln.size()-1 << endl;
	return 0;
}

void helperMethod(Point s, Point d, Point path[], int &path_index,Maze& maze)
{
	int rowNum[] = { -1, 1,0, 0 };
	int colNum[] = { 0 , 0, 1, -1 };
	visited[s.row][s.col] = true;
	path[path_index] = s;
	path_index++;

	//if destination is reached, then save the visited valid paths
	if (s.row == d.row && s.col == d.col)
	{
		vector<Point> xx;
	
		for (int i = 0; i < path_index; i++)
		{
			xx.push_back(path[i]);
		}
		temps.push_back(xx);
	}
	//this will evaluate the nearby valid points for the current point and find the path recursively to traverse depth wise
	else
	{
		for (int i = 0; i < 4; i++)
		{
			int ro = s.row + rowNum[i];
			int co = s.col + colNum[i];
			Point temp = { ro,co };
			if ((ro >= 0) && (ro < maze.Height()) && (co >= 0) && (co < maze.Width()) && (maze.GetVal(temp) == ' ' || maze.GetVal(temp) == 'F') && (!visited[ro][co]))
			{
				helperMethod(temp, d, path, path_index, maze);
			}
		}
	}
	path_index--;
	visited[s.row][s.col] = false;
}


vector<Point>  DFS(Maze& maze, bool(*isGoalState)(Point pt, Maze& maze)) {
	int maxVal = INT_MAX;
	const int height = maze.Height();
	const int width = maze.Width();
	vector<Point> ans;
	Point *path = new Point[height * width];
	int path_index = 0;
	//s and d is defined as source to destination
	Point s = { maze.StartPt().row,maze.StartPt().col };
	Point d = { maze.goalPt().row,maze.goalPt().col };

		//helperMethod to make a recursive call to find the munimum path for a maze
		helperMethod(s, d, path, path_index, maze);

		for (int i = 0; i < temps.size(); i++)
		{
			if (temps[i].size() < maxVal)
			{
				maxVal = i;
			}
		}

		//returns the path to main fn
		return	temps[maxVal];
}


//function to check if the point is visited or not
int isNotVisited(Point x, vector<Point>& path)
{
	int size = path.size();
	for (int i = 0; i < size; i++)
		if (path[i].row == x.row && path[i].col == x.col)
			return 0;
	return 1;
}


vector<Point> BFS(Maze& maze, bool(*isGoalState)(Point pt, Maze& maze)) {
	int rowNum[] = { -1, 1,0, 0 };
	int colNum[] = { 0 , 0, 1, -1 };
	int maxVal = INT_MAX;
	vector<Point> solns;
	vector<vector<Point>> temp;
	vector<Point> path;
	//queue to maintain which point paths were traversed during the calculation
	queue<vector<Point>> q;
	//initializing the start point
	Point s = { maze.StartPt().row,maze.StartPt().col};
	path.push_back(s);
	q.push(path);
	//it will evlauate untill all the paths were evaluated
	while (!q.empty())
	{
		path  = q.front();
		q.pop();
		Point current  = path[path.size() - 1];
		if(current.row == maze.goalPt().row && current.col == maze.goalPt().col)
		{
			temp.push_back(path);

		}
			//traversing the nearby nodes
			for (int i = 0; i < 4; i++)
			{
				int ro = current.row + rowNum[i];
				int co = current.col + colNum[i];
				Point temp = { ro,co };
				if ((ro >= 0) && (ro < maze.Height()) && (co >= 0) && (co < maze.Width()) && (maze.GetVal(temp) == ' ' || maze.GetVal(temp) == 'F') && (isNotVisited(temp, path)))
				{
					temp.cost = current.cost + 1;
					vector<Point> newpath(path);
					newpath.push_back(temp);
					q.push(newpath);
				}	
			}
			
	}

	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i].size() < maxVal)
		{
			maxVal = i;
		}
	}

	return	temp[maxVal];
}



vector<Point> UCS(Maze& maze, bool(*isGoalState)(Point pt, Maze& maze), double(*CostFunction)(Point pt)) {
	
	// Priority Queue of points. Container specified as vector and a compare function is provided
	priority_queue<Point, vector<Point>, decltype(&ComparePoints)> frontier(ComparePoints);
	priority_queue<Point, vector<Point>, decltype(&ComparePoints)> path(ComparePoints);
	int rowNum[] = { -1, 1,0, 0 };
	int colNum[] = { 0 , 0, 1, -1 };
	int maxVal = INT_MAX;
	vector<Point> solns;
	Point s = { maze.StartPt().row,maze.StartPt().col };
	frontier.push(s);
	visited[s.row][s.col] = true;
	while (!frontier.empty())
	{
		Point current = frontier.top();
		frontier.pop();
		solns.push_back(current);
		if (current.row == maze.goalPt().row && current.col == maze.goalPt().col)
		{
			break;
		}
		path.empty();
		for (int i = 0; i < 4; i++)
		{
			int ro = current.row + rowNum[i];
			int co = current.col + colNum[i];
			Point temp = { ro,co };
			if ((ro >= 0) && (ro < maze.Height()) && (co >= 0) && (co < maze.Width()) && (maze.GetVal(temp) == ' ' || maze.GetVal(temp) == 'F') && (!visited[ro][co]))
			{
				//The cost for each node to reach final destination is calculated with CostFunction
				temp.cost = current.cost + CostFunction(temp) + (temp.row);
				visited[ro][co] = true;
				frontier.push(temp);
			}
		}
		//frontier.push(path.top());

	}

	return solns;



}


//calculating ecluidian distance
double Ecluidian(Point pt, Maze maze)
{
	return (pow(pow((pt.row - maze.goalPt().row), 2.0) + pow((pt.col - maze.goalPt().col), 2.0),0.5));
}


//calculating manhattan distance
double Manhattan(Point pt, Maze maze)
{
	return (abs(pt.row - maze.goalPt().row) - abs(pt.col - maze.goalPt().col));
}

vector<Point> AStar(Maze& maze, bool(*isGoalState)(Point pt, Maze& maze), double(*Heuristic)(Point pt, Maze maze)) {
	// Your code Here
	
	priority_queue<Point, vector<Point>, decltype(&ComparePoints)> frontier(ComparePoints);
	vector<Point> path[maze.Height()][maze.Width()];
	int rowNum[] = { -1, 1,0, 0 };
	int colNum[] = { 0 , 0, 1, -1 };
	int maxVal = INT_MAX;
	float dist[maze.Height()][maze.Width()] = { 0 };
	vector<Point> solns;
	//to maintain and manipulate the distance of a Point along with the Heuristic value
	for (int i = 0; i<maze.Height(); i++)
		for (int j = 0; j<maze.Width(); j++)
			dist[i][j] = INT_MAX;

	Point s = { maze.StartPt().row,maze.StartPt().col };
	Point current;
	current.row = s.row;
	current.col = s.col;
	dist[current.row][current.col] = Heuristic(s, maze);
	frontier.push(s);
	
	while (!(current.row == maze.goalPt().row && current.col == maze.goalPt().col))
	{
		//Evaluating the neighbouring points based on Heuristic values and path cost
		visited[current.row][current.col] = true;
		for (int i = 0; i < 4; i++)
		{
			int ro = current.row + rowNum[i];
			int co = current.col + colNum[i];
			Point temp = { ro,co };
			if ((ro >= 0) && (ro < maze.Height()) && (co >= 0) && (co < maze.Width()) && (maze.GetVal(temp) == ' ' || maze.GetVal(temp) == 'F') && (Heuristic(temp,maze) + dist[current.row][current.col] < dist[temp.row][temp.col]) )
			{
				dist[temp.row][temp.col] = Heuristic(temp, maze) + dist[current.row][current.col];
				path[temp.row][temp.col] = path[current.row][current.col], path[temp.row][temp.col].push_back(current);
			}
		}
		//To select the next point from which the further searching of points should be evaluated
		current = Point(-1, -1);
		float md = INT_MAX;
		for (int i = 0; i < maze.Height(); i++)
		{
			for (int j = 0; j < maze.Width(); j++)
			{
				if (!visited[i][j] && dist[i][j] != INT_MAX && dist[i][j] < md) { current = Point(i, j), md = dist[i][j]; }
			}
		}

	}
	path[maze.goalPt().row][maze.goalPt().col].push_back(maze.goalPt());
	return path[maze.goalPt().row][maze.goalPt().col];
	/* Examples:

	   double cost = Heuristic(pt, maze);  // How to get a cost from Heuristic function

	 */
}

/*
* Cost function that penalizes being in
* positions on the East side of the board.
* The cost for stepping into a position(r, c) is 2^c.
*/
double StayWest(Point pt) {
	return pt.col*pt.col;
}
/*
* Cost function that penalizes being in
* positions on the West side of the board.
* The cost for stepping into a position(r, c) is 0.5^c.
*/
double StayEast(Point pt) {
	return pow(0.5, pt.col);
}


/*
* Goal function where navigating to 'F' position
* is considered the goal.
*/
bool FinalPositionGoal(Point pt, Maze& maze) {
	return maze[pt] == 'F';
}



/*
 * Checks if child is in mySet.
 */
bool NotInSet(Point child, set<Point>& mySet) {
	return mySet.find(child) == mySet.end();
}




/*
 * Checks if direction dir is valid given the point pt
 * and the map of the maze.
 */
bool ValidDirection(int dir, Point& pt, Maze& maze) {
	//cout << "Checking " << directions[dir] << endl;
	switch (dir) {
	case NORTH:
		pt.row = pt.row - 1;
		break;
	case EAST:
		pt.col = pt.col + 1;
		break;
	case SOUTH:
		pt.row = pt.row + 1;
		break;
	case WEST:
		pt.col = pt.col - 1;
		break;
	default:
		return false;
	}
	//cout << pt.row << "," << pt.col << endl;
	if (pt.row > maze.Height() || pt.row < 0 ||
		pt.col > maze.Width() || pt.col < 0 ||
		maze[pt] == '@' || maze[pt] == '%') {
		return false;
	}

	return true;
}