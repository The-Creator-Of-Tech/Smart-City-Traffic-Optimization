#!/usr/bin/env python3
"""Black-box integration tests for the C++ command-line application."""
from pathlib import Path
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[1]
BINARY = ROOT / "smart_city_test"

class SmartCityCLITests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        subprocess.run([
            "g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-Wpedantic",
            str(ROOT / "src" / "main.cpp"), "-o", str(BINARY)
        ], check=True, capture_output=True, text=True)

    @classmethod
    def tearDownClass(cls):
        BINARY.unlink(missing_ok=True)

    def run_app(self, input_text):
        return subprocess.run([str(BINARY)], input=input_text, capture_output=True,
                              text=True, check=True, timeout=10).stdout

    def test_location_listing(self):
        out = self.run_app("1\n0\n")
        self.assertIn("0 - Central Station", out)
        self.assertIn("6 - Residential Area", out)

    def test_bfs_arrow_format(self):
        out = self.run_app("2\n0\n0\n")
        self.assertIn("BFS traversal: Central Station -> City Hospital -> University", out)
        self.assertNotIn("Central StationCity Hospital", out)

    def test_dfs(self):
        out = self.run_app("3\n0\n0\n")
        self.assertIn("DFS traversal: Central Station -> City Hospital", out)

    def test_dijkstra(self):
        out = self.run_app("4\n0\n4\n0\n")
        self.assertIn("Recommended route", out)
        self.assertIn("Combined route score:", out)

    def test_prim_mst(self):
        out = self.run_app("5\n0\n")
        self.assertIn("Minimum Spanning Tree", out)
        self.assertIn("Total MST cost:", out)
        self.assertEqual(out.count("(cost "), 6)

    def test_traffic_update(self):
        out = self.run_app("6\n0\n3\n20\n0\n")
        self.assertIn("Traffic delay updated.", out)

    def test_traffic_update_changes_route_score(self):
        out = self.run_app("4\n0\n4\n6\n0\n3\n0\n4\n0\n4\n0\n")
        self.assertIn("Traffic delay updated.", out)
        self.assertIn("Combined route score: 15 units", out)
        self.assertIn("Combined route score: 13 units", out)

    def test_invalid_route_ids(self):
        out = self.run_app("4\n99\n1\n0\n")
        self.assertIn("Invalid IDs.", out)

    def test_benchmark(self):
        out = self.run_app("8\n0\n")
        self.assertIn("BFS total:", out)
        self.assertIn("Dijkstra total:", out)

if __name__ == "__main__":
    unittest.main(verbosity=2)
