#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

const int MAX_STATION = 10000;
const int MAX_LINE = 1000;

struct Station {
    string name;
    int stationId;
    vector<pair<int, int>> lines; // <lineId, InlineId> 该站点所属的线路, 以及在线路中的编号
    bool operator<(const Station &other) const {
	return stationId < other.stationId;
    }
    Station() {
	stationId = -1;
    }
};

struct Line { // 公交线路
    string name;
    int lineId;
    vector<int> stationIds;
    bool operator<(const Line &other) const {
	return lineId < other.lineId;
    }
    Line() {
	lineId = -1;
    }
};

vector<Station> stations(MAX_STATION);
vector<Line> lines(MAX_LINE);

void readCSV(const string &filename) {
    ifstream file(filename);
    string line, word;

    // Skip the header
    getline(file, line);

    while (getline(file, line)) {
	stringstream ss(line);
	vector<string> row;
	while (getline(ss, word, ',')) {
	    row.push_back(word);
	}
	string lineName = row[0];
	string stationName = row[1];
	int lineId = stoi(row[2]);
	int InlineId = stoi(row[3]);
	int stationId = stoi(row[4]);

	if (stations[stationId].stationId == -1) {
	    stations[stationId].name = stationName;
	    stations[stationId].stationId = stationId;
	}
	stations[stationId].lines.push_back({lineId, InlineId});
	if (lines[lineId].lineId == -1) {
	    lines[lineId].name = lineName;
	    lines[lineId].lineId = lineId;
	}
	lines[lineId].stationIds.push_back(stationId);
	// cerr << "lineId: " << lineId << " stationId: " << stationId << " InlineId: " << InlineId << endl;
	assert(lines[lineId].stationIds.size() == InlineId + 1); // 检查站点编号是否连续
    }
}

// 转车次数最少的乘车路线
vector<pair<string, string>> findTransferLeastPath(const int startStationId, const int endStationId) {
    vector<pair<string, string>> path; // <stationName, lineName>
    vector<int> dist(MAX_STATION, numeric_limits<int>::max());
    vector<pair<int, int>> prev(MAX_STATION, {-1, -1}); // <stationId, lineId>
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[startStationId] = 0;
    pq.push({0, startStationId});

    while (!pq.empty()) {
	int u = pq.top().second;
	pq.pop();

	if (u == endStationId) {
	    break;
	}

	for (const auto &line : stations[u].lines) {
	    int lineId = line.first;
	    for (const auto &stationId : lines[lineId].stationIds) {
		if (dist[stationId] > dist[u] + 1) {
		    dist[stationId] = dist[u] + 1;
		    prev[stationId] = make_pair(u, lineId);
		    pq.push({dist[stationId], stationId});
		}
	    }
	}
    }

    if (prev[endStationId] == make_pair(-1, -1)) {
	return path;
    }

    for (int v = endStationId; v != -1; v = prev[v].first) {
	path.push_back(make_pair(stations[v].name, lines[prev[v].second].name));
    }
    reverse(path.begin(), path.end());
    return path;
}

// 经过站点数量最少的乘车路线
vector<pair<string, string>> findLeastStationPath(const int startStationId, const int endStationId) {
    vector<pair<string, string>> path; // <stationName, lineName>
    vector<int> dist(MAX_STATION, numeric_limits<int>::max());
    vector<pair<int, int>> prev(MAX_STATION, {-1, -1}); // <stationId, lineId>
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[startStationId] = 0;
    pq.push({0, startStationId});

    while (!pq.empty()) {
	int u = pq.top().second;
	pq.pop();

	if (u == endStationId) {
	    break;
	}

	for (const auto &line : stations[u].lines) {
	    int lineId = line.first;
	    int inlineId = line.second;
	    if (inlineId + 1 < lines[lineId].stationIds.size()) {
		int v = lines[lineId].stationIds[inlineId + 1];
		if (dist[v] > dist[u] + 1) {
		    dist[v] = dist[u] + 1;
		    prev[v] = make_pair(u, lineId);
		    pq.push({dist[v], v});
		}
	    }
	    if (inlineId - 1 >= 0) {
		int v = lines[lineId].stationIds[inlineId - 1];
		if (dist[v] > dist[u] + 1) {
		    dist[v] = dist[u] + 1;
		    prev[v] = make_pair(u, lineId);
		    pq.push({dist[v], v});
		}
	    }
	}
    }

    if (prev[endStationId] == make_pair(-1, -1)) {
	return path;
    }

    for (int v = endStationId; v != -1; v = prev[v].first) {
	path.push_back(make_pair(stations[v].name, lines[prev[v].second].name));
    }
    reverse(path.begin(), path.end());
    return path;
}

int main() {
    readCSV("nanjing.csv");

    string start, end;
    cout << "请输入起始站点: ";
    cin >> start;
    cout << "请输入终点站点: ";
    cin >> end;

    int startStationId = -1, endStationId = -1;
    for (const auto &station : stations) {
	if (station.name == start) {
	    startStationId = station.stationId;
	}
	if (station.name == end) {
	    endStationId = station.stationId;
	}
    }
    assert(startStationId != -1 && endStationId != -1);
    cout << "---------------------------------" << endl;
    auto path = findTransferLeastPath(startStationId, endStationId);
    if (path.empty()) {
	cout << "无法到达终点站点" << endl;
    } else {
	cout << "最少转车次数: " << path.size() - 2 << endl;
	cout << "路线: " << endl;
	for (size_t i = 0; i < path.size(); ++i) {
	    if (i > 0) {
		cout << "  |" << endl;
		cout << "  |" << endl;
		cout << "  | 乘坐 " << path[i].second << endl;
		cout << "  |" << endl;
		cout << "  |" << endl;
	    }
	    cout << path[i].first << endl;
	}
    }
    cout << "---------------------------------" << endl;
    path = findLeastStationPath(startStationId, endStationId);
    if (path.empty()) {
	cout << "无法到达终点站点" << endl;
    } else {
	cout << "经过站点数量最少: " << path.size() - 2 << endl;
	cout << "路线: " << endl;
	for (size_t i = 0; i < path.size(); ++i) {
	    if (i > 0) {
		cout << "  |" << endl;
		cout << "  |" << endl;
		cout << "  | 乘坐 " << path[i].second << endl;
		cout << "  |" << endl;
		cout << "  |" << endl;
	    }
	    cout << path[i].first << endl;
	}
    }
    return 0;
}
