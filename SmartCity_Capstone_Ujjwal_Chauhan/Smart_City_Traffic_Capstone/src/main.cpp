#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <string>
#include <chrono>
#include <unordered_map>

using namespace std;
using Clock = chrono::high_resolution_clock;
const int INF = numeric_limits<int>::max() / 4;

struct Edge {
    int to;
    int distance;
    int traffic; // additional delay/cost in minutes
};

class CityGraph {
    vector<string> names;
    vector<vector<Edge>> adj;
public:
    explicit CityGraph(int n) : names(n), adj(n) {}
    int size() const { return (int)names.size(); }
    void setName(int i, const string& name) { names[i] = name; }
    string name(int i) const { return names.at(i); }

    void addRoad(int u, int v, int distance, int traffic) {
        if (u < 0 || v < 0 || u >= size() || v >= size() || distance <= 0 || traffic < 0)
            throw invalid_argument("Invalid road data.");
        adj[u].push_back({v, distance, traffic});
        adj[v].push_back({u, distance, traffic});
    }

    void updateTraffic(int u, int v, int traffic) {
        bool found = false;
        for (auto &e : adj.at(u)) if (e.to == v) { e.traffic = traffic; found = true; }
        for (auto &e : adj.at(v)) if (e.to == u) { e.traffic = traffic; found = true; }
        if (!found) throw invalid_argument("Road not found.");
    }

    void printLocations() const {
        cout << "\nLocations:\n";
        for (int i = 0; i < size(); ++i) cout << "  " << i << " - " << names[i] << '\n';
    }

    void bfs(int start) const {
        vector<bool> seen(size(), false);
        queue<int> q;
        vector<int> order;
        q.push(start);
        seen[start] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (const auto &e : adj[u]) {
                if (!seen[e.to]) {
                    seen[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        cout << "BFS traversal: ";
        for (size_t i = 0; i < order.size(); ++i)
            cout << names[order[i]] << (i + 1 == order.size() ? "" : " -> ");
        cout << '\n';
    }

    void dfsUtil(int u, vector<bool>& seen, vector<string>& order) const {
        seen[u] = true; order.push_back(names[u]);
        for (const auto &e : adj[u]) if (!seen[e.to]) dfsUtil(e.to, seen, order);
    }
    void dfs(int start) const {
        vector<bool> seen(size(), false); vector<string> order;
        dfsUtil(start, seen, order);
        cout << "DFS traversal: ";
        for (size_t i = 0; i < order.size(); ++i)
            cout << order[i] << (i + 1 == order.size() ? "" : " -> ");
        cout << '\n';
    }

    // Dijkstra minimizes distance + current traffic delay (minutes).
    void shortestPath(int source, int target) const {
        vector<int> dist(size(), INF), parent(size(), -1);
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
        dist[source] = 0; pq.push({0, source});
        while (!pq.empty()) {
            auto [cost, u] = pq.top(); pq.pop();
            if (cost != dist[u]) continue;
            for (const auto &e : adj[u]) {
                int weight = e.distance + e.traffic;
                if (dist[u] + weight < dist[e.to]) {
                    dist[e.to] = dist[u] + weight;
                    parent[e.to] = u;
                    pq.push({dist[e.to], e.to});
                }
            }
        }
        if (dist[target] == INF) { cout << "No route available.\n"; return; }
        vector<int> path;
        for (int v = target; v != -1; v = parent[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        int totalDistance = 0, totalTrafficDelay = 0;
        for (size_t i = 1; i < path.size(); ++i) {
            int u = path[i - 1], v = path[i];
            for (const auto &e : adj[u]) if (e.to == v) {
                totalDistance += e.distance;
                totalTrafficDelay += e.traffic;
                break;
            }
        }
        cout << "\nRecommended route (distance + traffic-delay cost):\n";
        for (size_t i = 0; i < path.size(); ++i)
            cout << names[path[i]] << (i + 1 == path.size() ? "" : " -> ");
        cout << "\nRoad-distance total: " << totalDistance
             << " units\nTraffic-delay total: " << totalTrafficDelay
             << " units\nCombined route score: " << dist[target] << " units\n";
        cout << "Note: this score combines road distance and traffic delay for demonstration; "
                "real deployments should normalize units and use live travel-time estimates.\n";
    }

    // Prim MST using distance as infrastructure construction cost.
    void primMST() const {
        int n = size();
        if (n == 0) { cout << "No locations available.\n"; return; }
        vector<int> key(n, INF), parent(n, -1);
        vector<bool> inMST(n, false);
        key[0] = 0;
        int total = 0;
        cout << "\nMinimum Spanning Tree (road distance as cost):\n";
        for (int count = 0; count < n; ++count) {
            int u = -1;
            for (int i = 0; i < n; ++i)
                if (!inMST[i] && (u == -1 || key[i] < key[u])) u = i;
            if (u == -1 || key[u] == INF) {
                cout << "Network is disconnected; an MST cannot span every location.\n"; return;
            }
            inMST[u] = true;
            if (parent[u] != -1) {
                cout << "  " << names[parent[u]] << " -- " << names[u]
                     << " (cost " << key[u] << ")\n";
                total += key[u];
            }
            for (const auto &e : adj[u])
                if (!inMST[e.to] && e.distance < key[e.to]) {
                    key[e.to] = e.distance; parent[e.to] = u;
                }
        }
        cout << "Total MST cost: " << total << '\n';
    }

    void listRoads() const {
        cout << "\nRoads (distance, traffic delay):\n";
        for (int u = 0; u < size(); ++u)
            for (const auto &e : adj[u]) if (u < e.to)
                cout << "  " << names[u] << " <-> " << names[e.to]
                     << " | distance=" << e.distance << ", traffic delay=" << e.traffic << '\n';
    }

    void benchmark() const {
        auto t1 = Clock::now();
        for (int i = 0; i < 1000; ++i) {
            vector<bool> seen(size(), false);
            queue<int> q; q.push(0); seen[0] = true;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (const auto &e : adj[u]) if (!seen[e.to]) {
                    seen[e.to] = true; q.push(e.to);
                }
            }
        }
        auto t2 = Clock::now();
        auto t3 = Clock::now();
        for (int i = 0; i < 1000; ++i) {
            vector<int> d(size(), INF);
            priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
            d[0] = 0; pq.push({0,0});
            while (!pq.empty()) {
                auto [cost,u] = pq.top(); pq.pop();
                if (cost != d[u]) continue;
                for (const auto &e : adj[u]) {
                    int w = e.distance + e.traffic;
                    if (d[u] + w < d[e.to]) { d[e.to] = d[u] + w; pq.push({d[e.to],e.to}); }
                }
            }
        }
        auto t4 = Clock::now();
        cout << "\nBenchmark (1000 runs; timings depend on computer/compiler):\n";
        cout << "  BFS total: " << chrono::duration<double, milli>(t2-t1).count() << " ms\n";
        cout << "  Dijkstra total: " << chrono::duration<double, milli>(t4-t3).count() << " ms\n";
        cout << "Complexities: BFS/DFS O(V+E); Dijkstra with priority queue O((V+E) log V); "
                "Prim here O(V^2 + E) due to linear minimum-key selection.\n";
    }
};

CityGraph sampleCity() {
    CityGraph g(7);
    vector<string> names = {"Central Station", "City Hospital", "University", "Market",
                            "Airport", "Tech Park", "Residential Area"};
    for (int i = 0; i < 7; ++i) g.setName(i, names[i]);
    g.addRoad(0,1,4,2); g.addRoad(0,2,5,1); g.addRoad(0,3,3,4);
    g.addRoad(1,2,2,1); g.addRoad(1,6,6,2); g.addRoad(2,3,4,2);
    g.addRoad(2,5,3,1); g.addRoad(3,4,7,3); g.addRoad(3,6,5,2);
    g.addRoad(4,5,4,1); g.addRoad(5,6,6,4);
    return g;
}

int main() {
    CityGraph city = sampleCity();
    int choice;
    cout << "SMART CITY TRAFFIC MANAGEMENT & ROUTE OPTIMIZATION\n";
    do {
        cout << "\n1. Show locations\n2. BFS traversal\n3. DFS traversal\n"
             << "4. Shortest route (Dijkstra)\n5. Infrastructure planning (Prim MST)\n"
             << "6. Update traffic delay\n7. List roads\n8. Performance benchmark\n0. Exit\n"
             << "Choose: ";
        if (!(cin >> choice)) return 0;
        try {
            if (choice == 1) city.printLocations();
            else if (choice == 2 || choice == 3) {
                city.printLocations(); int s; cout << "Start location ID: "; cin >> s;
                if (s < 0 || s >= city.size()) cout << "Invalid location ID.\n";
                else if (choice == 2) city.bfs(s); else city.dfs(s);
            } else if (choice == 4) {
                city.printLocations(); int s,t; cout << "Source ID and destination ID: "; cin >> s >> t;
                if (s < 0 || t < 0 || s >= city.size() || t >= city.size()) cout << "Invalid IDs.\n";
                else city.shortestPath(s,t);
            } else if (choice == 5) city.primMST();
            else if (choice == 6) {
                city.printLocations(); int u,v,delay;
                cout << "Enter two connected location IDs and new non-negative traffic delay: ";
                cin >> u >> v >> delay;
                if (u < 0 || v < 0 || u >= city.size() || v >= city.size() || delay < 0)
                    cout << "Invalid input.\n";
                else { city.updateTraffic(u,v,delay); cout << "Traffic delay updated.\n"; }
            } else if (choice == 7) city.listRoads();
            else if (choice == 8) city.benchmark();
            else if (choice != 0) cout << "Invalid menu option.\n";
        } catch (const exception& ex) { cout << "Error: " << ex.what() << '\n'; }
    } while (choice != 0);
    cout << "Thank you.\n";
    return 0;
}
