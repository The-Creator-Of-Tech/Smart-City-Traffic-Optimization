# Smart City Traffic Management and Route Optimization System

**Capstone project | C++17 core + interactive browser visualization**

## Student details
- **Name:** Ujjwal Chauhan
- **Roll number:** 2401201059
- **Course:** BCA AI & DS, Semester 5
- **University:** K.R. Mangalam University
- **Faculty:** Aarti Mam

## Project overview
This project models a simplified city's road network as an undirected weighted graph. The C++ command-line application implements BFS, DFS, Dijkstra's shortest path, and Prim's Minimum Spanning Tree. Traffic delay can be changed at runtime. A standalone browser dashboard provides an interactive schematic visualization of the same sample network and lets the user run the graph analyses, highlight routes/MST edges, and simulate traffic-delay updates.

## Features
- Adjacency-list graph representation with seven sample locations and eleven roads.
- BFS and DFS traversal.
- Dijkstra route recommendation using the illustrative edge score `distance + traffic delay`.
- Prim MST using road distance as infrastructure cost.
- Interactive traffic-delay updates in the C++ CLI and browser dashboard.
- Browser-based SVG network visualization with route and MST highlighting.
- Repeated-run BFS/Dijkstra benchmark in the C++ application.
- Automated black-box integration tests for the CLI.
- Sample data, execution logs, and capstone report.

## Project structure
```text
Smart_City_Traffic_Capstone/
├── README.md
├── src/main.cpp
├── data/sample_network.csv
├── visualization/dashboard.html
├── visualization/network_diagram.png
├── tests/test_cli.py
├── results/
│   ├── automated_test_results.txt
│   ├── dijkstra_demo.txt
│   ├── mst_demo.txt
│   ├── benchmark_demo.txt
│   ├── traffic_update_demo.txt
│   └── screenshots/                 # add your own screenshots before submission
└── Smart_City_Capstone_Report.docx
```

## Requirements
- C++17 compiler: GCC, Clang, or MSVC.
- Python 3.8+ only for running the automated integration test script.
- A modern browser (Chrome, Edge, Firefox) for the interactive dashboard.
- No third-party Python packages are required for tests or dashboard.

## Build and run the C++ application
Linux/macOS:
```bash
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic src/main.cpp -o smart_city
./smart_city
```

Windows (MinGW):
```powershell
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic src/main.cpp -o smart_city.exe
.\smart_city.exe
```

## Open the interactive visualization
Open `visualization/dashboard.html` directly in a modern web browser. No server or internet connection is required. Choose BFS, DFS, Dijkstra, or Prim; select source/destination; use the traffic-delay slider to update a road; then rerun the analysis. Yellow edges indicate the selected route and green edges indicate the MST.

**Important implementation note:** The C++ application is the primary assessed algorithm implementation. The standalone HTML dashboard is a visualization demonstrator that mirrors the same sample data and algorithm behaviour; traffic changes made in the dashboard are not persisted back into the C++ executable.

## Run automated tests
From the project root:
```bash
python tests/test_cli.py
```
The test script compiles the C++ source and runs nine black-box integration tests. The recorded result is in `results/automated_test_results.txt`. Re-run the tests on the submission computer and update the evidence if the environment differs.

## Demo workflow
1. Run the C++ application and select `1` to list locations.
2. Select `2` or `3` to run BFS or DFS from location `0`.
3. Select `4`, enter source `0` and destination `4`, to run Dijkstra.
4. Select `6`, update road `0–3`, then run Dijkstra again to compare the route score.
5. Select `5` to view the infrastructure MST.
6. Select `8` to measure repeated BFS and Dijkstra runs.
7. Open `visualization/dashboard.html` to view the interactive network.

## Data model and assumptions
Each road stores a positive distance value and a non-negative traffic-delay value. Dijkstra minimizes the sum of those two illustrative values. The score is not a physical unit; a production system should convert both inputs into a consistent travel-time or generalized-cost unit. Prim uses distance as the infrastructure connection cost. The supplied data is synthetic and intended for demonstration only.

## Algorithm complexity
| Algorithm | Time complexity | Auxiliary space |
|---|---:|---:|
| BFS | `O(V + E)` | `O(V)` |
| DFS | `O(V + E)` | `O(V)` |
| Dijkstra (binary heap) | `O((V + E) log V)` | `O(V + E)` |
| Prim (linear key selection) | `O(V² + E)` | `O(V + E)` |

## Validation summary
Nine automated integration tests pass: location listing, BFS formatting, DFS traversal, Dijkstra route, Prim MST edge count, traffic update, traffic-sensitive route-score change, invalid route IDs, and benchmark output. Actual run logs are saved under `results/`. Benchmark timings are machine/compiler dependent and must not be compared across systems without controlled conditions.

## GitHub and ZOSIMA submission
Create a public GitHub repository, push this complete folder, verify that faculty can access it, and paste the actual repository URL into the report and ZOSIMA. A repository URL is intentionally not fabricated or pre-filled. Add your own terminal/browser screenshots to `results/screenshots/` before final submission.

## Limitations and future work
The network is small and synthetic; the dashboard is a schematic network rather than a geographic map. There is no live traffic API, road-closure feed, persistent database, or integration between dashboard edits and the C++ process. Future work can import OpenStreetMap/GIS data, add real travel-time estimates, persist traffic changes, implement alternate-route comparisons, and benchmark larger generated graphs.
