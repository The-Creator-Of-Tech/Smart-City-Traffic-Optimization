# Test plan and execution evidence

## Automated test coverage
Run from the project root with `python tests/test_cli.py`.

| Test ID | Test case | Expected result | Recorded result |
|---|---|---|---|
| T1 | List locations | Seven IDs and names shown | PASS |
| T2 | BFS from location 0 | Breadth-first order printed with separators | PASS |
| T3 | DFS from location 0 | Depth-first order printed | PASS |
| T4 | Dijkstra from 0 to 4 | Route, distance, delay and combined score printed | PASS |
| T5 | Prim MST | Six edges for the connected seven-node graph | PASS |
| T6 | Update road 0–3 delay | Update confirmation appears | PASS |
| T7 | Invalid source/destination ID | Invalid IDs message appears | PASS |
| T8 | Benchmark | BFS and Dijkstra timings displayed | PASS |
| T9 | Reduce traffic delay on road 0–3, rerun Dijkstra | Composite route score changes from 15 to 13 on sample graph | PASS |

The automated run recorded in `automated_test_results.txt` completed 8 tests successfully. Tests compile the current `src/main.cpp` with GCC using C++17 and warning flags.

## Manual demonstration checklist
- [ ] Capture a screenshot of the C++ menu and location list.
- [ ] Capture BFS and DFS output.
- [ ] Capture Dijkstra route output.
- [ ] Capture Prim MST output.
- [ ] Capture a traffic-delay update and rerun Dijkstra.
- [ ] Capture the interactive browser dashboard in operation.
- [ ] Replace/add screenshots under `results/screenshots/` and include them in the final report if requested by faculty.

## Evidence files
- `dijkstra_demo.txt`: actual CLI output for route 0 to 4.
- `mst_demo.txt`: actual Prim MST output.
- `benchmark_demo.txt`: actual local benchmark output; timings are hardware-dependent.
- `traffic_update_demo.txt`: actual CLI traffic-update workflow.
- `automated_test_results.txt`: automated test run summary.
